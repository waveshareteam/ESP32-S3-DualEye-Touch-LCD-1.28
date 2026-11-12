# Touch_04_SD_test

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Read BMP images from microSD and display them on the two LCDs.

## Hardware Required

ESP32-S3-DualEye-Touch-LCD-1.28 development board. A microSD card is required.

## How to Run

Use the Arduino-ESP32 version and FQBN in [product.json](../../../../.github/product.json), and select ESP32S3 Dev Module. Set Flash Size to 16MB, Flash Mode to QIO 120MHz, Partition Scheme to ESP SR 16M, PSRAM to OPI PSRAM, and Events Run On to Core 0; use the bundled [libraries](../../libraries/).

Open `04_SD_test.ino` after copying the entire example directory into a folder named after the `.ino` file (without its extension), then upload.

## Expected Result

Read BMP images from microSD and display them on the two LCDs.

## Troubleshooting

Copy `BMP/test1.bmp` and `BMP/test2.bmp` to the card root; the sketch uses these exact names.
