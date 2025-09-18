# 初始化源文件和包含目录
set(SOURCES
    "./main.c"
    "./LCD_Driver/LCD_Driver.c"
    "./Touch_Driver/Touch_Driver.c"
    "./Audio_Driver/Audio_Driver.c" 
    "./MIC_Driver/MIC_Speech.c" 
    "./LVGL_Driver/LVGL_Driver.c"
    "./LVGL_UI/LVGL_Example.c"
    "./LVGL_UI/LVGL_Eyes.c"
    "./LVGL_UI/LVGL_Music.c"
    "./SD_Card/SD_MMC.c" 
    "./I2C_Driver/I2C_Driver.c"
    "./I2S_Driver/I2S_Driver.c"
    "./BAT_Driver/BAT_Driver.c"
    "./Wireless/Wireless.c"
    
    "./LVGL_UI/Eyes_Data/AA.c"
    "./LVGL_UI/lv_fs_port.c"
)

set(INCLUDE_DIRS 
    "./LCD_Driver"   
    "./Touch_Driver"
    "./Audio_Driver" 
    "./MIC_Driver" 
    "./LVGL_Driver" 
    "./LVGL_UI" 
    "./SD_Card" 
    "./I2C_Driver"
    "./I2S_Driver"
    "./BAT_Driver"
    "./Wireless"
    "."
)
 
# 根据配置添加源文件和包含目录,在 LCD_Driver.h 和 Touch_Driver.h 要同步修改

if(CONFIG_WAVESHARE_1_28INCH_TOUCH_LCD)
    list(APPEND SOURCES 
        "./LCD_Driver/GC9A01A/GC9A01A.c"
        "./Touch_Driver/CST816/CST816.c"
    )
    list(APPEND INCLUDE_DIRS
        "./LCD_Driver/GC9A01A"
        "./Touch_Driver/CST816"
    )
endif()

# 注册组件
idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS ${INCLUDE_DIRS}
)
