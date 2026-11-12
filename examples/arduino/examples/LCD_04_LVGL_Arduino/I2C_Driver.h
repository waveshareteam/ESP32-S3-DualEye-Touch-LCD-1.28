#pragma once

#include <stdint.h>
#include <string.h>  // For memcpy
#include "esp_log.h"
#include <Wire.h>

/********************* I2C *********************/
#define I2C_MASTER_NUM    I2C_NUM_0 
#define I2C_SCL_PIN       10
#define I2C_SDA_PIN       11


#define I2C2_MASTER_NUM   I2C_NUM_1  /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C2_SCL_PIN      2         /*!< GPIO number used for I2C master clock */
#define I2C2_SDA_PIN      3         /*!< GPIO number used for I2C master data  */

void I2C_Init(void);

bool I2C1_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length);
bool I2C1_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length);

bool I2C2_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length);
bool I2C2_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length);