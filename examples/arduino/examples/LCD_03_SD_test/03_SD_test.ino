#include "LCD_Driver.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"

void setup()
{
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(100));

  LCD_INIT();
  Lvgl_Init();

  Lvgl_BMP_Example();

  vTaskDelay(pdMS_TO_TICKS(100));
  LVGL_Start();
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(5));
}
