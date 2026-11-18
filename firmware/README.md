# Firmware

[English](README.md) | [简体中文](README_ZH.md)

Firmware for ESP32-S3-DualEye-Touch-LCD-1.28. Each BIN is a merged image flashed at `0x0`; the date or upstream version in its file name identifies the version.

| File | Purpose |
| --- | --- |
| [ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin](ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin) | Factory firmware, restores the music player demo |
| [ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin](ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin) | Xiaozhi AI firmware, version 2.5.4 |

Run these commands in this directory to restore the factory firmware, replacing `<PORT>` with the board's serial port. Erasing flash removes existing settings and user data.

```bash
python -m pip install esptool
esptool.py --chip esp32s3 --port <PORT> erase_flash
esptool.py --chip esp32s3 --port <PORT> write_flash 0x0 ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin
```

To install Xiaozhi AI, follow the same steps and replace the BIN file name in the final command with `ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin`.
