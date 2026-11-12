#include "Shapes_on_Circular.h"

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    SHAPE_SQUARE,
    SHAPE_TRIANGLE,
    SHAPE_CIRCLE,
    SHAPE_COUNT
} shape_type_t;

/**********************
 *      VARIABLES
 **********************/
static shape_type_t current_shape = SHAPE_SQUARE;
static lv_obj_t *current_obj_scr1 = NULL;
static lv_obj_t *current_obj_scr2 = NULL;
static lv_timer_t *shape_timer = NULL;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static lv_obj_t *create_single_triangle(lv_obj_t *scr, uint16_t side_len, uint16_t height, 
                                       lv_color_t border_color, lv_color_t fill_color);
static lv_obj_t *create_base_shape(lv_obj_t *scr, uint16_t width, uint16_t height, 
                                  lv_color_t color, int32_t radius_val);

// Create a random color
static lv_color_t random_color(void) {
    return lv_color_make(rand() % 256, rand() % 256, rand() % 256);
}

// Clear current shapes from both screens
static void clear_current_shapes() {
    if(current_obj_scr1) {
        lv_obj_del(current_obj_scr1);
        current_obj_scr1 = NULL;
    }
    if(current_obj_scr2) {
        lv_obj_del(current_obj_scr2);
        current_obj_scr2 = NULL;
    }
}

// Create square shapes on both screens
static void create_squares(lv_obj_t *scr1, lv_obj_t *scr2) {
    lv_color_t color = random_color();
    uint16_t size = 100;
    
    current_obj_scr1 = create_base_shape(scr1, size, size, color, 0);
    current_obj_scr2 = create_base_shape(scr2, size, size, color, 0);
}

// Create circle shapes on both screens
static void create_circles(lv_obj_t *scr1, lv_obj_t *scr2) {
    lv_color_t color = random_color();
    uint16_t diameter = 120; 
    
    current_obj_scr1 = create_base_shape(scr1, diameter, diameter, color, LV_RADIUS_CIRCLE);
    current_obj_scr2 = create_base_shape(scr2, diameter, diameter, color, LV_RADIUS_CIRCLE);
}

// Create equilateral triangle shapes on both screens
static void create_triangles(lv_obj_t *scr1, lv_obj_t *scr2) {
    lv_color_t border_color = random_color();  
    lv_color_t fill_color = random_color();    
    uint16_t side_len = 120;                   
    uint16_t height = (uint16_t)(side_len * 0.866);  // Height = side length × √3/2 ≈ 0.866

    current_obj_scr1 = create_single_triangle(scr1, side_len, height, border_color, fill_color);
    current_obj_scr2 = create_single_triangle(scr2, side_len, height, border_color, fill_color);
}

// Create base shape (used for squares and circles)
static lv_obj_t *create_base_shape(lv_obj_t *scr, uint16_t width, uint16_t height, 
                                  lv_color_t color, int32_t radius_val) { 
    lv_obj_t *shape = lv_obj_create(scr);
    
    lv_obj_set_size(shape, width, height);
    
    lv_obj_remove_style_all(shape);
    lv_obj_set_style_bg_color(shape, color, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(shape, 0, LV_STATE_DEFAULT); 
    lv_obj_set_style_bg_opa(shape, LV_OPA_COVER, LV_STATE_DEFAULT); 
    
    // Set corner radius
    lv_obj_set_style_radius(shape, radius_val, LV_STATE_DEFAULT);
    
    lv_obj_center(shape);
    
    return shape;
}

// Draw an equilateral triangle
static lv_obj_t *create_single_triangle(lv_obj_t *scr, uint16_t side_len, uint16_t height, 
                                       lv_color_t border_color, lv_color_t fill_color) {

    lv_obj_t *canvas = lv_canvas_create(scr);
    lv_obj_remove_style_all(canvas); 
    lv_obj_set_size(canvas, side_len, height); 
    lv_obj_center(canvas);                     

    uint32_t buf_size = LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(side_len, height);
    lv_color_t *buf = lv_mem_alloc(buf_size); 
    if (buf == NULL) { 
        lv_obj_del(canvas); 
        return NULL;
    }
    memset(buf, 0, buf_size); 
    lv_canvas_set_buffer(canvas, buf, side_len, height, LV_IMG_CF_TRUE_COLOR_ALPHA);

    lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_TRANSP);

    lv_point_t points[3] = {
        {side_len / 2, 0},              // Top vertex
        {1, height - 2},                // Bottom left vertex
        {side_len - 2, height - 2}      // Bottom right vertex
    };

    lv_draw_rect_dsc_t dsc;
    lv_draw_rect_dsc_init(&dsc);             
    dsc.border_color = border_color;         
    dsc.border_width = 2;                    
    dsc.bg_color = fill_color;              
    dsc.bg_opa = LV_OPA_COVER;              
    dsc.border_opa = LV_OPA_COVER;         

    lv_canvas_draw_polygon(canvas, points, 3, &dsc);

    return canvas;
}

// Timer callback: switch between shapes
static void change_shapes(lv_timer_t *timer) {
    lv_obj_t *scr1 = lv_disp_get_scr_act(disp);
    lv_obj_t *scr2 = lv_disp_get_scr_act(disp2);
    
    clear_current_shapes();
    
    current_shape = (current_shape + 1) % SHAPE_COUNT;
    
    switch(current_shape) {
        case SHAPE_SQUARE:  create_squares(scr1, scr2);  break;
        case SHAPE_TRIANGLE: create_triangles(scr1, scr2); break;
        case SHAPE_CIRCLE:   create_circles(scr1, scr2);   break;
        default: break;
    }
}

// Main initialization function
void Shapes_on_Circular(void) {
    // Initialize random number seed
    srand(lv_tick_get());
    
    lv_obj_t *scr1 = lv_disp_get_scr_act(disp);
    lv_obj_t *scr2 = lv_disp_get_scr_act(disp2);

    lv_obj_set_style_bg_color(scr1, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr1, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr1, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    
    lv_obj_set_style_bg_color(scr2, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(scr2, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(scr2, LV_RADIUS_CIRCLE, LV_STATE_DEFAULT);
    
    // Create initial shapes
    create_squares(scr1, scr2);
    
    // Create timer to switch shapes every 3 seconds
    if(shape_timer == NULL) {
        shape_timer = lv_timer_create(change_shapes, 3000, NULL);
    }
}
    