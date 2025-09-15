#pragma once
#include "display.h"
#include "display/lcd_display.h"
#include <vector>
#include <memory>

class DisplayManager : public Display {
private:
    static std::vector<Display*> displays_;
    static Display* primary_display_;

public:
    static void AddDisplay(Display* display, bool is_primary = false);
    
    static void RemoveDisplay(Display* display);
    
    static size_t GetDisplayCount();
    
    static Display* GetPrimaryDisplay();
    
    static const std::vector<Display*>& GetAllDisplays();
    
    //The Display interface implementation is applied to all screens
    virtual void SetEmotion(const char* emotion) override;
    virtual void SetIcon(const char* icon) override;
    virtual void SetPreviewImage(const lv_img_dsc_t* img_dsc) override;
    virtual void SetChatMessage(const char* role, const char* content) override;
    virtual void SetTheme(const std::string& theme_name) override;
    virtual void SetStatus(const char* status) override;
    
    virtual void ShowNotification(const char* message, int duration_ms) override;
    virtual void ShowNotification(const std::string& notification, int duration_ms) override;
    virtual void UpdateStatusBar(bool update_all = false) override;
    virtual std::string GetTheme() override;
    
    virtual bool Lock(int timeout_ms = 0) override;
    virtual void Unlock() override;
};
//SPI LCD display extended
class SpiLcdDisplayExtended : public LcdDisplay {
public:
    SpiLcdDisplayExtended(esp_lcd_panel_io_handle_t panel_io, 
                         esp_lcd_panel_handle_t panel,
                         int width, int height, 
                         int offset_x, int offset_y,
                         bool mirror_x, bool mirror_y, 
                         bool swap_xy,
                         DisplayFonts fonts); 

};