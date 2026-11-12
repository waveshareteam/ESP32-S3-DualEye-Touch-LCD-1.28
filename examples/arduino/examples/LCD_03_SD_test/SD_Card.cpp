#include "SD_Card.h"

#include <SD_MMC.h>

#define SD_MMC_CLK_PIN 17
#define SD_MMC_CMD_PIN 21
#define SD_MMC_D0_PIN  18
#define SD_MMC_D1_PIN  -1
#define SD_MMC_D2_PIN  -1
#define SD_MMC_D3_PIN  -1

static bool sd_card_mounted = false;

bool SD_Card_Init(void)
{
  if (sd_card_mounted) {
    return true;
  }

  if (!SD_MMC.setPins(SD_MMC_CLK_PIN, SD_MMC_CMD_PIN, SD_MMC_D0_PIN,
                      SD_MMC_D1_PIN, SD_MMC_D2_PIN, SD_MMC_D3_PIN)) {
    Serial.println("SD_MMC setPins failed");
    return false;
  }

  if (!SD_MMC.begin("/sdcard", true, false)) {
    Serial.println("SD_MMC mount failed");
    return false;
  }

  uint8_t card_type = SD_MMC.cardType();
  if (card_type == CARD_NONE) {
    Serial.println("No SD card attached");
    SD_MMC.end();
    return false;
  }

  sd_card_mounted = true;
  Serial.printf("SD card mounted, size: %llu MB\r\n", SD_MMC.cardSize() / (1024ULL * 1024ULL));
  return true;
}

fs::File SD_Card_Open(const char * path, const char * mode)
{
  if (!SD_Card_Init()) {
    return fs::File();
  }

  return SD_MMC.open(path, mode);
}
