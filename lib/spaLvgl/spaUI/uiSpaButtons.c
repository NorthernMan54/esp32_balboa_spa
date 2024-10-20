#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "../sqlUI/ui.h"

void spaLvglClickPump(lv_event_t *e)
{
  log_i("spaLvglClickPump");
}

void spaLvglClickLight(lv_event_t *e)
{
  log_i("spaLvglClickLight");
}

void spaLvglClickButton(lv_event_t *e)
{
  log_i("spaLvglClickButton");
}
#endif