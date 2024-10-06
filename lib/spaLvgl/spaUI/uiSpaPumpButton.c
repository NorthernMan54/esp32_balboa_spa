#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaPumpButton(lv_obj_t *parent)
{
  lv_obj_t *ui_Pump1 = lv_imagebutton_create(parent);
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
  return ui_Pump1;
}

#endif