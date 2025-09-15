#include "Touch_CST816.h"
#include "esp_lcd_touch_cst816s.h"

static const char *TAG = "CST816";

esp_lcd_touch_handle_t tp;


void Touch_Init(void)
{
/********************* Touch *********************/
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_WIDTH,
        .y_max = EXAMPLE_LCD_HEIGHT,
        .rst_gpio_num = I2C_Touch_RST_IO,
        .int_gpio_num = I2C_Touch_INT_IO,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 0,
        },
        .user_data = &Wire,
    };
    #ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
        tp_cfg.x_max = EXAMPLE_LCD_HEIGHT,
        tp_cfg.y_max = EXAMPLE_LCD_WIDTH,
        tp_cfg.flags.swap_xy = 1;
        tp_cfg.flags.mirror_x = 1;
        tp_cfg.flags.mirror_y = 0;
    #elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE)
        tp_cfg.flags.swap_xy = 0;
        tp_cfg.flags.mirror_x = 1;
        tp_cfg.flags.mirror_y = 1;
    #elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
        tp_cfg.x_max = EXAMPLE_LCD_HEIGHT,
        tp_cfg.y_max = EXAMPLE_LCD_WIDTH,
        tp_cfg.flags.swap_xy = 1;
        tp_cfg.flags.mirror_x = 0;
        tp_cfg.flags.mirror_y = 1;
    #endif
    /* Initialize touch */
    ESP_LOGI(TAG, "Initialize touch controller CST816");
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(&Wire, &tp_cfg, &tp));
}

