#include "Lvgl_Eyes.h"
#include <dirent.h>
#include <string.h>
#include "esp_log.h"

static const char *TAG = "Lvgl_Eyes";

static lv_obj_t *eye1_img;
static lv_obj_t *eye2_img;
static lv_obj_t *info_label;
static lv_timer_t *auto_switch_timer;

// Variables related to GIF file list
static char **gif_files = NULL;
static int gif_count = 0;
static int current_gif_index = 0;

void lv_gif_pause(lv_obj_t * obj)
{
    lv_gif_t * gifobj = (lv_gif_t *) obj;
    lv_timer_pause(gifobj->timer);
}

// Find all GIF files in the root directory of the SD card
static int find_gif_files(void)
{
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    // First traversal to count the number of GIF files
    dir = opendir("/sdcard");
    if (dir == NULL) {
        ESP_LOGE(TAG, "Failed to open SD card directory");
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *ext = strrchr(entry->d_name, '.');
            if (ext != NULL && strcasecmp(ext, ".gif") == 0) {
                count++;
            }
        }
    }
    closedir(dir);
    
    if (count == 0) {
        ESP_LOGW(TAG, "No GIF files found in SD card root directory");
        return 0;
    }
    
    // Allocate memory to store filenames
    gif_files = (char **)malloc(count * sizeof(char *));
    if (gif_files == NULL) {
        ESP_LOGE(TAG, "Memory allocation failed");
        return 0;
    }
    
    // Second traversal to store filenames
    dir = opendir("/sdcard");
    int index = 0;
    while ((entry = readdir(dir)) != NULL && index < count) {
        if (entry->d_type == DT_REG) {
            char *ext = strrchr(entry->d_name, '.');
            if (ext != NULL && strcasecmp(ext, ".gif") == 0) {
                gif_files[index] = (char *)malloc(strlen(entry->d_name) + 10);
                if (gif_files[index] != NULL) {
                    sprintf(gif_files[index], "S:%s", entry->d_name);
                    index++;
                }
            }
        }
    }
    closedir(dir);
    
    gif_count = index;
    ESP_LOGI(TAG, "Found %d GIF files", gif_count);
    return gif_count;
}

// Free memory for GIF file list
static void free_gif_files(void)
{
    if (gif_files != NULL) {
        for (int i = 0; i < gif_count; i++) {
            if (gif_files[i] != NULL) {
                free(gif_files[i]);
            }
        }
        free(gif_files);
        gif_files = NULL;
    }
    gif_count = 0;
    current_gif_index = 0;
}

// Load current GIF file
static void load_current_gif(void)
{
    if (gif_count == 0) return;
    
    char *gif_path = gif_files[current_gif_index];
    ESP_LOGI(TAG, "Loading GIF: %s", gif_path);
    
    lv_gif_set_src(eye1_img, gif_path);
    lv_gif_set_src(eye2_img, gif_path);
    
    // Restart GIF playback
    lv_gif_restart(eye1_img);
    lv_gif_restart(eye2_img);
    
    
    // Update information label
    if (info_label != NULL) {
        char info_text[50];
        snprintf(info_text, sizeof(info_text), "%d/%d: %s", 
                 current_gif_index + 1, gif_count, strrchr(gif_path, ':') + 1);
        lv_label_set_text(info_label, info_text);
    }
    
}

// Switch to next GIF
void eyes_next_gif(void)
{
    if (gif_count > 1) {
        current_gif_index = (current_gif_index + 1) % gif_count;
        char *gif_path = gif_files[current_gif_index];
        ESP_LOGI(TAG, "Loading GIF: %s", gif_path);
        lvgl_port_lock(0);
        lv_gif_pause(eye1_img); 
        lv_gif_pause(eye2_img); 

        // Set GIF source
        lv_gif_set_src(eye1_img, gif_path);
        lv_gif_set_src(eye2_img, gif_path);

        // Restart GIF playback
        lv_gif_restart(eye1_img);
        lv_gif_restart(eye2_img);
        
        // Update information label
        if (info_label != NULL) {
            char info_text[50];
            snprintf(info_text, sizeof(info_text), "%d/%d: %s", 
                    current_gif_index + 1, gif_count, strrchr(gif_path, ':') + 1);
            lv_label_set_text(info_label, info_text);
        }
        lvgl_port_unlock();
    }

}

static void gif_event_handler(lv_event_t * e)
{
  // Automatically restart playback after GIF finishes
    lv_gif_restart(eye1_img);
    lv_gif_restart(eye2_img);
}

void Lvgl_Eyes(void){
  // LV_IMG_DECLARE(AA);
  lv_fs_fatfs_init();

  // Find GIF files
    if (find_gif_files() == 0) {
        // No GIF files found, display error message
        lv_obj_t *error_label = lv_label_create(lv_disp_get_scr_act(disp));
        lv_label_set_text(error_label, "No GIF files found in SD card root directory");
        lv_obj_center(error_label);
        return;
    }

    eye1_img = lv_gif_create(lv_disp_get_scr_act(disp));
    lv_obj_set_size(eye1_img, 240, 240);
    lv_obj_align(eye1_img, LV_ALIGN_CENTER, 0, 0); 
    //lv_obj_add_event_cb(eye1_img, gif_event_handler, LV_EVENT_READY, NULL);

    eye2_img = lv_gif_create(lv_disp_get_scr_act(disp2));
    lv_obj_set_size(eye2_img, 240, 240);
    lv_obj_align(eye2_img, LV_ALIGN_CENTER, 0, 0); 
    //lv_obj_add_event_cb(eye2_img, gif_event_handler, LV_EVENT_READY, NULL);

  // Load first GIF
    load_current_gif();
  
}
