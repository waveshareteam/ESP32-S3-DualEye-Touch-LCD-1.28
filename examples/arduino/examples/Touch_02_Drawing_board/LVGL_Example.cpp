#include "LVGL_Example.h"

#define TOUCH_CANVAS_SIZE 240    // Canvas dimensions (square)
#define TOUCH_POINT_RADIUS 4     // Radius of drawn touch points in pixels

// Global UI components for dual displays
static lv_obj_t * canvas1 = NULL;        // Canvas object for primary display
static lv_obj_t * canvas2 = NULL;        // Canvas object for secondary display
static lv_color_t * canvas_buf1 = NULL;  // Frame buffer for primary canvas
static lv_color_t * canvas_buf2 = NULL;  // Frame buffer for secondary canvas

// Accent color definitions for each screen
static const lv_color_t touch_color1 = lv_color_hex(0x30C48D);  // Green for screen 1
static const lv_color_t touch_color2 = lv_color_hex(0xF2C14E);  // Yellow for screen 2

static lv_color_t * alloc_canvas_buffer(void)
{
  size_t size = TOUCH_CANVAS_SIZE * TOUCH_CANVAS_SIZE * sizeof(lv_color_t);
  lv_color_t * buf = (lv_color_t *)heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (buf == NULL) {
    buf = (lv_color_t *)heap_caps_malloc(size, MALLOC_CAP_8BIT);
  }
  return buf;
}

static void create_error_label(lv_obj_t * parent)
{
  lv_obj_t * label = lv_label_create(parent);
  lv_label_set_text(label, "Canvas buffer alloc failed");
  lv_obj_set_width(label, EXAMPLE_LCD_WIDTH - 32);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_center(label);
}

static void draw_touch_point(lv_obj_t * canvas, lv_coord_t x, lv_coord_t y, lv_color_t color)
{
  for (lv_coord_t dy = -TOUCH_POINT_RADIUS; dy <= TOUCH_POINT_RADIUS; dy++) {
    for (lv_coord_t dx = -TOUCH_POINT_RADIUS; dx <= TOUCH_POINT_RADIUS; dx++) {
      if (dx * dx + dy * dy > TOUCH_POINT_RADIUS * TOUCH_POINT_RADIUS) {
        continue;
      }

      lv_coord_t px = x + dx;
      lv_coord_t py = y + dy;
      if (px >= 0 && px < TOUCH_CANVAS_SIZE && py >= 0 && py < TOUCH_CANVAS_SIZE) {
        lv_canvas_set_px_color(canvas, px, py, color);
      }
    }
  }

  lv_obj_invalidate(canvas);
}

static void refresh_touch_canvas(lv_obj_t * canvas)
{
  if (canvas == NULL) {
    return;
  }

  lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
  lv_obj_invalidate(canvas);
}

static lv_obj_t * create_canvas_title(lv_obj_t * parent, lv_obj_t * canvas, const char * text)
{
  lv_obj_t * label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_width(label, TOUCH_CANVAS_SIZE - 16);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_color(label, lv_color_black(), 0);
  lv_obj_align_to(label, canvas, LV_ALIGN_TOP_MID, 0, 8);
  return label;
}

//touch event callback function
static void canvas_touch_event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code != LV_EVENT_PRESSED && code != LV_EVENT_PRESSING) {
    return;
  }

  lv_obj_t * canvas = lv_event_get_target(e);
  lv_indev_t * indev = lv_indev_get_act();
  if (indev == NULL) {
    return;
  }

  lv_point_t point;
  lv_area_t coords;
  lv_indev_get_point(indev, &point);
  lv_obj_get_coords(canvas, &coords);

  lv_coord_t canvas_x = point.x - coords.x1;
  lv_coord_t canvas_y = point.y - coords.y1;
  const char * canvas_name = canvas == canvas1 ? "canvas1" : "canvas2";

  printf("%s touch: x=%d, y=%d\r\n", canvas_name, (int)canvas_x, (int)canvas_y);//printf (x,y)
  draw_touch_point(canvas, canvas_x, canvas_y, canvas == canvas1 ? touch_color1 : touch_color2);
}

// create ui
static void create_touch_canvas(lv_disp_t * display, const char * title, lv_obj_t ** canvas,
                                lv_color_t * canvas_buf, lv_color_t bg_color, lv_color_t point_color)
{
  lv_obj_t * screen = lv_disp_get_scr_act(display);
  lv_obj_clean(screen);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, bg_color, 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  if (canvas_buf == NULL) {
    create_error_label(screen);
    return;
  }

  *canvas = lv_canvas_create(screen);
  lv_canvas_set_buffer(*canvas, canvas_buf, TOUCH_CANVAS_SIZE, TOUCH_CANVAS_SIZE, LV_IMG_CF_TRUE_COLOR);
  lv_canvas_fill_bg(*canvas, lv_color_white(), LV_OPA_COVER);
  lv_obj_add_flag(*canvas, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_border_width(*canvas, 2, 0);
  lv_obj_set_style_border_color(*canvas, point_color, 0);
  lv_obj_set_style_radius(*canvas, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(*canvas, true, 0);
  lv_obj_center(*canvas);
  lv_obj_add_event_cb(*canvas, canvas_touch_event_cb, LV_EVENT_PRESSED, NULL);
  lv_obj_add_event_cb(*canvas, canvas_touch_event_cb, LV_EVENT_PRESSING, NULL);

  create_canvas_title(screen, *canvas, title);
}

void Touch_Example(void)
{
  if (canvas_buf1 == NULL) {
    canvas_buf1 = alloc_canvas_buffer();
  }
  if (canvas_buf2 == NULL) {
    canvas_buf2 = alloc_canvas_buffer();
  }
  // Create touch canvas UI for both displays with distinct themes
  create_touch_canvas(disp, "canvas1", &canvas1, canvas_buf1,
                      lv_color_hex(0x16213E), touch_color1);
  create_touch_canvas(disp2, "canvas2", &canvas2, canvas_buf2,
                      lv_color_hex(0x273043), touch_color2);
}

/**
 * @brief Clear all drawings from both canvases
 * Resets both canvases to white background
 */
void Lvgl_Refresh_Canvas(void)
{
  refresh_touch_canvas(canvas1);
  refresh_touch_canvas(canvas2);
  printf("Canvas refreshed\r\n");
}

/**
 * @brief Adjust backlight brightness for all connected displays
 * @param Backlight Brightness level (0 = fully off, 255 = maximum brightness)
 */
void LVGL_Backlight_adjustment(uint8_t Backlight)
{
  Set_Backlight(Backlight);
}
