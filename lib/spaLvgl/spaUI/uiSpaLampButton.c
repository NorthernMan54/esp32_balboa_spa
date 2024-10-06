#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaLampButton(lv_obj_t *parent)
{
  lv_obj_t *ui_Lamp = lv_imagebutton_create(parent);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &ui_img_lightOff_png, NULL);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &ui_img_lightOff_png, NULL);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, &ui_img_lightOn_png, NULL);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_lightOn_png, NULL);
  lv_imagebutton_set_src(ui_Lamp, LV_IMAGEBUTTON_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_obj_set_size(ui_Lamp, BUTTON_WIDTH, BUTTON_HEIGHT);
  // lv_obj_set_height( ui_Lamp, 174);
  // lv_obj_set_width( ui_Lamp, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_state(ui_Lamp, LV_STATE_DISABLED, true);
  lv_obj_set_align(ui_Lamp, LV_ALIGN_CENTER);
  return ui_Lamp;
}
#endif