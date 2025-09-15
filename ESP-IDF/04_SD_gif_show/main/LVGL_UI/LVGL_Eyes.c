#include "Lvgl_Eyes.h"

static lv_obj_t *eye1_img;
static lv_obj_t *eye2_img;
static lv_timer_t *eyes_gif_timer;
static void gif_restart_cb(lv_timer_t * t)
{
    lv_gif_restart(eye1_img);
    lv_gif_restart(eye2_img);
    
    // lv_gif_t * gifobj1 = (lv_gif_t *) eye1_img;
    // lv_gif_t * gifobj2 = (lv_gif_t *) eye2_img;
    // lv_timer_resume(gifobj1->timer);
    // lv_timer_resume(gifobj2->timer);
}

static void gif_event_handler(lv_event_t * e)
{
    //Handle the logic after the GIF finishes playing
  lv_gif_set_src(eye1_img, "S:C1.gif");  // Use the lv_img_dsc_t structure as the source
  lv_gif_set_src(eye2_img, "S:C1.gif");  // Use the lv_img_dsc_t structure as the source
  lv_gif_restart(eye1_img);
  lv_gif_restart(eye2_img);
}
void Lvgl_Eyes(void){
  // LV_IMG_DECLARE(AA);
  lv_fs_fatfs_init();

  eye1_img = lv_gif_create(lv_disp_get_scr_act(disp));
  lv_obj_set_size(eye1_img, 240, 240);
  lv_obj_align(eye1_img, LV_ALIGN_CENTER, 0, 0); 
  lv_gif_set_src(eye1_img, "S:C1.gif");  
  lv_obj_add_event_cb(eye1_img, gif_event_handler, LV_EVENT_READY, NULL);
  eye2_img = lv_gif_create(lv_disp_get_scr_act(disp2));
  lv_obj_set_size(eye2_img, 240, 240);
  lv_obj_align(eye2_img, LV_ALIGN_CENTER, 0, 0); 
  lv_gif_set_src(eye2_img, "S:C1.gif"); 
  lv_obj_add_event_cb(eye2_img, gif_event_handler, LV_EVENT_READY, NULL);

  // Create another timer for the GIF of eye2
  //eyes_gif_timer = lv_timer_create(gif_restart_cb, 15000, NULL); // Check every 5 seconds
  // lv_timer_pause(eye2_gif_timer);  // Not activated yet. Waiting for the GIF to load
  // lv_timer_resume(eyes_gif_timer);  // 启动 eye1 的GIF循环计时器  // Start playing the GIF and enable looping
  
}