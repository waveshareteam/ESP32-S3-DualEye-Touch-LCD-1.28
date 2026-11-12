# Touch_05_LVGL_Arduino

[English](README.md) | [简体中文](README_ZH.md)

## 功能

运行 LVGL 板载信息和外设演示界面。

## 硬件需求

ESP32-S3-DualEye-Touch-LCD-1.28 开发板。音乐功能需要存有 MP3 文件的 microSD 卡，声音输出需要连接喇叭。

## 怎么运行

使用 [product.json](../../../../.github/product.json) 中的 Arduino-ESP32 版本和 FQBN，选择 ESP32S3 Dev Module。将 Flash Size 设为 16MB、Flash Mode 设为 QIO 120MHz、Partition Scheme 设为 ESP SR 16M、PSRAM 设为 OPI PSRAM、Events Run On 设为 Core 0，并使用附带的 [libraries](../../libraries/)。

打开 `05_LVGL_Arduino.ino` 前，将整个示例目录复制到与 `.ino` 文件同名（不含扩展名）的文件夹，再打开并上传。

## 预期现象

运行 LVGL 板载信息和外设演示界面。

## 常见问题

所附草图未启用音频输出初始化。
