#include "Display_GC9A01.h"
   
static const char *TAG_LCD = "GC9A01";

esp_lcd_panel_handle_t panel_handle = NULL;

void LCD_Init(void)
{
    ESP_LOGI(TAG_LCD, "Install panel IO");                                              
    esp_lcd_panel_io_handle_t io_handle = NULL;                                         
    esp_lcd_panel_io_spi_config_t io_config = {     
        .cs_gpio_num = EXAMPLE_PIN_NUM_LCD_CS,                                        
        .dc_gpio_num = EXAMPLE_PIN_NUM_LCD_DC,
        .spi_mode = 0,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ,
        .trans_queue_depth = 10,
        .on_color_trans_done = NULL,
        .user_ctx = NULL,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
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
    
}


uint8_t LCD_Backlight = 90;
// backlight
void Backlight_Init()
{
  ledcAttach(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, Frequency, Resolution);   
  ledcWrite(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, Dutyfactor);  
  Set_Backlight(LCD_Backlight);      //0~100                 
}

void Set_Backlight(uint8_t Light)                     
{
  if(Light > Backlight_MAX || Light < 0)
    printf("Set Backlight parameters in the range of 0 to 100 \r\n");
  else{
    uint32_t Backlight = Light*10;
    if(Backlight == 1000)
      Backlight = 1024;
    ledcWrite(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, Backlight);
  }
}