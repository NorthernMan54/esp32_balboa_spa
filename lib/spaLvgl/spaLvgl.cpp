#ifdef SPALVGL

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <lvgl.h> // Add this line to include the necessary header file

#ifdef SQUARELINE
#include <ui/ui.h>
#else
#include <spaUI/ui.h>
#endif

#include <spaMessage.h>
#include <spaUtilities.h>
#include "./spaUi/uiSpaShared.h"
#include "spaLvgl.h"

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

  lv_obj_t *loading[] = {ui_ThermostatLoading, ui_uiPumpLoading, ui_uiFilterLoading, ui_uiLightLoading, ui_HeatControlsLoading, nullptr};
  setStyle(loading, 8);

  lv_obj_t *components[] = {ui_uiThermostatPlaceholder, ui_uiClock, ui_uiPump1, ui_uiPump2, ui_uiLight1, ui_uiFilter, ui_uiTempRange, ui_uiTemperatureHistory, ui_uiHeaterHistory, nullptr};
  setStyle(components, 8);

  thermostatArc(ui_ThermostatLoading);
  thermostatArc(ui_uiThermostatPlaceholder);

/*
  lv_switch_set_orientation(ui_uiTempRangeSwitchLoading, LV_SWITCH_ORIENTATION_VERTICAL);
  lv_switch_set_orientation(ui_tempRangeSwitch, LV_SWITCH_ORIENTATION_VERTICAL);
  */
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

void spaLvglLoop()
{

  if (currentCrc != spaStatusData.crc)
  {
    if (loading)
    {
      loading = false;
      _ui_screen_change(&ui_Spa_Screen, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_Spa_Screen_screen_init);

      lv_obj_clean(ui_uiPumpLoading);
      lv_obj_clean(ui_uiFilterLoading);
      lv_obj_clean(ui_uiLightLoading);
      lv_obj_clean(ui_HeatControlsLoading);
      lv_obj_clean(ui_uiTempRangelLoading);
      lv_obj_clean(ui_ThermostatLoading);
      lv_obj_clean(ui_Loading_Screen);
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

    // Temperature History

    lv_chart_series_t *ui_uiTemperatureChart_series_1 = lv_chart_add_series(ui_uiTemperatureChart, lv_color_hex(0x808080), LV_CHART_AXIS_PRIMARY_Y);

    int32_t temperatureData[GRAPH_MAX_READINGS] = {0};

    for (int i = 0; i < GRAPH_MAX_READINGS - 1; i++)
    {
      temperatureData[GRAPH_MAX_READINGS - 2 - i] = (int32_t)spaStatusData.temperatureHistory[i];
      log_i("temperatureData[%d] = %d", i, temperatureData[i]);
    }
    temperatureData[GRAPH_MAX_READINGS - 1] = spaStatusData.currentTemp;
    lv_chart_set_point_count(ui_uiTemperatureChart, GRAPH_MAX_READINGS);
    lv_chart_set_ext_y_array(ui_uiTemperatureChart, ui_uiTemperatureChart_series_1, temperatureData);
    lv_chart_refresh(ui_uiTemperatureChart);

    // Heater History

    int32_t heaterData[GRAPH_MAX_READINGS] = {0};

    for (int i = 0; i < GRAPH_MAX_READINGS-1; i++)
    {
      heaterData[GRAPH_MAX_READINGS - 2 - i] = (int32_t)spaStatusData.heatOn->history()[i];
      log_i("heaterData[%d] = %d", i, heaterData[i]);
    }
    heaterData[GRAPH_MAX_READINGS - 1] = (int32_t)spaStatusData.heaterOnTimeToday;
    lv_chart_series_t *ui_uiHeaterChart_series_1 = lv_chart_add_series(ui_uiHeaterChart, lv_color_hex(0x808080), LV_CHART_AXIS_PRIMARY_Y);
    // static lv_coord_t ui_uiHeaterChart_series_1_array[] = { 0,10,20,40,8,32,40,20,10,0,22 };
    // lv_chart_set_ext_y_array(ui_uiHeaterChart, ui_uiHeaterChart_series_1, ui_uiHeaterChart_series_1_array);

    lv_chart_set_point_count(ui_uiHeaterChart, GRAPH_MAX_READINGS);
    lv_chart_set_ext_y_array(ui_uiHeaterChart, ui_uiHeaterChart_series_1, heaterData);
      lv_chart_refresh(ui_uiHeaterChart);

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

uint8_t *jpegBuffer = nullptr; // Buffer for storing the JPEG output
size_t jpegSize = 0;           // Size of the JPEG in memory

lv_draw_buf_t *snapshot = nullptr;

#ifdef LV_USE_100ASK_SCREENSHOT
#include "./screenShot/lv_100ask_screenshot.h"

size_t captureToJPEG()
{
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  if (jpegBuffer != nullptr)
  {
    free(jpegBuffer);
    jpegBuffer = nullptr;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  jpegSize = DISPLAY_WIDTH * DISPLAY_HEIGHT * 4;

  jpegBuffer = (uint8_t *)heap_caps_calloc(1, jpegSize, MALLOC_CAP_DEFAULT);

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  if (!lv_100ask_screenshot_memory(lv_scr_act(), LV_COLOR_FORMAT_ARGB8888, LV_100ASK_SCREENSHOT_SV_PNG, (uint8_t *)(&jpegBuffer), jpegSize))
  {
    log_i("lv_100ask_screenshot_create failed");
    return 0;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  return jpegSize;
}
#endif





size_t captureToJPEGOther()
{
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  if (jpegBuffer != nullptr)
  {
    free(jpegBuffer);
    jpegBuffer = nullptr;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  log_i("snapshot %p", snapshot);
  if (snapshot != nullptr)
  {
    lv_draw_buf_destroy(snapshot);
    snapshot = nullptr;
  }
  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  snapshot = lv_snapshot_take(lv_scr_act(), LV_COLOR_FORMAT_ARGB8888);
  if (snapshot == NULL)
  {
    log_e("snapshot_take failed");
    return 0;
  }

  // lv_image_set_src(img_snapshot, snapshot);
  log_i("snapshot %p", snapshot);
  // encode the screenshot as a PNG using lodePNG
  const unsigned char *image = snapshot->data;
  log_i("header.w");
  uint32_t width = snapshot->header.w;
  log_i("captureToJPEG() - header.h");
  uint32_t height = snapshot->header.h;
  log_i("captureToJPEG() - data_size");
  jpegSize = snapshot->data_size;
  log_i("width=%u, height=%u, jpegSize=%u", width, height, jpegSize);
  //  lodepng_encode24_file("A:screenshot24.png", image, width, height); //produces garbled image
  //  lodepng_encode32_file("A:screenshot32.png", image, width, height); //produces correct image

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  jpegBuffer = (uint8_t *)heap_caps_calloc(1, DISPLAY_WIDTH * DISPLAY_HEIGHT * 4, MALLOC_CAP_DEFAULT);

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);

  // lv_lodepng_encode_memory(&jpegBuffer, &jpegSize, snapshot->data, width, height, 8);

  log_i("Free Heap: %s, Free PSRAM: %s, Free Stack: %s, jpegBuffer: %p", formatNumberWithCommas(ESP.getFreeHeap()), formatNumberWithCommas(ESP.getFreePsram()), formatNumberWithCommas(uxTaskGetStackHighWaterMark(NULL)), jpegBuffer);
  return jpegSize;
}

#endif