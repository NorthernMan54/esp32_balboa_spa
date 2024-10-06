#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaHeaterDisplay(lv_obj_t *parent)
{
  lv_obj_t *ui_Heater = lv_led_create(parent);
  lv_obj_align(ui_Heater, LV_ALIGN_CENTER, -0, 0);
  lv_led_off(ui_Heater);
  return ui_Heater;
}
#endif