#pragma once

#include <Arduino.h>
#include <FS.h>

bool SD_Card_Init(void);
fs::File SD_Card_Open(const char * path, const char * mode);
