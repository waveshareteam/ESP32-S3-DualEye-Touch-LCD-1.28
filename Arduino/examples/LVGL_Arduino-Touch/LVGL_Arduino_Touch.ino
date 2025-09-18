#include "LCD_Driver.h"
#include "Audio_ES8311.h"
#include "LVGL_Driver.h"
#include "MIC_MSM.h"
#include "SD_Card.h"
#include "LVGL_Example.h"
#include "I2S_Driver.h"

void setup()
{
  Flash_test();
  I2C_Init();
  SD_Init();
  MIC_Init();
  //Audio_Init();
  LCD_INIT();
  Lvgl_Init();

  Lvgl_Example1();
  // lv_demo_widgets();
  // lv_demo_benchmark();
  // lv_demo_keypad_encoder();
  // lv_demo_music();
  // lv_demo_printer();
  // lv_demo_stress();

  vTaskDelay(pdMS_TO_TICKS(100));
  LVGL_Start();
}
int Time_Loop=0;
void loop() {
  vTaskDelay(pdMS_TO_TICKS(5));

}
