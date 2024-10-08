#ifdef SPALVGL

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#ifdef SQUARELINE
#include <ui/ui.h>
#else
#include <spaUI/ui.h>
#endif

#include <spaMessage.h>
#include <spaUtilities.h>

bool loading = true;

void OnAddOneClicked(lv_event_t *e)
{
  static uint32_t cnt = 0;
  cnt++;
  //  lv_label_set_text_fmt(ui_lblCountValue, "%u", cnt);
  log_i("OnAddOneClickedn: %lu", cnt);
}

void OnRotateClicked(lv_event_t *e)
{
  auto disp = lv_disp_get_default();
  auto rotation = (lv_display_rotation_t)((lv_disp_get_rotation(disp) + 1) % (LV_DISPLAY_ROTATION_270 + 1));
  lv_display_set_rotation(disp, rotation);
  // lv_label_set_text_fmt(ui_lblRotationValue, "%u", (int)rotation);
  log_i("OnRotateClicked: disp=%d, rotation=%d", (int)disp, (int)rotation);
}

void spaLvglSetup()
{
  smartdisplay_init();
  log_i("LV_USE_LOG %d", LV_USE_LOG);
#ifdef LV_USE_LOG
  log_i("LV_LOG_LEVEL %d", LV_LOG_LEVEL);
  log_i("LV_LOG_PRINTF %d", LV_LOG_PRINTF);
#endif

  __attribute__((unused)) auto disp = lv_disp_get_default();
// lv_disp_set_rotation(disp, LV_DISP_ROT_90);
// lv_disp_set_rotation(disp, LV_DISP_ROT_180);
// lv_disp_set_rotation(disp, LV_DISP_ROT_270);
#ifdef SQUARELINE
  log_i("ui_init");
  ui_init();
#else
  log_i("spa_ui_init");
  spa_ui_init();
#endif
  /*
  // To use third party libraries, enable the define in lv_conf.h: #define LV_USE_QRCODE 1
  auto ui_qrcode = lv_qrcode_create(ui_scrMain);
  lv_qrcode_set_size(ui_qrcode, 100);
  lv_qrcode_set_dark_color(ui_qrcode, lv_color_black());
  lv_qrcode_set_light_color(ui_qrcode, lv_color_white());
  const char *qr_data = "https://github.com/rzeldent/esp32-smartdisplay";
  lv_qrcode_update(ui_qrcode, qr_data, strlen(qr_data));
  lv_obj_center(ui_qrcode);
  */
}

ulong next_millis;
auto lv_last_tick = millis();

uint8_t currentCrc = 0;

void spaLvglLoop()
{

  if (currentCrc != spaStatusData.crc)
  {
    if (loading)
    {
      loading = false;
      _ui_screen_change(&ui_Spa_Screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Spa_Screen_screen_init);
      lv_obj_clean(ui_Loading_Screen);
      lv_obj_clean(ui_Loading_Pump_Off);
      lv_obj_clean(ui_Loading_Pump_High);
      lv_obj_clean(ui_Loading_Filter_Off);
      lv_obj_clean(ui_Loading_Spa_Light_On);
      lv_obj_clean(ui_Container3);
      lv_obj_clean(ui_Loading_Filter_On);
      lv_obj_clean(ui_Loading_Spa_Light_Off);
    }
    currentCrc = spaStatusData.crc;

    struct tm timeinfo;
    char day_output[30], final_output[30];
    while (!getLocalTime(&timeinfo, 10))
    { // Wait for 5-sec for time to synchronise
      Log.error("[EPaper]: Failed to obtain time");
      return;
    }

    sprintf(day_output, "%s, %02u %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    sprintf(final_output, "%s @ %s", day_output, spaStatusData.time); // Creates: '14:05:49'

#ifdef SQUARELINE
    lv_label_set_text(ui_uiClockLabel, final_output);
    lv_arc_set_value(ui_uiThermostatArc, spaStatusData.currentTemp);
    switch (spaStatusData.pump1)
    {
    case 0:
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
      lv_obj_set_state(ui_uiPumpButton_1, LV_STATE_DISABLED, false);

      break;
    case 1:
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
      lv_imagebutton_set_src(ui_uiPumpButton_1, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumplow_png, NULL);
      lv_obj_set_state(ui_uiPumpButton_1, LV_STATE_CHECKED, false);
      break;
    case 2:
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiPump_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
      lv_imagebutton_set_src(ui_uiPumpButton_1, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumphigh_png, NULL);
      lv_obj_set_state(ui_uiPumpButton_1, LV_STATE_CHECKED, false);
      break;
    }
    lv_obj_refresh_style(ui_uiPumpButton_1, LV_PART_ANY, LV_STYLE_PROP_ANY);

    if (spaStatusData.light1)
    {
      ui_object_set_themeable_style_property(ui_uiLight_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiLight_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
      lv_obj_set_state(ui_uiLight_1, LV_STATE_DISABLED, false);
    }
    else
    {
      ui_object_set_themeable_style_property(ui_uiLight_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
      ui_object_set_themeable_style_property(ui_uiLight_1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
      lv_obj_set_state(ui_uiLight_1, LV_STATE_CHECKED, false);
    }
    lv_obj_refresh_style(ui_uiLight_1, LV_PART_ANY, LV_STYLE_PROP_ANY);

    if (spaStatusData.filterMode)
    {
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
    }
    else
    {
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
    }

#else
    uiUpdateClock(final_output);
    uiUpdateThermostat(spaStatusData.currentTemp, spaStatusData.highSetTemp, spaStatusData.lowSetTemp);
    uiUpdateButtons(spaStatusData.pump1, spaStatusData.pump2, spaStatusData.light1, spaStatusData.filterMode);
    uiUpdateHeater(spaStatusData.heatingState, spaStatusData.tempRange);
#endif
  }

  auto const now = millis();
  // Update the ticker
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;
  // Update the UI
  lv_timer_handler();
}

#endif