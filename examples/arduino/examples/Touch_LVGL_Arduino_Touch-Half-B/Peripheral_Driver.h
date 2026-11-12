#pragma once

#include <stdint.h>
#include <string.h>  // For memcpy
#include "esp_log.h"
#include <Wire.h>
#include <SPI.h>

/********************* I2C *********************/
#define I2C_MASTER_NUM    I2C_NUM_0 
#define I2C_SCL_PIN       7
#define I2C_SDA_PIN       15


void I2C_Init(void);

bool I2C1_Read(uint8_t Driver_addr, uint8_t Reg_addr, uint8_t *Reg_data, uint32_t Length);
bool I2C1_Write(uint8_t Driver_addr, uint8_t Reg_addr, const uint8_t *Reg_data, uint32_t Length);

