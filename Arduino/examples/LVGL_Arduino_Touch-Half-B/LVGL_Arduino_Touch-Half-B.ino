#include "SD_Card.h"
#include "Display_GC9A01.h"
#include "Touch_CST816.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "Peripheral_Driver.h"
void setup()
{       
  Flash_test();
  I2C_Init();
  SD_Init();     
  LCD_Init();
  Backlight_Init();
  Touch_Init();

  Lvgl_Init();
  Lvgl_Example1();     
  // lv_demo_widgets();               
  // lv_demo_benchmark();          
  // lv_demo_keypad_encoder();     
  // lv_demo_music();  
  // lv_demo_stress();   

  vTaskDelay(pdMS_TO_TICKS(100));
  LVGL_Start();
  Wireless_Test2();  
}

void loop()
{
  // Timer_Loop();
  delay(5);
}
