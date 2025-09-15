#pragma once
#include "Board_Configuration.h"

#include "Display_GC9A01.h"

#include "driver/ledc.h"

#define Frequency       20000   // PWM frequencyconst        
#define Resolution      10       // PWM resolution ratio     MAX:13
#define Dutyfactor      500     // PWM Dutyfactor      
#define Backlight_MAX   100

extern uint8_t LCD_Backlight;
void Backlight1_Init(void);                             // Initialize the LCD backlight, which has been called in the LCD_Init function, ignore it      
void Backlight2_Init(void);                             // Initialize the LCD backlight, which has been called in the LCD_Init function, ignore it 
void Set_Backlight1(uint8_t Light);      
void Set_Backlight2(uint8_t Light);     

void LCD_INIT();

                                                      
void Set_Backlight(uint8_t Light);                   // Call this function to adjust the brightness of the backlight. The value of the parameter Light ranges from 0 to 100
