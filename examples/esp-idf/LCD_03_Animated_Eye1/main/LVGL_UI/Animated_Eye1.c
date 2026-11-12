#include "Animated_Eye1.h"
#include <stdlib.h> 
#include <time.h>  
#include <math.h>  // Added: For vector calculations

typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE
} disp_size_t;

lv_obj_t *tv;
lv_obj_t *tv2;

/**********************
 * Eye data structure
 **********************/
typedef struct {
    lv_obj_t *canvas;
    lv_coord_t eye_r;       // Eyeball (sclera) radius
    lv_point_t pupil_pos;   // Current pupil position (offset from sclera center)
    uint8_t blink_state; 
    uint16_t blink_timer; 
} eye_t;

/**********************
 * Globally shared variables
 **********************/
static lv_point_t global_target_pos = {0, 0};  // Global target position for both eyes
static uint32_t last_move_time = 0;            // Last time target position was updated
static eye_t eye1, eye2;
static const lv_font_t *font_large;
static const lv_font_t *font_normal;
static disp_size_t disp_size;

// Configuration parameters
#define SCALE_FACTOR     1.5    // Scaling factor
#define MOVE_INTERVAL    1000   // Target position update interval (ms)
#define MOVE_STEP        4      // Movement step (smaller = smoother)
#define MAX_OFFSET_RATIO 0.3    // Maximum movement range (relative to sclera radius)
#define PUPIL_OFFSET     6      // Additional pupil offset relative to iris

// Draw eye (Core: Correlate iris and pupil coordinates, add pupil offset)
static void draw_eye(eye_t *eye) {
    lv_canvas_fill_bg(eye->canvas, lv_color_hex(0xFFFFFF), LV_OPA_TRANSP);

    // 1. Calculate base coordinates (sclera center is fixed)
    lv_coord_t w = lv_obj_get_width(eye->canvas);
    lv_coord_t h = lv_obj_get_height(eye->canvas);
    lv_point_t sclera_center = {w / 2, h / 2};  // Sclera (white of eye) center is fixed
    lv_coord_t sclera_r = eye->eye_r * SCALE_FACTOR;  // Sclera radius

    // 2. Calculate actual iris position (with range restriction)
    lv_coord_t max_offset = sclera_r * MAX_OFFSET_RATIO;
    lv_coord_t iris_x = LV_CLAMP(
        sclera_center.x - max_offset,
        sclera_center.x + eye->pupil_pos.x,
        sclera_center.x + max_offset
    );
    lv_coord_t iris_y = LV_CLAMP(
        sclera_center.y - max_offset,
        sclera_center.y + eye->pupil_pos.y,
        sclera_center.y + max_offset
    );

    // 3. Calculate additional pupil offset (along iris movement direction)
    // Calculate direction vector from sclera center to iris
    lv_coord_t dir_x = iris_x - sclera_center.x;
    lv_coord_t dir_y = iris_y - sclera_center.y;
    
    // Calculate direction vector length (avoid division by zero)
    float length = sqrt(dir_x * dir_x + dir_y * dir_y);
    if (length < 0.1) length = 0.1;  // Prevent anomalies when near center
    
    // Normalize direction vector and calculate pupil offset
    float norm_x = dir_x / length;
    float norm_y = dir_y / length;
    
    // Pupil position = iris position + additional offset in same direction
    lv_coord_t pupil_x = iris_x + (lv_coord_t)(norm_x * PUPIL_OFFSET);
    lv_coord_t pupil_y = iris_y + (lv_coord_t)(norm_y * PUPIL_OFFSET);

    // 4. Draw sclera (white of eye, fixed position)
    lv_draw_rect_dsc_t sclera_dsc;
    lv_draw_rect_dsc_init(&sclera_dsc);
    sclera_dsc.radius = LV_RADIUS_CIRCLE;
    sclera_dsc.bg_color = lv_color_hex(0xFFFFFF);  // White of eye
    sclera_dsc.border_color = lv_color_hex(0xDDDDDD);
    sclera_dsc.border_width = 2 * SCALE_FACTOR;
    lv_canvas_draw_rect(
        eye->canvas,
        sclera_center.x - sclera_r,
        sclera_center.y - sclera_r,
        sclera_r * 2,
        sclera_r * 2,
        &sclera_dsc
    );

    // 5. Draw iris (brown circle, using iris center coordinates)
    lv_coord_t iris_r = sclera_r * 0.6;  // Iris radius (ensure larger than pupil)
    lv_draw_rect_dsc_t iris_dsc;
    lv_draw_rect_dsc_init(&iris_dsc);
    iris_dsc.radius = LV_RADIUS_CIRCLE;
    iris_dsc.bg_color = lv_color_hex(0x8B4513);  // Brown iris
    iris_dsc.border_color = lv_color_hex(0x603813);
    iris_dsc.border_width = 1 * SCALE_FACTOR;
    lv_canvas_draw_rect(
        eye->canvas,
        iris_x - iris_r,  // Use iris center
        iris_y - iris_r,
        iris_r * 2,
        iris_r * 2,
        &iris_dsc
    );

    // 6. Draw pupil (black, using offset pupil coordinates)
    lv_coord_t pupil_r = iris_r * 0.5;  // Pupil radius
    lv_draw_rect_dsc_t pupil_dsc;
    lv_draw_rect_dsc_init(&pupil_dsc);
    pupil_dsc.radius = LV_RADIUS_CIRCLE;
    pupil_dsc.bg_color = lv_color_hex(0x000000);
    lv_canvas_draw_rect(
        eye->canvas,
        pupil_x - pupil_r,  // Use offset pupil center
        pupil_y - pupil_r,
        pupil_r * 2,
        pupil_r * 2,
        &pupil_dsc
    );

    // 7. Draw highlight (moves with pupil)
    lv_draw_rect_dsc_t highlight_dsc;
    lv_draw_rect_dsc_init(&highlight_dsc);
    highlight_dsc.radius = LV_RADIUS_CIRCLE;
    highlight_dsc.bg_color = lv_color_hex(0xFFFFFF);
    highlight_dsc.bg_opa = LV_OPA_80;
    lv_canvas_draw_rect(
        eye->canvas,
        pupil_x - pupil_r * 0.7,
        pupil_y - pupil_r * 0.7,
        pupil_r / 2,
        pupil_r / 2,
        &highlight_dsc
    );
    // highlight_dsc.bg_opa = LV_OPA_60;
    // lv_canvas_draw_rect(eye->canvas,
    //                    pupil_x + pupil_r * 0.2,
    //                    pupil_y - pupil_r * 0.3,
    //                    pupil_r / 4,
    //                    pupil_r / 4,
    //                    &highlight_dsc);
}

// Eye animation update
static void eye_anim_timer(lv_timer_t *timer) {
    uint32_t current_time = lv_tick_get();

    // Update global target position at regular intervals
    if (current_time - last_move_time > MOVE_INTERVAL) {
        last_move_time = current_time;
        // Generate same random target position
        global_target_pos.x = (rand() % (int)(MAX_OFFSET_RATIO * 200)) - (MAX_OFFSET_RATIO * 100);
        global_target_pos.y = (rand() % (int)(MAX_OFFSET_RATIO * 200)) - (MAX_OFFSET_RATIO * 100);
    }

    // Both screen eyes move toward the same target position
    // Eye 1 movement logic
    if (eye1.pupil_pos.x < global_target_pos.x) {
        eye1.pupil_pos.x += MOVE_STEP;
        if (eye1.pupil_pos.x > global_target_pos.x) eye1.pupil_pos.x = global_target_pos.x;
    } else if (eye1.pupil_pos.x > global_target_pos.x) {
        eye1.pupil_pos.x -= MOVE_STEP;
        if (eye1.pupil_pos.x < global_target_pos.x) eye1.pupil_pos.x = global_target_pos.x;
    }
    if (eye1.pupil_pos.y < global_target_pos.y) {
        eye1.pupil_pos.y += MOVE_STEP;
        if (eye1.pupil_pos.y > global_target_pos.y) eye1.pupil_pos.y = global_target_pos.y;
    } else if (eye1.pupil_pos.y > global_target_pos.y) {
        eye1.pupil_pos.y -= MOVE_STEP;
        if (eye1.pupil_pos.y < global_target_pos.y) eye1.pupil_pos.y = global_target_pos.y;
    }

    // Eye 2 exactly copies eye 1's position (ensure dual-screen consistency)
    eye2.pupil_pos = eye1.pupil_pos;

    // Draw both screen eyes
    draw_eye(&eye1);
    draw_eye(&eye2);
}

/* Initialize eye */
static void init_eye(eye_t *eye, lv_obj_t *parent, lv_coord_t base_radius) {
    eye->canvas = lv_canvas_create(parent);
    eye->eye_r = base_radius;
    eye->pupil_pos.x = 0;  // Initial position centered
    eye->pupil_pos.y = 0;
    eye->blink_state = 0;
    eye->blink_timer = 0;

    // Calculate canvas size
    lv_coord_t size = (base_radius * 2 + 40) * SCALE_FACTOR;
    lv_obj_set_size(eye->canvas, size, size);
    lv_obj_align(eye->canvas, LV_ALIGN_CENTER, 0, 0);

    // Allocate canvas buffer
    lv_color_t *buf = lv_mem_alloc(size * size * sizeof(lv_color_t));
    lv_canvas_set_buffer(eye->canvas, buf, size, size, LV_IMG_CF_TRUE_COLOR);

    draw_eye(eye);
}

void Animated_Eye1(void) {
    srand(12345);  // Fixed random seed
    disp_size = DISP_SMALL;
    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    tv = lv_disp_get_scr_act(disp);
    tv2 = lv_disp_get_scr_act(disp2);

    // Initialize both eyes
    init_eye(&eye1, tv, 50);
    init_eye(&eye2, tv2, 50);

    // Create animation timer
    lv_timer_create(eye_anim_timer, 50, NULL);
}
