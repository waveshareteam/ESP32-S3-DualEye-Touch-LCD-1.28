#include "Audio_ES8311.h"

static const char *TAG = "Audio";
uint8_t Volume = Volume_MAX - 2;
Audio audio(false, 3, I2S_NUM_0);
es8311_handle_t es_handle ;
static esp_err_t es8311_codec_init(void) {

  /* 初始化es8311芯片 */
  es_handle = es8311_create(I2C_NUM_0, ES8311_ADDRRES_0);
  ESP_RETURN_ON_FALSE(es_handle, ESP_FAIL, TAG, "es8311 create failed");
  const es8311_clock_config_t es_clk = {
    .mclk_inverted = false,
    .sclk_inverted = false,
    .mclk_from_mclk_pin = true,
    .mclk_frequency = EXAMPLE_MCLK_FREQ_HZ,
    .sample_frequency = EXAMPLE_SAMPLE_RATE
  };

  ESP_ERROR_CHECK(es8311_init(es_handle, &es_clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16));
  // ESP_RETURN_ON_ERROR(es8311_sample_frequency_config(es_handle, EXAMPLE_SAMPLE_RATE * EXAMPLE_MCLK_MULTIPLE, EXAMPLE_SAMPLE_RATE), TAG, "set es8311 sample frequency failed");
  ESP_RETURN_ON_ERROR(es8311_voice_volume_set(es_handle, Volume, NULL), TAG, "set es8311 volume failed");
  ESP_RETURN_ON_ERROR(es8311_microphone_config(es_handle, false), TAG, "set es8311 microphone failed");

  return ESP_OK;
}

void Audio_PA_EN(){
  digitalWrite(BOARD_PA_PIN, HIGH);
  vTaskDelay(pdMS_TO_TICKS(50));
}
void Audio_PA_DIS(){
  digitalWrite(BOARD_PA_PIN, LOW); 
  vTaskDelay(pdMS_TO_TICKS(50));
}
void IRAM_ATTR example_increase_audio_tick(void *arg)
{
  audio.loop();
}
void Audio_Init() {
  // Driver
  pinMode(BOARD_PA_PIN, OUTPUT);
  es8311_codec_init();
  Audio_PA_EN();
  // Audio
  audio.setPinout(BSP_I2S_SCLK, BSP_I2S_LCLK, BSP_I2S_DOUT, BSP_I2S_MCLK);
  audio.setVolume(4); // 0...21    

  esp_timer_handle_t audio_tick_timer = NULL;
  const esp_timer_create_args_t audio_tick_timer_args = {
    .callback = &example_increase_audio_tick,
    .dispatch_method = ESP_TIMER_TASK,  
    .name = "audio_tick",
    .skip_unhandled_events = true       
  };
  esp_timer_create(&audio_tick_timer_args, &audio_tick_timer);
  esp_timer_start_periodic(audio_tick_timer, EXAMPLE_Audio_TICK_PERIOD_MS * 1000);
  
}
uint32_t GetSampleRate(void){
  return audio.getSampleRate();
}


void Volume_adjustment(uint8_t Volume) {
  if(Volume > Volume_MAX )
    printf("Audio : The volume value is incorrect. Please enter 0 to 100\r\n");
  else
   es8311_voice_volume_set(es_handle, Volume, NULL); // 0...100    
}


void Play_Music_test() {
  // SD Card
  if (SD_MMC.exists("/A.mp3")) {
    printf("File 'A.mp3' found in root directory.\r\n");
  } else {
    printf("File 'A.mp3' not found in root directory.\r\n");
  }
  bool ret = audio.connecttoFS(SD_MMC,"/A.mp3");
  if(ret) 
    printf("Music Read OK\r\n");
  else
    printf("Music Read Failed\r\n");
}

void Play_Music(const char* directory, const char* fileName) {

  // SD Card
  if (!File_Search(directory,fileName) ) {
    printf("%s file not found.\r\n",fileName);
  }
  const int maxPathLength = 100; 
  char filePath[maxPathLength];
  if (strcmp(directory, "/") == 0) {                                               
    snprintf(filePath, maxPathLength, "%s%s", directory, fileName);   
  } else {                                                            
    snprintf(filePath, maxPathLength, "%s/%s", directory, fileName);
  }
  // printf("%s AAAAAAAA.\r\n",filePath);        
  audio.pauseResume();     
  bool ret = audio.connecttoFS(SD_MMC,(char*)filePath);
  if(ret) 
    printf("Music Read OK\r\n");
  else
    printf("Music Read Failed\r\n");
  Music_pause();           
  Music_resume();               
  Music_pause();     
  vTaskDelay(pdMS_TO_TICKS(100));    
}
void Music_pause() {
  if (audio.isRunning()) {            
    audio.pauseResume();             
    printf("The music pause\r\n");
  }
}
void Music_resume() {
  if (!audio.isRunning()) {           
    audio.pauseResume();             
    printf("The music begins\r\n");
  } 
}

uint32_t Music_Duration() {
  uint32_t Audio_duration = audio.getAudioFileDuration(); 
  // Audio_duration = 360;
  if(Audio_duration > 60)
    printf("Audio duration is %d minutes and %d seconds\r\n",Audio_duration/60,Audio_duration%60);
  else{
    if(Audio_duration != 0)
      printf("Audio duration is %d seconds\r\n",Audio_duration);
    else
      printf("Fail : Failed to obtain the audio duration.\r\n");
  }
  vTaskDelay(pdMS_TO_TICKS(10));
  return Audio_duration;
}
uint32_t Music_Elapsed() {
  uint32_t Audio_elapsed = audio.getAudioCurrentTime(); 
  return Audio_elapsed;
}
uint16_t Music_Energy() {
  uint16_t Audio_Energy = audio.getVUlevel(); 
  return Audio_Energy;
}

void Audio_Loop()
{
  if(!audio.isRunning())
    Play_Music_test();
}


