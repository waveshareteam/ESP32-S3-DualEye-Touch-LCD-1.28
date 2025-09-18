#pragma once
#include "Board_Configuration.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_lcd_touch_cst816s.h"

#include "Peripheral_Driver.h"
#include "Display_GC9A01.h"


#define I2C_Touch_INT_IO            GPIO_NUM_17               /*!< GPIO number used for I2C master data  */
#define I2C_Touch_RST_IO            GPIO_NUM_16              /*!< GPIO number used for I2C master clock */

extern esp_lcd_touch_handle_t tp;

void Touch_Init(void);

