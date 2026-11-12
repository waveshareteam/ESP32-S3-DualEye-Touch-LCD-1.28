# Touch_02_Drawing_board

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Draw on the touch screens and clear the canvas with BOOT.

## Hardware Required

ESP32-S3-DualEye-Touch-LCD-1.28 development board.

## How to Run

Use the Arduino-ESP32 version and FQBN in [product.json](../../../../.github/product.json), and select ESP32S3 Dev Module. Set Flash Size to 16MB, Flash Mode to QIO 120MHz, Partition Scheme to ESP SR 16M, PSRAM to OPI PSRAM, and Events Run On to Core 0; use the bundled [libraries](../../libraries/).

Open `02_Drawing_board.ino` after copying the entire example directory into a folder named after the `.ino` file (without its extension), then upload.

## Expected Result

Draw on the touch screens and clear the canvas with BOOT.

## Troubleshooting

None yet.
