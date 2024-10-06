#ifdef SPALVGL
#ifndef EXAMPLE_METER_H
#define EXAMPLE_METER_H

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100

#define SCALE_WIDTH 150
#define SCALE_HEIGHT 150

#define THERMO_WIDTH 200
#define THERMO_HEIGHT 150

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100

#define CHART_WIDTH 200
#define CHART_HEIGHT 150

#define TEXT_SIZE 50

#ifdef __cplusplus
extern "C"
{
#endif
#include "lvgl.h"

  LV_IMG_DECLARE(ui__temporary_image);
  LV_IMG_DECLARE(ui_img_pumpoff_png);
  LV_IMG_DECLARE(ui_img_pumplow_png);
  LV_IMG_DECLARE(ui_img_pumpHigh_png);

  LV_IMG_DECLARE(ui_img_lightOff_png);
  LV_IMG_DECLARE(ui_img_lightOn_png);

  LV_IMG_DECLARE(ui_img_filterOff_png);
  LV_IMG_DECLARE(ui_img_filterOn_png);

  LV_IMAGE_DECLARE(img_hand);

  lv_obj_t *thermostatArc(lv_obj_t *);
  lv_obj_t *spa_clock_lable(lv_obj_t *);
  lv_obj_t *chart_obj(lv_obj_t *);

  lv_obj_t *spaPumpButton(lv_obj_t *);
  lv_obj_t *spaLampButton(lv_obj_t *);
  lv_obj_t *spaFilterButton(lv_obj_t *);
  lv_obj_t *spaHeaterDisplay(lv_obj_t *);

  /*
    lv_obj_t *thermostatLine(lv_obj_t *);

    lv_obj_t *scale_3(lv_obj_t *);

    lv_obj_t *scale_5(lv_obj_t *);
    lv_obj_t *pump_button_1(lv_obj_t *);
    lv_obj_t *pump_button_2(lv_obj_t *);
    lv_obj_t *lamp_button_1(lv_obj_t *);

    lv_obj_t *example_scale_4(lv_obj_t *);

    void spaPumpButtonState();

    void spaLampButtonState();

    void uiUpdateClock(const char *);
    */

  /**
   * @brief uiUpdateClock - Update the clock display
   * From the file uiSpaClock.c
   */
  void uiUpdateClock(const char *);

  /**
   * @brief uiUpdateHeater - Update the heater display
   * heater - 0=OFF, 1=ON
   * tempRange - 0=Low, 1=High
   *
   */
  void uiUpdateHeater(uint8_t heater, uint8_t tempRange);
  
  /**
   * @brief uiUpdateThermostat
   * current - current temperature
   * high - high setpoint
   * low - low setpoint
   */
  void uiUpdateThermostat(float, float, float);

  /**
   * @brief uiUpdateButtons
   * pump1 - 0=OFF, 1=Low, 2=High
   * pump2 - 0=OFF, 1=Low, 2=High
   * lamp - 0=OFF, 1=ON
   * filter - 0=OFF, 1=ON
   */
  void uiUpdateButtons(uint8_t pump1, uint8_t pump2, bool lamp, uint8_t filter);

  lv_obj_t *decorateTile(lv_obj_t *);

  lv_obj_t *currentTempNeedle;
  lv_obj_t *highTempNeedle;
  lv_obj_t *lowTempNeedle;

  lv_obj_t *temperatureGuage;

  void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
#endif