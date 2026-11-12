#include "LVGL_Example.h"

#include "SD_Card.h"

#include <stdlib.h>
#include <string.h>

// ==============================================
// LVGL Dual Screen Different BMP Example
// - 240x240 circular displays
// ==============================================

// SD card file paths for left and right screens
static const char * BMP_FILE_PATH_LEFT = "/test1.bmp";  // Left LCD
static const char * BMP_FILE_PATH_RIGHT = "/test2.bmp"; // Right LCD

// Separate image descriptors and buffers for each screen
static lv_img_dsc_t bmp_img_dsc_left;   
static lv_img_dsc_t bmp_img_dsc_right; 
static uint8_t * bmp_img_data_left = NULL;
static uint8_t * bmp_img_data_right = NULL;

/**
 * @brief Read 16-bit little-endian value from BMP file
 */
static uint16_t read_le16(fs::File & file)
{
  uint8_t b[2] = {0};
  if (file.read(b, sizeof(b)) != sizeof(b)) {
    return 0;
  }
  return (uint16_t)b[0] | ((uint16_t)b[1] << 8);
}

/**
 * @brief Read 32-bit little-endian value from BMP file
 */
static uint32_t read_le32(fs::File & file)
{
  uint8_t b[4] = {0};
  if (file.read(b, sizeof(b)) != sizeof(b)) {
    return 0;
  }
  return (uint32_t)b[0] | ((uint32_t)b[1] << 8) | ((uint32_t)b[2] << 16) | ((uint32_t)b[3] << 24);
}

/**
 * @brief Read 32-bit signed little-endian value from BMP file
 */
static int32_t read_le32s(fs::File & file)
{
  return (int32_t)read_le32(file);
}

/**
 * @brief Display a message on the specified display
 */
static void show_message_on_display(lv_disp_t * display, const char * message)
{
  lv_obj_t * screen = lv_disp_get_scr_act(display);
  lv_obj_clean(screen);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  lv_obj_t * label = lv_label_create(screen);
  lv_label_set_text(label, message);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_width(label, EXAMPLE_LCD_WIDTH - 32);
  lv_obj_center(label);
}

/**
 * @brief Free memory for a single BMP image
 * @param img_data Pointer to the image data buffer pointer
 * @param img_dsc Pointer to the LVGL image descriptor
 */
static void free_bmp_image(uint8_t ** img_data, lv_img_dsc_t * img_dsc)
{
  if (*img_data != NULL) {
    heap_caps_free(*img_data);
    *img_data = NULL;
  }
  memset(img_dsc, 0, sizeof(lv_img_dsc_t));
}

/**
 * @brief Convert RGB components to LVGL color format
 */
static lv_color_t make_lv_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return lv_color_make(red, green, blue);
}

/**
 * @brief Load and decode a BMP image from SD card into specified buffers
 * @param path Path to BMP file on SD card
 * @param img_dsc Output: LVGL image descriptor to fill
 * @param img_data Output: Pointer to allocated image data buffer
 * @return true if loaded successfully, false otherwise
 */
static bool load_bmp_from_sd(const char * path, lv_img_dsc_t * img_dsc, uint8_t ** img_data)
{
  Serial.printf("Loading BMP from SD: %s\r\n", path);

  fs::File file = SD_Card_Open(path, FILE_READ);
  if (!file) {
    Serial.printf("Failed to open %s\r\n", path);
    return false;
  }

  if (read_le16(file) != 0x4D42) {
    Serial.println("Not a valid BMP file");
    file.close();
    return false;
  }

  (void)read_le32(file);
  (void)read_le16(file);
  (void)read_le16(file);
  uint32_t pixel_offset = read_le32(file);
  uint32_t dib_header_size = read_le32(file);
  int32_t bmp_width = read_le32s(file);
  int32_t bmp_height = read_le32s(file);
  uint16_t planes = read_le16(file);
  uint16_t bits_per_pixel = read_le16(file);
  uint32_t compression = read_le32(file);

  if (dib_header_size < 40 || planes != 1 || bmp_width <= 0 || bmp_height == 0 ||
      (bits_per_pixel != 16 && bits_per_pixel != 24 && bits_per_pixel != 32) ||
      compression != 0) {
    Serial.printf("Unsupported BMP: %ldx%ld, %u bpp, compression %lu\r\n",
                  (long)bmp_width, (long)bmp_height, bits_per_pixel, (unsigned long)compression);
    file.close();
    return false;
  }

  uint32_t src_width = (uint32_t)bmp_width;
  uint32_t src_height = (bmp_height > 0) ? (uint32_t)bmp_height : (uint32_t)(-bmp_height);
  bool bottom_up = bmp_height > 0;
  uint32_t row_stride = ((src_width * bits_per_pixel + 31) / 32) * 4;

  // Calculate scaled dimensions to fit 240x240 screen
  uint32_t out_width = src_width;
  uint32_t out_height = src_height;
  if (out_width > EXAMPLE_LCD_WIDTH || out_height > EXAMPLE_LCD_HEIGHT) {
    float scale_x = (float)EXAMPLE_LCD_WIDTH / (float)out_width;
    float scale_y = (float)EXAMPLE_LCD_HEIGHT / (float)out_height;
    float scale = (scale_x < scale_y) ? scale_x : scale_y;
    out_width = (uint32_t)((float)out_width * scale);
    out_height = (uint32_t)((float)out_height * scale);
    if (out_width == 0) out_width = 1;
    if (out_height == 0) out_height = 1;
  }

  uint8_t * row = (uint8_t *)malloc(row_stride);
  if (row == NULL) {
    Serial.println("Failed to allocate BMP row buffer");
    file.close();
    return false;
  }

  // Free any previously loaded image for this buffer
  free_bmp_image(img_data, img_dsc);

  // Allocate image buffer (PSRAM first, then internal RAM)
  size_t image_size = out_width * out_height * sizeof(lv_color_t);
  *img_data = (uint8_t *)heap_caps_malloc(image_size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  if (*img_data == NULL) {
    *img_data = (uint8_t *)heap_caps_malloc(image_size, MALLOC_CAP_8BIT);
  }
  if (*img_data == NULL) {
    Serial.println("Failed to allocate BMP image buffer");
    free(row);
    file.close();
    return false;
  }

  lv_color_t * pixels = (lv_color_t *)(*img_data);
  for (uint32_t out_y = 0; out_y < out_height; out_y++) {
    uint32_t src_y = (out_y * src_height) / out_height;
    uint32_t file_y = bottom_up ? (src_height - 1 - src_y) : src_y;
    if (!file.seek(pixel_offset + file_y * row_stride)) {
      Serial.println("BMP seek failed");
      free(row);
      file.close();
      free_bmp_image(img_data, img_dsc);
      return false;
    }

    if (file.read(row, row_stride) != row_stride) {
      Serial.println("BMP read failed");
      free(row);
      file.close();
      free_bmp_image(img_data, img_dsc);
      return false;
    }

    for (uint32_t out_x = 0; out_x < out_width; out_x++) {
      uint32_t src_x = (out_x * src_width) / out_width;
      uint8_t red = 0, green = 0, blue = 0;

      if (bits_per_pixel == 16) {
        uint16_t color = (uint16_t)row[src_x * 2] | ((uint16_t)row[src_x * 2 + 1] << 8);
        red = (uint8_t)(((color >> 11) & 0x1F) << 3);
        green = (uint8_t)(((color >> 5) & 0x3F) << 2);
        blue = (uint8_t)((color & 0x1F) << 3);
      } else if (bits_per_pixel == 24) {
        uint32_t offset = src_x * 3;
        blue = row[offset];
        green = row[offset + 1];
        red = row[offset + 2];
      } else {
        uint32_t offset = src_x * 4;
        blue = row[offset];
        green = row[offset + 1];
        red = row[offset + 2];
      }

      pixels[out_y * out_width + out_x] = make_lv_color(red, green, blue);
    }
  }

  free(row);
  file.close();

  // Fill LVGL image descriptor
  img_dsc->header.always_zero = 0;
  img_dsc->header.w = out_width;
  img_dsc->header.h = out_height;
  img_dsc->header.cf = LV_IMG_CF_TRUE_COLOR;
  img_dsc->data_size = image_size;
  img_dsc->data = *img_data;

  Serial.printf("Loaded BMP %s: %ldx%ld -> %lux%lu\r\n",
                path, (long)src_width, (long)src_height,
                (unsigned long)out_width, (unsigned long)out_height);
  return true;
}

/**
 * @brief Display a BMP image on the specified display
 * @param display Target LVGL display device
 * @param img_dsc Pointer to the LVGL image descriptor to display
 */
static void show_bmp_on_display(lv_disp_t * display, lv_img_dsc_t * img_dsc)
{
  lv_obj_t * screen = lv_disp_get_scr_act(display);
  lv_obj_clean(screen);
  lv_obj_clear_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(screen, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

  lv_obj_t * img = lv_img_create(screen);
  lv_img_set_src(img, img_dsc);
  lv_obj_center(img);
}

/**
 * @brief Main entry point for dual BMP example
 * Loads different images for left and right screens
 */
void Lvgl_BMP_Example(void)
{
  Serial.println("Starting dual BMP example for DualEye display");

  // Clean up any previously loaded images
  free_bmp_image(&bmp_img_data_left, &bmp_img_dsc_left);
  free_bmp_image(&bmp_img_data_right, &bmp_img_dsc_right);

  // Load images for both screens independently
  bool left_loaded = load_bmp_from_sd(BMP_FILE_PATH_LEFT, &bmp_img_dsc_left, &bmp_img_data_left);
  bool right_loaded = load_bmp_from_sd(BMP_FILE_PATH_RIGHT, &bmp_img_dsc_right, &bmp_img_data_right);

  // Display left screen content
  if (left_loaded) {
    show_bmp_on_display(disp, &bmp_img_dsc_left);
    Serial.println("Left eye image displayed");
  } else {
    show_message_on_display(disp, "Load left eye failed");
    Serial.println("Failed to load left eye image");
  }

  // Display right screen content
  if (right_loaded) {
    show_bmp_on_display(disp2, &bmp_img_dsc_right);
    Serial.println("Right eye image displayed");
  } else {
    show_message_on_display(disp2, "Load right eye failed");
    Serial.println("Failed to load right eye image");
  }

  Serial.println("Dual BMP example completed");
}

/**
 * @brief Adjust backlight brightness for all displays
 */
void LVGL_Backlight_adjustment(uint8_t Backlight)
{
  Set_Backlight(Backlight);
}