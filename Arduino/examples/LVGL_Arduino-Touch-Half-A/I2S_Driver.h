#pragma once

#include "ESP_I2S.h"
#include "esp32-hal-sr.h"
#include "Audio_ES8311.h"
#include "MIC_MSM.h"

#define I2S_MAX_KEEP            SOC_I2S_NUM
#define I2S_MASTER_NUM          I2S_NUM_0

#define BSP_I2S_MCLK            (GPIO_NUM_12)
#define BSP_I2S_SCLK            (GPIO_NUM_13)  
#define BSP_I2S_LCLK            (GPIO_NUM_14) 
#define BSP_I2S_DSIN            (GPIO_NUM_15) 
#define BSP_I2S_DOUT            (GPIO_NUM_16) 

#define Sample_Rate             16000

typedef void (*sr_cb)(sr_event_t event, int command_id, int phrase_id);

class ESP_SR_Class_My {
private:
  sr_cb cb;
  I2SClass *i2s;

public:
  ESP_SR_Class_My();
  ~ESP_SR_Class_My();

  void onEvent_My(sr_cb cb);
  bool begin_My(I2SClass &i2s, const sr_cmd_t *sr_commands, size_t sr_commands_len, sr_channels_t rx_chan = SR_CHANNELS_STEREO, sr_mode_t mode = SR_MODE_WAKEWORD);
  bool end_My(void);
  bool setMode_My(sr_mode_t mode);
  bool pause_My(void);
  bool resume_My(void);

  void _sr_event_My(sr_event_t event, int command_id, int phrase_id);
  esp_err_t _fill_My(void *out, size_t len, size_t *bytes_read, uint32_t timeout_ms);
};

extern ESP_SR_Class_My ESP_SR_My;