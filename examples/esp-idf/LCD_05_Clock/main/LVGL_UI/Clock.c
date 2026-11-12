#include "Clock.h"
#include "lvgl.h"
#include <math.h>
#include <stdio.h>
#include "esp_system.h"
#include "esp_sntp.h"
#include "time.h"
#include "sys/time.h"

/**********************
 *      VARIABLES
 **********************/
static lv_obj_t *clock_canvas1 = NULL;
static lv_obj_t *clock_canvas2 = NULL;
static lv_obj_t *time_label1 = NULL;  
static lv_obj_t *time_label2 = NULL;  
static int hour_angle = 0;    
static int minute_angle = 0;  
static int second_angle = 0;  
static lv_timer_t *update_timer = NULL;
static char time_str[9];      

/**********************
 *      DEFINES
 **********************/
#define ANGLE_TO_RAD 0.01745329251994329576923690768489

#define CLOCK_RADIUS_RATIO 0.85     
#define MAIN_TICK_LENGTH 15         
#define MINOR_TICK_LENGTH 8         
#define MAIN_TICK_WIDTH 3           
#define MINOR_TICK_WIDTH 2          

#define HOUR_HAND_LENGTH  70        
#define MINUTE_HAND_LENGTH 80       
#define SECOND_HAND_LENGTH 90      
#define HOUR_HAND_WIDTH 5          
#define MINUTE_HAND_WIDTH 3         
#define SECOND_HAND_WIDTH 2        

// Initialize RTC time synchronization
static void rtc_init(void) {
    
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_setservername(0, "pool.ntp.org");
    sntp_init();

    int retry = 0;
    const int retry_count = 5;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    setenv("TZ", "CST-8", 1);
    tzset();
}

// Get current time (hours, minutes, seconds)
static void get_current_time(int *hour, int *minute, int *second, int *is_24h) {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    *is_24h = timeinfo.tm_hour;       
    *hour = timeinfo.tm_hour % 12;    
    if (*hour == 0) *hour = 12;       
    *minute = timeinfo.tm_min;
    *second = timeinfo.tm_sec;
}

// Update digital time label
static void update_time_label(int hour, int minute, int second, int is_24h) {
 
    sprintf(time_str, "%02d:%02d:%02d", is_24h, minute, second);
    
    lv_label_set_text(time_label1, time_str);
    lv_label_set_text(time_label2, time_str);
}

// Draw clock ticks
static void draw_clock_ticks(lv_obj_t *canvas, int size) {
    int center_x = size / 2;
    int center_y = size / 2;
    int clock_radius = (size / 2) * CLOCK_RADIUS_RATIO;
    
    lv_draw_line_dsc_t tick_dsc;
    lv_draw_line_dsc_init(&tick_dsc);
    tick_dsc.color = lv_color_hex(0xFFFFFF); 
    tick_dsc.opa = LV_OPA_COVER;
    
    for (int i = 0; i < 12; i++) {
        int angle = i * 30;
        float rad = angle * ANGLE_TO_RAD;
        
        int outer_x = center_x + cos(rad) * clock_radius;
        int outer_y = center_y + sin(rad) * clock_radius;
        
        int tick_length;
        if (i == 0 || i == 3 || i == 6 || i == 9) {  
            tick_length = MAIN_TICK_LENGTH;
            tick_dsc.width = MAIN_TICK_WIDTH;
        } else {
            tick_length = MINOR_TICK_LENGTH;
            tick_dsc.width = MINOR_TICK_WIDTH;
        }
        
        int inner_x = center_x + cos(rad) * (clock_radius - tick_length);
        int inner_y = center_y + sin(rad) * (clock_radius - tick_length);
        
        lv_point_t tick_points[] = {
            {outer_x, outer_y},
            {inner_x, inner_y}
        };
        lv_canvas_draw_line(canvas, tick_points, 2, &tick_dsc);
    }
}

// Draw a single clock hand
static void draw_hand(lv_obj_t *canvas, int center_x, int center_y, 
                     int length, int width, lv_color_t color, int angle) {
    float rad = angle * ANGLE_TO_RAD;
    
    int end_x = center_x + cos(rad) * length;
    int end_y = center_y - sin(rad) * length;  

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.color = color;
    line_dsc.width = width;
    line_dsc.opa = LV_OPA_COVER;

    lv_point_t points[] = {{center_x, center_y}, {end_x, end_y}};
    lv_canvas_draw_line(canvas, points, 2, &line_dsc);
}

// Draw clock face with all three hands
static void draw_clock(lv_obj_t *canvas, int size) {
    
    lv_canvas_fill_bg(canvas, lv_color_hex(0x000000), LV_OPA_COVER);
    
    int center_x = size / 2;
    int center_y = size / 2;
    
    draw_clock_ticks(canvas, size);
    
    draw_hand(canvas, center_x, center_y, 
             HOUR_HAND_LENGTH, HOUR_HAND_WIDTH, 
             lv_color_hex(0xFF0000), hour_angle);
    
    draw_hand(canvas, center_x, center_y, 
             MINUTE_HAND_LENGTH, MINUTE_HAND_WIDTH, 
             lv_color_hex(0xFFFF00), minute_angle);
    
    draw_hand(canvas, center_x, center_y, 
             SECOND_HAND_LENGTH, SECOND_HAND_WIDTH, 
             lv_color_hex(0x00FF00), second_angle);
    
    lv_draw_rect_dsc_t dot_dsc;
    lv_draw_rect_dsc_init(&dot_dsc);
    dot_dsc.bg_color = lv_color_hex(0xFFFFFF);
    dot_dsc.radius = LV_RADIUS_CIRCLE;
    lv_canvas_draw_rect(canvas, center_x - 3, center_y - 3, 6, 6, &dot_dsc);
}

// Create clock canvas and time label elements
static void create_clock_elements(lv_obj_t *scr, lv_obj_t **canvas, lv_obj_t **label, int size) {

    *canvas = lv_canvas_create(scr);
    lv_obj_set_size(*canvas, size, size);
    lv_obj_center(*canvas);
    
    uint32_t buf_size = LV_CANVAS_BUF_SIZE_TRUE_COLOR(size, size);
    lv_color_t *buf = lv_mem_alloc(buf_size);
    if (buf) memset(buf, 0, buf_size);
    lv_canvas_set_buffer(*canvas, buf, size, size, LV_IMG_CF_TRUE_COLOR);
    
    *label = lv_label_create(scr);
    lv_obj_set_style_text_color(*label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);  
    lv_obj_set_style_text_font(*label, LV_FONT_DEFAULT, LV_PART_MAIN); 
    lv_obj_align_to(*label, *canvas, LV_ALIGN_CENTER, -10, 20); 
}

// Timer callback: update time and redraw clock
static void update_callback(lv_timer_t *timer) {
    int hour, minute, second, is_24h;
    get_current_time(&hour, &minute, &second, &is_24h);

    hour_angle = 90 - (is_24h % 12) * 30 - minute * 0.5; 
    minute_angle = 90 - minute * 6 - second * 0.1;         
    second_angle = 90 - second * 6;                        

    hour_angle = (hour_angle + 360) % 360;
    minute_angle = (minute_angle + 360) % 360;
    second_angle = (second_angle + 360) % 360;

    draw_clock(clock_canvas1, 240);
    draw_clock(clock_canvas2, 240);
    update_time_label(hour, minute, second, is_24h);
}

// Main clock initialization function
void Clock(void) {

    lv_obj_t *scr1 = lv_disp_get_scr_act(disp);
    lv_obj_t *scr2 = lv_disp_get_scr_act(disp2);
    
    // Initialization
    if (clock_canvas1 == NULL) {

        //rtc_init();
        
        create_clock_elements(scr1, &clock_canvas1, &time_label1, 240);
        create_clock_elements(scr2, &clock_canvas2, &time_label2, 240);
        
        int hour, minute, second, is_24h;
        get_current_time(&hour, &minute, &second, &is_24h);
        hour_angle = (hour * 30) + (minute * 0.5);
        minute_angle = (minute * 6) + (second * 0.1);
        second_angle = second * 6;
        
        draw_clock(clock_canvas1, 240);
        draw_clock(clock_canvas2, 240);
        update_time_label(hour, minute, second, is_24h);
        
        // Create timer (update every 100ms)
        if (update_timer == NULL) {
            update_timer = lv_timer_create(update_callback, 100, NULL);
        }
    }
}

    