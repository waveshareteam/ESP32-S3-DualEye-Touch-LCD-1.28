#include "MIC_MSM.h"

static const char *TAG = "MIC";
static es7210_dev_handle_t es7210_handle = NULL;
static uint32_t mic_sample_rate = 0;

static bool es7210_codec_init(uint32_t sample_rate)
{
  es7210_i2c_config_t es7210_i2c_conf = {
    .i2c_port = I2C_NUM_0,
    .i2c_addr = ES7210_I2C_ADDR
  };

  ESP_ERROR_CHECK(es7210_new_codec(&es7210_i2c_conf, &es7210_handle));

  es7210_codec_config_t codec_conf = {
    .sample_rate_hz = sample_rate,
    .mclk_ratio = ES7210_MCLK_MULTIPLE,
    .i2s_format = ES7210_I2S_FORMAT,
    .bit_width = ES7210_BIT_WIDTH,
    .mic_bias = ES7210_MIC_BIAS,
    .mic_gain = ES7210_MIC_GAIN,
  };

  codec_conf.flags.tdm_enable = true;
  ESP_ERROR_CHECK(es7210_config_codec(es7210_handle, &codec_conf));
  ESP_ERROR_CHECK(es7210_config_volume(es7210_handle, ES7210_ADC_VOLUME));

  mic_sample_rate = sample_rate;
  ESP_LOGI(TAG, "ES7210 initialized at %lu Hz", (unsigned long)sample_rate);
  return true;
}

bool MIC_Init(uint32_t sample_rate)
{
  if (es7210_handle != NULL) {
    return MIC_SetSampleRate(sample_rate);
  }

  return es7210_codec_init(sample_rate);
}

bool MIC_SetSampleRate(uint32_t sample_rate)
{
  if (mic_sample_rate == sample_rate) {
    return true;
  }

  if (es7210_handle == NULL) {
    return MIC_Init(sample_rate);
  }

  es7210_codec_config_t codec_conf = {
    .sample_rate_hz = sample_rate,
    .mclk_ratio = ES7210_MCLK_MULTIPLE,
    .i2s_format = ES7210_I2S_FORMAT,
    .bit_width = ES7210_BIT_WIDTH,
    .mic_bias = ES7210_MIC_BIAS,
    .mic_gain = ES7210_MIC_GAIN,
  };

  codec_conf.flags.tdm_enable = true;
  esp_err_t err = es7210_config_codec(es7210_handle, &codec_conf);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "set ES7210 sample rate %lu failed: %s",
             (unsigned long)sample_rate, esp_err_to_name(err));
    return false;
  }

  mic_sample_rate = sample_rate;
  return true;
}

uint32_t GetInputSampleRate(void)
{
  return mic_sample_rate;
}
