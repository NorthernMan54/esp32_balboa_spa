#include "epd47.h"
#include <Arduino.h>

#ifdef spaEpaper
#include "epd_driver.h" // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47
#include <WiFi.h>       // In-built
#include <SPI.h>        // In-built
#include <TickTwo.h>
#include <ArduinoLog.h>

#include "to_jpg.h"
#include "esp_camera.h"
#include "img_converters.h"

#include <spaMessage.h>

GFXfont currentFont;
uint8_t *frameBuffer;

// local functions

void DrawRSSI(int x, int y, int rssi);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

void epd47Setup()
{
  epd_init();
  frameBuffer = (uint8_t *)ps_calloc(sizeof(uint8_t), EPD_WIDTH * EPD_HEIGHT / 2);
  if (!frameBuffer)
    Serial.println("Memory alloc failed!");
  memset(frameBuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
}

void epd47Loop()
{
}

void setFont(GFXfont const &font)
{
  currentFont = font;
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  epd_fill_rect(x, y, w, h, color, frameBuffer);
}

void drawString(int x, int y, String text, alignment align)
{
  char *data = const_cast<char *>(text.c_str());
  // char *fontData = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  int x1, y1; // the bounds of x,y and w and h of the variable 'text' in pixels.
  int w, h;
  int xx = x, yy = y;
  //  Log.verbose("drawString before: %s, x %d, y %d\n", text.c_str(), x, y);
  get_text_bounds(&currentFont, data, &xx, &yy, &x1, &y1, &w, &h, NULL);
  if (align == RIGHT)
    x = x - w;
  if (align == CENTER)
    x = x - w / 2;
  int cursor_y = y + h;
  write_string(&currentFont, data, &x, &y, frameBuffer);
  //  Log.verbose("drawString after: %s, x %d, y %d\n", text.c_str(), x, y);
}

void epd_update()
{
  epd_draw_grayscale_image(epd_full_screen(), frameBuffer); // Update the screen
}

uint8_t *jpegBuffer = nullptr; // Buffer for storing the JPEG output
size_t jpegSize = 0;           // Size of the JPEG in memory

size_t captureToJPEG()
{
  // Ensure previous JPEG buffer is freed
  if (jpegBuffer != nullptr)
  {
    free(jpegBuffer);
  }

  // Use fmt2jpg to convert frameBuffer (grayscale) to JPEG
  bool success = newfmt2jpg(frameBuffer, SCREEN_WIDTH * SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, NEW_PIXFORMAT_GRAYSCALE16, 100, &jpegBuffer, &jpegSize);

  if (success)
  {
    Log.verbose("epd->JPEG created successfully, size: %d bytes\n", jpegSize);
  }
  else
  {
    Log.error("epd->JPEG conversion failed.");
    jpegSize = 0;
  }
  return jpegSize;
}

/**
 * @brief Draw a picture centered to a given area. The image area is not cleared and
 *        assumed to be white before drawing.
 *
 * @param area The display area to draw to. `width` and `height` of the area
 *             must correspond to the image dimensions in pixels.
 * @param data The image data, as a buffer of 4 bit wide brightness values.
 *             Pixel data is packed (two pixels per byte). A byte cannot wrap
 *             over multiple rows, images of uneven width must add a padding
 *             nibble per line.
 */
void drawCenteredIcon(Rect_t area, uint8_t *data)
{
  area.x = area.x + (ICON_WIDTH - area.width) / 2;
  area.y = area.y + (ICON_HEIGHT - area.height) / 2;
  epd_copy_to_framebuffer(area, data, frameBuffer);
}

/**
 * @brief Draw a picture centered to a given area. The image area is not cleared and
 *        assumed to be white before drawing.
 *
 * @param area The display area to draw to. `width` and `height` of the area
 *             must correspond to the image dimensions in pixels.
 * @param data The image data, as a buffer of 4 bit wide brightness values.
 *             Pixel data is packed (two pixels per byte). A byte cannot wrap
 *             over multiple rows, images of uneven width must add a padding
 *             nibble per line.
 * @param text The text to draw below the icon.
 */
void drawCenteredIconWithText(Rect_t area, uint8_t *data, String text)
{
  drawCenteredIcon(area, data);
  setFont(OpenSans12B);
  drawString(area.x + ICON_WIDTH / 2, area.y + ICON_HEIGHT + 30, text, CENTER);
  // Log.verbose("drawCenteredIconWithText: %s, x %d, y %d\n", text.c_str(), area.x + ICON_WIDTH / 2, area.y + (ICON_HEIGHT - area.height) / 2 + ICON_HEIGHT + 30);
}

const String TXT_PRESSURE_HPA = "Pressure (hPa)";
const String TXT_PRESSURE_IN = "Pressure (in)";

/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos-the x axis top-left position of the graph
    y_pos-the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width-the width of the graph in pixels
    height-height of the graph in pixels
    Y1_Max-sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale-a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on-a logical value (TRUE or FALSE) that switches the drawing mode between barhcart and line graph
    barchart_colour-a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/

void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode)
{
  DrawGraph(x_pos, y_pos, gwidth, gheight, Y1Min, Y1Max, title, DataArray, readings, auto_scale, barchart_mode, true, "d");
}

void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode, boolean forward, String legend)
{
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up fter a change of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
  // Log.notice("DrawGraph: %s, x_pos %d, y_pos %d\n", title.c_str(), x_pos, y_pos);
  setFont(OpenSans10B);
  int maxYscale = -10000;
  int minYscale = 10000;
  int last_x, last_y;
  float x2, y2;
  if (auto_scale == true)
  {
    for (int i = 0; i < readings; i++)
    {
      if (DataArray[i] >= maxYscale)
        maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale)
        minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0)
      minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, Grey);
  drawString(x_pos - 20 + gwidth / 2, y_pos - 28, title, CENTER);
  if (forward)
  {
    for (int gx = 0; gx < readings; gx++)
    {
      x2 = x_pos + gx * gwidth / (readings - 1) - 1; // max_readings is the global variable that sets the maximum data that can be plotted
      y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
      if (barchart_mode)
      {
        fillRect(last_x + 2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 2, Black);
      }
      else
      {
        drawLine(last_x, last_y - 1, x2, y2 - 1, Black); // Two lines for hi-res display
        drawLine(last_x, last_y, x2, y2, Black);
      }
      last_x = x2;
      last_y = y2;
    }
  }
  else
  {
    for (int gx = 0; gx < readings; gx++)
    {
      //      Log.verbose("gx: %d, readings: %d, element: %d, DataArray: %F, Reg: %F\n", gx, readings, readings - gx - 1, DataArray[(readings - gx - 1)], DataArray[gx]);
      x2 = x_pos + gx * gwidth / (readings - 1) - 1; // max_readings is the global variable that sets the maximum data that can be plotted
      y2 = y_pos + (Y1Max - constrain(DataArray[(readings - gx - 1)], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
      if (barchart_mode)
      {
        fillRect(last_x + 2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 2, Black);
      }
      else
      {
        drawLine(last_x, last_y - 1, x2, y2 - 1, Black); // Two lines for hi-res display
        drawLine(last_x, last_y, x2, y2, Black);
      }
      last_x = x2;
      last_y = y2;
    }
  }
  // Draw the Y-axis scale
#define number_of_dashes 20
  for (int spacing = 0; spacing <= y_minor_axis; spacing++)
  {
    for (int j = 0; j < number_of_dashes; j++)
    { // Draw dashed graph grid lines
      if (spacing < y_minor_axis)
        drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), Grey);
    }

    // Y1Max: 8.00, Y1Min: 0.00, y_minor_axis: 5, spacing: 0-5

    //  if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5 || title == TXT_PRESSURE_IN)
    //  {
    //    Log.verbose("A - Y1Max: %F, Y1Min: %F, y_minor_axis: %d, spacing: %d\n", Y1Max, Y1Min, y_minor_axis, spacing);
    //    drawString(x_pos - 10, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    //  }
    //  else
    //  {
    if (Y1Max - Y1Min < 10 && Y1Max < 100)
    {
      //  Log.verbose("B - Y1Max: %F, Y1Min: %F, y_minor_axis: %d, spacing: %d\n", Y1Max, Y1Min, y_minor_axis, spacing);
      drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis + 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    }
    else
    {
      //  Log.verbose("C - Y1Max: %F, Y1Min: %F, y_minor_axis: %d, spacing: %d\n", Y1Max, Y1Min, y_minor_axis, spacing);
      drawString(x_pos - 7, y_pos + gheight * spacing / y_minor_axis + 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
    }
    //  }
  }
  // Draw the X-axis legend
  for (int i = 0; i < 3; i++)
  {
    if (forward)
    {
      drawString(30 + x_pos + gwidth / 3 * i, y_pos + gheight + 10, String(i * (readings / 2)) + legend, CENTER);
    }
    else
    {
      drawString(30 + x_pos + gwidth / 3 * i, y_pos + gheight + 10, String((2 - i) * (readings / 2)) + legend, CENTER);
    }
    if (i < 2)
      drawFastVLine(x_pos + gwidth / 3 * i + gwidth / 3, y_pos, gheight, LightGrey);
  }
}

/**
 * @brief Write a line.  Bresenham's algorithm - thx wikpedia
 *
 * @param x0          Start point x coordinate
 * @param y0          Start point y coordinate
 * @param x1          End point x coordinate
 * @param y1          End point y coordinate
 * @param color       The gray value of the line (0-255);
 */
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  epd_write_line(x0, y0, x1, y1, color, frameBuffer);
}

/**
 * @brief Draw a rectanle with no fill color
 *
 * @param x           Top left corner x coordinate
 * @param y           Top left corner y coordinate
 * @param w           Width in pixels
 * @param h           Height in pixels
 * @param color       The gray value of the line (0-255);
 */
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  epd_draw_rect(x, y, w, h, color, frameBuffer);
}

/**
 * @brief Draw a horizontal line
 *
 * @param x           Horizontal start position in pixels.
 * @param y           Vertical start position in pixels.
 * @param length      Length of the line in pixels.
 * @param color       The gray value of the line (0-255);
 */
void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color)
{
  epd_draw_hline(x0, y0, length, color, frameBuffer);
}

/**
 * @brief Draw a vertical line
 *
 * @param x           Horizontal start position in pixels.
 * @param y           Vertical start position in pixels.
 * @param length      Length of the line in pixels.
 * @param color       The gray value of the line (0-255);
 */
void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color)
{
  epd_draw_vline(x0, y0, length, color, frameBuffer);
}

/*
x0 - x coordinate of the first point
y0 - y coordinate of the first point
x1 - x coordinate of the second point
y1 - y coordinate of the second point
x2 - x coordinate of the third point
y2 - y coordinate of the third point
color - color of the triangle
*/
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                  int16_t x2, int16_t y2, uint16_t color)
{
  epd_fill_triangle(x0, y0, x1, y1, x2, y2, color, frameBuffer);
}

/**
 * @brief Draw an arrow pointing up
 *
 * @param x - x coordinate of the center of the circle
 * @param y - y coordinate of the center of the circle
 * @param asize - distance from the center of the circle to the botton of the arrow ( radius )
 * @param aangle - angle of the arrow, 0 is up, 90 is right, 180 is down, 270 is left
 * @param pwidth - width of the arrow
 * @param plength - length of the arrow
 */
void arrowU(int x, int y, int asize, float aangle, int pwidth, int plength, uint16_t colour)
{
  float dx = (asize * 2 / 3) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize * 2 / 3) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  // Point
  float x1 = 0;
  float y1 = plength;
  // Left
  float x2 = pwidth / 2;
  float y2 = pwidth / 2;
  // Right
  float x3 = -pwidth / 2;
  float y3 = pwidth / 2;

  float angle = aangle * PI / 180 - 135;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;

//  Log.verbose("arrowU: x %d, y %d, asize %d, aangle %F, pwidth %d, plength %d, colour %d\n", x, y, asize, aangle, pwidth, plength, colour);
//  Log.verbose("arrowU 1 dx-dy %F-%F, Point %F-%F, 2 Left %F-%F, 3 Right %F-%F\n", dx, dy, xx1, yy1, xx2, yy2, xx3, yy3);
  fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, colour);
}

/**
 * @brief Draw an arrow pointing down
 *
 * @param x - x coordinate of the center of the circle
 * @param y - y coordinate of the center of the circle
 * @param asize - distance from the center of the circle to the botton of the arrow ( radius )
 * @param aangle - angle of the arrow, 0 is up, 90 is right, 180 is down, 270 is left
 * @param pwidth - width of the arrow
 * @param plength - length of the arrow
 */
void arrowD(int x, int y, int asize, float aangle, int pwidth, int plength, uint16_t colour)
{
  // 00:00:01.635 V: arrowD: x 90, y 320, asize 60, aangle 54.00, pwidth 18, plength 33, colour 0
  // 00:00:01.637 V: 1 158.76-273.65, 2 133.55-278.26, 3 142.80-293.70
  float dx = (asize * 2 / 3) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize * 2 / 3) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  // Point
  float x1 = 0;
  float y1 = 0;
  // Right
  float x2 = pwidth / 2;
  float y2 = plength;
  // Left
  float x3 = -pwidth / 2;
  float y3 = plength;
  float angle = aangle * PI / 180 - 135;
  // Point
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  // Left
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  // Right
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
//  Log.verbose("arrowD: x %d, y %d, asize %d, aangle %F, pwidth %d, plength %d, colour %d\n", x, y, asize, aangle, pwidth, plength, colour);
//  Log.verbose("arrowD 1 dx-dy %F-%F, Point %F-%F, 2 Left %F-%F, 3 Right %F-%F\n", dx, dy, xx1, yy1, xx2, yy2, xx3, yy3);
  fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, colour);
}

#endif
