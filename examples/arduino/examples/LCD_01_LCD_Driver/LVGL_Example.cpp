#include "LVGL_Example.h"

static lv_obj_t * number_label1 = NULL;  // Counter label on primary display
static lv_obj_t * number_label2 = NULL;  // Counter label on secondary display
static lv_timer_t * number_timer = NULL; // Periodic timer for counter updates
static uint8_t number_value = 0;         // Current counter value (cycles 0-99)

static lv_obj_t * create_label(lv_obj_t * parent, const char * text, const lv_font_t * font)
{
  lv_obj_t * label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(label, EXAMPLE_LCD_WIDTH - 32);
  return label;
}

static void update_number_labels(void)
{
  char text[3];
  lv_snprintf(text, sizeof(text), "%02u", (unsigned int)number_value);

  if (number_label1 != NULL) {
    lv_label_set_text(number_label1, text);
  }
  if (number_label2 != NULL) {
    lv_label_set_text(number_label2, text);
  }
}

static void number_timer_cb(lv_timer_t * timer)
{
  (void)timer;
  number_value = (number_value + 1) % 100;
  update_number_labels();
}

//create ui
static void create_display_content(lv_disp_t * display, const char * title, lv_obj_t ** number_label,
                                   lv_color_t bg_color, lv_color_t accent_color)
{
  lv_obj_t * screen = lv_disp_get_scr_act(display);
  lv_obj_clean(screen);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, bg_color, 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  lv_obj_t * ring = lv_obj_create(screen);
  lv_obj_remove_style_all(ring);
  lv_obj_set_size(ring, 210, 210);
  lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_border_width(ring, 8, 0);
  lv_obj_set_style_border_color(ring, accent_color, 0);
  lv_obj_set_style_bg_opa(ring, LV_OPA_TRANSP, 0);
  lv_obj_center(ring);

  lv_obj_t * title_label = create_label(screen, title, LV_FONT_DEFAULT);
  lv_obj_set_style_text_color(title_label, lv_color_white(), 0);
  lv_obj_align(title_label, LV_ALIGN_CENTER, 0, -56);

  *number_label = create_label(screen, "00", LV_FONT_DEFAULT);
  lv_obj_set_style_text_color(*number_label, accent_color, 0);
  lv_obj_align(*number_label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_t * dot = lv_obj_create(screen);
  lv_obj_remove_style_all(dot);
  lv_obj_set_size(dot, 18, 18);
  lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_bg_color(dot, accent_color, 0);
  lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);
  lv_obj_align(dot, LV_ALIGN_CENTER, 0, 56);
}

/**
 * @brief Main entry point for LVGL Example 1
 * Initializes UI on both displays and starts the counter timer
 * Automatically cleans up previous timer instances to prevent duplicates
 */
void Lvgl_Example1(void)
{
  if (number_timer != NULL) {
    lv_timer_del(number_timer);
    number_timer = NULL;
  }

  number_value = 0;
  //create ui
  create_display_content(disp, "Screen 1", &number_label1,
                         lv_color_hex(0x16213E), lv_color_hex(0x30C48D));
  create_display_content(disp2, "Screen 2", &number_label2,
                         lv_color_hex(0x273043), lv_color_hex(0xF2C14E));
  update_number_labels();
  number_timer = lv_timer_create(number_timer_cb, 500, NULL);
}

/**
 * @brief Adjust backlight brightness for all connected displays
 * @param Backlight Brightness level (0 = fully off, 255 = maximum brightness)
 */
void LVGL_Backlight_adjustment(uint8_t Backlight)
{
  Set_Backlight(Backlight);
}
