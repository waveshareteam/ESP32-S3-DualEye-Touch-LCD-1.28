#include "I2S_Driver.h"

i2s_keep_t *i2s_keep[I2S_MAX_KEEP];
esp_codec_dev_handle_t output_dev;
esp_codec_dev_handle_t input_dev;

esp_err_t i2s_init(uint8_t port)
{
    if (port >= I2S_MAX_KEEP) {
        return -1;
    }
    // Already installed
    if (i2s_keep[port]) {
        return 0;
    }
    i2s_keep[port] = (i2s_keep_t *) calloc(1, sizeof(i2s_keep_t));
    if (i2s_keep[port] == NULL) {
        return -1;
    }
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_MASTER_NUM, I2S_ROLE_MASTER);
    // chan_cfg.auto_clear_after_cb = true;
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &i2s_keep[port]->tx_handle, &i2s_keep[port]->rx_handle));
    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(Sample_Rate),
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg ={
            .mclk = BSP_I2S_MCLK,
            .bclk = BSP_I2S_SCLK,
            .ws = BSP_I2S_LCLK,
            .dout = BSP_I2S_DOUT,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false
            }
        },
    };
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(i2s_keep[port]->tx_handle, &std_cfg));
    i2s_tdm_slot_mask_t slot_mask = I2S_TDM_SLOT0 | I2S_TDM_SLOT1 | I2S_TDM_SLOT2 | I2S_TDM_SLOT3;
    i2s_tdm_config_t tdm_cfg = {
        .slot_cfg = I2S_TDM_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO, slot_mask),
        .clk_cfg  = I2S_TDM_CLK_DEFAULT_CONFIG(Sample_Rate),
        .gpio_cfg ={
            .mclk = BSP_I2S_MCLK,
            .bclk = BSP_I2S_SCLK,
            .ws = BSP_I2S_LCLK,
            .dout = I2S_GPIO_UNUSED,
            .din = BSP_I2S_DSIN,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false
            }
        },
    };
    tdm_cfg.slot_cfg.total_slot = 4;
    ESP_ERROR_CHECK(i2s_channel_init_tdm_mode(i2s_keep[port]->rx_handle, &tdm_cfg));

    i2s_channel_enable(i2s_keep[port]->tx_handle);
    i2s_channel_enable(i2s_keep[port]->rx_handle);

    return ESP_OK;
}

esp_err_t i2s_deinit(uint8_t port)
{
    if (port >= I2S_MAX_KEEP) {
        return -1;
    }
    // already installed
    if (i2s_keep[port] == NULL) {
        return 0;
    }
    i2s_channel_disable(i2s_keep[port]->tx_handle);
    i2s_channel_disable(i2s_keep[port]->rx_handle);
    i2s_del_channel(i2s_keep[port]->tx_handle);
    i2s_del_channel(i2s_keep[port]->rx_handle);
    free(i2s_keep[port]);
    i2s_keep[port] = NULL;
    return 0;
}
 
void I2S_Init(void){
    i2s_init(0);
    Audio_Driver_Init(0);
}
void Audio_Driver_Init(uint8_t port)
{
    audio_codec_i2s_cfg_t i2s_cfg = {
        .port = I2S_MASTER_NUM,
        .rx_handle = i2s_keep[port]->rx_handle,
        .tx_handle = i2s_keep[port]->tx_handle,
    };
    const audio_codec_data_if_t *data_if = audio_codec_new_i2s_data(&i2s_cfg);
    assert(data_if != NULL);


    // New output codec interface
    audio_codec_i2c_cfg_t i2c_cfg = {
        .port = I2C_MASTER_NUM,
        .bus_handle = i2c_handle,
        .addr = ES8311_CODEC_DEFAULT_ADDR,
    };
    const audio_codec_ctrl_if_t *out_ctrl_if = audio_codec_new_i2c_ctrl(&i2c_cfg);
    assert(out_ctrl_if != NULL);
    const audio_codec_gpio_if_t *gpio_if = audio_codec_new_gpio();
    assert(gpio_if != NULL);
    es8311_codec_cfg_t es8311_cfg = {
        .codec_mode = ESP_CODEC_DEV_WORK_MODE_DAC,
        .ctrl_if = out_ctrl_if,
        .gpio_if = gpio_if,
        .pa_pin = BOARD_PA_PIN,
        .use_mclk = true,
    };
    es8311_cfg.hw_gain.pa_voltage = 5.0;
    es8311_cfg.hw_gain.codec_dac_voltage = 3.3;
    const audio_codec_if_t *out_codec_if = es8311_codec_new(&es8311_cfg);
    assert(out_codec_if != NULL);
    esp_codec_dev_cfg_t dev_cfg = {
        .codec_if = out_codec_if,
        .data_if = data_if,
        .dev_type = ESP_CODEC_DEV_TYPE_OUT,
    };
    output_dev = esp_codec_dev_new(&dev_cfg);
    assert(output_dev != NULL);
    esp_codec_dev_sample_info_t fs = {
        .sample_rate = 16000,
        .channel = 2,
        .bits_per_sample = 16,
    };
    ESP_ERROR_CHECK(esp_codec_dev_open(output_dev, &fs));
    ESP_ERROR_CHECK(esp_codec_dev_set_out_vol(output_dev, (int)Volume));

    // New input codec interface
    i2c_cfg.addr = ES7210_CODEC_DEFAULT_ADDR;
    const audio_codec_ctrl_if_t *in_ctrl_if = audio_codec_new_i2c_ctrl(&i2c_cfg);
    assert(in_ctrl_if != NULL);
    es7210_codec_cfg_t es7210_cfg = {
        .ctrl_if = in_ctrl_if,
        .mic_selected = ES7120_SEL_MIC1 | ES7120_SEL_MIC3,
    };
    const audio_codec_if_t *in_codec_if = es7210_codec_new(&es7210_cfg);
    assert(in_codec_if != NULL);
    dev_cfg.codec_if = in_codec_if;
    dev_cfg.dev_type = ESP_CODEC_DEV_TYPE_IN;
    input_dev = esp_codec_dev_new(&dev_cfg);
    assert(input_dev != NULL);
    ESP_ERROR_CHECK(esp_codec_dev_open(input_dev, &fs));
    ESP_ERROR_CHECK(esp_codec_dev_set_in_gain(input_dev, 30.0));
}