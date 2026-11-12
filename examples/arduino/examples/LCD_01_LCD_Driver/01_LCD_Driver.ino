#include "LCD_Driver.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"

void setup()
{
  LCD_INIT();
  Lvgl_Init();
  //example
  Lvgl_Example1();

  vTaskDelay(pdMS_TO_TICKS(100));
  LVGL_Start();
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(5));
}
