#pragma once
#include <Arduino.h> 

// LCD rotation Angle selection, remove the front #, leaving only one drive
// #define CONFIG_EXAMPLE_DISPLAY_ROTATION_0_DEGREE         1 
#define CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE        1
// #define CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE       1
// #define CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE       1



#define CONFIG_Round_LCD_screen 1

#define CONFIG_LVGL_COLOR_16_SWAP 1




#define TouchPad_swap_xy     0
#define TouchPad_mirror_x    0
#define TouchPad_mirror_y    0
/*************************************************************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************************************************/
#ifdef CONFIG_EXAMPLE_DISPLAY_ROTATION_90_DEGREE
  #define Touch_swap_xy     (!TouchPad_swap_xy)
  #define Touch_mirror_x    (!TouchPad_mirror_x)
  #define Touch_mirror_y    (TouchPad_mirror_y)
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_180_DEGREE)
  #define Touch_swap_xy     (TouchPad_swap_xy)
  #define Touch_mirror_x    (!TouchPad_mirror_x)
  #define Touch_mirror_y    (!TouchPad_mirror_y)
#elif defined(CONFIG_EXAMPLE_DISPLAY_ROTATION_270_DEGREE)
  #define Touch_swap_xy     (!TouchPad_swap_xy)
  #define Touch_mirror_x    (TouchPad_mirror_x)
  #define Touch_mirror_y    (!TouchPad_mirror_y)
#else
  #define Touch_swap_xy     (TouchPad_swap_xy)
  #define Touch_mirror_x    (TouchPad_mirror_x)
  #define Touch_mirror_y    (TouchPad_mirror_y)
#endif