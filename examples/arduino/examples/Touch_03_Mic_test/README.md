# Touch_03_Mic_test

[English](README.md) | [简体中文](README_ZH.md)

## What It Does

Use BOOT to play built-in audio and record microphone audio for playback.

## Hardware Required

ESP32-S3-DualEye-Touch-LCD-1.28 development board. Connect a speaker to the onboard speaker connector.

## How to Run

Use the Arduino-ESP32 version and FQBN in [product.json](../../../../.github/product.json), and select ESP32S3 Dev Module. Set Flash Size to 16MB, Flash Mode to QIO 120MHz, Partition Scheme to ESP SR 16M, PSRAM to OPI PSRAM, and Events Run On to Core 0; use the bundled [libraries](../../libraries/).

Open `03_Mic_test.ino` after copying the entire example directory into a folder named after the `.ino` file (without its extension), then upload.

## Expected Result

Use BOOT to play built-in audio and record microphone audio for playback.

## Troubleshooting

Click BOOT to play/pause audio; hold it to record, then release to play back. Recording is limited to five seconds.
