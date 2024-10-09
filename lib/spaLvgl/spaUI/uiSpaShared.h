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

#ifdef __cplusplus
} /*extern "C"*/
#endif
#endif
#endif