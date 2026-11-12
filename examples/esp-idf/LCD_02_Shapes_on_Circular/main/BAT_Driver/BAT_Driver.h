#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"


/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#define EXAMPLE_ADC1_CHAN           ADC_CHANNEL_0              
#define EXAMPLE_ADC_ATTEN           ADC_ATTEN_DB_12             // ADC_ATTEN_DB_12

#define Measurement_offset 0.992357 

extern float BAT_analogVolts;

void BAT_Init(void);
float BAT_Get_Volts(void);
void GetVolts_Start(void);