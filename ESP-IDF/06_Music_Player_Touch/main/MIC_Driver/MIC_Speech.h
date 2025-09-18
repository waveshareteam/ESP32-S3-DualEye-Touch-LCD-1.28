#pragma once

#include "esp_afe_sr_iface.h"
#include "esp_process_sdkconfig.h"
#include "model_path.h"
#include "LVGL_Music.h"
#include "LCD_Driver.h"
#include "I2S_Driver.h"


typedef enum
{
    COMMAND_TIMEOUT = -2,
    COMMAND_NOT_DETECTED = -1,

    COMMAND_ID1 = 0,
    COMMAND_ID2 = 1,
    COMMAND_ID3 = 2,
    COMMAND_ID4 = 3,
    COMMAND_ID5 = 4,
    COMMAND_ID6 = 5,
} command_word_t;

typedef struct {
    const esp_afe_sr_iface_t *afe_handle;
    esp_afe_sr_data_t *afe_data;
    srmodel_list_t *models;
    bool detected;
    command_word_t command;
} AppSpeech;

void MIC_init();

