#include "display_manager.h"
#include "esp_log.h"

static const char* TAG = "SpiLcdDisplayExt";

std::vector<Display*> DisplayManager::displays_;
Display* DisplayManager::primary_display_ = nullptr;

void DisplayManager::AddDisplay(Display* display, bool is_primary) {
    if (!display) return;
    
    displays_.push_back(display);
    if (is_primary || displays_.size() == 1) {
        primary_display_ = display;
    }
    ESP_LOGI("DisplayManager", "Display added, total: %d", displays_.size());
}

void DisplayManager::RemoveDisplay(Display* display) {
    auto it = std::find(displays_.begin(), displays_.end(), display);
    if (it != displays_.end()) {
        displays_.erase(it);
        if (primary_display_ == display) {
            primary_display_ = displays_.empty() ? nullptr : displays_[0];
        }
    }
}

size_t DisplayManager::GetDisplayCount() {
    return displays_.size();
}

Display* DisplayManager::GetPrimaryDisplay() {
    return primary_display_;
}

const std::vector<Display*>& DisplayManager::GetAllDisplays() {
    return displays_;
}

// 应用到所有屏幕
void DisplayManager::SetEmotion(const char* emotion) {
    for (auto display : displays_) {
        if (display) display->SetEmotion(emotion);
    }
}

void DisplayManager::SetIcon(const char* icon) {
    for (auto display : displays_) {
        if (display) display->SetIcon(icon);
    }
}

void DisplayManager::SetPreviewImage(const lv_img_dsc_t* img_dsc) {
    for (auto display : displays_) {
        if (display) display->SetPreviewImage(img_dsc);
    }
}

void DisplayManager::SetChatMessage(const char* role, const char* content) {
    for (auto display : displays_) {
        if (display) display->SetChatMessage(role, content);
    }
}

void DisplayManager::SetTheme(const std::string& theme_name) {
    for (auto display : displays_) {
        if (display) display->SetTheme(theme_name);
    }
}

void DisplayManager::SetStatus(const char* status) {
    for (auto display : displays_) {
        if (display) display->SetStatus(status);
    }
}
void DisplayManager::ShowNotification(const char* message, int duration_ms) {
    for (auto display : displays_) {
        if (display) {
            display->ShowNotification(message, duration_ms);
        }
    }
}

void DisplayManager::ShowNotification(const std::string& notification, int duration_ms) {
    for (auto display : displays_) {
        if (display) {
            display->ShowNotification(notification, duration_ms);
        }
    }
}

void DisplayManager::UpdateStatusBar(bool update_all) {
    for (auto display : displays_) {
        if (display) {
            display->UpdateStatusBar(update_all);
        }
    }
}

std::string DisplayManager::GetTheme() {
    if (primary_display_) {
        return primary_display_->GetTheme();
    }
    return displays_.empty() ? "" : displays_[0]->GetTheme();
}

bool DisplayManager::Lock(int timeout_ms) {
    return true;
}

void DisplayManager::Unlock() {
}

SpiLcdDisplayExtended::SpiLcdDisplayExtended(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                           int width, int height, int offset_x, int offset_y, bool mirror_x, bool mirror_y, bool swap_xy,
                           DisplayFonts fonts)
    : LcdDisplay(panel_io, panel, fonts, width, height) {

    // draw white
    std::vector<uint16_t> buffer(width_, 0xFFFF);
    for (int y = 0; y < height_; y++) {
        esp_lcd_panel_draw_bitmap(panel_, 0, y, width_, y + 1, buffer.data());
    }

    // Set the display to on
    ESP_LOGI(TAG, "Turning display on");
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_, true));

    //Initialize LVGL only once
    static bool lvgl_inited = false;
    if (!lvgl_inited) {
    //Initialize LVGL only once
        ESP_LOGI(TAG, "Initialize LVGL library");
        lv_init();

        ESP_LOGI(TAG, "Initialize LVGL port");
        lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
        port_cfg.task_priority = 1;
        port_cfg.timer_period_ms = 50;
        lvgl_port_init(&port_cfg);
        lvgl_inited = true;
    }
    

    ESP_LOGI(TAG, "Adding LCD display");
    const lvgl_port_display_cfg_t display_cfg = {
        .io_handle = panel_io_,
        .panel_handle = panel_,
        .control_handle = nullptr,
        .buffer_size = static_cast<uint32_t>(width_ * 20),
        .double_buffer = false,
        .trans_size = 0,
        .hres = static_cast<uint32_t>(width_),
        .vres = static_cast<uint32_t>(height_),
        .monochrome = false,
        .rotation = {
            .swap_xy = swap_xy,
            .mirror_x = mirror_x,
            .mirror_y = mirror_y,
        },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {
            .buff_dma = 1,
            .buff_spiram = 0,
            .sw_rotate = 0,
            .swap_bytes = 1,
            .full_refresh = 0,
            .direct_mode = 0,
        },
    };

    display_ = lvgl_port_add_disp(&display_cfg);
    if (display_ == nullptr) {
        ESP_LOGE(TAG, "Failed to add display");
        return;
    }

    if (offset_x != 0 || offset_y != 0) {
        lv_display_set_offset(display_, offset_x, offset_y);
    }

    lv_display_set_default(display_);

    SetupUI();
}