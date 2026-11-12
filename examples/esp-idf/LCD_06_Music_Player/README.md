# LCD_06_Music_Player

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Show board information and an SD music player with microphone support.

## Hardware Required

ESP32-S3-DualEye-LCD-1.28 development board. Use a microSD card with MP3 files; connect a speaker for audio output.

## How to Run

Open this directory in an ESP-IDF terminal using the version in [product.json](../../../.github/product.json), then run:

```bash
idf.py set-target esp32s3 build flash monitor
```

## Expected Result

Show board information and an SD music player with microphone support.

## Troubleshooting

Place MP3 files in the card root for the music browser.
