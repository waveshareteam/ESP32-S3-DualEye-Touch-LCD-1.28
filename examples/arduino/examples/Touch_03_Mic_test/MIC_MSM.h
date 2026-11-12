#pragma once

#include "Arduino.h"
#include "esp_check.h"
#include "esp_log.h"
#include "I2C_Driver.h"
#include "I2S_Driver.h"
#include "es7210.h"
#include "es7210_reg.h"

#define ES7210_I2C_ADDR             (0x40)
#define ES7210_SAMPLE_RATE          (16000)
#define ES7210_I2S_FORMAT           ES7210_I2S_FMT_I2S
#define ES7210_MCLK_MULTIPLE        (256)
#define ES7210_BIT_WIDTH            ES7210_I2S_BITS_16B
#define ES7210_MIC_BIAS             ES7210_MIC_BIAS_2V87
#define ES7210_MIC_GAIN             ES7210_MIC_GAIN_30DB
#define ES7210_ADC_VOLUME           (10)

bool MIC_Init(uint32_t sample_rate = ES7210_SAMPLE_RATE);
bool MIC_SetSampleRate(uint32_t sample_rate);
uint32_t GetInputSampleRate(void);
