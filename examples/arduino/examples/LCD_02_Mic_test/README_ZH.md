# LCD_02_Mic_test

[English](README.md) | [简体中文](README_ZH.md)

## 功能

通过 BOOT 播放内置音频，并录制麦克风声音后回放。

## 硬件需求

ESP32-S3-DualEye-LCD-1.28 开发板。请在板载喇叭接口连接喇叭。

## 怎么运行

使用 [product.json](../../../../.github/product.json) 中的 Arduino-ESP32 版本和 FQBN，选择 ESP32S3 Dev Module。将 Flash Size 设为 16MB、Flash Mode 设为 QIO 120MHz、Partition Scheme 设为 ESP SR 16M、PSRAM 设为 OPI PSRAM、Events Run On 设为 Core 0，并使用附带的 [libraries](../../libraries/)。

打开 `02_Mic_test.ino` 前，将整个示例目录复制到与 `.ino` 文件同名（不含扩展名）的文件夹，再打开并上传。

## 预期现象

通过 BOOT 播放内置音频，并录制麦克风声音后回放。

## 常见问题

单击 BOOT 播放或暂停，长按录音，松开后回放；录音最长五秒。
