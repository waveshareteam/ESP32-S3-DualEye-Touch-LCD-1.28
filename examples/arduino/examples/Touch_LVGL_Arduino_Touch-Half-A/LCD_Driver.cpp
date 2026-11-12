#include "LCD_Driver.h"

void LCD_INIT() {  
  LCD_Init();
  Backlight1_Init();
  TOUCH_INIT();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Backlight program


uint8_t LCD_Backlight = 90;
// backlight
void Backlight1_Init()
{
  ledcAttach(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, Frequency, Resolution);   
  ledcWrite(EXAMPLE_LCD_PIN_NUM_BK_LIGHT, Dutyfactor);  
  Set_Backlight1(LCD_Backlight);      //0~100                 
}

void Set_Backlight1(uint8_t Light)                     
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
void Set_Backlight(uint8_t Light){
    Set_Backlight1(Light);      //0~100   
}