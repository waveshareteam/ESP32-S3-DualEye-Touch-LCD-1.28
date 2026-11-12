[English edition](./README.md)


### Arduino
在 Arduino IDE 软件下使用：
- `example` 文件夹：存放可直接编译的示例。
- `libraries` 文件夹：存放库文件（仅在 Arduino 环境下生效）。

**注意**：`esp32 by Espressif Systems` 库版本需为 3.0.0 或以上（当前示例基于 V3.2.0 编写）。


#### Arduino 示例参数设置：
![Arduino 参数配置示意图](ESP32-S3-DualEye-Touch-LCD-1.28-Arduino-Setting.png)


### ESP-IDF
在 VSCode 软件下使用：
本文件夹内的示例可直接编译。在 VSCode 中选择工程时，**请勿直接选择 ESP-IDF 根目录**，需选择 ESP-IDF 下的*工程文件夹*（通常命名为「序号_程序名称」）。

**注意**：`ESP-IDF` 版本需为 5.4.0 或以上。


### Firmware（测试固件）
通过「flash_download_tool_3.9.5」工具烧录，烧录地址为 `0x00`。烧录时请务必勾选工具界面中对应的选项框。


### 编译异常处理
若首次编译成功，但后续测试中出现编译失败：请重新解压本项目文件夹，使用全新的文件再次尝试编译。


### 产品更多信息
产品详细介绍请访问：  
[https://www.waveshare.net/shop/ESP32-S3-DualEye-LCD-1.28.htm](https://www.waveshare.net/shop/ESP32-S3-DualEye-LCD-1.28.htm)
