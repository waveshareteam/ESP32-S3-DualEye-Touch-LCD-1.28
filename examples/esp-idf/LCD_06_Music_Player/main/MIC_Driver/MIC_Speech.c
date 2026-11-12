#include "MIC_Speech.h"

#include "driver/gpio.h"
#include "driver/i2s_std.h"
#include "driver/i2s_tdm.h"
#include "soc/soc_caps.h"
#include "esp_err.h"
#include "esp_log.h"

#include "esp_wn_iface.h"
#include "esp_wn_models.h"
#include "esp_afe_sr_models.h"
#include "esp_mn_iface.h"
#include "esp_mn_models.h"

#define I2S_CHANNEL_NUM 1

static const char *TAG = "App/Speech";

static i2s_chan_handle_t rx_handle = NULL;        // I2S rx channel handler
static AppSpeech MIC_Speech;
bool play_Music_Flag = 0;
uint8_t LCD_Backlight_original = 0;


// static void feed_handler(AppSpeech *self)
// {
//     esp_afe_sr_data_t *afe_data = self->afe_data;
//     int audio_chunksize = self->afe_handle->get_feed_chunksize(afe_data);
//     int nch = self->afe_handle->get_channel_num(afe_data);
//     (void)nch;
//     size_t samp_len = audio_chunksize;
//     size_t samp_len_bytes = samp_len * I2S_CHANNEL_NUM * sizeof(uint8_t);
//     uint8_t *i2s_buff = (uint8_t *)malloc(samp_len_bytes);
//     assert(i2s_buff);

//     while (true)
//     {
//         esp_err_t ret = esp_codec_dev_read(input_dev, i2s_buff, samp_len_bytes);
//         if (ret != ESP_OK) {
//             printf("EEEEEERRROOOERR\n");
//             break;
//         }
//         // for (int i = 0; i < samp_len; ++i)
//         // {
//         //     i2s_buff[i] = i2s_buff[i] >> 14; // 放大语音信号，右移14位
//         // }
//         self->afe_handle->feed(afe_data, (int16_t *)i2s_buff);
//     }
//     self->afe_handle->destroy(afe_data);
//     if (i2s_buff) {
//         free(i2s_buff);
//         i2s_buff = NULL;
//     }
//     vTaskDelete(NULL);
// }


static void feed_handler(AppSpeech *self)
{
    esp_afe_sr_data_t *afe_data = self->afe_data;
    int audio_chunksize = self->afe_handle->get_feed_chunksize(afe_data);
    int nch = self->afe_handle->get_channel_num(afe_data);
    (void)nch;
    size_t samp_len = audio_chunksize;
    size_t samp_len_bytes = samp_len * I2S_CHANNEL_NUM * sizeof(int32_t);
    int32_t *i2s_buff = (int32_t *)malloc(samp_len_bytes);
    assert(i2s_buff);
    size_t bytes_read;

    while (true)
    {
        i2s_channel_read(rx_handle, i2s_buff, samp_len_bytes, &bytes_read, portMAX_DELAY);

        // for (int i = 0; i < samp_len; ++i)
        // {
        //     i2s_buff[i] = i2s_buff[i] >> 14; // 32:8 is the significant bit, 8:0 is the low 8 bits, all 0, the AFE input is 16 bits of voice data, the 29:13 bit is to amplify the voice signal.
        // }
        // FatfsComboWrite(i2s_buff, audio_chunksize * I2S_CHANNEL_NUM * sizeof(int16_t), 1, fp);

        self->afe_handle->feed(afe_data, (int16_t *)i2s_buff);
    }
    self->afe_handle->destroy(afe_data);
    if (i2s_buff) {
        free(i2s_buff);
        i2s_buff = NULL;
    }
    vTaskDelete(NULL);
}

static void detect_hander(AppSpeech *self)
{
    esp_afe_sr_data_t *afe_data = self->afe_data;
    int afe_chunksize = self->afe_handle->get_fetch_chunksize(afe_data);
#if defined(CONFIG_SR_MN_CN_MULTINET5_RECOGNITION_QUANT8) || defined(CONFIG_SR_MN_CN_MULTINET6_QUANT) || defined(CONFIG_SR_MN_CN_MULTINET6_AC_QUANT)
    char *mn_name = esp_srmodel_filter(self->models, ESP_MN_PREFIX, ESP_MN_CHINESE);
#else
    char *mn_name = esp_srmodel_filter(self->models, ESP_MN_PREFIX, ESP_MN_ENGLISH);
#endif // CONFIG_IDF_TARGET_ESP32S3
    ESP_LOGI(TAG, "multinet:%s\n", mn_name);
    esp_mn_iface_t *multinet = esp_mn_handle_from_name(mn_name);
    model_iface_data_t *model_data = multinet->create(mn_name, 6000);
    esp_mn_commands_update_from_sdkconfig(multinet, model_data); // Add speech commands from sdkconfig
    int mu_chunksize = multinet->get_samp_chunksize(model_data);
    assert(mu_chunksize == afe_chunksize);


    //print active speech commands
    multinet->print_active_speech_commands(model_data);
    ESP_LOGI(TAG, "Ready");

    self->detected = false;

    while (true)
    {
        afe_fetch_result_t* res = self->afe_handle->fetch(afe_data); 
        if (!res || res->ret_value == ESP_FAIL) {
            ESP_LOGE(TAG, "fetch error!\n");
            break;
        }

        if (res->wakeup_state == WAKENET_DETECTED) {
            ESP_LOGI(TAG, "WAKEWORD DETECTED\n");
	        multinet->clean(model_data);  // clean all status of multinet
            LCD_Backlight_original = LCD_Backlight;
        } else if (res->wakeup_state == WAKENET_CHANNEL_VERIFIED) {
            ESP_LOGI(TAG, "AFE_FETCH_CHANNEL_VERIFIED, channel index: %d\n", res->trigger_channel_id);
            ESP_LOGI(TAG, ">>> Say your command <<<");
            self->detected = true;
            self->afe_handle->disable_wakenet(afe_data);
            LCD_Backlight = 35;
            
        }

        if (self->detected) {
            esp_mn_state_t mn_state = multinet->detect(model_data, res->data);

            if (mn_state == ESP_MN_STATE_DETECTING) {
                self->command = COMMAND_NOT_DETECTED;
                continue;
            } else if (mn_state == ESP_MN_STATE_DETECTED) {
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                // for (int i = 0; i < mn_result->num; i++) {
                //     ESP_LOGI(TAG, "TOP %d, command_id: %d, phrase_id: %d, string:%s prob: %f\n", 
                //     i+1, mn_result->command_id[i], mn_result->phrase_id[i], mn_result->string, mn_result->prob[i]);
                // }
                ESP_LOGI(TAG, "TOP %d, command_id: %d, phrase_id: %d, string:%s prob: %f\n", 
                1, mn_result->command_id[0], mn_result->phrase_id[0], mn_result->string, mn_result->prob[0]);
                switch (mn_result->command_id[0]) {
                    case 0:      
                        LCD_Backlight = 100;  
                        break;
                    case 1:     
                        LCD_Backlight = 30;    
                        break;
                    case 2:     
                        LCD_Backlight = 0;  
                        break;
                    case 3:       
                        LCD_Backlight = 100;   
                        break;
                    case 4:                 
                        play_Music_Flag = 1;              
                        break;
                    default: printf("Unknown Command!\r\n"); break;
                }
                self->command = (command_word_t)mn_result->command_id[0];
                // self->afe_handle->enable_wakenet(afe_data);
                // self->detected = false;
                
                self->afe_handle->disable_wakenet(afe_data);
                self->detected = true;
                ESP_LOGI(TAG, ">>> Say your command <<<");
                self->command = COMMAND_TIMEOUT;
            } else if (mn_state == ESP_MN_STATE_TIMEOUT) {
                esp_mn_results_t *mn_result = multinet->get_results(model_data);
                ESP_LOGI(TAG, "timeout, string:%s\n", mn_result->string);
                self->command = COMMAND_TIMEOUT;
                self->afe_handle->enable_wakenet(afe_data);
                self->detected = false;
                ESP_LOGI(TAG, ">>> Waiting to be waken up <<<");
                LCD_Backlight = LCD_Backlight_original;
                if(play_Music_Flag){
                    play_Music_Flag = 0;
                    if(ACTIVE_TRACK_CNT)
                    _lv_demo_music_resume();   
                    else
                    printf("No MP3 file found in SD card!\r\n");    
                }
            }
        }
    }
    if (model_data) {
        multinet->destroy(model_data);
        model_data = NULL;
    }
    self->afe_handle->destroy(afe_data);
    vTaskDelete(NULL);
}


void MIC_init() 
{
   
    MIC_Speech.afe_handle = &ESP_AFE_SR_HANDLE;

    MIC_Speech.detected = false;
    MIC_Speech.command = COMMAND_TIMEOUT;
    MIC_Speech.models = esp_srmodel_init("model");
    rx_handle = i2s_keep[0]->rx_handle;
    // i2s_channel_enable(rx_handle);
    // sd_card_mount("/sdcard");
    afe_config_t afe_config = {
        .aec_init = true,
        .se_init = true,
        .vad_init = true,
        .wakenet_init = true,
        .voice_communication_init = false,
        .voice_communication_agc_init = false,
        .voice_communication_agc_gain = 15,
        .vad_mode = VAD_MODE_3,
        .wakenet_model_name = NULL,
        .wakenet_model_name_2 = NULL,
        .wakenet_mode = DET_MODE_2CH_90,
        .afe_mode = SR_MODE_LOW_COST,
        .afe_perferred_core = 0,
        .afe_perferred_priority = 5,
        .afe_ringbuf_size = 50,
        .memory_alloc_mode = AFE_MEMORY_ALLOC_MORE_PSRAM,
        .afe_linear_gain = 1.0,
        .agc_mode = AFE_MN_PEAK_AGC_MODE_2,
        .pcm_config = {
            .total_ch_num = 3,
            .mic_num = 2,
            .ref_num = 1,
            .sample_rate = 16000,
        },
        .debug_init = false,
        .debug_hook = {{AFE_DEBUG_HOOK_MASE_TASK_IN, NULL}, {AFE_DEBUG_HOOK_FETCH_TASK_IN, NULL}},
    };
    afe_config.aec_init = false;
    afe_config.se_init = false;
    afe_config.vad_init = false;
    afe_config.afe_ringbuf_size = 50;
    afe_config.pcm_config.total_ch_num = 2;
    afe_config.pcm_config.mic_num = 1;
    afe_config.pcm_config.ref_num = 1;
    afe_config.pcm_config.sample_rate = 16000;
    afe_config.wakenet_model_name = esp_srmodel_filter(MIC_Speech.models, ESP_WN_PREFIX, NULL);
    MIC_Speech.afe_data = MIC_Speech.afe_handle->create_from_config(&afe_config);
    xTaskCreatePinnedToCore((TaskFunction_t)feed_handler, "App/SR/Feed", 4 * 1024, &MIC_Speech, 5, NULL, 0);
    xTaskCreatePinnedToCore((TaskFunction_t)detect_hander, "App/SR/Detect", 5 * 1024, &MIC_Speech, 5, NULL, 0);
}
