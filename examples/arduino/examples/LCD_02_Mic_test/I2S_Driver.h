#pragma once

#include "Arduino.h"
#include "ESP_I2S.h"

#define BSP_I2S_MCLK            (GPIO_NUM_12)
#define BSP_I2S_SCLK            (GPIO_NUM_13)
#define BSP_I2S_LCLK            (GPIO_NUM_14)
#define BSP_I2S_DSIN            (GPIO_NUM_15)
#define BSP_I2S_DOUT            (GPIO_NUM_16)

#define I2S_DEFAULT_SAMPLE_RATE 16000

extern I2SClass i2s;

bool I2S_Init(uint32_t sample_rate = I2S_DEFAULT_SAMPLE_RATE);
bool I2S_SetSampleRate(uint32_t sample_rate);
uint32_t I2S_GetSampleRate(void);
