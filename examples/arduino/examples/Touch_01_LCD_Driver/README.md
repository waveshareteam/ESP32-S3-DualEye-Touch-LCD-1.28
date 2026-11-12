# Touch_01_LCD_Driver

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Initialize both LCDs and display LVGL text and numbers.

## Hardware Required

ESP32-S3-DualEye-Touch-LCD-1.28 development board.

## How to Run

Use the Arduino-ESP32 version and FQBN in [product.json](../../../../.github/product.json), and select ESP32S3 Dev Module. Set Flash Size to 16MB, Flash Mode to QIO 120MHz, Partition Scheme to ESP SR 16M, PSRAM to OPI PSRAM, and Events Run On to Core 0; use the bundled [libraries](../../libraries/).

Open `01_LCD_Driver.ino` after copying the entire example directory into a folder named after the `.ino` file (without its extension), then upload.

## Expected Result

Initialize both LCDs and display LVGL text and numbers.

## Troubleshooting

None yet.
