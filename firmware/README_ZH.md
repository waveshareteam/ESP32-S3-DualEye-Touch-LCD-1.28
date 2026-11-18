# 固件

[English](README.md) | [简体中文](README_ZH.md)

ESP32-S3-DualEye-Touch-LCD-1.28 的固件。每个 BIN 都是从 `0x0` 烧录的合并镜像，文件名中的日期或上游版本号即版本。

| 文件 | 用途 |
| --- | --- |
| [ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin](ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin) | 出厂固件，恢复音乐播放器演示 |
| [ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin](ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin) | 小智 AI 固件，版本 2.5.4 |

在本目录运行以下命令恢复出厂固件，将 `<PORT>` 替换为开发板对应的串口。整片擦除会清除已有设置和用户数据。

```bash
python -m pip install esptool
esptool.py --chip esp32s3 --port <PORT> erase_flash
esptool.py --chip esp32s3 --port <PORT> write_flash 0x0 ESP32-S3-DualEye-Touch-LCD-1.28_FactoryOnly_20250923.bin
```

烧录小智 AI 固件时，按相同步骤操作，将最后一条命令中的 BIN 文件名替换为 `ESP32-S3-DualEye-Touch-LCD-1.28_XiaoZhi_v2.5.4.bin`。
