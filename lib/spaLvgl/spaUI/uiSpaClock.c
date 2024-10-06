#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *clock;

lv_obj_t *spa_clock_lable(lv_obj_t *parent)
{
  clock = lv_label_create(parent);
  lv_label_set_long_mode(clock, LV_LABEL_LONG_DOT); /*Break the long lines*/
  lv_obj_set_width(clock, 200);                     /*Set smaller width to make the lines wrap*/
  lv_obj_set_style_text_align(clock, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(clock, LV_ALIGN_CENTER, 0, -40);
  uiUpdateClock("Loading ....");
  return clock;
}

void uiUpdateClock(const char *timeStr)
{
  lv_label_set_text(clock, timeStr);
}
#endif