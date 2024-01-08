#ifndef AEA19BFF_B0DE_4C5A_A378_00C8A84ED525
#define AEA19BFF_B0DE_4C5A_A378_00C8A84ED525
#include "lvgl.h"
#include "custom_icons.h"


#define LVGL_TICK_PERIOD_MS   2

void idleScreen(lv_disp_t *disp);
void waitingConnectionScreen(lv_disp_t *disp);
void temperatureSensorScreen(lv_disp_t *disp);
void testScreen(lv_disp_t *disp);


void clearScreen(lv_disp_t *disp);


void example_increase_lvgl_tick(void *arg);
void example_lvgl_touch_cb(lv_indev_drv_t * drv, lv_indev_data_t * data);
void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

#endif /* AEA19BFF_B0DE_4C5A_A378_00C8A84ED525 */
