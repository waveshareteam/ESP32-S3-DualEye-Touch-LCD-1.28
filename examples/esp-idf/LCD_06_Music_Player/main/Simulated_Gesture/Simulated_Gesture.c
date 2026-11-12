/*****************************************************************************
  | File        :   Simulated_Gesture.c
  
  | help        : 
    The provided LVGL library file must be installed first
******************************************************************************/
#include "Simulated_Gesture.h"


static lv_style_t style_checked;
lv_obj_t *current_obj = NULL;
uint16_t current_obj_sequence = 0;
uint16_t current_Page_sequence = 0;
size_t Page_Number;
bool is_scrollable_y(const struct _lv_obj_t * obj) {

  lv_dir_t dir = lv_obj_get_scroll_dir(obj);                          
  lv_scrollbar_mode_t mode = lv_obj_get_scrollbar_mode(obj);            
  bool can_scroll_y = (dir & LV_DIR_VER) != 0;                       
  bool scroll_allowed = (mode == LV_SCROLLBAR_MODE_ON) || (mode == LV_SCROLLBAR_MODE_AUTO);    
  if (!can_scroll_y || !scroll_allowed) {                              
    return false;
  }

  lv_obj_t * modifiable_obj =  (lv_obj_t *)(obj);             
  lv_coord_t scroll_top = lv_obj_get_scroll_top(modifiable_obj);
  lv_coord_t scroll_bottom = lv_obj_get_scroll_bottom(modifiable_obj);
  return (scroll_top != 0 || scroll_bottom != 0);                    
}


void scroll_up_fixed(lv_obj_t * container, lv_coord_t pixels) {
  if(container){
    if(is_scrollable_y(container)){
      lv_obj_scroll_by(container, 0, -pixels, LV_ANIM_ON); 
    }

  }
}

uint8_t page_Flag = 0;
void Page_switching(bool Flag)
{
  if(Flag){
    if(page_Flag){
      page_Flag = 0;
    }
    else{
      page_Flag = 1;
    }
  }
  lv_obj_t *screen = lv_scr_act();
  lv_obj_invalidate(screen);

}
void TouchTask(void *parameter) {
  lv_obj_t **Simulated_panel[]={Simulated_panel1,Simulated_panel2};
  size_t panel_size[] = {Simulated_panel1_Size, Simulated_panel2_Size};
  Page_Number = sizeof(panel_size) / sizeof(size_t);
  uint8_t Old_Button = 0;
  while(1){
    if(BOOT_KEY_State == SINGLE_CLICK){      
      Del_Style_Label(current_obj, &style_checked);                                       // Clear selected style
      if(Old_Button == 2){
        current_obj_sequence ++;                                                          // Next OBJ
        if(current_obj_sequence >= panel_size[current_Page_sequence])                     // the last one
          current_obj_sequence = 0;                                                       // Back to the first
      }
      current_obj = Simulated_panel[current_Page_sequence][current_obj_sequence] ;      // Switch next
      Add_Style_Label(current_obj, &style_checked);                                       // Add selected style
      current_obj_sequence ++;                                                          // Next OBJ
      printf("panel_size[current_Page_sequence] =%d\r\n",panel_size[current_Page_sequence]);
      if(current_obj_sequence >= panel_size[current_Page_sequence])                     // the last one
        current_obj_sequence = 0;                                                       // Back to the first
      Old_Button = 1;
      lv_obj_t *screen = lv_scr_act();
      lv_obj_invalidate(screen);
      BOOT_KEY_State = NONE_PRESS;  
    }
    else if(BOOT_KEY_State == DOUBLE_CLICK){  
      Simulated_Touch(current_obj);
      BOOT_KEY_State = NONE_PRESS;  
    }
    else if(BOOT_KEY_State == LONG_PRESS_START){    
      current_obj_sequence = 0;                                                         // Back to the first
      Old_Button = 0;
      Page_switching(true);
      current_Page_sequence ++;                                                         // Next Page
      if(current_Page_sequence == Page_Number)                                          // the last one
        current_Page_sequence = 0;                                                       // Back to the first
      lv_obj_t *screen = lv_scr_act();
      lv_obj_invalidate(screen);
      BOOT_KEY_State = SINGLE_CLICK;  
    }     
   
    vTaskDelay(pdMS_TO_TICKS(100));    
  }                      
  vTaskDelete(NULL);
  
}
void Simulated_Touch_Init(){   
  lv_style_init(&style_checked);
  lv_style_set_bg_color(&style_checked, lv_color_white());
  lv_style_set_bg_opa(&style_checked, LV_OPA_COVER);
  lv_style_set_border_color(&style_checked, lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_border_width(&style_checked, 1);
  lv_style_set_border_opa(&style_checked, LV_OPA_COVER);
  lv_style_set_pad_all(&style_checked, 6);
  lv_style_set_radius(&style_checked, 8);
  lv_style_set_shadow_color(&style_checked, lv_color_black());
  lv_style_set_shadow_width(&style_checked, 8);
  lv_style_set_shadow_spread(&style_checked, 0);
  lv_style_set_shadow_ofs_x(&style_checked, 4);
  lv_style_set_shadow_ofs_y(&style_checked, 4);

  xTaskCreatePinnedToCore(
    TouchTask,    
    "TouchTask",   
    4096,                
    NULL,                
    4,                    
    NULL,                
    1                  
  );
}

struct Simulated_XY Simulated_touch_data[2] = {0};
void Simulated_Touch(lv_obj_t *Label_obj) { 
  lv_area_t coords = {0};                                      
  if(Label_obj){                                                         
    if(lv_obj_has_flag(Label_obj, LV_OBJ_FLAG_CLICKABLE)){           
      lv_obj_get_coords(Label_obj, &coords);
      if(coords.y2 != 0 || coords.x2 != 0){                              
        Simulated_touch_data[page_Flag].x = coords.x1 + (coords.x2 - coords.x1)/2;
        Simulated_touch_data[page_Flag].y = coords.y1 + (coords.y2 - coords.y1)/2;
        Simulated_touch_data[page_Flag].points = 1;
      }
    }
  }
}
void Add_Style_Label(lv_obj_t *Label_obj, lv_style_t * Label_style){
  if(Label_obj){
    lv_obj_add_style(Label_obj,Label_style, 0);         
    lv_area_t coords;                                 
    lv_obj_get_coords(Label_obj, &coords);
    if (coords.y2 > EXAMPLE_LCD_WIDTH*7/10) {   
      scroll_up_fixed(Page_panel[current_Page_sequence],coords.y2 - (EXAMPLE_LCD_WIDTH*7/10));
    }
    else if (coords.y1 < EXAMPLE_LCD_WIDTH*3/10) {   
      scroll_up_fixed(Page_panel[current_Page_sequence],coords.y1 - (EXAMPLE_LCD_WIDTH*3/10));
    }

  }
  else{
    printf("current_obj is NULL!!!!!\r\n");
  }
}
void Del_Style_Label(lv_obj_t *Label_obj, lv_style_t * Label_style){
  if(Label_obj)
    lv_obj_remove_style(Label_obj, Label_style, 0);  
}



