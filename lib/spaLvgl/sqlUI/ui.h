// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SPA

#ifndef _SPA_UI_H
#define _SPA_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
  #if __has_include("lvgl.h")
    #include "lvgl.h"
  #elif __has_include("lvgl/lvgl.h")
    #include "lvgl/lvgl.h"
  #else
    #include "lvgl.h"
  #endif
#else
  #include "lvgl.h"
#endif

#include "ui_helpers.h"
#include "components/ui_comp.h"
#include "components/ui_comp_hook.h"
#include "ui_events.h"
#include "ui_theme_manager.h"
#include "ui_themes.h"

// SCREEN: ui_Loading_Screen
void ui_Loading_Screen_screen_init(void);
extern lv_obj_t *ui_Loading_Screen;
extern lv_obj_t *ui_ThermostatLoading;
extern lv_obj_t *ui_HeatControlsLoading;
extern lv_obj_t *ui_uiHeatStateLoading;
extern lv_obj_t *ui_heatSwtichLabelLoading;
extern lv_obj_t *ui_heatStateSwitchlLoading;
extern lv_obj_t *ui_uiTempRangelLoading;
extern lv_obj_t *ui_tempRangeHighLabellLoading;
extern lv_obj_t *ui_tempRangeSwitchlLoading;
extern lv_obj_t *ui_tempRangeLowLabellLoading;
extern lv_obj_t *ui_uiPumpLoading;
extern lv_obj_t *ui_uiPumpLoading2;
extern lv_obj_t *ui_uiLightLoading;
extern lv_obj_t *ui_uiFilterLoading;
extern lv_obj_t *ui_uiTemperatureHistory2;
extern lv_obj_t *ui_uiTemperatureChart2;
extern lv_obj_t *ui_uiTemperatureChart2_Xaxis;
extern lv_obj_t *ui_uiTemperatureChart2_Yaxis1;
extern lv_obj_t *ui_uiTemperatureChart2_Yaxis2;
extern lv_obj_t *ui_temperatureChartLabel2;
extern lv_obj_t *ui_uiHeaterHistory2;
extern lv_obj_t *ui_uiHeaterChart2;
extern lv_obj_t *ui_uiHeaterChart2_Xaxis;
extern lv_obj_t *ui_uiHeaterChart2_Yaxis1;
extern lv_obj_t *ui_uiHeaterChart2_Yaxis2;
extern lv_obj_t *ui_heaterChartLabel2;
extern lv_obj_t *ui_uiClock2;
extern lv_obj_t *ui_uiClockLabel2;
// SCREEN: ui_Spa_Screen
void ui_Spa_Screen_screen_init(void);
void ui_event_Spa_Screen( lv_event_t * e);
extern lv_obj_t *ui_Spa_Screen;
extern lv_obj_t *ui_uiThermostatPlaceholder;
extern lv_obj_t *ui_uiClock;
extern lv_obj_t *ui_uiClockLabel;
extern lv_obj_t *ui_HeatControls;
extern lv_obj_t *ui_uiHeatState;
extern lv_obj_t *ui_heatSwtichLabel;
extern lv_obj_t *ui_heatStateSwitch;
extern lv_obj_t *ui_uiTempRange;
extern lv_obj_t *ui_tempRangeHighLabel;
void ui_event_tempRangeSwitch( lv_event_t * e);
extern lv_obj_t *ui_tempRangeSwitch;
extern lv_obj_t *ui_tempRangeLowLabel;
extern lv_obj_t *ui_uiPump1;
extern lv_obj_t *ui_uiPump2;
extern lv_obj_t *ui_uiLight1;
extern lv_obj_t *ui_uiFilter;
extern lv_obj_t *ui_uiTemperatureHistory;
extern lv_obj_t *ui_uiTemperatureChart;
extern lv_obj_t *ui_uiTemperatureChart_Xaxis;
extern lv_obj_t *ui_uiTemperatureChart_Yaxis1;
extern lv_obj_t *ui_uiTemperatureChart_Yaxis2;
extern lv_obj_t *ui_temperatureChartLabel;
extern lv_obj_t *ui_uiHeaterHistory;
extern lv_obj_t *ui_uiHeaterChart;
extern lv_obj_t *ui_uiHeaterChart_Xaxis;
extern lv_obj_t *ui_uiHeaterChart_Yaxis1;
extern lv_obj_t *ui_uiHeaterChart_Yaxis2;
extern lv_obj_t *ui_heaterChartLabel;
// SCREEN: ui_settingsAndAbout
void ui_settingsAndAbout_screen_init(void);
void ui_event_settingsAndAbout( lv_event_t * e);
extern lv_obj_t *ui_settingsAndAbout;
extern lv_obj_t *ui_TabView1;
extern lv_obj_t *ui_settingsPage;
extern lv_obj_t *ui_settingsContariner;
extern lv_obj_t *ui_brightnessContainer3;
extern lv_obj_t *ui_brightnessSlider3;
extern lv_obj_t *ui_brightnessLabel3;
extern lv_obj_t *ui_Checkbox2;
extern lv_obj_t *ui_aboutPage;
extern lv_obj_t *ui_Container9;
extern lv_obj_t *ui_aboutLabel;
extern lv_obj_t *ui____initial_actions0;

LV_IMG_DECLARE( ui__temporary_image );
LV_IMG_DECLARE( ui_img_uipumpoff_png);   // assets/uiPumpOff.png
LV_IMG_DECLARE( ui_img_uipumplow_png);   // assets/uiPumpLow.png
LV_IMG_DECLARE( ui_img_uipumphigh_png);   // assets/uiPumpHigh.png
LV_IMG_DECLARE( ui_img_lightoff_png);   // assets/lightOff.png
LV_IMG_DECLARE( ui_img_lighton_png);   // assets/lightOn.png
LV_IMG_DECLARE( ui_img_filteron_png);   // assets/filterOn.png


LV_FONT_DECLARE( ui_font_Montserrat_Bold_14);
LV_FONT_DECLARE( ui_font_Montserrat_Bold_16);
LV_FONT_DECLARE( ui_font_Montserrat_Bold_18);
LV_FONT_DECLARE( ui_font_Open_Sans_12);
LV_FONT_DECLARE( ui_font_Open_Sans_14);
LV_FONT_DECLARE( ui_font_Open_Sans_16);
LV_FONT_DECLARE( ui_font_Open_Sans_18);
LV_FONT_DECLARE( ui_font_Open_Sans_20);
LV_FONT_DECLARE( ui_font_Open_Sans_24);
LV_FONT_DECLARE( ui_font_Open_Sans_26);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_12);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_14);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_16);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_18);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_20);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_24);
LV_FONT_DECLARE( ui_font_Open_Sans_Bold_26);


void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
