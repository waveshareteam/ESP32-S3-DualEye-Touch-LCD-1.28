# LCD_04_SD_gif_show

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Load GIF animations from the microSD card and switch them with BOOT.

## Hardware Required

ESP32-S3-DualEye-LCD-1.28 development board. A microSD card is required.

## How to Run

Open this directory in an ESP-IDF terminal using the version in [product.json](../../../.github/product.json), then run:

```bash
idf.py set-target esp32s3 build flash monitor
```

## Expected Result

Load GIF animations from the microSD card and switch them with BOOT.

## Troubleshooting

Copy the supplied `C1.gif` and `C2.gif` to the card root before starting.
