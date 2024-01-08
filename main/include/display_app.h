#ifndef B9988981_08D0_44FE_B90F_5979FC33609D
#define B9988981_08D0_44FE_B90F_5979FC33609D

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/portmacro.h"

#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"

#include "esp_lcd_gc9a01.h"
#include "esp_lcd_touch_cst816s.h"

#define LCD_HOST  SPI2_HOST
#define I2C_HOST  I2C_NUM_0

#define EXAMPLE_LCD_PIXEL_CLOCK_HZ     (20 * 1000 * 1000)
#define EXAMPLE_LCD_BK_LIGHT_ON_LEVEL  1
#define EXAMPLE_LCD_BK_LIGHT_OFF_LEVEL !EXAMPLE_LCD_BK_LIGHT_ON_LEVEL
#define EXAMPLE_PIN_NUM_SCLK           6
#define EXAMPLE_PIN_NUM_MOSI           7
#define EXAMPLE_PIN_NUM_MISO           -1
#define EXAMPLE_PIN_NUM_LCD_DC         2
#define EXAMPLE_PIN_NUM_LCD_RST        -1
#define EXAMPLE_PIN_NUM_LCD_CS         10
#define EXAMPLE_PIN_NUM_BK_LIGHT       3
#define EXAMPLE_PIN_NUM_TOUCH_CS       15

#define EXAMPLE_PIN_NUM_SDA            4
#define EXAMPLE_PIN_NUM_SCL            5
#define EXAMPLE_PIN_NUM_TOUCH_RST      1
#define EXAMPLE_PIN_NUM_TOUCH_INT      0
#define GPIO_PULLUP_ENABLE             1
#define EXAMPLE_TOUCH_CLOCK_HZ         100000

#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              240

// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS           8
#define EXAMPLE_LCD_PARAM_BITS         8

void display_main(void);

bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);
void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
void example_lvgl_port_update_callback(lv_disp_drv_t *drv);
void example_lvgl_touch_cb(lv_indev_drv_t * drv, lv_indev_data_t * data);
void example_increase_lvgl_tick(void *arg);




#endif /* B9988981_08D0_44FE_B90F_5979FC33609D */
