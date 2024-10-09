// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SPA

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////


// SCREEN: ui_Loading_Screen
void ui_Loading_Screen_screen_init(void);
lv_obj_t *ui_Loading_Screen;
lv_obj_t *ui_ThermostatLoading;
lv_obj_t *ui_HeatControlsLoading;
lv_obj_t *ui_uiHeatStateLoading;
lv_obj_t *ui_heatSwtichLabelLoading;
lv_obj_t *ui_heatStateSwitchlLoading;
lv_obj_t *ui_uiTempRangelLoading;
lv_obj_t *ui_tempRangeHighLabellLoading;
lv_obj_t *ui_tempRangeSwitchlLoading;
lv_obj_t *ui_tempRangeLowLabellLoading;
lv_obj_t *ui_uiPumpLoading;
lv_obj_t *ui_uiLightLoading;
lv_obj_t *ui_uiFilterLoading;
lv_obj_t *ui_LoadingContainer;
lv_obj_t *ui_LoadingLabel;


// SCREEN: ui_Spa_Screen
void ui_Spa_Screen_screen_init(void);
lv_obj_t *ui_Spa_Screen;
lv_obj_t *ui_uiThermostatPlaceholder;
lv_obj_t *ui_uiClock;
lv_obj_t *ui_uiClockLabel;
lv_obj_t *ui_HeatControls;
lv_obj_t *ui_uiHeatState;
lv_obj_t *ui_heatSwtichLabel;
lv_obj_t *ui_heatStateSwitch;
lv_obj_t *ui_uiTempRange;
lv_obj_t *ui_tempRangeHighLabel;
void ui_event_tempRangeSwitch( lv_event_t * e);
lv_obj_t *ui_tempRangeSwitch;
lv_obj_t *ui_tempRangeLowLabel;
lv_obj_t *ui_uiPump1;
lv_obj_t *ui_uiPump2;
lv_obj_t *ui_uiLight1;
lv_obj_t *ui_uiFilter;
lv_obj_t *ui_uiThermostat;
lv_obj_t *ui_uiThermostatLabel;
lv_obj_t *ui_uiThermostatArc;
lv_obj_t *ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
void ui_event_tempRangeSwitch( lv_event_t * e) {
    lv_event_code_t event_code = lv_event_get_code(e);lv_obj_t * target = lv_event_get_target(e);
if ( event_code == LV_EVENT_CLICKED) {
      heatRangeSwitch( e );
}
}

///////////////////// SCREENS ////////////////////

void ui_init( void )
{LV_EVENT_GET_COMP_CHILD = lv_event_register_id();

lv_disp_t *dispp = lv_display_get_default();
lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
lv_disp_set_theme(dispp, theme);
ui_Loading_Screen_screen_init();
ui_Spa_Screen_screen_init();
ui____initial_actions0 = lv_obj_create(NULL);
lv_disp_load_scr( ui_Loading_Screen);
}
