#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaPumpButton(lv_obj_t *parent, char *label)
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

  lv_obj_t *ui_Pump1 = lv_imagebutton_create(container);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &ui_img_pumpoff_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &ui_img_pumpoff_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, &ui_img_pumplow_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_pumplow_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_obj_set_size(ui_Pump1, BUTTON_WIDTH, BUTTON_HEIGHT);
  // lv_obj_set_height( ui_Pump1, 174);
  // lv_obj_set_width( ui_Pump1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_state(ui_Pump1, LV_STATE_DISABLED, true);
  lv_obj_set_align(ui_Pump1, LV_ALIGN_CENTER);

  lv_obj_t *ui_PumpLabel = lv_label_create(container);
  lv_obj_align(ui_PumpLabel, LV_ALIGN_CENTER, 5, 0);
  lv_label_set_text(ui_PumpLabel, label);
  // lv_obj_set_width(ui_PumpLabel, 65); /*Set smaller width to make the lines wrap*/
  lv_obj_set_style_text_align(ui_PumpLabel, LV_TEXT_ALIGN_CENTER, 0);
  return ui_Pump1;
}

#endif