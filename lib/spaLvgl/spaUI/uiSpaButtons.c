#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "../sqlUI/ui.h"

void spaLvglClickPump(lv_event_t *e, int pump)
{
  log_i("spaLvglClickPump %i", pump);
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = lv_event_get_user_data(e);

  switch (code)
  {
  case LV_EVENT_PRESSED:
    log_i( "The last button event:\nLV_EVENT_PRESSED");
    break;
  case LV_EVENT_CLICKED:
    log_i( "The last button event:\nLV_EVENT_CLICKED");
    break;
  case LV_EVENT_LONG_PRESSED:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED");
    break;
  case LV_EVENT_LONG_PRESSED_REPEAT:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
    break;
  default:
    break;
  }
}


void spaLvglClickLight(lv_event_t *e, int light)
{
  log_i("spaLvglClickLight %i", light);
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = lv_event_get_user_data(e);

  switch (code)
  {
  case LV_EVENT_PRESSED:
    log_i( "The last button event:\nLV_EVENT_PRESSED");
    break;
  case LV_EVENT_CLICKED:
    log_i( "The last button event:\nLV_EVENT_CLICKED");
    break;
  case LV_EVENT_LONG_PRESSED:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED");
    break;
  case LV_EVENT_LONG_PRESSED_REPEAT:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
    break;
  default:
    break;
  }
}

void spaLvglClickHeat(lv_event_t *e)
{
  log_i("spaLvglClickButton");
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *label = lv_event_get_user_data(e);

  switch (code)
  {
  case LV_EVENT_PRESSED:
    log_i( "The last button event:\nLV_EVENT_PRESSED");
    break;
  case LV_EVENT_CLICKED:
    log_i( "The last button event:\nLV_EVENT_CLICKED");
    break;
  case LV_EVENT_LONG_PRESSED:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED");
    break;
  case LV_EVENT_LONG_PRESSED_REPEAT:
    log_i( "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
    break;
  default:
    break;
  }
}
#endif