#pragma once
#include "Arduino.h"

#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "esp_flash.h"
 
#include "Display_GC9A01.h"
#include "Peripheral_Driver.h"

#define PIN_NUM_MISO    EXAMPLE_PIN_NUM_MISO   
#define PIN_NUM_MOSI    EXAMPLE_PIN_NUM_MOSI    
#define PIN_NUM_SCLK    EXAMPLE_PIN_NUM_SCLK    
#define PIN_NUM_CS      GPIO_NUM_38            

esp_err_t s_example_write_file(const char *path, char *data);
esp_err_t s_example_read_file(const char *path);

extern uint32_t SDCard_Size;
extern uint32_t Flash_Size;
void SD_Init(void);
void Flash_test(void);