#ifdef SPALVGL
#ifndef EXAMPLE_METER_H
#define EXAMPLE_METER_H

#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"

lv_obj_t *thermostatLine(lv_obj_t *);
lv_obj_t *chart_obj(lv_obj_t *);
lv_obj_t *scale_3(lv_obj_t *);
lv_obj_t *thermostatArc(lv_obj_t *);
lv_obj_t *scale_5(lv_obj_t *);
lv_obj_t *pump_button_1(lv_obj_t *);
lv_obj_t *pump_button_2(lv_obj_t *);
lv_obj_t *lamp_button_1(lv_obj_t *);
lv_obj_t *spa_clock_lable(lv_obj_t *);

lv_obj_t *example_scale_4(lv_obj_t *);

void uiUpdateClock(const char *);
void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
#endif