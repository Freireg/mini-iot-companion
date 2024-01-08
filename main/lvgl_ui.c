#include "lvgl_ui.h"

void cancelBtnCb(lv_event_t * e)
{
    lv_disp_t *disp = lv_event_get_user_data(e);
    clearScreen(disp);
    idleScreen(disp);
}

void waitingConnectionScreen(lv_disp_t *disp)
{   
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *spinner;
    lv_obj_t *cancelBtn = lv_btn_create(scr);
    spinner = lv_spinner_create(scr, 1000, 60);
    lv_obj_center(spinner);
    lv_obj_set_size(spinner, 200, 200);

    lv_obj_t * lbl = lv_label_create(scr);
    lv_label_set_text_static(lbl, "Connecting...");
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);

    lv_obj_align(cancelBtn, LV_ALIGN_BOTTOM_MID, 0, -50);
    lv_obj_set_size(cancelBtn, 40, 25);
    /*Button event*/
    lv_obj_add_event_cb(cancelBtn, cancelBtnCb, LV_EVENT_CLICKED, disp);

}

void temperatureSensorScreen(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *lbl = lv_label_create(scr);
    lv_obj_t *icon = lv_img_create(scr);
   
    // lv_obj_set_size(lbl, lv_pct(30), lv_pct(30));
    
    // LV_IMG_DECLARE(temperature);
    // lv_obj_set_size(icon, 10, 16);
    // lv_img_set_src(icon, &temperature);
    // lv_obj_align(icon, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_img_opa(icon, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    lv_label_set_text_static(lbl, "C");
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 25);

    lv_obj_t *sensorValue = lv_label_create(scr);
    // lv_obj_set_size(sensorValue, lv_pct(10), lv_pct(10));

    lv_label_set_text_fmt(sensorValue, "%d", 25);
    lv_obj_align(sensorValue, LV_ALIGN_CENTER, 0, 0);



}

void __attribute__((weak)) connectBtnCb(lv_event_t * e)
{
    lv_disp_t *disp = lv_event_get_user_data(e);
    clearScreen(disp);
    waitingConnectionScreen(disp);
}

void idleScreen(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *configBtn = lv_btn_create(scr);
    lv_obj_t *connectBtn = lv_btn_create(scr);
    lv_obj_t *lbl = lv_label_create(connectBtn);

    lv_obj_align(connectBtn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(connectBtn, 80, 50);
    /*Button event*/
    lv_obj_add_event_cb(connectBtn, connectBtnCb, LV_EVENT_CLICKED, disp);


    lv_label_set_text_static(lbl, LV_SYMBOL_WIFI);
    lv_obj_align(lbl, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(lbl, 20, 20);
   
}

static void set_temp(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

void testScreen(lv_disp_t *disp)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    lv_obj_t * bar = lv_bar_create(lv_scr_act());
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 20, 200);
    lv_obj_center(bar);
    lv_bar_set_range(bar, -20, 40);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_temp);
    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 3000);
    lv_anim_set_var(&a, bar);
    lv_anim_set_values(&a, -20, 40);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
}

void clearScreen(lv_disp_t *disp)
{
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_clean(scr);
}

void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

// void example_lvgl_port_update_callback(lv_disp_drv_t *drv)
// {
//     esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

//     switch (drv->rotated) {
//     case LV_DISP_ROT_NONE:
//         // Rotate LCD display
//         esp_lcd_panel_swap_xy(panel_handle, false);
//         esp_lcd_panel_mirror(panel_handle, true, false);
// #if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
//         // Rotate LCD touch
//         esp_lcd_touch_set_mirror_y(tp, false);
//         esp_lcd_touch_set_mirror_x(tp, false);
// #endif
//         break;
//     case LV_DISP_ROT_90:
//         // Rotate LCD display
//         esp_lcd_panel_swap_xy(panel_handle, true);
//         esp_lcd_panel_mirror(panel_handle, true, true);
// #if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
//         // Rotate LCD touch
//         esp_lcd_touch_set_mirror_y(tp, false);
//         esp_lcd_touch_set_mirror_x(tp, false);
// #endif
//         break;
//     case LV_DISP_ROT_180:
//         // Rotate LCD display
//         esp_lcd_panel_swap_xy(panel_handle, false);
//         esp_lcd_panel_mirror(panel_handle, false, true);
// #if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
//         // Rotate LCD touch
//         esp_lcd_touch_set_mirror_y(tp, false);
//         esp_lcd_touch_set_mirror_x(tp, false);
// #endif
//         break;
//     case LV_DISP_ROT_270:
//         // Rotate LCD display
//         esp_lcd_panel_swap_xy(panel_handle, true);
//         esp_lcd_panel_mirror(panel_handle, false, false);
// #if CONFIG_EXAMPLE_LCD_TOUCH_ENABLED
//         // Rotate LCD touch
//         esp_lcd_touch_set_mirror_y(tp, false);
//         esp_lcd_touch_set_mirror_x(tp, false);
// #endif
//         break;
//     }
// }



//------------------------------------------------------

// static lv_obj_t *meter;
// static lv_obj_t * btn;
// static lv_disp_rot_t rotation = LV_DISP_ROT_NONE;

// static void set_value(void *indic, int32_t v)
// {
//     lv_meter_set_indicator_end_value(meter, indic, v);
// }

// static void btn_cb(lv_event_t * e)
// {
//     lv_disp_t *disp = lv_event_get_user_data(e);
//     rotation++;
//     if (rotation > LV_DISP_ROT_270) {
//         rotation = LV_DISP_ROT_NONE;
//     }
//     lv_disp_set_rotation(disp, rotation);
// }
// //------------------------------------------------------