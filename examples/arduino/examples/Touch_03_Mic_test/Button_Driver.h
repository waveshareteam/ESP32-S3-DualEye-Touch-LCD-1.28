#pragma once
#include <Arduino.h>   
#include "OneButton.h"

#define BOOT_KEY_PIN     0

#define Button_PIN1   BOOT_KEY_PIN  

typedef enum {
  None = 0,               // no-operation
  Click = 1,              // Click
  LongPressStart = 2,     // LongPressStart
  LongPressStop = 3,      // LongPressStop
} Status_Button;
                
extern volatile Status_Button BOOT_KEY_State;

void Button_Init(void);                                           
void ButtonTask(void *parameter);                                

// 1
void LongPressStart1(void *oneButton);                             
void LongPressStop1(void *oneButton);
void Click1(void *oneButton);                                    
