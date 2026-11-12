#pragma once
#include "esp_check.h"
#include "esp_log.h"
#include "Arduino.h"
#include "Audio.h"
#include "SD_Card.h"
#include "MIC_MSM.h"
#include "es8311.h"
#include "LVGL_Music.h"

#define BOARD_PA_PIN            (GPIO_NUM_9) 
#define EXAMPLE_SAMPLE_RATE     (16000)
#define EXAMPLE_MCLK_MULTIPLE   (256) // If not using 24-bit data width, 256 should be enough
#define EXAMPLE_MCLK_FREQ_HZ    (EXAMPLE_SAMPLE_RATE * EXAMPLE_MCLK_MULTIPLE)
#define EXAMPLE_Audio_TICK_PERIOD_MS  20
#define Volume_MAX  100

extern Audio audio;
extern uint8_t Volume;

void Play_Music_test();
void Audio_Loop();

void Audio_Init();
uint32_t GetSampleRate(void);
void Volume_adjustment(uint8_t Volume);
void Play_Music(const char* directory, const char* fileName);
void Music_pause(); 
void Music_resume();    
uint32_t Music_Duration();  
uint32_t Music_Elapsed();   
uint16_t Music_Energy();    
