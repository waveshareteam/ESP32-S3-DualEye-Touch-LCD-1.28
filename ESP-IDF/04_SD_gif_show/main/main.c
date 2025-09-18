
#include "SD_MMC.h"
#include "LVGL_Eyes.h"
#include "LCD_Driver.h"
#include "BAT_Driver.h"
#include "Wireless.h"
#include "iot_button.h"
#include "button_gpio.h"

#define BOOT_BUTTON_NUM       0

button_handle_t boot_btn;

void Driver_Init(void)
{
    //Wireless_Init();
    Flash_Searching();
    BAT_Init();    // !!!!! Note that if GPIO1 is used for screen 3, please disable the battery voltage acquisition function
    I2C_Init();
}
static void direct_button_cb(void *arg, void *data)
{
    eyes_next_gif();
}

void button_init(void)
{
    button_config_t btn_cfg = {0};
    button_gpio_config_t gpio_cfg = {
        .gpio_num = BOOT_BUTTON_NUM,
        .active_level = 0,
        .enable_power_save = true,
    };
    
    esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &boot_btn);
    assert(ret == ESP_OK);
    
     ret = iot_button_register_cb(boot_btn, BUTTON_PRESS_DOWN, NULL, direct_button_cb, NULL);
    assert(ret == ESP_OK);
}

void app_main(void)
{
    Driver_Init();
    button_init();
    SD_Init();
    LCD_INIT();
    LVGL_Init();   // returns the screen object
// // /********************* Demo *********************/

    Lvgl_Eyes();


    vTaskDelay(pdMS_TO_TICKS(100));
    LVGL_Start();
    while (1) {
        // raise the task priority of LVGL and/or reduce the handler period can improve the performance
        vTaskDelay(pdMS_TO_TICKS(100));
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        // lv_timer_handler();
    }
}



