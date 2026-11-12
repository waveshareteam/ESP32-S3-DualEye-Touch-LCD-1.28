# 初始化源文件和包含目录
set(SOURCES
    "./main.c"
    "./LCD_Driver/LCD_Driver.c"
    "./Touch_Driver/Touch_Driver.c"
    "./LVGL_Driver/LVGL_Driver.c"
    "./LVGL_UI/Text_Number.c"
    "./I2C_Driver/I2C_Driver.c"
    "./BAT_Driver/BAT_Driver.c"
    "./Wireless/Wireless.c"
    "./LVGL_UI/Eyes_Data/AA.c"
)

set(INCLUDE_DIRS 
    "./LCD_Driver"   
    "./Touch_Driver"
    "./LVGL_Driver" 
    "./LVGL_UI" 
    "./I2C_Driver"
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
