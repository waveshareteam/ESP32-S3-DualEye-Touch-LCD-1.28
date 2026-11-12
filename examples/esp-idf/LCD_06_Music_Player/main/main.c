
#include "SD_MMC.h"
#include "LVGL_Example.h"
#include "LVGL_Eyes.h"
#include "Audio_Driver.h"
#include "MIC_Speech.h"
#include "LCD_Driver.h"
#include "I2S_Driver.h"
#include "BAT_Driver.h"
#include "Wireless.h"


void Driver_Init(void)
{
    Wireless_Init();
    Flash_Searching();
    BAT_Init();    // !!!!! Note that if GPIO1 is used for screen 3, please disable the battery voltage acquisition function
    I2C_Init();
    I2S_Init();
    button_Init();
}
void app_main(void)
{
    Driver_Init();

    SD_Init();
    LCD_INIT();
    Audio_Init();
    MIC_init();
    LVGL_Init();   // returns the screen object
// // /********************* Demo *********************/
    Lvgl_Example1();
    // Lvgl_Eyes();
    // lv_demo_widgets();
    // lv_demo_keypad_encoder();
    // lv_demo_benchmark();
    // lv_demo_stress();
    // lv_demo_music();

    vTaskDelay(pdMS_TO_TICKS(100));
    LVGL_Start();
    Simulated_Touch_Init();
    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(100));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        // lv_timer_handler();
    }
}



