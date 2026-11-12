#include "GC9A01A.h"

static const char *TAG_LCD = "GC9A01";

esp_lcd_panel_handle_t panel_handle = NULL;
esp_lcd_panel_handle_t panel_handle2 = NULL;

void LCD_Init(void)
{
    ESP_LOGI(TAG_LCD, "Initialize SPI bus");                                            
    spi_bus_config_t buscfg = {                                                         
        .sclk_io_num = EXAMPLE_PIN_NUM_SCLK,                                            
        .mosi_io_num = EXAMPLE_PIN_NUM_MOSI,                                            
        .miso_io_num = EXAMPLE_PIN_NUM_MISO,                                            
        .quadwp_io_num = -1,                                                            
        .quadhd_io_num = -1,                                                            
        .max_transfer_sz = EXAMPLE_LCD_WIDTH * EXAMPLE_LCD_HEIGHT * sizeof(uint16_t),    
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));            

    ESP_LOGI(TAG_LCD, "Install panel IO");                                              
    esp_lcd_panel_io_handle_t io_handle = NULL;                                         
    esp_lcd_panel_io_spi_config_t io_config = {                                             
        .dc_gpio_num = EXAMPLE_PIN_NUM_LCD_DC,
        .cs_gpio_num = EXAMPLE_PIN_NUM_LCD_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
    };
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
    };
    ESP_LOGI(TAG_LCD, "Install GC9A01 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));

#ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE)
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, false, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, false));
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle, true));
#else
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));
#endif
    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG_LCD, "Turn on LCD backlight");
    // gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
    
}


void LCD2_Init(void){
    ESP_LOGI(TAG_LCD, "Install panel IO");                                              
    esp_lcd_panel_io_handle_t io_handle = NULL;                                         
    esp_lcd_panel_io_spi_config_t io_config = {                                             
        .dc_gpio_num = EXAMPLE_PIN_NUM_LCD_DC,
        .cs_gpio_num = EXAMPLE_PIN_NUM_LCD2_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
    };
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_PIN_NUM_LCD2_RST,
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
        .bits_per_pixel = 16,
    };
    ESP_LOGI(TAG_LCD, "Install GC9A01 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle2));

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle2));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle2));
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle2, true));

#ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle2, true, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle2, true));
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE)
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle2, true, false));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle2, false));
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle2, false, false));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle2, true));
#else
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle2, false, true));
    ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle2, false));
#endif

    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle2, true));

    ESP_LOGI(TAG_LCD, "Turn on LCD backlight");
    // gpio_set_level(EXAMPLE_PIN_NUM_BK_LIGHT, EXAMPLE_LCD_BK_LIGHT_ON_LEVEL);
}