#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "../sqlUI/ui.h"

void spaButtonUpdate(lv_obj_t *component, uint8_t state)
{

  lv_obj_t *image = ui_comp_get_child(component, UI_COMP_UIPUMP_BUTTON);
  switch (state)
  {
  case 1:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);

    lv_imagebutton_set_src(image, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumplow_png, NULL);
    lv_obj_set_state(image, LV_STATE_CHECKED, true);
    break;
  case 2:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
    lv_imagebutton_set_src(image, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumplow_png, NULL);
    lv_obj_set_state(image, LV_STATE_CHECKED, true);
    break;
  case 0:
  default:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
    lv_obj_set_state(image, LV_STATE_CHECKED, false);
    break;
  }
  lv_obj_refresh_style(image, LV_PART_ANY, LV_STYLE_PROP_ANY);
}

#ifdef LV_USE_100ASK_SCREENSHOT
uint8_t *jpegBuffer = nullptr; // Buffer for storing the JPEG output
size_t jpegSize = 0;           // Size of the JPEG in memory

lv_draw_buf_t *snapshot = nullptr;

#include "./screenShot/lv_100ask_screenshot.h"

size_t captureToJPEG()
{
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  jpegSize = DISPLAY_WIDTH * DISPLAY_HEIGHT * 2;
  if (jpegBuffer != nullptr)
  {
    //  free(jpegBuffer);
    //  jpegBuffer = nullptr;
  }
  else
  {
    jpegBuffer = (uint8_t *)heap_caps_calloc(1, jpegSize, MALLOC_CAP_DEFAULT);
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p, jpegSize: %u", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer, jpegSize);

  if (!lv_100ask_screenshot_memory(lv_scr_act(), LV_COLOR_FORMAT_RGB565, LV_100ASK_SCREENSHOT_SV_PNG, 8, &jpegBuffer, &jpegSize))
  {
    log_i("lv_100ask_screenshot_create failed");
    return 0;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p, jpegSize: %u", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer, jpegSize);

  return jpegSize;
}
#endif

#endif