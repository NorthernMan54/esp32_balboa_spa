#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"

lv_obj_t *spaFilterButton(lv_obj_t *parent)
{
  lv_obj_t *ui_Filter = lv_imagebutton_create(parent);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &ui_img_filterOff_png, NULL);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &ui_img_filterOff_png, NULL);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, &ui_img_filterOn_png, NULL);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_filterOn_png, NULL);
  lv_imagebutton_set_src(ui_Filter, LV_IMAGEBUTTON_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_obj_set_size(ui_Filter, ui_img_filterOff_png.header.w, ui_img_filterOff_png.header.h);
  // lv_obj_set_height( ui_Filter, 174);
  // lv_obj_set_width( ui_Filter, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_state(ui_Filter, LV_STATE_DISABLED, true);
  lv_obj_set_align(ui_Filter, LV_ALIGN_CENTER);
  return ui_Filter;
}
#endif