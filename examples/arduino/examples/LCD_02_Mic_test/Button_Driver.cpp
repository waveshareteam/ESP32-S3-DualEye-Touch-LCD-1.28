#include "Button_Driver.h"

OneButton button1(Button_PIN1, true);                   
volatile Status_Button BOOT_KEY_State = None;


void Button_Init() {   
  button1.attachLongPressStart(LongPressStart1, &button1);      
  button1.attachLongPressStop(LongPressStop1, &button1);
  button1.setLongPressIntervalMs(1000);                        
  button1.attachClick(Click1, &button1);                        

  xTaskCreatePinnedToCore(                                     
    ButtonTask,                                            
    "ButtonTask",                                            
    4096,                                                      
    NULL,                   
    3,                                                          
    NULL,                 
    1                                                           
  );  
}

void ButtonTask(void *parameter) {                             
  while(1){
    button1.tick();                                             
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}


void LongPressStart1(void *oneButton){   
  BOOT_KEY_State = LongPressStart;                             
  printf("BOOT LongPressStart\r\n");
}
void LongPressStop1(void *oneButton){
  BOOT_KEY_State = LongPressStop;
  printf("BOOT LongPressStop\r\n");
}
void Click1(void *oneButton){      
  BOOT_KEY_State = Click;                                                            
  printf("BOOT Click\r\n");
}
