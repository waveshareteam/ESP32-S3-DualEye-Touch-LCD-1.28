#pragma once
#include "esp_log.h" 
#include "esp_check.h" 
#include "unity.h" 
#include "audio_player.h" 
#include "driver/gpio.h" 
#include "freertos/semphr.h" 

#include "esp_idf_version.h"
#include "driver/i2s_std.h"
#include "soc/soc_caps.h"
#include "esp_codec_dev.h"
#include "esp_codec_dev_defaults.h"


#include "SD_MMC.h"
#include "I2C_Driver.h"
#include "I2S_Driver.h"
#include "LVGL_Music.h"

#define CONFIG_BSP_I2S_NUM    0
#define I2S_MAX_KEEP          SOC_I2S_NUM

#define BSP_I2S_SCLK          (GPIO_NUM_13) 
#define BSP_I2S_MCLK          (GPIO_NUM_12) 
#define BSP_I2S_LCLK          (GPIO_NUM_14) 
#define BSP_I2S_DSIN          (GPIO_NUM_15) 
#define BSP_I2S_DOUT          (GPIO_NUM_16) 
#define BOARD_PA_PIN          (GPIO_NUM_9) 

#define Audio_sample_rate 16000

#define BSP_I2S_GPIO_CFG       \
    {                          \
        .mclk = BSP_I2S_MCLK,  \
        .bclk = BSP_I2S_SCLK,  \
        .ws = BSP_I2S_LCLK,    \
        .dout = BSP_I2S_DOUT,  \
        .din = BSP_I2S_DSIN,   \
        .invert_flags = {      \
            .mclk_inv = false, \
            .bclk_inv = false, \
            .ws_inv = false,   \
        },                     \
    }
    
#define Volume_MAX  100


 
extern esp_codec_dev_handle_t input_dev;
extern bool Music_Next_Flag;
extern uint8_t Volume;
void Audio_Init(void);
void Play_Music(const char* directory, const char* fileName);
void Music_resume(void);
void Music_pause(void);

void Volume_adjustment(uint8_t Volume);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#include "esp_log.h" 
#include "esp_check.h" 
#include "unity.h" 
#include "audio_player.h" 
#include "driver/gpio.h" 
#include "freertos/semphr.h" 

#include "SD_MMC.h"

#define CONFIG_BSP_I2S_NUM 0


#define BSP_I2S_SCLK          (GPIO_NUM_13) 
#define BSP_I2S_MCLK          (GPIO_NUM_12) 
#define BSP_I2S_LCLK          (GPIO_NUM_14) 
#define BSP_I2S_DOUT          (GPIO_NUM_15) 
#define BSP_I2S_DSIN          (GPIO_NUM_14) 

#define BSP_I2S_GPIO_CFG       \
    {                          \
        .mclk = BSP_I2S_MCLK,  \
        .bclk = BSP_I2S_SCLK,  \
        .ws = BSP_I2S_LCLK,    \
        .dout = BSP_I2S_DOUT,  \
        .din = BSP_I2S_DSIN,   \
        .invert_flags = {      \
            .mclk_inv = false, \
            .bclk_inv = false, \
            .ws_inv = false,   \
        },                     \
    }


#define BSP_I2S_DUPLEX_MONO_CFG(_sample_rate)                                                         \
    {                                                                                                 \
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(_sample_rate),                                          \
        .slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO), \
        .gpio_cfg = BSP_I2S_GPIO_CFG,                                                                 \
    }

#define Volume_MAX  100
extern bool Music_Next_Flag;
extern uint8_t Volume;
void Audio_Init(void);
void Play_Music(const char* directory, const char* fileName);
void Music_resume(void);
void Music_pause(void);

uint32_t Music_Duration(void);
uint32_t Music_Elapsed(void);
uint16_t Music_Energy(void);
void Volume_adjustment(uint8_t Volume);
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#include "esp_log.h" 
#include "esp_check.h" 
#include "unity.h" 
#include "audio_player.h" 
#include "driver/gpio.h" 
#include "freertos/semphr.h" 


#include "esp32_s3_audio_board.h" // 引入开发板配置
#include "esp_peripherals.h" // 引入ESP外设支持

#include "audio_mem.h" // 引入音频内存管理
#include "esp_ssdp.h" // 引入SSDP库

#include "esp_audio.h" // 引入ESP音频库
#include "esp_decoder.h" // 引入音频解码器库
#include "i2s_stream.h" // 引入I2S流库
#include "fatfs_stream.h"
#include "media_lib_adapter.h" // 引入媒体库适配器
#include "audio_idf_version.h" // 引入音频版本信息
#include "SD_MMC.h"
#include "I2C_Driver.h"
#define CONFIG_BSP_I2S_NUM 0


#define BSP_I2S_SCLK          (GPIO_NUM_48) 
#define BSP_I2S_MCLK          (GPIO_NUM_NC) 
#define BSP_I2S_LCLK          (GPIO_NUM_38) 
#define BSP_I2S_DOUT          (GPIO_NUM_47) 
#define BSP_I2S_DSIN          (GPIO_NUM_NC) 

#define BSP_I2S_GPIO_CFG       \
    {                          \
        .mclk = BSP_I2S_MCLK,  \
        .bclk = BSP_I2S_SCLK,  \
        .ws = BSP_I2S_LCLK,    \
        .dout = BSP_I2S_DOUT,  \
        .din = BSP_I2S_DSIN,   \
        .invert_flags = {      \
            .mclk_inv = false, \
            .bclk_inv = false, \
            .ws_inv = false,   \
        },                     \
    }


#define BSP_I2S_DUPLEX_MONO_CFG(_sample_rate)                                                         \
    {                                                                                                 \
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(_sample_rate),                                          \
        .slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO), \
        .gpio_cfg = BSP_I2S_GPIO_CFG,                                                                 \
    }

#define Volume_MAX  100
extern bool Music_Next_Flag;
extern uint8_t Volume;
void Audio_Init(void);
void Play_Music(const char* directory, const char* fileName);
void Music_resume(void);
void Music_pause(void);

uint32_t Music_Duration(void);
uint32_t Music_Elapsed(void);
uint16_t Music_Energy(void);
void Volume_adjustment(int Volume);
*/