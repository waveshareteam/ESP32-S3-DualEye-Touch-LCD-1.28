#pragma once

#include <stdint.h>
#include <string.h>  // For memcpy
#include "esp_log.h"
#include "driver/gpio.h"

#include "driver/i2c_master.h"

/********************* I2C *********************/
#define I2C_SCL_IO                  10         /*!< GPIO number used for I2C master clock */
#define I2C_SDA_IO                  11         /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              I2C_NUM_0  /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000    /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0         /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0         /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000


#define I2C2_MASTER_NUM             I2C_NUM_1  /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C2_SCL_IO                 2         /*!< GPIO number used for I2C master clock */
#define I2C2_SDA_IO                 3         /*!< GPIO number used for I2C master data  */
extern i2c_master_bus_handle_t i2c_handle;
extern i2c_master_bus_handle_t i2c2_handle;
void I2C_Init(void);