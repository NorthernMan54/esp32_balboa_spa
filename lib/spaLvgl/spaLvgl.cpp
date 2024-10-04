#ifdef LVGL

#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>

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
  log_i("ui_init");
  ui_init();
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

void spaLvglLoop()
{
  auto const now = millis();
  if (now > next_millis)
  {
    next_millis = now + 500;

    char text_buffer[32];
    sprintf(text_buffer, "%lu", now);
//    lv_label_set_text(ui_lblMillisecondsValue, text_buffer);

#ifdef BOARD_HAS_RGB_LED
    auto const rgb = (now / 2000) % 8;
    smartdisplay_led_set_rgb(rgb & 0x01, rgb & 0x02, rgb & 0x04);
#endif

#ifdef BOARD_HAS_CDS
    auto cdr = analogReadMilliVolts(CDS);
    sprintf(text_buffer, "%d", cdr);
    lv_label_set_text(ui_lblCdrValue, text_buffer);
#endif
  }

  // Update the ticker
  lv_tick_inc(now - lv_last_tick);
  lv_last_tick = now;
  // Update the UI
  lv_timer_handler();
}

#endif