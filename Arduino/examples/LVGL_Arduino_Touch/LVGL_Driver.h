#pragma once

#include <lvgl.h>
#include "lv_conf.h"
#include <demos/lv_demos.h>
#include <esp_heap_caps.h>
#include "LCD_Driver.h"
#include "Touch_Driver.h"
#include "Board_Configuration.h"

#define LVGL_BUF_LEN  (EXAMPLE_LCD_WIDTH * EXAMPLE_LCD_HEIGHT / 10)

#define EXAMPLE_LVGL_TICK_PERIOD_MS  10
extern lv_disp_t *disp;    
extern lv_disp_t *disp2;   

void refresh_screen();
void Lvgl_print(const char * buf);
void example_increase_lvgl_tick(void *arg);

void Lvgl_Init(void);
void LVGL_Start(void);