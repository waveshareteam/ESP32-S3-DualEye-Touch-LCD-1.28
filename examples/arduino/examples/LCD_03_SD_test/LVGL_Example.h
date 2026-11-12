#pragma once

#include "LVGL_Driver.h"
#include "LCD_Driver.h"

// Read /test.bmp from SD card, decode it to an LVGL image buffer, and show it on both displays.
void Lvgl_BMP_Example(void);

void LVGL_Backlight_adjustment(uint8_t Backlight);
