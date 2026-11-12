#pragma once
#include <memory>
#include <vector>
#include "display.h"
#include "display/lcd_display.h"

class DisplayManager : public Display {
private:
    static std::vector<Display*> displays_;
    static Display* primary_display_;

public:
    static void AddDisplay(Display* display);

    static void RemoveDisplay(Display* display);

    static size_t GetDisplayCount();

    static Display* GetPrimaryDisplay();

    static const std::vector<Display*>& GetAllDisplays();

    // The Display interface implementation is applied to all screens
    virtual void SetStatus(const char* status) override;
    virtual void ShowNotification(const char* message, int duration_ms) override;
    virtual void ShowNotification(const std::string& notification, int duration_ms) override;
    virtual bool SetEmotion(const char* emotion) override;
    virtual void SetChatMessage(const char* role, const char* content) override;
    virtual void SetTheme(Theme* theme) override;
    virtual Theme* GetTheme() override;
    virtual void UpdateStatusBar(bool update_all = false) override;
    virtual void SetPowerSaveMode(bool on) override;
    virtual void SetupUI() override;
    virtual bool SupportsGuiOperations() const override;
    virtual bool SetTextFont(std::shared_ptr<LvglFont> text_font) override;
    virtual bool AddTextGlyphs(const std::vector<TextGlyph>& glyphs, uint8_t bpp) override;
    virtual void ClearTextGlyphs() override;
    virtual void ClearChatMessages() override;
    virtual void RemoveChatMessage(const char* role, const char* content) override;
    virtual void SetHideSubtitle(bool hide) override;

    virtual bool Lock(int timeout_ms = 0) override;
    virtual void Unlock() override;
};
// SPI LCD display extended
class SpiLcdDisplayExtended : public LcdDisplay {
public:
    SpiLcdDisplayExtended(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                          int width, int height, int offset_x, int offset_y, bool mirror_x,
                          bool mirror_y, bool swap_xy);
    void SetupUI() override;
};
