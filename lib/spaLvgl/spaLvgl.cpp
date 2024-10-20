#ifdef SPALVGL

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <lvgl.h> // Add this line to include the necessary header file

#include <sqlUI/ui.h>

#include <spaMessage.h>
#include <spaUtilities.h>
#include "./spaUi/uiSpaShared.h"
#include "spaLvgl.h"

bool loading = true;

static const char *temperatureLabels[] = {"24h", "18h", "12h", "6h", "0h", NULL};
static const char *heaterLabels[] = {"24d", "18d", "12d", "6d", "0d", NULL};

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

/**
 * @brief Set the Style object
 *
 * @param components
 * @param radius
 */
void setStyle(lv_obj_t *components[], lv_coord_t radius)
{
  for (uint16_t i = 0; components[i] != nullptr; i++)
  {
    // lv_obj_set_style_radius(components[i], radius, LV_PART_MAIN);
  }
}

#define auto_scale_margin 0.05 // Sets the autoscale increment, so axis steps up fter a change of e.g. 3

/**
 * @brief Calulate the min and max range of the graph
 *
 * @param array of graph values
 * @param min Addrees of min value &minYscale
 * @param max Addrees of max value &maxYscale
 */
void calculateMinMaxRange(int32_t *dataArray, int32_t *minYscale, int32_t *maxYscale)
{
  for (int i = 0; i < GRAPH_MAX_READINGS; i++)
  {
    if (dataArray[i] >= (*maxYscale))
      (*maxYscale) = dataArray[i];
    if (dataArray[i] <= (*minYscale))
      (*minYscale) = dataArray[i];
  }
  // log_i("maxYscale: %i, minYscale: %i", (*maxYscale), (*minYscale));
  // log_i("maxYscale: %i, minYscale: %i", ((int32_t)(((*maxYscale) + ((*maxYscale) - (*minYscale)) * auto_scale_margin) / 10) + 1), (((*minYscale) - ((*maxYscale) - (*minYscale)) * auto_scale_margin) / 10 + 1) );
  (*maxYscale) = ((int32_t)(((*maxYscale) + ((*maxYscale) - (*minYscale)) * auto_scale_margin) / 10) + 1) * 10;

  if ((*minYscale) > 0)
    (*minYscale) = ((int32_t)(((*minYscale) - ((*maxYscale) - (*minYscale)) * auto_scale_margin) / 10) + 1) * 10;
  if ((*minYscale) < 0)
    (*minYscale) = 0;
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

  log_i("ui_init");
  ui_init();

  lv_obj_t *loading[] = {ui_ThermostatLoading, ui_uiPumpLoading, ui_uiFilterLoading, ui_uiLightLoading, ui_HeatControlsLoading, nullptr};
  setStyle(loading, 8);

  lv_obj_t *components[] = {ui_uiThermostatPlaceholder, ui_uiClock, ui_uiPump1, ui_uiPump2, ui_uiLight1, ui_uiFilter, ui_uiTempRange, ui_uiTemperatureHistory, ui_uiHeaterHistory, nullptr};
  setStyle(components, 8);

  thermostatArc(ui_ThermostatLoading);
  temperatureGuage = thermostatArc(ui_uiThermostatPlaceholder);

  lv_scale_set_text_src(ui_uiTemperatureChart2_Xaxis, temperatureLabels);
  lv_scale_set_text_src(ui_uiHeaterChart2_Xaxis, heaterLabels);

  lv_scale_set_text_src(ui_uiTemperatureChart_Xaxis, temperatureLabels);
  lv_scale_set_text_src(ui_uiHeaterChart_Xaxis, heaterLabels);

  /*
    lv_switch_set_orientation(ui_uiTempRangeSwitchLoading, LV_SWITCH_ORIENTATION_VERTICAL);
    lv_switch_set_orientation(ui_tempRangeSwitch, LV_SWITCH_ORIENTATION_VERTICAL);
    */

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

void spaButtonUpdate(lv_obj_t *component, uint8_t state)
{

  lv_obj_t *image = ui_comp_get_child(component, UI_COMP_UIPUMP_BUTTON);
  switch (state)
  {
  case 1:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);

    lv_imagebutton_set_src(image, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumplow_png, NULL);
    lv_obj_set_state(image, LV_STATE_CHECKED, true);
    break;
  case 2:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
    lv_imagebutton_set_src(image, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui_img_uipumplow_png, NULL);
    lv_obj_set_state(image, LV_STATE_CHECKED, true);
    break;
  case 0:
  default:
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
    ui_object_set_themeable_style_property(component, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
    lv_obj_set_state(image, LV_STATE_CHECKED, false);
    break;
  }
  lv_obj_refresh_style(image, LV_PART_ANY, LV_STYLE_PROP_ANY);
}

ulong next_millis;
auto lv_last_tick = millis();

uint8_t currentCrc = 0;

// Chart primitives
int32_t heaterData[GRAPH_MAX_READINGS] = {0};
int32_t temperatureData[GRAPH_MAX_READINGS] = {0};
lv_chart_series_t *ui_uiHeaterChart_series_1;
lv_chart_series_t *ui_uiTemperatureChart_series_1;

void spaLvglLoop()
{

  if (currentCrc != spaStatusData.crc)
  {
    struct tm timeinfo;
    char day_output[30], final_output[30];
    while (!getLocalTime(&timeinfo, 10))
    { // Wait for 5-sec for time to synchronise
      Log.error("[EPaper]: Failed to obtain time");
      return;
    }

    sprintf(day_output, "%s, %02u %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    sprintf(final_output, "%s @ %s", day_output, spaStatusData.time); // Creates: '14:05:49'

    // Thermostat

    lv_label_set_text(ui_uiClockLabel, final_output);

    // Pump 1

    spaButtonUpdate(ui_uiPump1, spaStatusData.pump1);

    // Pump 2
    spaButtonUpdate(ui_uiPump2, spaStatusData.pump2);

    // Light

    lv_obj_t *lightImage = ui_comp_get_child(ui_uiLight1, UI_COMP_UILIGHT_BUTTON);
    if (spaStatusData.light1)
    {
      ui_object_set_themeable_style_property(ui_uiLight1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiLight1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
      lv_obj_set_state(lightImage, LV_STATE_CHECKED, true);
    }
    else
    {
      ui_object_set_themeable_style_property(ui_uiLight1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
      ui_object_set_themeable_style_property(ui_uiLight1, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
      lv_obj_set_state(lightImage, LV_STATE_CHECKED, false);
    }
    lv_obj_refresh_style(lightImage, LV_PART_ANY, LV_STYLE_PROP_ANY);

    // Filter

    if (spaStatusData.filterMode)
    {
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOnBkg);
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOnBkg);
      lv_obj_set_state(ui_uiFilter, LV_STATE_CHECKED, true);
    }
    else
    {
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_AccessoryOffBkg);
      ui_object_set_themeable_style_property(ui_uiFilter, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_AccessoryOffBkg);
      lv_obj_set_state(ui_uiFilter, LV_STATE_CHECKED, false);
    }

    // Heating Controls

    if (spaStatusData.heatingState)
    {
      lv_obj_set_state(ui_heatStateSwitch, LV_STATE_CHECKED, true);
    }
    else
    {
      lv_obj_set_state(ui_heatStateSwitch, LV_STATE_CHECKED, false);
    }
    if (spaStatusData.tempRange)
    {
      lv_obj_set_state(ui_tempRangeSwitch, LV_STATE_CHECKED, true);
    }
    else
    {
      lv_obj_set_state(ui_tempRangeSwitch, LV_STATE_CHECKED, false);
    }

    lv_scale_set_line_needle_value(temperatureGuage, currentTempNeedle, 60, spaStatusData.currentTemp);

    int32_t max = -10000;
    int32_t min = 10000;

    // Temperature History

    for (int i = 0; i < GRAPH_MAX_READINGS; i++)
    {
      temperatureData[GRAPH_MAX_READINGS - 1 - i] = (int32_t)spaStatusData.temperatureHistory[i];
      log_i("temperatureData[%d] = %d", i, temperatureData[i]);
    }

    if (temperatureData[GRAPH_MAX_READINGS - 1] == 0)
    {
      temperatureData[GRAPH_MAX_READINGS - 1] = spaStatusData.currentTemp;
    }

    calculateMinMaxRange(temperatureData, &min, &max);
    log_i("temperatureData min: %i, max: %i", min, max);
    lv_chart_set_range(ui_uiTemperatureChart, LV_CHART_AXIS_PRIMARY_Y, static_cast<lv_chart_axis_t>(min), static_cast<lv_chart_axis_t>(max));
    lv_scale_set_range(ui_uiTemperatureChart_Yaxis1, min, max);

    ui_uiTemperatureChart_series_1 = lv_chart_add_series(ui_uiTemperatureChart, lv_color_hex(0x808080), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_point_count(ui_uiTemperatureChart, GRAPH_MAX_READINGS);
    lv_chart_set_ext_y_array(ui_uiTemperatureChart, ui_uiTemperatureChart_series_1, temperatureData);
    lv_chart_refresh(ui_uiTemperatureChart);

    // Heater History

    heaterData[GRAPH_MAX_READINGS - 1] = (int32_t)spaStatusData.heaterOnTimeToday;
    for (int i = 0; i < GRAPH_MAX_READINGS - 1; i++)
    {
      heaterData[GRAPH_MAX_READINGS - 2 - i] = (int32_t)spaStatusData.heatOn->history()[i];
      log_i("heaterData[%d] = %d", i, heaterData[i]);
    }

    max = -10000;
    min = 10000;

    calculateMinMaxRange(heaterData, &min, &max);
    log_i("temperatureData min: %i, max: %i", min, max);
    lv_chart_set_range(ui_uiHeaterChart, LV_CHART_AXIS_PRIMARY_Y, static_cast<lv_chart_axis_t>(min), static_cast<lv_chart_axis_t>(max));
    lv_scale_set_range(ui_uiHeaterChart_Yaxis1, min, max);

    ui_uiHeaterChart_series_1 = lv_chart_add_series(ui_uiHeaterChart, lv_color_hex(0x808080), LV_CHART_AXIS_PRIMARY_Y);
    // static lv_coord_t ui_uiHeaterChart_series_1_array[] = { 0,10,20,40,8,32,40,20,10,0,22 };
    // lv_chart_set_ext_y_array(ui_uiHeaterChart, ui_uiHeaterChart_series_1, ui_uiHeaterChart_series_1_array);

    lv_chart_set_point_count(ui_uiHeaterChart, GRAPH_MAX_READINGS);
    lv_chart_set_ext_y_array(ui_uiHeaterChart, ui_uiHeaterChart_series_1, heaterData);
    lv_chart_refresh(ui_uiHeaterChart);

    if (loading)
    {
      loading = false;
      _ui_screen_change(&ui_Spa_Screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Spa_Screen_screen_init);
      lv_obj_clean(ui_Loading_Screen);
    }
    currentCrc = spaStatusData.crc;
  }

  auto const now = millis();
  // Update the ticker
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;
  // Update the UI
  lv_timer_handler();
}

uint8_t *jpegBuffer = nullptr; // Buffer for storing the JPEG output
size_t jpegSize = 0;           // Size of the JPEG in memory

lv_draw_buf_t *snapshot = nullptr;

#ifdef LV_USE_100ASK_SCREENSHOT
#include "./screenShot/lv_100ask_screenshot.h"

size_t captureToJPEG()
{
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  jpegSize = DISPLAY_WIDTH * DISPLAY_HEIGHT * 2;
  if (jpegBuffer != nullptr)
  {
    //  free(jpegBuffer);
    //  jpegBuffer = nullptr;
  }
  else
  {
    jpegBuffer = (uint8_t *)heap_caps_calloc(1, jpegSize, MALLOC_CAP_DEFAULT);
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p, jpegSize: %u", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer, jpegSize);

  if (!lv_100ask_screenshot_memory(lv_scr_act(), LV_COLOR_FORMAT_RGB565, LV_100ASK_SCREENSHOT_SV_PNG, 8, &jpegBuffer, &jpegSize))
  {
    log_i("lv_100ask_screenshot_create failed");
    return 0;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p, jpegSize: %u", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer, jpegSize);

  return jpegSize;
}
#endif
#endif