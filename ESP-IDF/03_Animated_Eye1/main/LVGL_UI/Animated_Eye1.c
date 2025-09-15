#include "Animated_Eye1.h"
#include <stdlib.h> 

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
    lv_coord_t eye_r; 
    lv_point_t pupil_pos;
    lv_point_t last_pupil; 
    uint8_t blink_state; 
    uint16_t blink_timer; 
} eye_t;

/**********************
 *  STATIC VARIABLES
 **********************/
static eye_t eye1, eye2;
static const lv_font_t *font_large;
static const lv_font_t *font_normal;
static disp_size_t disp_size;

#define SCALE_FACTOR 1.5

// Draw eye 
static void draw_eye(eye_t *eye) {
     lv_canvas_fill_bg(eye->canvas, lv_color_hex(0xFFFFFF), LV_OPA_TRANSP);

    lv_coord_t w = lv_obj_get_width(eye->canvas);
    lv_coord_t h = lv_obj_get_height(eye->canvas);
    lv_point_t center = {w / 2, h / 2};

    lv_coord_t scaled_r = eye->eye_r * SCALE_FACTOR;

    // 1. Draw sclera (white of the eye)
    lv_draw_rect_dsc_t sclera_dsc;
    lv_draw_rect_dsc_init(&sclera_dsc);
    sclera_dsc.radius = LV_RADIUS_CIRCLE;
    sclera_dsc.bg_color = lv_color_hex(0xF5F9FF); 
    sclera_dsc.border_color = lv_color_hex(0xDDDDDD);
    sclera_dsc.border_width = 2 * SCALE_FACTOR; 
    lv_canvas_draw_rect(eye->canvas,
                       center.x - scaled_r,
                       center.y - scaled_r,
                       scaled_r * 2,
                       scaled_r * 2,
                       &sclera_dsc);

    // 2. Draw iris
    lv_draw_rect_dsc_t iris_dsc;
    lv_draw_rect_dsc_init(&iris_dsc);
    iris_dsc.radius = LV_RADIUS_CIRCLE;
    iris_dsc.bg_color = lv_color_hex(0x8B4513); 
    iris_dsc.border_width = 1 * SCALE_FACTOR;
    iris_dsc.border_color = lv_color_hex(0x603813); 

    lv_coord_t iris_r = scaled_r * 0.7;
    lv_canvas_draw_rect(eye->canvas,
                       center.x - iris_r,
                       center.y - iris_r,
                       iris_r * 2,
                       iris_r * 2,
                       &iris_dsc);

    // Iris details: Add a small ring
    lv_draw_rect_dsc_t iris_ring_dsc;
    lv_draw_rect_dsc_init(&iris_ring_dsc);
    iris_ring_dsc.radius = LV_RADIUS_CIRCLE;
    iris_ring_dsc.bg_opa = LV_OPA_TRANSP; 
    iris_ring_dsc.border_color = lv_color_hex(0xA0522D); 
    iris_ring_dsc.border_width = 2 * SCALE_FACTOR; 
    lv_canvas_draw_rect(eye->canvas,
                       center.x - iris_r * 0.8,
                       center.y - iris_r * 0.8,
                       iris_r * 1.6,
                       iris_r * 1.6,
                       &iris_ring_dsc);

    // 3. Draw pupil
    lv_draw_rect_dsc_t pupil_dsc;
    lv_draw_rect_dsc_init(&pupil_dsc);
    pupil_dsc.radius = LV_RADIUS_CIRCLE;
    pupil_dsc.bg_color = lv_color_hex(0x000000); 

    // Limit pupil movement range
    lv_coord_t max_offset = iris_r - 12 * SCALE_FACTOR;
    lv_coord_t pupil_x = LV_CLAMP(center.x - max_offset,
                                center.x + eye->pupil_pos.x,
                                center.x + max_offset);
    lv_coord_t pupil_y = LV_CLAMP(center.y - max_offset,
                                center.y + eye->pupil_pos.y,
                                center.y + max_offset);

    lv_coord_t pupil_r = iris_r * 0.5;
    lv_canvas_draw_rect(eye->canvas,
                       pupil_x - pupil_r,
                       pupil_y - pupil_r,
                       pupil_r * 2,
                       pupil_r * 2,
                       &pupil_dsc);

    // 4. Draw highlights
    lv_draw_rect_dsc_t highlight_dsc;
    lv_draw_rect_dsc_init(&highlight_dsc);
    highlight_dsc.radius = LV_RADIUS_CIRCLE;
    highlight_dsc.bg_color = lv_color_hex(0xFFFFFF);
    highlight_dsc.bg_opa = LV_OPA_90; 

    lv_canvas_draw_rect(eye->canvas,
                       pupil_x - pupil_r * 0.6,
                       pupil_y - pupil_r * 0.6,
                       pupil_r / 2,
                       pupil_r / 2,
                       &highlight_dsc);

    highlight_dsc.bg_opa = LV_OPA_60;
    lv_canvas_draw_rect(eye->canvas,
                       pupil_x + pupil_r * 0.2,
                       pupil_y - pupil_r * 0.3,
                       pupil_r / 4,
                       pupil_r / 4,
                       &highlight_dsc);
}

//Eye animation update 
static void eye_anim_timer(lv_timer_t *timer) {
    static uint8_t move_step = 0;
    move_step++;
    if (move_step >= 8) {
        move_step = 0;
        eye1.last_pupil = eye1.pupil_pos;
        eye2.last_pupil = eye2.pupil_pos;

        // Random pupil movement (-50 to +50 range)
        eye1.pupil_pos.x = (rand() % 101) - 50;
        eye1.pupil_pos.y = (rand() % 101) - 50;
        eye2.pupil_pos.x = (rand() % 101) - 50;
        eye2.pupil_pos.y = (rand() % 101) - 50;
    }

    // Smooth transition between positions
    eye1.pupil_pos.x = eye1.last_pupil.x + (eye1.pupil_pos.x - eye1.last_pupil.x) / 8;
    eye1.pupil_pos.y = eye1.last_pupil.y + (eye1.pupil_pos.y - eye1.last_pupil.y) / 8;
    eye2.pupil_pos.x = eye2.last_pupil.x + (eye2.pupil_pos.x - eye2.last_pupil.x) / 8;
    eye2.pupil_pos.y = eye2.last_pupil.y + (eye2.pupil_pos.y - eye2.last_pupil.y) / 8;

    draw_eye(&eye1);
    draw_eye(&eye2);
}

/* Initialize eye */
static void init_eye(eye_t *eye, lv_obj_t *parent, lv_coord_t base_radius) {
    eye->canvas = lv_canvas_create(parent);
    eye->eye_r = base_radius; 
    eye->pupil_pos.x = 0;
    eye->pupil_pos.y = 0;
    eye->last_pupil.x = 0;
    eye->last_pupil.y = 0;
    eye->blink_state = 0;
    eye->blink_timer = 0;

    lv_coord_t size = (base_radius * 2 + 30) * SCALE_FACTOR;
    lv_obj_set_size(eye->canvas, size, size);
    lv_obj_align(eye->canvas, LV_ALIGN_CENTER, 0, 0);

    lv_color_t *buf = lv_mem_alloc(size * size * sizeof(lv_color_t));
    lv_canvas_set_buffer(eye->canvas, buf, size, size, LV_IMG_CF_TRUE_COLOR);

    draw_eye(eye);
}

void Animated_Eye1(void) {
    disp_size = DISP_SMALL;
    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    tv = lv_disp_get_scr_act(disp);
    tv2 = lv_disp_get_scr_act(disp2);

    lv_obj_set_style_text_font(lv_scr_act(), font_normal, 0);

    // Initialize two eyes (base radius 50)
    init_eye(&eye1, tv, 50);
    init_eye(&eye2, tv2, 50);

    // Create animation timer (100ms interval)
    lv_timer_create(eye_anim_timer, 100, NULL);
}