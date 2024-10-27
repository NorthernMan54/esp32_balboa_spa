#ifdef SPALVGL
// #include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "./sqlUI/ui.h"

#define THERMO_WIDTH 120
#define THERMO_HEIGHT 100

/**
 * @brief Create a circular scale meter with a needle indicator
 *
 */
lv_obj_t *thermostatArc(lv_obj_t *parent)
{

  lv_obj_t *scale = lv_scale_create(parent);
  lv_obj_set_size(scale, 100, 100);
  lv_scale_set_label_show(scale, true);
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);
  lv_obj_center(scale);

  lv_obj_align(scale, LV_ALIGN_CENTER, 0, 40);

  lv_scale_set_angle_range(scale, 180);
  lv_scale_set_rotation(scale, 180);

  lv_scale_set_total_tick_count(scale, 21);
  lv_scale_set_major_tick_every(scale, 5);

  lv_obj_set_style_length(scale, 5, LV_PART_ITEMS);
  lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);
  lv_scale_set_range(scale, 20, 40);

  static const char *custom_labels[] = {"20 °C", "25 °C", "30 °C", "35 °C", "40 °C", NULL};
  lv_scale_set_text_src(scale, custom_labels);

  static lv_style_t indicator_style;
  lv_style_init(&indicator_style);

  /* Label style properties */
  lv_style_set_text_font(&indicator_style, &ui_font_Open_Sans_Bold_16);
  // lv_obj_set_style_text_font(ui_uiTemperatureChart2_Xaxis, &ui_font_Open_Sans_Bold_12, LV_PART_MAIN| LV_STATE_DEFAULT);
  // lv_style_set_text_color(&indicator_style, lv_color_black());

  /* Major tick properties */
  lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_GREY, 3));
  lv_style_set_width(&indicator_style, 10U);     /*Tick length*/
  lv_style_set_line_width(&indicator_style, 2U); /*Tick width*/
  lv_obj_add_style(scale, &indicator_style, LV_PART_INDICATOR);

  static lv_style_t minor_ticks_style;
  lv_style_init(&minor_ticks_style);
  lv_style_set_line_color(&minor_ticks_style, lv_palette_darken(LV_PALETTE_GREY, 2));
  lv_style_set_width(&minor_ticks_style, 5U);      /*Tick length*/
  lv_style_set_line_width(&minor_ticks_style, 2U); /*Tick width*/
  lv_obj_add_style(scale, &minor_ticks_style, LV_PART_ITEMS);

  static lv_style_t main_line_style;
  lv_style_init(&main_line_style);
  /* Main line properties */
  lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_GREY, 3));
  lv_style_set_arc_width(&main_line_style, 2U); /*Tick width*/
  lv_obj_add_style(scale, &main_line_style, LV_PART_MAIN);

  lv_obj_add_flag(scale, LV_OBJ_FLAG_OVERFLOW_VISIBLE);

  currentTempNeedle = lv_line_create(scale);
  lv_obj_set_style_line_width(currentTempNeedle, 6, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(currentTempNeedle, true, LV_PART_MAIN);
  lv_obj_set_style_line_color(currentTempNeedle, lv_palette_darken(LV_PALETTE_RED, 1), 0);

  lv_scale_set_line_needle_value(scale, currentTempNeedle, -1, 35);

  highTempNeedle = lv_image_create(scale);
  lv_image_set_src(highTempNeedle, &ui_img_hightemp_png);
  lv_obj_align(highTempNeedle, LV_ALIGN_CENTER, 47, -2);
  lv_image_set_pivot(highTempNeedle, 3, 7);
  lv_scale_set_image_needle_value(scale, highTempNeedle, 37);
  lv_obj_set_style_image_recolor(highTempNeedle, lv_color_hex(0xA2741E), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_image_recolor_opa(highTempNeedle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  lowTempNeedle = lv_image_create(scale);
  lv_image_set_src(lowTempNeedle, &ui_img_hightemp_png);
  lv_obj_align(lowTempNeedle, LV_ALIGN_CENTER, 47, -2);
  lv_image_set_pivot(lowTempNeedle, 3, 7);
  lv_scale_set_image_needle_value(scale, lowTempNeedle, 22);
  lv_obj_set_style_image_recolor(lowTempNeedle, lv_color_hex(0x10AAF0), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_image_recolor_opa(lowTempNeedle, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  return scale;
}
#endif