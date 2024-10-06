#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>
// #if LV_USE_METER && LV_BUILD_EXAMPLES
// #include "display.h"
#include "ui.h"

// #include "pumpOff.h"
// #include "pumpLow.h"
// #include "pumpHigh.h"

#include "ui_img_pumpoff_png.c"
#include "ui_img_pumplow_png.c"
#include "ui_temporary_image.c"

#include "LightOff.c"
#include "LightOn.c"

#define SCALE_WIDTH 150
#define SCALE_HEIGHT 150

#define THERMO_WIDTH 200
#define THERMO_HEIGHT 150

#define BUTTON_WIDTH 100
#define BUTTON_HEIGHT 100

#define CHART_WIDTH 200
#define CHART_HEIGHT 150

#define TEXT_SIZE 50

static const char *TAG = "GRID";

/**
 * Demonstrate grid's "free unit"
 */
void ui_init_old(void)
{
  /*Column 1: fix width 60 px
   *Column 2: 1 unit from the remaining free space
   *Column 3: 2 unit from the remaining free space*/
  static lv_coord_t col_dsc[] = {SCALE_WIDTH, CHART_WIDTH, SCALE_WIDTH, LV_GRID_TEMPLATE_LAST};

  /*Row 1: fix width 50 px
   *Row 2: 1 unit from the remaining free space
   *Row 3: fix width 50 px*/
  static lv_coord_t row_dsc[] = {SCALE_HEIGHT, SCALE_HEIGHT, SCALE_HEIGHT, LV_GRID_TEMPLATE_LAST};

  /*Create a container with grid*/
  lv_obj_t *cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_obj_center(cont);
  lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

  lv_obj_t *label;
  lv_obj_t *obj;
  uint32_t i;
  for (i = 0; i < 6; i++)
  {
    uint8_t col = i % 2;
    uint8_t row = i / 2;

    if (row == 0)
    {
      if (col == 0)
      {
        log_i("scale_2 i=%lu, col=%d, row=%d, ", i, col, row);
        obj = scale_2(cont);
      }
      else
      {
        log_i("scale_4 i=%lu, col=%d, row=%d, ", i, col, row);
        obj = scale_4(cont);
      }
    }
    else if (row == 1)
    {
      log_i("chart i=%lu, col=%d, row=%d, ", i, col, row);
      obj = chart_obj(cont);
    }
    else
    {
      log_i("scale-needle i=%lu, col=%d, row=%d, ", i, col, row);
      //    obj = scale_3(cont);
    }
    log_i("cell i=%lu, x=%d, x2=%d, y=%d, y2=%d", i, lv_obj_get_x(obj), lv_obj_get_x2(obj), lv_obj_get_y(obj), lv_obj_get_y2(obj));
    /*Stretch the cell horizontally and vertically too
     *Set span to 1 to make the cell 1 column/row sized*/

    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
    lv_obj_set_style_pad_left(obj, 8, 0);
    lv_obj_set_style_radius(obj, 8, 0);
    lv_obj_set_style_pad_ver(obj, 8, 0);

    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_SPACE_AROUND, col, 1,
                         LV_GRID_ALIGN_SPACE_AROUND, row, 1);

    label = lv_label_create(obj);
    lv_label_set_text_fmt(label, "%d,%d", col, row);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);
  }
}

void ui_init(void)
{
  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
  lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
  lv_style_set_layout(&style, LV_LAYOUT_FLEX);

  lv_obj_t *cont = lv_obj_create(lv_screen_active());
  lv_obj_set_size(cont, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_obj_center(cont);
  lv_obj_add_style(cont, &style, 0);

  uint32_t i;

  lv_obj_t *obj = scale_2(cont);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  obj = scale_4(cont);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  obj = scale_5(cont);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  obj = chart_obj(cont);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  /*
    obj = pump_button_1(cont);
    lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
    lv_obj_set_style_pad_left(obj, 8, 0);
    lv_obj_set_style_radius(obj, 8, 0);
    lv_obj_set_style_pad_ver(obj, 8, 0);
  */

  obj = pump_button_2(cont);
  // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  // lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  obj = pump_button_2(cont);
  // lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  // lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  obj = lamp_button_1(cont);
  lv_obj_set_style_bg_color(obj, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(obj, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(obj, 8, 0);
  lv_obj_set_style_radius(obj, 8, 0);
  lv_obj_set_style_pad_ver(obj, 8, 0);

  /*
  for(i = 0; i < 8; i++) {
      lv_obj_t * obj = lv_obj_create(cont);
      lv_obj_set_size(obj, 70, LV_SIZE_CONTENT);
      lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);

      lv_obj_t * label = lv_label_create(obj);
      lv_label_set_text_fmt(label, "%"LV_PRIu32, i);
      lv_obj_center(label);
  }
  */
}

/**
 * Scale meter - Vertical Line
 */
lv_obj_t *scale_2(lv_obj_t *parent)
{
  lv_obj_t *scale = lv_scale_create(parent);
  lv_obj_set_size(scale, 60, SCALE_HEIGHT);
  lv_scale_set_label_show(scale, true);
  lv_scale_set_mode(scale, LV_SCALE_MODE_VERTICAL_RIGHT);
  lv_obj_center(scale);

  lv_scale_set_total_tick_count(scale, 21);
  lv_scale_set_major_tick_every(scale, 5);

  lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);
  lv_obj_set_style_length(scale, 5, LV_PART_ITEMS);
  lv_scale_set_range(scale, 20, 40);

  static const char *custom_labels[] = {"20 °C", "25 °C", "30 °C", "35 °C", "40 °C", NULL};
  lv_scale_set_text_src(scale, custom_labels);

  static lv_style_t indicator_style;
  lv_style_init(&indicator_style);

  /* Label style properties */
  lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
  lv_style_set_text_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));

  /* Major tick properties */
  lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_width(&indicator_style, 10U);     /*Tick length*/
  lv_style_set_line_width(&indicator_style, 2U); /*Tick width*/
  lv_obj_add_style(scale, &indicator_style, LV_PART_INDICATOR);

  static lv_style_t minor_ticks_style;
  lv_style_init(&minor_ticks_style);
  lv_style_set_line_color(&minor_ticks_style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
  lv_style_set_width(&minor_ticks_style, 5U);      /*Tick length*/
  lv_style_set_line_width(&minor_ticks_style, 2U); /*Tick width*/
  lv_obj_add_style(scale, &minor_ticks_style, LV_PART_ITEMS);

  static lv_style_t main_line_style;
  lv_style_init(&main_line_style);
  /* Main line properties */
  lv_style_set_line_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_line_width(&main_line_style, 2U); // Tick width
  lv_obj_add_style(scale, &main_line_style, LV_PART_MAIN);

  /* Add a section */
  static lv_style_t section_minor_tick_style;
  static lv_style_t section_label_style;
  static lv_style_t section_main_line_style;

  lv_style_init(&section_label_style);
  lv_style_init(&section_minor_tick_style);
  lv_style_init(&section_main_line_style);

  /* Label style properties */
  lv_style_set_text_font(&section_label_style, LV_FONT_DEFAULT);
  lv_style_set_text_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));

  lv_style_set_line_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_line_width(&section_label_style, 5U); /*Tick width*/

  lv_style_set_line_color(&section_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
  lv_style_set_line_width(&section_minor_tick_style, 4U); /*Tick width*/

  /* Main line properties */
  lv_style_set_line_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_line_width(&section_main_line_style, 4U); /*Tick width*/

  /* Configure section styles */
  lv_scale_section_t *section = lv_scale_add_section(scale);
  lv_scale_section_set_range(section, 75, 100);
  lv_scale_section_set_style(section, LV_PART_INDICATOR, &section_label_style);
  lv_scale_section_set_style(section, LV_PART_ITEMS, &section_minor_tick_style);
  lv_scale_section_set_style(section, LV_PART_MAIN, &section_main_line_style);

  /*
    lv_obj_set_style_bg_color(scale, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
    lv_obj_set_style_bg_opa(scale, LV_OPA_50, 0);
    lv_obj_set_style_pad_left(scale, 8, 0);
    lv_obj_set_style_radius(scale, 8, 0);
    lv_obj_set_style_pad_ver(scale, 20, 0);
    */

  return scale;
}

/**
 * @brief Create a circular scale meter with a needle indicator
 *
 */
lv_obj_t *scale_4(lv_obj_t *parent)
{
  lv_obj_t *container = lv_obj_create(parent);

  lv_obj_set_size(container, THERMO_WIDTH, 110);
  lv_obj_set_align(container, LV_ALIGN_CENTER);

  lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_t *scale = lv_scale_create(container);

  lv_scale_set_label_show(scale, true);
  lv_obj_set_size(scale, THERMO_WIDTH * .7, THERMO_HEIGHT * .7);
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);

  lv_obj_align(scale, LV_ALIGN_CENTER, 22, 40);

  lv_scale_set_total_tick_count(scale, 21);
  lv_scale_set_major_tick_every(scale, 5);

  lv_obj_set_style_length(scale, 5, LV_PART_ITEMS);
  lv_obj_set_style_length(scale, 10, LV_PART_INDICATOR);
  lv_scale_set_range(scale, 20, 40);

  lv_scale_set_angle_range(scale, 180);
  lv_scale_set_rotation(scale, 180);

  static const char *custom_labels[] = {"20 °C", "25 °C", "30 °C", "35 °C", "40 °C", NULL};
  lv_scale_set_text_src(scale, custom_labels);

  static lv_style_t indicator_style;
  lv_style_init(&indicator_style);

  lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
  lv_style_set_text_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));

  lv_style_set_line_color(&indicator_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_width(&indicator_style, 10U);
  lv_style_set_line_width(&indicator_style, 2U);
  lv_obj_add_style(scale, &indicator_style, LV_PART_INDICATOR);

  static lv_style_t minor_ticks_style;
  lv_style_init(&minor_ticks_style);
  lv_style_set_line_color(&minor_ticks_style, lv_palette_lighten(LV_PALETTE_BLUE, 2));
  lv_style_set_width(&minor_ticks_style, 5U);
  lv_style_set_line_width(&minor_ticks_style, 2U);
  lv_obj_add_style(scale, &minor_ticks_style, LV_PART_ITEMS);

  static lv_style_t main_line_style;
  lv_style_init(&main_line_style);

  lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_arc_width(&main_line_style, 2U);
  lv_obj_add_style(scale, &main_line_style, LV_PART_MAIN);

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

  lv_scale_section_t *section = lv_scale_add_section(scale);
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

  lv_obj_t *needle_line = lv_line_create(scale);

  lv_obj_set_style_line_width(needle_line, 6, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(needle_line, true, LV_PART_MAIN);

  lv_scale_set_line_needle_value(scale, needle_line, 60, 36);

  return container;
}

lv_obj_t *scale_5(lv_obj_t *parent)
{
  lv_obj_t *container = lv_obj_create(parent);

  lv_obj_set_size(container, THERMO_WIDTH, THERMO_HEIGHT);
  lv_obj_set_align(container, LV_ALIGN_CENTER);

  return container;
}

lv_obj_t *chart_obj(lv_obj_t *parent)
{
  /*Create a chart*/
  lv_obj_t *chart = lv_chart_create(parent);
  lv_obj_set_size(chart, CHART_WIDTH, CHART_HEIGHT);
  lv_obj_center(chart);
  lv_chart_set_type(chart, LV_CHART_TYPE_LINE); /*Show lines and points too*/

  /*Add two data series*/
  lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);
  lv_chart_series_t *ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_SECONDARY_Y);
  int32_t *ser2_y_points = lv_chart_get_y_array(chart, ser2);

  uint32_t i;
  for (i = 0; i < 10; i++)
  {
    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(chart, ser1, lv_rand(10, 50));

    /*Directly set points on 'ser2'*/
    ser2_y_points[i] = lv_rand(50, 90);
  }

  lv_chart_refresh(chart); /*Required after direct set*/
  return chart;
}

LV_IMAGE_DECLARE(img_hand);

lv_obj_t *needle_line;
lv_obj_t *needle_img;

static void set_needle_line_value(void *obj, int32_t v)
{
  lv_scale_set_line_needle_value(obj, needle_line, 60, v);
}

static void set_needle_img_value(void *obj, int32_t v)
{
  lv_scale_set_image_needle_value(obj, needle_img, v);
}

/**
 * A simple round scale with needles that move
 */
lv_obj_t *scale_3(lv_obj_t *parent)
{
  lv_obj_t *scale_line = lv_scale_create(parent);

  lv_obj_set_size(scale_line, SCALE_WIDTH, SCALE_HEIGHT);
  lv_scale_set_mode(scale_line, LV_SCALE_MODE_ROUND_INNER);
  lv_obj_set_style_bg_opa(scale_line, LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(scale_line, lv_palette_lighten(LV_PALETTE_RED, 5), 0);
  lv_obj_set_style_radius(scale_line, LV_RADIUS_CIRCLE, 0);
  lv_obj_set_style_clip_corner(scale_line, true, 0);
  //  lv_obj_align(scale_line, LV_ALIGN_LEFT_MID, LV_PCT(2), 0);

  lv_scale_set_label_show(scale_line, true);

  lv_scale_set_total_tick_count(scale_line, 31);
  lv_scale_set_major_tick_every(scale_line, 5);

  lv_obj_set_style_length(scale_line, 5, LV_PART_ITEMS);
  lv_obj_set_style_length(scale_line, 10, LV_PART_INDICATOR);
  lv_scale_set_range(scale_line, 10, 40);

  lv_scale_set_angle_range(scale_line, 270);
  lv_scale_set_rotation(scale_line, 135);

  needle_line = lv_line_create(scale_line);

  lv_obj_set_style_line_width(needle_line, 6, LV_PART_MAIN);
  lv_obj_set_style_line_rounded(needle_line, true, LV_PART_MAIN);

  lv_anim_t anim_scale_line;
  lv_anim_init(&anim_scale_line);
  lv_anim_set_var(&anim_scale_line, scale_line);
  lv_anim_set_exec_cb(&anim_scale_line, set_needle_line_value);
  lv_anim_set_duration(&anim_scale_line, 1000);
  lv_anim_set_repeat_count(&anim_scale_line, LV_ANIM_REPEAT_INFINITE);
  lv_anim_set_playback_duration(&anim_scale_line, 1000);
  lv_anim_set_values(&anim_scale_line, 10, 40);
  lv_anim_start(&anim_scale_line);

  /*
    lv_obj_t *scale_img = lv_scale_create(lv_screen_active());

    lv_obj_set_size(scale_img, 150, 150);
    lv_scale_set_mode(scale_img, LV_SCALE_MODE_ROUND_INNER);
    lv_obj_set_style_bg_opa(scale_img, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(scale_img, lv_palette_lighten(LV_PALETTE_RED, 5), 0);
    lv_obj_set_style_radius(scale_img, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(scale_img, true, 0);
    lv_obj_align(scale_img, LV_ALIGN_RIGHT_MID, LV_PCT(-2), 0);

    lv_scale_set_label_show(scale_img, true);

    lv_scale_set_total_tick_count(scale_img, 31);
    lv_scale_set_major_tick_every(scale_img, 5);

    lv_obj_set_style_length(scale_img, 5, LV_PART_ITEMS);
    lv_obj_set_style_length(scale_img, 10, LV_PART_INDICATOR);
    lv_scale_set_range(scale_img, 10, 40);

    lv_scale_set_angle_range(scale_img, 270);
    lv_scale_set_rotation(scale_img, 135);

    needle_img = lv_image_create(scale_img);
    lv_image_set_src(needle_img, &img_hand);
    lv_obj_align(needle_img, LV_ALIGN_CENTER, 47, -2);
    lv_image_set_pivot(needle_img, 3, 4);

    lv_anim_t anim_scale_img;
    lv_anim_init(&anim_scale_img);
    lv_anim_set_var(&anim_scale_img, scale_img);
    lv_anim_set_exec_cb(&anim_scale_img, set_needle_img_value);
    lv_anim_set_duration(&anim_scale_img, 1000);
    lv_anim_set_repeat_count(&anim_scale_img, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_playback_duration(&anim_scale_img, 1000);
    lv_anim_set_values(&anim_scale_img, 10, 40);
    lv_anim_start(&anim_scale_img);
  */
  return scale_line;
}

lv_obj_t *pump_button_1(lv_obj_t *parent)
{
  // LV_IMAGE_DECLARE(pumpOff_data);
  // LV_IMAGE_DECLARE(pumpLow_data);
  // LV_IMAGE_DECLARE(pumpHigh_data);

  /*Create a transition animation on width transformation and recolor.*/
  static lv_style_prop_t tr_prop[] = {LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_IMAGE_RECOLOR_OPA, 0};
  static lv_style_transition_dsc_t tr;
  lv_style_transition_dsc_init(&tr, tr_prop, lv_anim_path_linear, 200, 0, NULL);

  static lv_style_t style_def;
  lv_style_init(&style_def);
  lv_style_set_text_color(&style_def, lv_color_white());
  lv_style_set_transition(&style_def, &tr);

  /*Darken the button when pressed and make it wider*/
  static lv_style_t style_pr;
  lv_style_init(&style_pr);
  lv_style_set_image_recolor_opa(&style_pr, LV_OPA_30);
  lv_style_set_image_recolor(&style_pr, lv_color_black());
  lv_style_set_transform_width(&style_pr, 20);

  /*Create an image button*/
  lv_obj_t *imagebutton1 = lv_imagebutton_create(lv_screen_active());
  lv_imagebutton_set_src(imagebutton1, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &ui_img_pumpoff_png, NULL);
  lv_obj_add_style(imagebutton1, &style_def, 0);
  lv_obj_add_style(imagebutton1, &style_pr, LV_STATE_PRESSED);

  lv_obj_set_width(imagebutton1, CHART_WIDTH);
  lv_obj_align(imagebutton1, LV_ALIGN_CENTER, 0, 0);

  /*Create a label on the image button*/
  lv_obj_t *label = lv_label_create(imagebutton1);
  lv_label_set_text(label, "Button");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
  return imagebutton1;
}
lv_obj_t *pump_button_2(lv_obj_t *parent)
{
  lv_obj_t *ui_Pump1 = lv_imagebutton_create(parent);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &ui_img_pumpoff_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &ui_img_pumplow_png, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_obj_set_size(ui_Pump1, BUTTON_WIDTH, BUTTON_HEIGHT);
  // lv_obj_set_height( ui_Pump1, 174);
  // lv_obj_set_width( ui_Pump1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_align(ui_Pump1, LV_ALIGN_CENTER);
  return ui_Pump1;
}

lv_obj_t *lamp_button_1(lv_obj_t *parent)
{
  lv_obj_t *ui_Pump1 = lv_imagebutton_create(parent);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_RELEASED, NULL, &LightOff, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_PRESSED, NULL, &lightOn, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, NULL, &ui__temporary_image, NULL);
  lv_imagebutton_set_src(ui_Pump1, LV_IMAGEBUTTON_STATE_CHECKED_DISABLED, NULL, &ui__temporary_image, NULL);
  lv_obj_set_size(ui_Pump1, BUTTON_WIDTH, BUTTON_HEIGHT);
  // lv_obj_set_height( ui_Pump1, 174);
  // lv_obj_set_width( ui_Pump1, LV_SIZE_CONTENT);  /// 1
  lv_obj_set_align(ui_Pump1, LV_ALIGN_CENTER);
  return ui_Pump1;
}
#endif
