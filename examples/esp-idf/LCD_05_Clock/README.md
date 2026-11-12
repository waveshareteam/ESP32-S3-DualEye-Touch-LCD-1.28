# LCD_05_Clock

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Draw analog clock faces and digital time on both screens.

## Hardware Required

ESP32-S3-DualEye-LCD-1.28 development board.

## How to Run

Open this directory in an ESP-IDF terminal using the version in [product.json](../../../.github/product.json), then run:

```bash
idf.py set-target esp32s3 build flash monitor
```

## Expected Result

Draw analog clock faces and digital time on both screens.

## Troubleshooting

The supplied entry point does not enable Wi-Fi or RTC initialization, so it does not automatically synchronize the time.
