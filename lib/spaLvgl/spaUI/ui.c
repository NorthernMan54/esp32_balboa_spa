#ifdef SPALVGL
#include <lvgl.h>
// #if LV_USE_METER && LV_BUILD_EXAMPLES
// #include "display.h"
#include "ui.h"
#include <esp_log.h>

#define METER_WIDTH 150
#define METER_HEIGHT 150

#define CHART_WIDTH 200
#define CHART_HEIGHT 150

static const char *TAG = "GRID";

/**
 * Demonstrate grid's "free unit"
 */
void ui_init(void)
{
  /*Column 1: fix width 60 px
   *Column 2: 1 unit from the remaining free space
   *Column 3: 2 unit from the remaining free space*/
  static lv_coord_t col_dsc[] = {METER_WIDTH, CHART_WIDTH, LV_GRID_TEMPLATE_LAST};

  /*Row 1: fix width 50 px
   *Row 2: 1 unit from the remaining free space
   *Row 3: fix width 50 px*/
  static lv_coord_t row_dsc[] = {METER_HEIGHT, METER_HEIGHT, LV_GRID_TEMPLATE_LAST};

  /*Create a container with grid*/
  lv_obj_t *cont = lv_obj_create(lv_scr_act());
  lv_obj_set_size(cont, DISPLAY_WIDTH, DISPLAY_HEIGHT);
  lv_obj_center(cont);
  lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);

  lv_obj_t *label;
  lv_obj_t *obj;
  uint32_t i;
  for (i = 0; i < 4; i++)
  {
    uint8_t col = i / 2;
    uint8_t row = i % 2;

    if (col == 0)
    {
      ESP_LOGI(TAG, "meter i=%lu, col=%d, row=%d, ", i, col, row);
      obj = scale_obj(cont);
    }
    else
    {
      ESP_LOGI(TAG, "chart i=%lu, col=%d, row=%d, ", i, col, row);
      obj = chart_obj(cont);
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
 * A simple meter
 */
lv_obj_t *scale_obj(lv_obj_t *parent)
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

#endif
