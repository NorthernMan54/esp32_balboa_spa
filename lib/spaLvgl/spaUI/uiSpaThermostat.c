#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "ui.h"




/**
 * @brief Create a circular scale meter with a needle indicator
 *
 */
lv_obj_t *thermostatArc(lv_obj_t *parent)
{
  lv_obj_t *container = lv_obj_create(parent);

  lv_obj_set_size(container, THERMO_WIDTH, 110);
  lv_obj_set_align(container, LV_ALIGN_CENTER);

  lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *container1 = lv_obj_create(container);

  lv_obj_set_size(container1, THERMO_WIDTH * .7, THERMO_HEIGHT * .7);
  lv_obj_set_align(container1, LV_ALIGN_CENTER);

  lv_obj_remove_flag(container1, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(container1, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(container1, LV_OPA_50, 0);

  temperatureGuage = lv_scale_create(container1);

  lv_scale_set_label_show(temperatureGuage, true);
  //  lv_obj_set_size(temperatureGuage, THERMO_WIDTH * .7, THERMO_HEIGHT * .7);
  lv_scale_set_mode(temperatureGuage, LV_SCALE_MODE_ROUND_OUTER);

  lv_obj_align(temperatureGuage, LV_ALIGN_CENTER, 0, 45);

  lv_scale_set_total_tick_count(temperatureGuage, 21);
  lv_scale_set_major_tick_every(temperatureGuage, 5);

  lv_obj_set_style_length(temperatureGuage, 5, LV_PART_ITEMS);
  lv_obj_set_style_length(temperatureGuage, 10, LV_PART_INDICATOR);
  lv_scale_set_range(temperatureGuage, 20, 40);

  lv_scale_set_angle_range(temperatureGuage, 180);
  lv_scale_set_rotation(temperatureGuage, 180);

  static const char *custom_labels[] = {"20 °C", "25 °C", "30 °C", "35 °C", "40 °C", NULL};
  lv_scale_set_text_src(temperatureGuage, custom_labels);

  static lv_style_t indicator_style;
  lv_style_init(&indicator_style);

  lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
  lv_style_set_text_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));

  lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_width(&indicator_style, 10U);
  lv_style_set_line_width(&indicator_style, 2U);
  lv_obj_add_style(temperatureGuage, &indicator_style, LV_PART_INDICATOR);

  static lv_style_t minor_ticks_style;
  lv_style_init(&minor_ticks_style);
  lv_style_set_line_color(&minor_ticks_style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
  lv_style_set_width(&minor_ticks_style, 5U);
  lv_style_set_line_width(&minor_ticks_style, 2U);
  lv_obj_add_style(temperatureGuage, &minor_ticks_style, LV_PART_ITEMS);

  static lv_style_t main_line_style;
  lv_style_init(&main_line_style);

  lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_arc_width(&main_line_style, 2U);
  lv_obj_add_style(temperatureGuage, &main_line_style, LV_PART_MAIN);

  static lv_style_t section_minor_tick_style;
  static lv_style_t section_label_style;
  static lv_style_t section_main_line_style;

  lv_style_init(&section_label_style);
  lv_style_init(&section_minor_tick_style);
  lv_style_init(&section_main_line_style);

  lv_style_set_text_font(&section_label_style, LV_FONT_DEFAULT);
  lv_style_set_text_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));

  lv_style_set_line_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_line_width(&section_label_style, 5U);

  lv_style_set_line_color(&section_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
  lv_style_set_line_width(&section_minor_tick_style, 4U);

  lv_style_set_arc_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_arc_width(&section_main_line_style, 4U);

  lv_scale_section_t *section = lv_scale_add_section(temperatureGuage);
  lv_scale_section_set_range(section, 36, 40);
  lv_scale_section_set_style(section, LV_PART_INDICATOR, &section_label_style);
  lv_scale_section_set_style(section, LV_PART_ITEMS, &section_minor_tick_style);
  lv_scale_section_set_style(section, LV_PART_MAIN, &section_main_line_style);

  /*
    lv_obj_set_style_bg_color(scale, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
    lv_obj_set_style_bg_opa(scale, LV_OPA_50, 0);
    lv_obj_set_style_pad_left(scale, 0, 0);
    lv_obj_set_style_radius(scale, 0, 0);
    lv_obj_set_style_pad_ver(scale, 0, 0);
    */

  currentTempNeedle = lv_line_create(temperatureGuage);

  lv_obj_set_style_line_width(currentTempNeedle, 6, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(currentTempNeedle, true, LV_PART_MAIN);
  lv_obj_set_style_line_color(currentTempNeedle, lv_palette_main(LV_PALETTE_RED), 0);

  return container;
}
#endif