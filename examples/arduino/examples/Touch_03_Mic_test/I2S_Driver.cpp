#include "I2S_Driver.h"

I2SClass i2s;

static uint32_t i2s_sample_rate = 0;

bool I2S_Init(uint32_t sample_rate)
{
  i2s.setPins(BSP_I2S_SCLK, BSP_I2S_LCLK, BSP_I2S_DOUT, BSP_I2S_DSIN, BSP_I2S_MCLK);
  i2s.setTimeout(100);

  if (!i2s.begin(I2S_MODE_STD, sample_rate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO)) {
    Serial.println("I2S init failed");
    return false;
  }

  i2s_sample_rate = sample_rate;
  return true;
}

bool I2S_SetSampleRate(uint32_t sample_rate)
{
  if (i2s_sample_rate == sample_rate) {
    return true;
  }

  i2s.configureTX(sample_rate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
  i2s.configureRX(sample_rate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);
  i2s_sample_rate = sample_rate;
  return true;
}

uint32_t I2S_GetSampleRate(void)
{
  return i2s_sample_rate;
}
