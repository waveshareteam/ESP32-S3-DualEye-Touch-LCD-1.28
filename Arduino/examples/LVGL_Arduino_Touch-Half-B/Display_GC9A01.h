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
#include "driver/ledc.h"

#define Frequency       20000   // PWM frequencyconst        
#define Resolution      10       // PWM resolution ratio     MAX:13
#define Dutyfactor      500     // PWM Dutyfactor      
#define Backlight_MAX   100

// LCD SPI GPIO
// Using SPI2 
#define LCD_HOST  SPI2_HOST

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (80 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_MISO           GPIO_NUM_42
#define EXAMPLE_PIN_NUM_MOSI           GPIO_NUM_2
#define EXAMPLE_PIN_NUM_SCLK           GPIO_NUM_1
#define EXAMPLE_PIN_NUM_LCD_DC         GPIO_NUM_41
#define EXAMPLE_PIN_NUM_LCD_CS         GPIO_NUM_39
#define EXAMPLE_PIN_NUM_LCD_RST        GPIO_NUM_40
#define EXAMPLE_LCD_PIN_NUM_BK_LIGHT   GPIO_NUM_6

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

   
extern uint8_t LCD_Backlight;
extern esp_lcd_panel_handle_t panel_handle;

void LCD_Init(void);                     // Call this function to initialize the screen (must be called in the main function) !!!!!

void Backlight_Init(void);                             // Initialize the LCD backlight, which has been called in the LCD_Init function, ignore it      
void Set_Backlight(uint8_t Light);                   // Call this function to adjust the brightness of the backlight. The value of the parameter Light ranges from 0 to 100