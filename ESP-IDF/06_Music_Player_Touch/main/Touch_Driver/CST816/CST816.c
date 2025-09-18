#include "CST816.h"

static const char *TAG = "CST816";

esp_lcd_touch_handle_t tp = NULL;
esp_lcd_touch_handle_t tp2 = NULL;


void Touch_Init(void)
{


/********************* Touch *********************/
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    // tp_io_config.dev_addr = ;
    ESP_LOGI(TAG, "Initialize touch IO (I2C)");
    /* Touch IO handle */
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_handle, &tp_io_config, &tp_io_handle));
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
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &tp));
}


void Touch2_Init(void)
{
/********************* Touch *********************/
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t tp_io_config = ESP_LCD_TOUCH_IO_I2C_CST816S_CONFIG();
    // tp_io_config.dev_addr = ;
    ESP_LOGI(TAG, "Initialize touch IO (I2C)");
    /* Touch IO handle */
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c2_handle, &tp_io_config, &tp_io_handle));
    esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_WIDTH,
        .y_max = EXAMPLE_LCD_HEIGHT,
        .rst_gpio_num = I2C_Touch2_RST_IO,
        .int_gpio_num = I2C_Touch2_INT_IO,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 1,
            .mirror_y = 1,
        },
    };
    #ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
        tp_cfg.x_max = EXAMPLE_LCD_HEIGHT,
        tp_cfg.y_max = EXAMPLE_LCD_WIDTH,
        tp_cfg.flags.swap_xy = 1;
        tp_cfg.flags.mirror_x = 0;
        tp_cfg.flags.mirror_y = 1;
    #elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE)
        tp_cfg.flags.swap_xy = 0;
        tp_cfg.flags.mirror_x = 0;
        tp_cfg.flags.mirror_y = 0;
    #elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
        tp_cfg.x_max = EXAMPLE_LCD_HEIGHT,
        tp_cfg.y_max = EXAMPLE_LCD_WIDTH,
        tp_cfg.flags.swap_xy = 1;
        tp_cfg.flags.mirror_x = 1;
        tp_cfg.flags.mirror_y = 0;
    #endif
    /* Initialize touch */
    ESP_LOGI(TAG, "Initialize touch controller CST816");
    ESP_ERROR_CHECK(esp_lcd_touch_new_i2c_cst816s(tp_io_handle, &tp_cfg, &tp2));
}

