#ifdef SPALVGL
#include <Arduino.h>
#include <lvgl.h>
// #if LV_USE_METER && LV_BUILD_EXAMPLES
// #include "display.h"
#include "ui.h"

#define SCALE_WIDTH 150
#define SCALE_HEIGHT 150

#define CHART_WIDTH 200
#define CHART_HEIGHT 150

#define TEXT_SIZE 50

static const char *TAG = "GRID";

/**
 * Demonstrate grid's "free unit"
 */
void ui_init(void)
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
        obj = scale_4(cont);
      }
      else
      {
        log_i("scale_4 i=%lu, col=%d, row=%d, ", i, col, row);
        obj = scale_2(cont);
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
    /*Stretch the cell horizontally and vertically too
     *Set span to 1 to make the cell 1 column/row sized*/
    lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                         LV_GRID_ALIGN_STRETCH, row, 1);

    label = lv_label_create(obj);
    lv_label_set_text_fmt(label, "%d,%d", col, row);
    lv_obj_align(label, LV_ALIGN_BOTTOM_MID, 0, 0);
  }
}

/**
 * Scale meter - Vertical Line
 */
lv_obj_t *scale_2(lv_obj_t *parent)
{
  lv_obj_t *scale = lv_scale_create(parent);
  lv_obj_set_size(scale, 60, 200);
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

  lv_obj_set_style_bg_color(scale, lv_palette_main(LV_PALETTE_BLUE_GREY), 0);
  lv_obj_set_style_bg_opa(scale, LV_OPA_50, 0);
  lv_obj_set_style_pad_left(scale, 8, 0);
  lv_obj_set_style_radius(scale, 8, 0);
  lv_obj_set_style_pad_ver(scale, 20, 0);

  return scale;
}

/**
 * @brief Create a circular scale meter with a needle indicator
 *
 */
lv_obj_t *scale_4(lv_obj_t *parent)
{
  lv_obj_t *scale = lv_scale_create(parent);
//  lv_obj_set_size(scale, 100, 100);
  lv_scale_set_label_show(scale, true);
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);
  lv_obj_center(scale);

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
  lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
  lv_style_set_arc_width(&main_line_style, 2U); /*Tick width*/
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
  lv_style_set_arc_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
  lv_style_set_arc_width(&section_main_line_style, 4U); /*Tick width*/

  /* Configure section styles */
  lv_scale_section_t *section = lv_scale_add_section(scale);
  lv_scale_section_set_range(section, 35.5, 36.5);
  lv_scale_section_set_style(section, LV_PART_INDICATOR, &section_label_style);
  lv_scale_section_set_style(section, LV_PART_ITEMS, &section_minor_tick_style);
  lv_scale_section_set_style(section, LV_PART_MAIN, &section_main_line_style);

  return scale;
}

lv_obj_t *chart_obj(lv_obj_t *parent)
{
  /*Create a chart*/
  lv_obj_t *chart;
  chart = lv_chart_create(parent);
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
 * A simple round scale
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

#endif
