// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: SPA

#include "../ui.h"

void ui_Spa_Screen_screen_init(void)
{
ui_Spa_Screen = lv_obj_create(NULL);
lv_obj_remove_flag( ui_Spa_Screen, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_scrollbar_mode(ui_Spa_Screen, LV_SCROLLBAR_MODE_OFF);
lv_obj_set_style_bg_color(ui_Spa_Screen, lv_color_hex(0xBFE0F8), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Spa_Screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_Spa_Screen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_Spa_Screen, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Spa_Screen, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_Spa_Screen, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_Spa_Screen, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_Spa_Screen, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_Spa_Screen, &ui_font_Open_Sans_Bold_18, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_uiThermostatPlaceholder = lv_obj_create(ui_Spa_Screen);
lv_obj_remove_style_all(ui_uiThermostatPlaceholder);
lv_obj_set_width( ui_uiThermostatPlaceholder, 240);
lv_obj_set_height( ui_uiThermostatPlaceholder, 130);
lv_obj_set_x( ui_uiThermostatPlaceholder, -100 );
lv_obj_set_y( ui_uiThermostatPlaceholder, -157 );
lv_obj_set_align( ui_uiThermostatPlaceholder, LV_ALIGN_CENTER );
lv_obj_remove_flag( ui_uiThermostatPlaceholder, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_uiThermostatPlaceholder, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_uiThermostatPlaceholder, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiThermostatPlaceholder, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiThermostatPlaceholder, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_widgetBorder);
ui_object_set_themeable_style_property(ui_uiThermostatPlaceholder, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_widgetBorder);
lv_obj_set_style_border_width(ui_uiThermostatPlaceholder, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_uiThermostatPlaceholder, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_uiThermostatPlaceholder, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiThermostatPlaceholder, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiThermostatPlaceholder, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiThermostatPlaceholder, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_uiClock = lv_obj_create(ui_Spa_Screen);
lv_obj_remove_style_all(ui_uiClock);
lv_obj_set_width( ui_uiClock, LV_SIZE_CONTENT);  /// 250
lv_obj_set_height( ui_uiClock, LV_SIZE_CONTENT);   /// 30
lv_obj_set_x( ui_uiClock, 100 );
lv_obj_set_y( ui_uiClock, -199 );
lv_obj_set_align( ui_uiClock, LV_ALIGN_CENTER );
lv_obj_remove_flag( ui_uiClock, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_uiClock, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_uiClock, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiClock, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiClock, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_widgetBorder);
ui_object_set_themeable_style_property(ui_uiClock, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_widgetBorder);
lv_obj_set_style_border_width(ui_uiClock, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_uiClock, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiClock, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiClock, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiClock, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_uiClock, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_uiClockLabel = lv_label_create(ui_uiClock);
lv_obj_set_width( ui_uiClockLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_uiClockLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_uiClockLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_uiClockLabel,"Mon, 07 Oct 2024 @ 22:30");
lv_obj_set_style_pad_left(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_uiClockLabel, 3, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_HeatControls = lv_obj_create(ui_Spa_Screen);
lv_obj_remove_style_all(ui_HeatControls);
lv_obj_set_width( ui_HeatControls, LV_SIZE_CONTENT);  /// 100
lv_obj_set_height( ui_HeatControls, LV_SIZE_CONTENT);   /// 50
lv_obj_set_x( ui_HeatControls, 123 );
lv_obj_set_y( ui_HeatControls, -127 );
lv_obj_set_align( ui_HeatControls, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_HeatControls,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_HeatControls, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
lv_obj_remove_flag( ui_HeatControls, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_HeatControls, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_HeatControls, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_HeatControls, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_HeatControls, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_widgetBorder);
ui_object_set_themeable_style_property(ui_HeatControls, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_widgetBorder);
lv_obj_set_style_border_width(ui_HeatControls, 3, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_uiHeatState = lv_obj_create(ui_HeatControls);
lv_obj_remove_style_all(ui_uiHeatState);
lv_obj_set_width( ui_uiHeatState, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_uiHeatState, LV_SIZE_CONTENT);   /// 100
lv_obj_set_x( ui_uiHeatState, 60 );
lv_obj_set_y( ui_uiHeatState, 63 );
lv_obj_set_align( ui_uiHeatState, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_uiHeatState,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_uiHeatState, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_remove_flag( ui_uiHeatState, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
ui_object_set_themeable_style_property(ui_uiHeatState, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiHeatState, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
lv_obj_set_style_pad_left(ui_uiHeatState, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiHeatState, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiHeatState, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiHeatState, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_uiHeatState, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_uiHeatState, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_uiHeatState, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_uiHeatState, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_heatSwtichLabel = lv_label_create(ui_uiHeatState);
lv_obj_set_width( ui_heatSwtichLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_heatSwtichLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_heatSwtichLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_heatSwtichLabel,"Heat");

ui_heatStateSwitch = lv_switch_create(ui_uiHeatState);
lv_obj_set_width( ui_heatStateSwitch, 50);
lv_obj_set_height( ui_heatStateSwitch, 25);
lv_obj_set_align( ui_heatStateSwitch, LV_ALIGN_CENTER );
lv_obj_remove_flag( ui_heatStateSwitch, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK | LV_OBJ_FLAG_CLICK_FOCUSABLE );    /// Flags


ui_uiTempRange = lv_obj_create(ui_HeatControls);
lv_obj_remove_style_all(ui_uiTempRange);
lv_obj_set_width( ui_uiTempRange, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_uiTempRange, LV_SIZE_CONTENT);   /// 100
lv_obj_set_x( ui_uiTempRange, 60 );
lv_obj_set_y( ui_uiTempRange, 63 );
lv_obj_set_align( ui_uiTempRange, LV_ALIGN_CENTER );
lv_obj_set_flex_flow(ui_uiTempRange,LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(ui_uiTempRange, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
lv_obj_remove_flag( ui_uiTempRange, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
ui_object_set_themeable_style_property(ui_uiTempRange, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiTempRange, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
lv_obj_set_style_pad_left(ui_uiTempRange, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiTempRange, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiTempRange, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiTempRange, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_row(ui_uiTempRange, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_column(ui_uiTempRange, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_uiTempRange, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_uiTempRange, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_tempRangeHighLabel = lv_label_create(ui_uiTempRange);
lv_obj_set_width( ui_tempRangeHighLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_tempRangeHighLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_tempRangeHighLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_tempRangeHighLabel,"Low");

ui_tempRangeSwitch = lv_switch_create(ui_uiTempRange);
lv_obj_set_width( ui_tempRangeSwitch, 50);
lv_obj_set_height( ui_tempRangeSwitch, 25);
lv_obj_set_align( ui_tempRangeSwitch, LV_ALIGN_CENTER );


ui_tempRangeLowLabel = lv_label_create(ui_uiTempRange);
lv_obj_set_width( ui_tempRangeLowLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_tempRangeLowLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_tempRangeLowLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_tempRangeLowLabel,"High");

ui_uiPump1 = ui_uiPump_create(ui_Spa_Screen);
lv_obj_set_x( ui_uiPump1, -162 );
lv_obj_set_y( ui_uiPump1, 0 );



ui_uiPump2 = ui_uiPump_create(ui_Spa_Screen);
lv_obj_set_x( ui_uiPump2, -29 );
lv_obj_set_y( ui_uiPump2, 0 );



ui_uiLight1 = ui_uiLight_create(ui_Spa_Screen);
lv_obj_set_x( ui_uiLight1, 100 );
lv_obj_set_y( ui_uiLight1, 0 );



ui_uiFilter = ui_uiFilter_create(ui_Spa_Screen);
lv_obj_set_x( ui_uiFilter, 197 );
lv_obj_set_y( ui_uiFilter, 0 );



ui_uiThermostat = lv_obj_create(ui_Spa_Screen);
lv_obj_remove_style_all(ui_uiThermostat);
lv_obj_set_width( ui_uiThermostat, 120);
lv_obj_set_height( ui_uiThermostat, 100);
lv_obj_set_x( ui_uiThermostat, -193 );
lv_obj_set_y( ui_uiThermostat, -187 );
lv_obj_set_align( ui_uiThermostat, LV_ALIGN_CENTER );
lv_obj_add_flag( ui_uiThermostat, LV_OBJ_FLAG_HIDDEN );   /// Flags
lv_obj_remove_flag( ui_uiThermostat, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_radius(ui_uiThermostat, 8, LV_PART_MAIN| LV_STATE_DEFAULT);
ui_object_set_themeable_style_property(ui_uiThermostat, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiThermostat, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
ui_object_set_themeable_style_property(ui_uiThermostat, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_COLOR, _ui_theme_color_widgetBorder);
ui_object_set_themeable_style_property(ui_uiThermostat, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_BORDER_OPA, _ui_theme_alpha_widgetBorder);
lv_obj_set_style_border_width(ui_uiThermostat, 3, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_side(ui_uiThermostat, LV_BORDER_SIDE_FULL, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_left(ui_uiThermostat, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_right(ui_uiThermostat, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_top(ui_uiThermostat, 10, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_pad_bottom(ui_uiThermostat, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_uiThermostatLabel = lv_label_create(ui_uiThermostat);
lv_obj_set_width( ui_uiThermostatLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_uiThermostatLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_uiThermostatLabel, LV_ALIGN_BOTTOM_MID );
lv_obj_set_flex_flow(ui_uiThermostatLabel,LV_FLEX_FLOW_COLUMN);
lv_obj_set_flex_align(ui_uiThermostatLabel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
lv_label_set_text(ui_uiThermostatLabel,"Thermostat");

ui_uiThermostatArc = lv_arc_create(ui_uiThermostat);
lv_obj_set_width( ui_uiThermostatArc, 100);
lv_obj_set_height( ui_uiThermostatArc, 100);
lv_obj_set_align( ui_uiThermostatArc, LV_ALIGN_CENTER );
lv_arc_set_range(ui_uiThermostatArc, 20,40);
lv_arc_set_value(ui_uiThermostatArc, 36);
lv_arc_set_bg_angles(ui_uiThermostatArc,180,0);


lv_obj_add_event_cb(ui_tempRangeSwitch, ui_event_tempRangeSwitch, LV_EVENT_ALL, NULL);

}
