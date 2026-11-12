# LCD_06_Music_Player

[English](README.md) | [简体中文](README_ZH.md)

## 功能

显示板载信息，并提供 SD 音乐播放器和麦克风功能。

## 硬件需求

ESP32-S3-DualEye-LCD-1.28 开发板。音乐功能需要存有 MP3 文件的 microSD 卡，声音输出需要连接喇叭。

## 怎么运行

在 ESP-IDF 终端打开本目录，版本见 [product.json](../../../.github/product.json)，然后运行：

```bash
idf.py set-target esp32s3 build flash monitor
```

## 预期现象

显示板载信息，并提供 SD 音乐播放器和麦克风功能。

## 常见问题

音乐列表读取卡根目录中的 MP3 文件。
