#ifdef SPALVGL
#ifndef SPA_SHARED_H
#define SPA_SHARED_H

#ifdef __cplusplus
extern "C"
{
#endif
#include "lvgl.h"

  lv_obj_t *currentTempNeedle;
  lv_obj_t *highTempNeedle;
  lv_obj_t *lowTempNeedle;

  lv_obj_t *temperatureGuage;
  lv_obj_t *thermostatArc(lv_obj_t *parent);

  void spaLvglClickButton(lv_event_t *e);
  void spaLvglClickPump(lv_event_t *e, int);
  void spaLvglClickLight(lv_event_t *e, int);
  void spaLvglClickHeat(lv_event_t *e);

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif
#endif