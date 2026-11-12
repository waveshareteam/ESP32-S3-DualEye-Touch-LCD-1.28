#include "LCD_Driver.h"
#include "I2C_Driver.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "Button_Driver.h"

void setup()
{
  I2C_Init();
  LCD_INIT();
  Lvgl_Init();
  Button_Init();
  //Touch test
  Touch_Example();

  vTaskDelay(pdMS_TO_TICKS(100));
  LVGL_Start();
}

void loop() {
  if (BOOT_KEY_State == Click) {
    BOOT_KEY_State = None;
    Lvgl_Refresh_Canvas();
  }

  vTaskDelay(pdMS_TO_TICKS(5));
}
