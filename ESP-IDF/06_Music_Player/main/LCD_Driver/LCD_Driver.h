#pragma once
#include "sdkconfig.h"
#include "GC9A01A.h"

#include "driver/ledc.h"

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_HS_CH0_GPIO       EXAMPLE_LCD_PIN_NUM_BK_LIGHT
#define LEDC_HS_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_TEST_DUTY         (4000)
#define LEDC_ResolutionRatio   LEDC_TIMER_13_BIT
#define LEDC_MAX_Duty          ((1 << LEDC_ResolutionRatio) - 1)
#define Backlight_MAX           100    

extern uint8_t LCD_Backlight;
void LCD_INIT();

                       
void Set_Backlight(uint8_t Light);                   // Call this function to adjust the brightness of the backlight. The value of the parameter Light ranges from 0 to 100
void Backlight1_Init(void);                             // Initialize the LCD backlight, which has been called in the LCD_Init function, ignore it                                  
void Set_Backlight1(uint8_t Light);
void Backlight2_Init(void);
void Set_Backlight2(uint8_t Light);