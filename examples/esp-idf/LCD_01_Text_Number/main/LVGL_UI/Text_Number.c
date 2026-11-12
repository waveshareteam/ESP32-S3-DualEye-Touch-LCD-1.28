#include "Text_Number.h"

static const lv_font_t *font_large = LV_FONT_DEFAULT;
static const lv_font_t *font_large_number = &lv_font_montserrat_48; 

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *cont1 = NULL;
static lv_obj_t *text_label1 = NULL;
static lv_obj_t *number_label1 = NULL;
static uint8_t number1 = 0;

static lv_obj_t *cont2 = NULL;
static lv_obj_t *text_label2 = NULL;
static lv_obj_t *number_label2 = NULL;
static uint8_t number2 = 0;

static lv_timer_t *number_timer = NULL;

// Timer callback: Update numbers on both screens simultaneously
static void update_numbers(lv_timer_t *timer) {
    // Increment numbers (cycle between 0-99)
    number1 = (number1 + 1) % 100;
    number2 = (number2 + 1) % 100;  

    char displayNumber1[3];
    displayNumber1[0] = '0' + (number1 / 10);
    displayNumber1[1] = '0' + (number1 % 10);
    displayNumber1[2] = '\0';
    lv_label_set_text(number_label1, displayNumber1);

    char displayNumber2[3];
    displayNumber2[0] = '0' + (number2 / 10);
    displayNumber2[1] = '0' + (number2 % 10);
    displayNumber2[2] = '\0';
    lv_label_set_text(number_label2, displayNumber2);
}

// Universal function to create UI for a single screen
static void create_screen_ui(lv_obj_t *scr, 
                            lv_obj_t **cont, 
                            lv_obj_t **text_label, 
                            lv_obj_t **number_label) {
    // Create main container
    *cont = lv_obj_create(scr);
    lv_obj_remove_style_all(*cont); 
    lv_obj_set_size(*cont, lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL)); 
    lv_obj_set_style_bg_color(*cont, lv_color_hex(0x0000FF), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(*cont, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(*cont, 0, LV_STATE_DEFAULT); 
    lv_obj_align(*cont, LV_ALIGN_CENTER, 0, 0);
    
    // Create text label
    *text_label = lv_label_create(*cont);
    lv_label_set_text(*text_label, "Hello, Waveshare!");
    lv_obj_set_style_text_color(*text_label, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(*text_label, font_large, LV_STATE_DEFAULT);
    lv_obj_align(*text_label, LV_ALIGN_CENTER, 0, -30);
    
    // Create number label
    *number_label = lv_label_create(*cont);
    lv_obj_set_style_text_color(*number_label, lv_color_white(), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(*number_label, font_large_number, LV_STATE_DEFAULT);
    lv_obj_align(*number_label, LV_ALIGN_CENTER, 0, 30);
    
    // Initialize display to "00"
    lv_label_set_text(*number_label, "00");
}

void Text_Number(void) {
    // Get active screens for both displays
    lv_obj_t *scr1 = lv_disp_get_scr_act(disp); 
    lv_obj_t *scr2 = lv_disp_get_scr_act(disp2);  

    // Create UI for both screens respectively
    create_screen_ui(scr1, &cont1, &text_label1, &number_label1);
    create_screen_ui(scr2, &cont2, &text_label2, &number_label2);
    
    // Create timer to update numbers on both screens every second
    if (number_timer == NULL) {
        number_timer = lv_timer_create(update_numbers, 1000, NULL);
    }
}