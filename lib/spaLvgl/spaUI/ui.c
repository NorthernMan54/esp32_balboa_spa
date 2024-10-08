#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>
#include "ui.h"

lv_obj_t *spaPumpButton1;
lv_obj_t *spaPumpButton2;
lv_obj_t *spaLamp;
lv_obj_t *spaFilter;
lv_obj_t *spaHeater;

void spa_ui_init(void)
{
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
  lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
  lv_style_set_layout(&style, LV_LAYOUT_FLEX);

  lv_obj_t *cont = lv_obj_create(lv_screen_active());
  lv_obj_set_size(cont, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_obj_center(cont);
  lv_obj_add_style(cont, &style, 0);

  uint32_t i;

  lv_obj_t *obj;
  obj = decorateTile(thermostatArc(cont));
  obj = decorateTile(spa_clock_lable(cont));
  obj = decorateTile(chart_obj(cont));

  spaPumpButton1 = decorateTile(spaPumpButton(cont, "Pump 1"));
  spaPumpButton2 = decorateTile(spaPumpButton(cont, "Pump 2"));
  spaLamp = decorateTile(spaLampButton(cont));
  spaFilter = decorateTile(spaFilterButton(cont));
  spaHeater = decorateTile(spaHeaterDisplay(cont));
}

lv_obj_t *decorateTile(lv_obj_t *tile)
{
  lv_obj_set_style_bg_color(tile, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(tile, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(tile, 8, 0);
  lv_obj_set_style_radius(tile, 8, 0);
  lv_obj_set_style_pad_ver(tile, 8, 0);
  return tile;
}

lv_obj_t *chart_obj(lv_obj_t *parent)
{
  /*Create a chart*/
  lv_obj_t *chart = lv_chart_create(parent);
  lv_obj_set_size(chart, CHART_WIDTH, CHART_HEIGHT);
  lv_obj_center(chart);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/

  /*Add two data series*/
  lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_SECONDARY_Y);
  int32_t *ser2_y_points = lv_chart_get_y_array(chart, ser2);

  uint32_t i;
  for (i = 0; i < 10; i++)
  {
    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(chart, ser1, lv_rand(10, 50));

    /*Directly set points on 'ser2'*/
    ser2_y_points[i] = lv_rand(50, 90);
  }

  lv_chart_refresh(chart); /*Required after direct set*/
  return chart;
}

void uiUpdateThermostat(float currentTemp, float highSetPoint, float lowSetPoint)
{
  // log_i("uiUpdateThermostat currentTemp=%f, highSetPoint=%f, lowSetPoint=%f", currentTemp, highSetPoint, lowSetPoint);
  lv_scale_set_line_needle_value(temperatureGuage, currentTempNeedle, 60, currentTemp);
}

void uiUpdateButton(lv_obj_t *button, u_int8_t state)
{
  // log_i("uiUpdateButton: %d", state);
  lv_obj_set_state(button, LV_STATE_DISABLED, false); 
  if (state)
  {
    lv_obj_set_state(button, LV_STATE_CHECKED, true);
  }
  else
  {
    lv_obj_set_state(button, LV_STATE_CHECKED, false);
  }
  lv_obj_refresh_style(button, LV_PART_ANY, LV_STYLE_PROP_ANY);
}

void uiUpdateButtons(uint8_t pump1, uint8_t pump2, bool lamp, uint8_t filter)
{
  // log_i("uiUpdateButtons pump1=%d, pump2=%d, lamp=%d", pump1, pump2, lamp);
  uiUpdateButton(spaPumpButton1, pump1);
  uiUpdateButton(spaPumpButton2, pump2);
  uiUpdateButton(spaLamp, (uint8_t)lamp);
  uiUpdateButton(spaFilter, filter);
}

void uiUpdateHeater(uint8_t heater, uint8_t tempRange)
{
  log_i("uiUpdateHeater: heater: %d, tempRange: %d ", heater, tempRange);
  lv_led_set_brightness(spaHeater, 150);
  if (heater)
  {
    lv_led_on(spaHeater);
  }
  else
  {
    lv_led_off(spaHeater);
  }
  // lv_obj_refresh_style(spaHeater, LV_PART_ANY, LV_STYLE_PROP_ANY);
}
#endif
