#include "LCD_Driver.h"

void LCD_INIT() {  
  LCD_Init();
  LCD2_Init();
  TOUCH_INIT();
  Backlight1_Init();
  Backlight2_Init();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Backlight program

uint8_t LCD_Backlight = 70;
static ledc_channel_config_t ledc_channel;
void Backlight1_Init(void)
{
    ESP_LOGI("LCD_Driver", "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_LCD_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 5000,
        .speed_mode = LEDC_LS_MODE,
        .timer_num = LEDC_HS_TIMER,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel.channel    = LEDC_HS_CH0_CHANNEL;
    ledc_channel.duty       = 0;
    ledc_channel.gpio_num   = EXAMPLE_LCD_PIN_NUM_BK_LIGHT;
    ledc_channel.speed_mode = LEDC_LS_MODE;
    ledc_channel.timer_sel  = LEDC_HS_TIMER;
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0);
    
    Set_Backlight1(LCD_Backlight);      //0~100    
}
void Set_Backlight1(uint8_t Light)
{   
    if(Light > Backlight_MAX) Light = Backlight_MAX;
    uint16_t Duty = LEDC_MAX_Duty-(81*(Backlight_MAX-Light));
    if(Light == 0) 
        Duty = 0;
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, Duty);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
}
// end Backlight program
static ledc_channel_config_t ledc_channel2;
void Backlight2_Init(void)
{
    ESP_LOGI("LCD_Driver", "Turn on LCD backlight");
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_LCD2_PIN_NUM_BK_LIGHT
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 5000,
        .speed_mode = LEDC_LS_MODE,
        .timer_num = LEDC_HS_TIMER,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel2.channel    = LEDC_CHANNEL_1;
    ledc_channel2.duty       = 0;
    ledc_channel2.gpio_num   = EXAMPLE_LCD2_PIN_NUM_BK_LIGHT;
    ledc_channel2.speed_mode = LEDC_LS_MODE;
    ledc_channel2.timer_sel  = LEDC_HS_TIMER;
    ledc_channel_config(&ledc_channel2);
    
    Set_Backlight2(LCD_Backlight);      //0~100    
}
void Set_Backlight2(uint8_t Light)
{   
    if(Light > Backlight_MAX) Light = Backlight_MAX;
    uint16_t Duty = LEDC_MAX_Duty-(81*(Backlight_MAX-Light));
    if(Light == 0) 
        Duty = 0;
    ledc_set_duty(ledc_channel2.speed_mode, ledc_channel2.channel, Duty);
    ledc_update_duty(ledc_channel2.speed_mode, ledc_channel2.channel);
}
void Set_Backlight(uint8_t Light){
    Set_Backlight1(Light);      //0~100   
    Set_Backlight2(Light);      //0~100    
}