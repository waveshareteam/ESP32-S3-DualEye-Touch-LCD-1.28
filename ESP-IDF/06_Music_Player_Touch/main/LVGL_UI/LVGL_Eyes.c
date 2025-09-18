#include "Lvgl_Eyes.h"

static lv_obj_t *eye1_img;
static lv_obj_t *eye2_img;
// static lv_timer_t *eyes_gif_timer;
// static void gif_restart_cb(lv_timer_t * t)
// {
//     lv_gif_restart(eye1_img);
//     lv_gif_restart(eye2_img);
    
//     // lv_gif_t * gifobj1 = (lv_gif_t *) eye1_img;
//     // lv_gif_t * gifobj2 = (lv_gif_t *) eye2_img;
//     // lv_timer_resume(gifobj1->timer);
//     // lv_timer_resume(gifobj2->timer);
// }

// static void gif_event_handler(lv_event_t * e)
// {
//     // 处理 GIF 播放完成后的逻辑
//   lv_gif_set_src(eye1_img, "S:CC1.gif");  // 使用lv_img_dsc_t结构体作为源
//   lv_gif_set_src(eye2_img, "S:CC2.gif");  // 使用lv_img_dsc_t结构体作为源
//   lv_gif_restart(eye1_img);
//   lv_gif_restart(eye2_img);
// }
void Lvgl_Eyes(void){
  // LV_IMG_DECLARE(AA);
  lv_fs_fatfs_init();

  eye1_img = lv_gif_create(lv_disp_get_scr_act(disp));
  lv_obj_set_size(eye1_img, 240, 240);
  lv_obj_align(eye1_img, LV_ALIGN_CENTER, 0, 0); 
  lv_gif_set_src(eye1_img, "S:CC1.gif");  
  // lv_obj_add_event_cb(eye1_img, gif_event_handler, LV_EVENT_READY, NULL);
  eye2_img = lv_gif_create(lv_disp_get_scr_act(disp2));
  lv_obj_set_size(eye2_img, 240, 240);
  lv_obj_align(eye2_img, LV_ALIGN_CENTER, 0, 0); 
  lv_gif_set_src(eye2_img, "S:CC2.gif"); 
  // lv_obj_add_event_cb(eye2_img, gif_event_handler, LV_EVENT_READY, NULL);

  // // 创建另一个定时器，用于 eye2 的GIF
  // eyes_gif_timer = lv_timer_create(gif_restart_cb, 15000, NULL); // 每5s检查一次
  // // lv_timer_pause(eye2_gif_timer);  // 暂时不启动，等待GIF加载
  // // lv_timer_resume(eyes_gif_timer);  // 启动 eye1 的GIF循环计时器  // 启动GIF播放并开始循环
  
}