#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>

#include "uiSpaShared.h"
#include "../sqlUI/ui.h"

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

  /*

      // High Temp Range

      //Add a section
      static lv_style_t high_minor_tick_style;
      static lv_style_t high_label_style;
      static lv_style_t high_main_line_style;

      lv_style_init(&high_label_style);
      lv_style_init(&high_minor_tick_style);
      lv_style_init(&high_main_line_style);

      //Label style properties
      lv_style_set_text_font(&high_label_style, LV_FONT_DEFAULT);
      lv_style_set_text_color(&high_label_style, lv_palette_darken(LV_PALETTE_RED, 3));

      lv_style_set_line_color(&high_label_style, lv_palette_darken(LV_PALETTE_RED, 3));
      lv_style_set_line_width(&high_label_style, 5U); // Tick width

      lv_style_set_line_color(&high_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
      lv_style_set_line_width(&high_minor_tick_style, 4U); // Tick width

      //Main line properties
      lv_style_set_arc_color(&high_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
      lv_style_set_arc_width(&high_main_line_style, 4U); // Tick width

      //Configure section styles
      lv_scale_section_t *highSection = lv_scale_add_section(scale);
      lv_scale_section_set_range(highSection, 35.5, 36.5);
      lv_scale_section_set_style(highSection, LV_PART_INDICATOR, &high_label_style);
      lv_scale_section_set_style(highSection, LV_PART_ITEMS, &high_minor_tick_style);
      lv_scale_section_set_style(highSection, LV_PART_MAIN, &high_main_line_style);

       // High Temp Range

      //Add a section
      static lv_style_t low_minor_tick_style;
      static lv_style_t low_label_style;
      static lv_style_t low_main_line_style;

      lv_style_init(&low_label_style);
      lv_style_init(&low_minor_tick_style);
      lv_style_init(&low_main_line_style);

      //Label style properties
      lv_style_set_text_font(&low_label_style, LV_FONT_DEFAULT);
      lv_style_set_text_color(&low_label_style, lv_palette_darken(LV_PALETTE_GREEN, 3));

      lv_style_set_line_color(&low_label_style, lv_palette_darken(LV_PALETTE_GREEN, 3));
      lv_style_set_line_width(&low_label_style, 5U); // Tick width

      lv_style_set_line_color(&low_minor_tick_style, lv_palette_lighten(LV_PALETTE_GREEN, 2));
      lv_style_set_line_width(&low_minor_tick_style, 4U); // Tick width

      //Main line properties
      lv_style_set_arc_color(&low_main_line_style, lv_palette_darken(LV_PALETTE_GREEN, 3));
      lv_style_set_arc_width(&low_main_line_style, 4U); // Tick width

      //Configure section styles
      lv_scale_section_t *lowSection = lv_scale_add_section(scale);
      lv_scale_section_set_range(lowSection, 25.5, 26.5);
      lv_scale_section_set_style(lowSection, LV_PART_INDICATOR, &low_label_style);
      lv_scale_section_set_style(lowSection, LV_PART_ITEMS, &low_minor_tick_style);
      lv_scale_section_set_style(lowSection, LV_PART_MAIN, &low_main_line_style);
  */

  currentTempNeedle = lv_line_create(scale);

  lv_obj_set_style_line_width(currentTempNeedle, 6, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(currentTempNeedle, true, LV_PART_MAIN);
  lv_obj_set_style_line_color(currentTempNeedle, lv_palette_darken(LV_PALETTE_RED, 1), 0);

  return scale;
}
#endif