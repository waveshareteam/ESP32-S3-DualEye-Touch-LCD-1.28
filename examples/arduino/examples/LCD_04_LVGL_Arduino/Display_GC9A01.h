#pragma once
#include "Board_Configuration.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lcd_gc9a01.h"

// LCD SPI GPIO
// Using SPI2 
#define LCD_HOST  SPI2_HOST

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_MISO           40
#define EXAMPLE_PIN_NUM_MOSI           42
#define EXAMPLE_PIN_NUM_SCLK           41
#define EXAMPLE_PIN_NUM_LCD_CS         47
#define EXAMPLE_PIN_NUM_LCD_DC         45
#define EXAMPLE_PIN_NUM_LCD_RST        48
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT   46

#define EXAMPLE_PIN_NUM_LCD2_CS        38
#define EXAMPLE_PIN_NUM_LCD2_RST       8
#define EXAMPLE_LCD2_PIN_NUM_BK_LIGHT  39
// The pixel number in horizontal and vertical
#ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
#define EXAMPLE_LCD_WIDTH   240
#define EXAMPLE_LCD_HEIGHT  240
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
#define EXAMPLE_LCD_WIDTH   240
#define EXAMPLE_LCD_HEIGHT  240
#else
#define EXAMPLE_LCD_WIDTH   240
#define EXAMPLE_LCD_HEIGHT  240
#endif
// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

#define Offset_X 0
#define Offset_Y 0

   
extern esp_lcd_panel_handle_t panel_handle;
extern esp_lcd_panel_handle_t panel_handle2;

void LCD_Init(void);                     // Call this function to initialize the screen (must be called in the main function) !!!!!
void LCD2_Init(void);


void LCD_INIT(void);