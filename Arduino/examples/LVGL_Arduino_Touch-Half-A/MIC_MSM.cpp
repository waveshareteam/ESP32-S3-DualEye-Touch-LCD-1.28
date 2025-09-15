#include "MIC_MSM.h"
#include "LVGL_Music.h"
// English wakeword : Hi ESP！！！！
// Chinese wakeword : Hi 乐鑫！！！！
// 英文唤醒词 : Hi ESP！！！！
// 中文唤醒词 : Hi 乐鑫！！！！


I2SClass i2s;

// Generated using the following command:
// python3 tools/gen_sr_commands.py "Turn on the light,Switch on the light;Turn off the light,Switch off the light,Go dark;Start fan;Stop fan"
enum {
  SR_CMD_TURN_ON_THE_BACKLIGHT,
  SR_CMD_TURN_OFF_THE_BACKLIGHT,
  SR_CMD_BACKLIGHT_IS_BRIGHTEST,
  SR_CMD_BACKLIGHT_IS_DARKEST,
  SR_CMD_PLAY_MUSIC,
};
static const sr_cmd_t sr_commands[] = {
  {0, "Turn on the backlight", "TkN nN jc BaKLiT"},                 // English
  {1, "Turn off the backlight", "TkN eF jc BaKLiT"},                // English
  {2, "backlight is brightest", "BaKLiT gZ BRiTcST"},               // English
  {3, "backlight is darkest", "BaKLiT gZ DnRKcST"},                 // English
  {4, "play music", "PLd MYoZgK"},                                  // English

  // {0, "bei guang zui liang", "bei guang zui liang"},            // chinese
  // {1, "bei guang zui an", "bei guang zui an"},                  // chinese
  // {2, "guan bi bei guang", "guan bi bei guang"},                // chinese
  // {3, "da kai bei guang", "da kai bei guang"},                  // chinese
  // {4, "bo fang yin yue", "bo fang yin yue"},                    // chinese
};
bool play_Music_Flag = 0;
uint8_t LCD_Backlight_original = 0;
void Awaken_Event(sr_event_t event, int command_id, int phrase_id) {
  switch (event) {
    case SR_EVENT_WAKEWORD: 
      if(ACTIVE_TRACK_CNT)
        _lv_demo_music_pause();
      printf("WakeWord Detected!\r\n"); 
      LCD_Backlight_original = LCD_Backlight;
      break;
    case SR_EVENT_WAKEWORD_CHANNEL:
      printf("WakeWord Channel %d Verified!\r\n", command_id);
      ESP_SR_My.setMode_My(SR_MODE_COMMAND);  // Switch to Command detection
      LCD_Backlight = 35;
      break;
    case SR_EVENT_TIMEOUT:
      printf("Timeout Detected!\r\n");
      ESP_SR_My.setMode_My(SR_MODE_WAKEWORD);  // Switch back to WakeWord detection
      LCD_Backlight = LCD_Backlight_original;
      if(play_Music_Flag){
        play_Music_Flag = 0;
        if(ACTIVE_TRACK_CNT)
          _lv_demo_music_resume();   
        else
          printf("No MP3 file found in SD card!\r\n");    
      }
      break;
    case SR_EVENT_COMMAND:
      printf("Command %d Detected! %s\r\n", command_id, sr_commands[phrase_id].str);
      switch (command_id) {
        case SR_CMD_TURN_ON_THE_BACKLIGHT:      
          LCD_Backlight = 100;  
          break;
        case SR_CMD_TURN_OFF_THE_BACKLIGHT:     
          LCD_Backlight = 0;    
          break;
        case SR_CMD_BACKLIGHT_IS_BRIGHTEST:     
          LCD_Backlight = 100;  
          break;
        case SR_CMD_BACKLIGHT_IS_DARKEST:       
          LCD_Backlight = 30;   
          break;
        case SR_CMD_PLAY_MUSIC:                 
          play_Music_Flag = 1;              
          break;
        default:                        printf("Unknown Command!\r\n"); break;
      }
      ESP_SR_My.setMode_My(SR_MODE_COMMAND);  // Allow for more commands to be given, before timeout
      // ESP_SR_My.setMode_My(SR_MODE_WAKEWORD); // Switch back to WakeWord detection
      break;
    default: printf("Unknown Event!\r\n"); break;
  }
}
static es7210_dev_handle_t es7210_handle = NULL;

static void es7210_codec_init(bool is_tdm)
{
    /* Create ES7210 device handle */
    es7210_i2c_config_t es7210_i2c_conf = {
        .i2c_port = I2C_NUM_0,
        .i2c_addr = ES7210_I2C_ADDR
    };
    ESP_ERROR_CHECK(es7210_new_codec(&es7210_i2c_conf, &es7210_handle));
    ESP_LOGI(TAG, "Configure ES7210 codec parameters");
    es7210_codec_config_t codec_conf = {
        .sample_rate_hz = ES7210_SAMPLE_RATE,
        .mclk_ratio = ES7210_MCLK_MULTIPLE,
        .i2s_format = ES7210_I2S_FORMAT,
        .bit_width = ES7210_BIT_WIDTH,
        .mic_bias = ES7210_MIC_BIAS,
        .mic_gain = ES7210_MIC_GAIN,
    };
    
    codec_conf.flags.tdm_enable = is_tdm;
    ESP_ERROR_CHECK(es7210_config_codec(es7210_handle, &codec_conf));
    ESP_ERROR_CHECK(es7210_config_volume(es7210_handle, ES7210_ADC_VOLUME));
}
void _MIC_Init() {
  i2s.setPins(BSP_I2S_SCLK, BSP_I2S_LCLK, BSP_I2S_DOUT, BSP_I2S_DSIN, BSP_I2S_MCLK);
  i2s.setTimeout(1000);
  i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);

  es7210_codec_init(true);
  ESP_SR_My.onEvent_My(Awaken_Event);
  ESP_SR_My.begin_My(i2s, sr_commands, sizeof(sr_commands) / sizeof(sr_cmd_t), SR_CHANNELS_STEREO, SR_MODE_WAKEWORD);
}

uint32_t GetInputSampleRate(void){
  return i2s.rxSampleRate();
}

uint32_t Old_SampleRate=16000,Now_SampleRate = 16000;
void MICTask(void *parameter) {
  _MIC_Init();
  esp_task_wdt_add(NULL);
  Old_SampleRate = GetSampleRate();
  while(1){
    esp_task_wdt_reset();
    Now_SampleRate = GetSampleRate();  
    if(Old_SampleRate != Now_SampleRate){
      printf("Update ES7210 Rate Change from %ld to %ld\r\n",Old_SampleRate, Now_SampleRate);
      Old_SampleRate = Now_SampleRate;
      es7210_codec_config_t codec_conf = {
        .sample_rate_hz = Now_SampleRate,
        .mclk_ratio = ES7210_MCLK_MULTIPLE,
        .i2s_format = ES7210_I2S_FORMAT,
        .bit_width = ES7210_BIT_WIDTH,
        .mic_bias = ES7210_MIC_BIAS,
        .mic_gain = ES7210_MIC_GAIN,
      };
      codec_conf.flags.tdm_enable = true;
      ESP_ERROR_CHECK(es7210_config_codec(es7210_handle, &codec_conf));
      i2s.configureTX(Now_SampleRate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
      i2s.configureRX(Now_SampleRate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
      printf("IN Rate: %ld  ,OUT Rate: %ld  \r\n",i2s.rxSampleRate(), i2s.txSampleRate());
    }
    vTaskDelay(pdMS_TO_TICKS(50));
  }
  vTaskDelete(NULL);
  
}
void MIC_Init() {
  xTaskCreatePinnedToCore(
    MICTask,     
    "MICTask",  
    4096,                
    NULL,                 
    5,                   
    NULL,                 
    0                     
  );
}

