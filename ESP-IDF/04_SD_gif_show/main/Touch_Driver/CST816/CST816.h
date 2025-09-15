/**
 * @file
 * @brief ESP LCD touch: CST820
 */

#pragma once
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_touch_cst816s.h"

#include "I2C_Driver.h"
#include "LCD_Driver.h"


#define I2C_Touch_INT_IO            5               /*!< GPIO number used for I2C master data  */
#define I2C_Touch_RST_IO            4              /*!< GPIO number used for I2C master clock */

extern esp_lcd_touch_handle_t tp;

void Touch_Init(void);




#define I2C_Touch2_INT_IO            7               /*!< GPIO number used for I2C master data  */
#define I2C_Touch2_RST_IO            6              /*!< GPIO number used for I2C master clock */

extern esp_lcd_touch_handle_t tp2;

void Touch2_Init(void);
