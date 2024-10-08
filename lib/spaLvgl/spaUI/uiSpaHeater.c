#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaHeaterDisplay(lv_obj_t *parent)
{
  /*Create a container with COLUMN flex direction*/
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_width(container, 80);
  lv_obj_set_height(container, 80);
  lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);
  //   lv_obj_set_size(cont_col, 200, 150);
  lv_obj_align(container, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

  decorateTile(container);
  lv_obj_t *ui_Heater = lv_led_create(container);
  lv_obj_align(ui_Heater, LV_ALIGN_CENTER, 5, 0);
  lv_led_set_color(ui_Heater, lv_palette_main(LV_PALETTE_ORANGE));
  lv_led_off(ui_Heater);

  lv_obj_t *ui_HeaterLabel = lv_label_create(container);
  lv_obj_align(ui_HeaterLabel, LV_ALIGN_CENTER, 5, 0);
  lv_label_set_text(ui_HeaterLabel, "Heater");
  // lv_obj_set_width(ui_HeaterLabel, 65); /*Set smaller width to make the lines wrap*/
  lv_obj_set_style_text_align(ui_HeaterLabel, LV_TEXT_ALIGN_CENTER, 0);

  return ui_Heater;
}
#endif