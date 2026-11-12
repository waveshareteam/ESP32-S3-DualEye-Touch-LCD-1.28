#pragma once
#include "esp_log.h" 
#include "audio_player.h" 
#include "driver/gpio.h" 
#include "driver/i2s_std.h"
#include "driver/i2s_tdm.h"

#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"


#include "I2C_Driver.h"
#include "Audio_Driver.h"
#define I2S_MASTER_NUM          I2S_NUM_0
#define I2S_MAX_KEEP            SOC_I2S_NUM

#define BOARD_PA_PIN            (GPIO_NUM_9) 
#define BSP_I2S_MCLK            (GPIO_NUM_12) 
#define BSP_I2S_SCLK            (GPIO_NUM_13) 
#define BSP_I2S_LCLK            (GPIO_NUM_14) 
#define BSP_I2S_DSIN            (GPIO_NUM_15) 
#define BSP_I2S_DOUT            (GPIO_NUM_16) 

#define Sample_Rate             16000

typedef struct {
    i2s_chan_handle_t tx_handle;
    i2s_chan_handle_t rx_handle;
} i2s_keep_t;


extern i2s_keep_t *i2s_keep[I2S_MAX_KEEP];
extern esp_codec_dev_handle_t output_dev;
extern esp_codec_dev_handle_t input_dev;

esp_err_t i2s_init(uint8_t port);
esp_err_t i2s_deinit(uint8_t port);

void I2S_Init(void);
void Audio_Driver_Init(uint8_t port);