#include "I2C_Driver.h"

static const char *I2C_TAG = "I2C";

i2c_master_bus_handle_t i2c_handle = NULL;
i2c_master_dev_handle_t i2c_driver = NULL;
i2c_master_bus_handle_t i2c2_handle = NULL;
i2c_master_dev_handle_t i2c2_driver = NULL;
static esp_err_t i2c_master_init(void)
{
    const i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_SDA_IO,
        .scl_io_num = I2C_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &i2c_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(I2C_TAG, "Failed to initialize I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(I2C_TAG, "I2C bus initialized successfully");
    return ESP_OK;  // 返回 ESP_OK 表示成功
}

static esp_err_t i2c2_master_init(void)
{
    const i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C2_MASTER_NUM,
        .sda_io_num = I2C2_SDA_IO,
        .scl_io_num = I2C2_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &i2c2_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(I2C_TAG, "Failed to initialize I2C bus: %s", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(I2C_TAG, "I2C bus initialized successfully");
    return ESP_OK;  // 返回 ESP_OK 表示成功
}


void I2C_Init(void)
{
    /********************* I2C *********************/
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_ERROR_CHECK(i2c2_master_init());
}
