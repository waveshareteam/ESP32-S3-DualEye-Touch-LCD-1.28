#pragma once

#include "Arduino.h"
#include "esp_check.h"
#include "esp_log.h"
#include "es8311.h"
#include "I2C_Driver.h"
#include "I2S_Driver.h"

#define BOARD_PA_PIN            (GPIO_NUM_9)
#define AUDIO_DEFAULT_RATE      16000
#define AUDIO_MCLK_MULTIPLE     256
#define Volume_MAX              100

extern uint8_t Volume;

bool Audio_Init(uint32_t sample_rate = AUDIO_DEFAULT_RATE);
bool Audio_SetSampleRate(uint32_t sample_rate);
void Audio_PA_EN(void);
void Audio_PA_DIS(void);
void Volume_adjustment(uint8_t volume);
