#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "LVGL_Music.h"
#include "Button_Driver.h"


struct Simulated_XY{
  uint8_t points;     // Number of touch points
  uint16_t x;         /*!< X coordinate */
  uint16_t y;         /*!< Y coordinate */
};
extern uint8_t page_Flag;
extern lv_obj_t *current_obj;
extern struct Simulated_XY Simulated_touch_data[2];

void Simulated_Touch_Init();
void Simulated_Touch(lv_obj_t *Label_obj);                                // Simulate touching the part
void Add_Style_Label(lv_obj_t *Label_obj, lv_style_t * Label_style);      // Add selected style
void Del_Style_Label(lv_obj_t *Label_obj, lv_style_t * Label_style);      // Clear selected style
