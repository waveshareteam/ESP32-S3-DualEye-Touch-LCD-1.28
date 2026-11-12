# Touch_04_SD_test

[English](README.md) | [简体中文](README_ZH.md)

## 功能

读取 microSD 卡中的 BMP 图片并在双 LCD 上显示。

## 硬件需求

ESP32-S3-DualEye-Touch-LCD-1.28 开发板。需要 microSD 卡。

## 怎么运行

使用 [product.json](../../../../.github/product.json) 中的 Arduino-ESP32 版本和 FQBN，选择 ESP32S3 Dev Module。将 Flash Size 设为 16MB、Flash Mode 设为 QIO 120MHz、Partition Scheme 设为 ESP SR 16M、PSRAM 设为 OPI PSRAM、Events Run On 设为 Core 0，并使用附带的 [libraries](../../libraries/)。

打开 `04_SD_test.ino` 前，将整个示例目录复制到与 `.ino` 文件同名（不含扩展名）的文件夹，再打开并上传。

## 预期现象

读取 microSD 卡中的 BMP 图片并在双 LCD 上显示。

## 常见问题

将 `BMP/test1.bmp` 和 `BMP/test2.bmp` 复制到卡根目录；草图使用这两个固定文件名。
