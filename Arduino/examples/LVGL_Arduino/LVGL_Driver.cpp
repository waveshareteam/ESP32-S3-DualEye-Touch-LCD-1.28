/*****************************************************************************
  | File        :   LVGL_Driver.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "LVGL_Driver.h"

static lv_disp_draw_buf_t draw_buf,draw_buf2;
lv_disp_drv_t disp_drv,disp_drv2;  
lv_indev_drv_t indev_drv,indev_drv2;
lv_disp_t *disp;
lv_disp_t *disp2;
static lv_color_t buf1[ LVGL_BUF_LEN];
static lv_color_t buf2[ LVGL_BUF_LEN];
static lv_color_t buf3[ LVGL_BUF_LEN];
static lv_color_t buf4[ LVGL_BUF_LEN];
// static lv_color_t* buf1 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
// static lv_color_t* buf2 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
// static lv_color_t* buf3 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
// static lv_color_t* buf4 = (lv_color_t*) heap_caps_malloc(LVGL_BUF_LEN , MALLOC_CAP_SPIRAM);
    
/*  Display flushing 
    Displays LVGL content on the LCD
    This function implements associating LVGL data to the LCD screen
*/
void example_lvgl_flush_cb( lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map )
{
  esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
  int offsetx1 = area->x1;
  int offsetx2 = area->x2;
  int offsety1 = area->y1;
  int offsety2 = area->y2;
  // copy a buffer's content to a specific area of the display
  uint32_t size = (offsetx2 - offsetx1 +1 ) * (offsety2 - offsety1 + 1);
  uint16_t * color = (uint16_t *)color_map;
  for (size_t i = 0; i < size; i++) {
    color[i] = (((color[i] >> 8) & 0xFF) | ((color[i] << 8) & 0xFF00));
  }
  esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 +1, offsety2 + 1, color_map);
  lv_disp_flush_ready(drv);
}
/*Read the touchpad*/
void example_touchpad_read( lv_indev_drv_t * drv, lv_indev_data_t * data )
{
  // printf("CCCCCCCCCCCCC=%d  \r\n",touchpad_cnt);
  if (Simulated_touch_data[1].points != 0x00) {
    data->point.x = Simulated_touch_data[1].x;
    data->point.y = Simulated_touch_data[1].y;
    data->state = LV_INDEV_STATE_PR;
      // printf("X=%u Y=%u num=%d \r\n", data->point.x, data->point.y,touchpad_cnt);
  } else {
      data->state = LV_INDEV_STATE_REL;
  }
  Simulated_touch_data[1].x = 0;
  Simulated_touch_data[1].y = 0;
  Simulated_touch_data[1].points = 0;

}
/*Read the touchpad*/
void example_touchpad1_read( lv_indev_drv_t * drv, lv_indev_data_t * data )
{

  // printf("CCCCCCCCCCCCC=%d  \r\n",touchpad_cnt);
  if (Simulated_touch_data[0].points != 0x00) {
    data->point.x = Simulated_touch_data[0].x;
    data->point.y = Simulated_touch_data[0].y;
    data->state = LV_INDEV_STATE_PR;
      // printf("X=%u Y=%u num=%d \r\n", data->point.x, data->point.y,touchpad_cnt);
  } else {
      data->state = LV_INDEV_STATE_REL;
  }
  Simulated_touch_data[0].x = 0;
  Simulated_touch_data[0].y = 0;
  Simulated_touch_data[0].points = 0;
}
/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
void example_lvgl_port_update_callback(lv_disp_drv_t *drv)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    switch (drv->rotated) {
    case LV_DISP_ROT_NONE:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, true, false);
        break;
    case LV_DISP_ROT_90:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, true, true);
        break;
    case LV_DISP_ROT_180:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, false, true);
        break;
    case LV_DISP_ROT_270:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, false, false);
        break;
    }
}

void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}
void example_increase_lvgl_Loop_tick(void *arg)
{
  lv_timer_handler(); /* let the GUI do its work */
}
void Lvgl_Init(void)
{
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf1, buf2, LVGL_BUF_LEN);
  lv_disp_draw_buf_init( &draw_buf2, buf3, buf4, LVGL_BUF_LEN);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv,disp_drv2;
  lv_disp_drv_init( &disp_drv );
  /*Change the following line to your display resolution*/
  disp_drv.hor_res = EXAMPLE_LCD_WIDTH;
  disp_drv.ver_res = EXAMPLE_LCD_HEIGHT;
  disp_drv.flush_cb = example_lvgl_flush_cb;
  disp_drv.drv_update_cb = example_lvgl_port_update_callback;                                         // Function : Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. 
  disp_drv.draw_buf = &draw_buf;
  disp_drv.full_refresh = 1;                    /**< 1: Always make the whole screen redrawn*/
  disp_drv.user_data = panel_handle;                
  disp = lv_disp_drv_register( &disp_drv );
  
  lv_disp_drv_init(&disp_drv2);                                                                        // Create a new screen object and initialize the associated device
  disp_drv2.hor_res = EXAMPLE_LCD_WIDTH;             
  disp_drv2.ver_res = EXAMPLE_LCD_HEIGHT;                                                                       // Vertical axis pixel count
  disp_drv2.flush_cb = example_lvgl_flush_cb;                                                          // Function : copy a buffer's content to a specific area of the display
  disp_drv2.drv_update_cb = example_lvgl_port_update_callback;                                         // Function : Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. 
  disp_drv2.draw_buf = &draw_buf2;                                                                      // LVGL will use this buffer(s) to draw the screens contents
  disp_drv2.full_refresh = 1;
  disp_drv2.user_data = panel_handle2;                
  disp2 = lv_disp_drv_register(&disp_drv2);  

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_init ( &indev_drv );
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.disp = disp;
  indev_drv.read_cb = example_touchpad_read;
  indev_drv.user_data = disp;
  lv_indev_drv_register( &indev_drv);

  lv_indev_drv_init ( &indev_drv2 );
  indev_drv2.type = LV_INDEV_TYPE_POINTER;
  indev_drv2.disp = disp2;
  indev_drv2.read_cb = example_touchpad1_read;
  indev_drv2.user_data = disp2;
  lv_indev_drv_register( &indev_drv2 );

  /* Create simple label */
  lv_obj_t *label = lv_label_create( lv_scr_act());
  lv_label_set_text( label, "Hello Ardino and LVGL!");
  lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

  const esp_timer_create_args_t lvgl_tick_timer_args = {
    .callback = &example_increase_lvgl_tick,
    .name = "lvgl_tick"
  };
  esp_timer_handle_t lvgl_tick_timer = NULL;
  esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
  esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000);

}

void LVGL_Loop(void *parameter)
{
    while(1)
    {
        // The task running lv_timer_handler should have lower priority than that running `lv_tick_inc`
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelete(NULL);
}
void LVGL_Start(void){

    xTaskCreatePinnedToCore(
        LVGL_Loop, 
        "LVGL Driver task",
        4096, 
        NULL, 
        3, 
        NULL, 
        1);
}
void refresh_screen() {
    lv_obj_t *screen = lv_scr_act();
    lv_obj_invalidate(screen);
}
