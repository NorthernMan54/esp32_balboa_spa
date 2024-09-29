#include "lvglDisplay.h"
#include <Arduino.h>
#include <ArduinoLog.h>
#include <lvgl.h> // Include the lvgl header file
#include <lv_version.h>
#include <lv_init.h>
#include <tick/lv_tick.h>
#include <display/lv_display.h>
#include <indev/lv_indev.h>
#include <ondev/lv_ondev.h>
#include <TickTwo.h>

void lvglTickFunction()
{
    lv_tick_inc(10);
}

TickTwo lvglTick(lvglTickFunction, 10);

void my_disp_flush(lv_display_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    int32_t x, y;
    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    for (y = area->y1; y <= area->y2; y++)
    {
        for (x = area->x1; x <= area->x2; x++)
        {
            // set_pixel(x, y, *color_p);
            color_p++;
        }
    }

    lv_display_flush_ready(disp); /* Indicate you are ready with the flushing*/
}

void lvglDisplayBegin()
{
    lv_init();

    lv_display_t *display = lv_display_create(LVGL_DISPLAY_WIDTH, LVGL_DISPLAY_HEIGHT);

/* Declare a buffer for 1/10 screen size */
#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */
    static uint8_t buf1[LVGL_DISPLAY_WIDTH * LVGL_DISPLAY_HEIGHT / 10 * BYTE_PER_PIXEL];
    lv_display_set_buffers(display, buf1, NULL, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL); /* Initialize the display buffer. */
    lv_display_set_flush_cb(display, my_disp_flush);

    lv_indev_t *indev = lv_indev_create();           /*Create an input device*/
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touch pad is a pointer-like device*/
    // lv_ondev_set_read_cb(indev, my_touchpad_read);   /*Set your driver function*/
    lvglTick.start();
}

void lvglDisplayLoop()
{
    lvglTick.update();
}