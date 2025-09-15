
#include "Text_Number.h"
#include "LCD_Driver.h"
#include "BAT_Driver.h"
#include "Wireless.h"

void Driver_Init(void)
{
    //Wireless_Init();
    //Flash_Searching();
    BAT_Init();    // !!!!! Note that if GPIO1 is used for screen 3, please disable the battery voltage acquisition function
    I2C_Init();
    //I2S_Init();

}
void app_main(void)
{
    Driver_Init();
    LCD_INIT();
    LVGL_Init();   // returns the screen object
// // /********************* Demo *********************/

    Text_Number();

    vTaskDelay(pdMS_TO_TICKS(100));
    LVGL_Start();
    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(100));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        // lv_timer_handler();
    }
}



