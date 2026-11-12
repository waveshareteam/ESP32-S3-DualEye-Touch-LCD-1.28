# LCD_04_SD_gif_show

[English](README.md) | [简体中文](README_ZH.md)

## 功能

从 microSD 卡读取 GIF 动画，使用 BOOT 按键切换。

## 硬件需求

ESP32-S3-DualEye-LCD-1.28 开发板。需要 microSD 卡。

## 怎么运行

在 ESP-IDF 终端打开本目录，版本见 [product.json](../../../.github/product.json)，然后运行：

```bash
idf.py set-target esp32s3 build flash monitor
```

## 预期现象

从 microSD 卡读取 GIF 动画，使用 BOOT 按键切换。

## 常见问题

启动前将附带的 `C1.gif` 和 `C2.gif` 复制到卡根目录。
