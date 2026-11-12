#include "Audio_Driver.h"
/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

static const char *TAG = "AUDIO"; 
uint8_t Volume = Volume_MAX - 20;
bool Music_Next_Flag = 0;

static esp_err_t bsp_i2s_write(void *audio_buffer, size_t len, size_t *bytes_written, uint32_t timeout_ms) {
    uint8_t *samples = (uint8_t *)audio_buffer;
    // size_t sample_count = len / sizeof(uint8_t);
    // 使用 esp_codec_dev_write 写入音频数据
    // 假设 output_dev 是音频设备的句柄， sample_count 是每次写入的字节数
    // esp_err_t ret = esp_codec_dev_write(output_dev, samples, sample_count);
    esp_err_t ret = esp_codec_dev_write(output_dev, samples, len);
    
    // 如果需要更新已写入的字节数
    if (bytes_written) {
        // *bytes_written = sample_count;  // 假设每次写入 sample_count 字节
        *bytes_written = len;  // 假设每次写入 sample_count 字节
    }

    return ret;
}

static esp_err_t bsp_i2s_reconfig_clk(uint32_t rate, uint32_t bits_cfg, i2s_slot_mode_t ch) {                                   // I2S Init
    esp_err_t ret = ESP_OK; 
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(rate),
        .slot_cfg = I2S_STD_PHILIP_SLOT_DEFAULT_CONFIG((i2s_data_bit_width_t)bits_cfg, ch),
        .gpio_cfg ={
            .mclk = BSP_I2S_MCLK,
            .bclk = BSP_I2S_SCLK,
            .ws = BSP_I2S_LCLK,
            .dout = BSP_I2S_DOUT,
            .din = I2S_GPIO_UNUSED,
        },
    };
    ret |= i2s_channel_disable(i2s_keep[0]->tx_handle);
    ret |= i2s_channel_reconfig_std_clock(i2s_keep[0]->tx_handle, &std_cfg.clk_cfg);
    ret |= i2s_channel_reconfig_std_slot(i2s_keep[0]->tx_handle, &std_cfg.slot_cfg);
    ret |= i2s_channel_enable(i2s_keep[0]->tx_handle); 
    return ret; 
}

static esp_err_t audio_mute_function(AUDIO_PLAYER_MUTE_SETTING setting) {                                                       // audio mute function
    ESP_LOGI(TAG, "mute setting %d", setting); 
    return ESP_OK; 
}


static FILE * Music_File = NULL;
static audio_player_callback_event_t expected_event; 
static QueueHandle_t event_queue; 
static audio_player_callback_event_t event; 

static void audio_player_callback(audio_player_cb_ctx_t *ctx) {
    if (ctx->audio_event == AUDIO_PLAYER_CALLBACK_EVENT_IDLE) {
        ESP_LOGI(TAG, "Playback finished");
        Music_Next_Flag = 1;
    }
    if (ctx->audio_event == expected_event) {
        xQueueSend(event_queue, &(ctx->audio_event), 0);
    }
}



void Audio_Init(void) 
{
    audio_player_config_t config = { 
        .mute_fn = audio_mute_function,
        .write_fn = bsp_i2s_write,
        .clk_set_fn = bsp_i2s_reconfig_clk,
        .priority = 3,
        .coreID = 1 
    };
    esp_err_t ret = audio_player_new(config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create audio player: %s", esp_err_to_name(ret));
        return;
    }
    event_queue = xQueueCreate(1, sizeof(audio_player_callback_event_t));
    if (!event_queue) {
        ESP_LOGE(TAG, "Failed to create event queue");
        return;
    }
    ret = audio_player_callback_register(audio_player_callback, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to register callback: %s", esp_err_to_name(ret));
        return;
    }
    if (audio_player_get_state() != AUDIO_PLAYER_STATE_IDLE) {
        ESP_LOGE(TAG, "Expected state to be IDLE");                 // The player is not idle
        return;
    }
    esp_codec_dev_set_out_mute(output_dev,false);
    
}
void Play_Music(const char* directory, const char* fileName)
{  
    Music_pause();
    const int maxPathLength = 100; 
    char filePath[maxPathLength];
    if (strcmp(directory, "/") == 0) {                                               
        snprintf(filePath, maxPathLength, "%s%s", directory, fileName);   
    } else {                                                            
        snprintf(filePath, maxPathLength, "%s/%s", directory, fileName);
    }
    Music_File = Open_File(filePath);
    if (!Music_File) {
        ESP_LOGE(TAG, "Failed to open MP3 file: %s", filePath);
        return;
    }

    expected_event = AUDIO_PLAYER_CALLBACK_EVENT_PLAYING;
    esp_err_t ret = audio_player_play(Music_File);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to play audio: %s", esp_err_to_name(ret));
        fclose(Music_File);
        return;
    }
    if (xQueueReceive(event_queue, &event, pdMS_TO_TICKS(100)) != pdPASS) {
        ESP_LOGE(TAG, "Failed to receive playing event");
        fclose(Music_File);
        return;
    }
    if (audio_player_get_state() != AUDIO_PLAYER_STATE_PLAYING) {
        ESP_LOGE(TAG, "Expected state to be PLAYING");
        fclose(Music_File);
        return;
    }
    esp_codec_dev_set_out_mute(output_dev,false);
}
void Music_resume(void)
{
    if(audio_player_get_state() != AUDIO_PLAYER_STATE_PAUSE){
        LVGL_Play_Music(0);    
        ESP_LOGE(TAG, "Enable the audio player for the first time!!!");
    }
    if (audio_player_get_state() != AUDIO_PLAYER_STATE_PLAYING){
        expected_event = AUDIO_PLAYER_CALLBACK_EVENT_PLAYING;
        esp_err_t ret = audio_player_resume();
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to resume audio: %s", esp_err_to_name(ret));
            fclose(Music_File);
            return;
        }
        if (xQueueReceive(event_queue, &event, pdMS_TO_TICKS(100)) != pdPASS) {
            ESP_LOGE(TAG, "Failed to receive playing event after resume");
            fclose(Music_File);
            return;
        }
        if (audio_player_get_state() != AUDIO_PLAYER_STATE_PLAYING) {
            ESP_LOGE(TAG, "Expected state to be RESUME");
            fclose(Music_File);
            return;
        }
    }
    esp_codec_dev_set_out_mute(output_dev,false);
}
void Music_pause(void) 
{
    if (audio_player_get_state() == AUDIO_PLAYER_STATE_PLAYING){
        expected_event = AUDIO_PLAYER_CALLBACK_EVENT_PAUSE;
        esp_err_t ret = audio_player_pause();
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to pause audio: %s", esp_err_to_name(ret));
            fclose(Music_File);
            return;
        }
        if (xQueueReceive(event_queue, &event, pdMS_TO_TICKS(100)) != pdPASS) {
            ESP_LOGE(TAG, "Failed to receive pause event");
            fclose(Music_File);
            return;
        }
        if (audio_player_get_state() != AUDIO_PLAYER_STATE_PAUSE) {
            ESP_LOGE(TAG, "Expected state to be PAUSE");
            fclose(Music_File);
            return;
        }
    }
    esp_codec_dev_set_out_mute(output_dev,true);
}


void Volume_adjustment(uint8_t Vol) {
    if(Vol > Volume_MAX )
        printf("Audio : The volume value is incorrect. Please enter 0 to 100\r\n");
    else  
        Volume = Vol;
    esp_err_t ret = esp_codec_dev_set_out_vol(output_dev, Volume);
    if(ret != ESP_OK)
        ESP_LOGE("Audio", "esp_codec_dev_set_out_vol - The return value is not ESP_OK");
    ESP_LOGI(TAG, "Volume set to %d", Volume);
}
 