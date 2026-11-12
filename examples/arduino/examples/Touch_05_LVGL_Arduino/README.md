# Touch_05_LVGL_Arduino

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Run the LVGL board information and peripheral demonstration.

## Hardware Required

ESP32-S3-DualEye-Touch-LCD-1.28 development board. Use a microSD card with MP3 files; connect a speaker for audio output.

## How to Run

Use the Arduino-ESP32 version and FQBN in [product.json](../../../../.github/product.json), and select ESP32S3 Dev Module. Set Flash Size to 16MB, Flash Mode to QIO 120MHz, Partition Scheme to ESP SR 16M, PSRAM to OPI PSRAM, and Events Run On to Core 0; use the bundled [libraries](../../libraries/).

Open `05_LVGL_Arduino.ino` after copying the entire example directory into a folder named after the `.ino` file (without its extension), then upload.

## Expected Result

Run the LVGL board information and peripheral demonstration.

## Troubleshooting

Audio output initialization is disabled in the supplied sketch.
