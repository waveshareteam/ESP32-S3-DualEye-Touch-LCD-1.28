#include "Audio_ES8311.h"

static const char *TAG = "Audio";

uint8_t Volume = Volume_MAX - 2;
static es8311_handle_t es_handle = NULL;
static uint32_t audio_sample_rate = 0;

static esp_err_t es8311_codec_init(uint32_t sample_rate)
{
  es_handle = es8311_create(I2C_NUM_0, ES8311_ADDRRES_0);
  ESP_RETURN_ON_FALSE(es_handle, ESP_FAIL, TAG, "es8311 create failed");

  const es8311_clock_config_t es_clk = {
    .mclk_inverted = false,
    .sclk_inverted = false,
    .mclk_from_mclk_pin = true,
    .mclk_frequency = sample_rate * AUDIO_MCLK_MULTIPLE,
    .sample_frequency = sample_rate
  };

  ESP_RETURN_ON_ERROR(es8311_init(es_handle, &es_clk, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16),
                      TAG, "es8311 init failed");
  ESP_RETURN_ON_ERROR(es8311_voice_volume_set(es_handle, Volume, NULL),
                      TAG, "set es8311 volume failed");
  ESP_RETURN_ON_ERROR(es8311_microphone_config(es_handle, false),
                      TAG, "set es8311 microphone failed");

  audio_sample_rate = sample_rate;
  return ESP_OK;
}

void Audio_PA_EN(void)
{
  digitalWrite(BOARD_PA_PIN, HIGH);
  vTaskDelay(pdMS_TO_TICKS(50));
}

void Audio_PA_DIS(void)
{
  digitalWrite(BOARD_PA_PIN, LOW);
  vTaskDelay(pdMS_TO_TICKS(50));
}

bool Audio_Init(uint32_t sample_rate)
{
  pinMode(BOARD_PA_PIN, OUTPUT);
  Audio_PA_DIS();

  if (es8311_codec_init(sample_rate) != ESP_OK) {
    return false;
  }

  Audio_PA_EN();
  return true;
}

bool Audio_SetSampleRate(uint32_t sample_rate)
{
  if (audio_sample_rate == sample_rate) {
    return I2S_SetSampleRate(sample_rate);
  }

  if (es_handle != NULL) {
    esp_err_t err = es8311_sample_frequency_config(es_handle,
                                                   sample_rate * AUDIO_MCLK_MULTIPLE,
                                                   sample_rate);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "set es8311 sample rate %lu failed: %s",
               (unsigned long)sample_rate, esp_err_to_name(err));
      return false;
    }
  }

  audio_sample_rate = sample_rate;
  return I2S_SetSampleRate(sample_rate);
}

void Volume_adjustment(uint8_t volume)
{
  if (volume > Volume_MAX) {
    Serial.println("Audio volume must be 0..100");
    return;
  }

  Volume = volume;
  if (es_handle != NULL) {
    es8311_voice_volume_set(es_handle, Volume, NULL);
  }
}
