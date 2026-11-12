# LCD_05_Clock

[English](README.md) | [简体中文](README_ZH.md)

## 功能

在两块屏幕上绘制模拟表盘和数字时间。

## 硬件需求

ESP32-S3-DualEye-LCD-1.28 开发板。

## 怎么运行

在 ESP-IDF 终端打开本目录，版本见 [product.json](../../../.github/product.json)，然后运行：

```bash
idf.py set-target esp32s3 build flash monitor
```

## 预期现象

在两块屏幕上绘制模拟表盘和数字时间。

## 常见问题

所附入口未启用 Wi-Fi 或 RTC 初始化，因此不会自动校时。
