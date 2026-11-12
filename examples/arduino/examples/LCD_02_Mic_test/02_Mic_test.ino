#include <Arduino.h>
#include "Audio_Test.h"

void setup()
{
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(100));

  Audio_Test_Init();
}

void loop()
{
  Audio_Test_Loop();
  vTaskDelay(pdMS_TO_TICKS(10));
}
