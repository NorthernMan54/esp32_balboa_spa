#ifndef epd47_H
#define epd47_H

#ifdef spaEpaper

#include <Arduino.h>
#include "epd_driver.h" // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

#define SCREEN_WIDTH EPD_WIDTH
#define SCREEN_HEIGHT EPD_HEIGHT

#define ICON_WIDTH 120
#define ICON_HEIGHT 120

#define White 0xFF
#define LightGrey 0xBB
#define Grey 0x88
#define DarkGrey 0x44
#define Black 0x00

extern uint8_t *frameBuffer;

enum alignment
{
  LEFT,
  RIGHT,
  CENTER
};

void epd47Setup();
void epd47Loop();

void epd_update();
void setFont(GFXfont const &font);
void drawString(int x, int y, String text, alignment align);

void arrowU(int x, int y, int asize, float aangle, int pwidth, int plength, uint16_t color);
void arrowD(int x, int y, int asize, float aangle, int pwidth, int plength, uint16_t color);

void drawCenteredIcon(Rect_t area, uint8_t *data);
void drawCenteredIconWithText(Rect_t area, uint8_t *data, String text);

size_t captureToJPEG();

extern uint8_t *jpegBuffer; // Buffer for storing the JPEG output
extern size_t jpegSize;     // Size of the JPEG in memory

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

    forward - draw the graph data from left to right or right to left
*/

#define autoscale_on  true
#define autoscale_off false
#define barchart_on   true
#define barchart_off  false
#define left_to_right true
#define right_to_left false
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode);
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode, boolean forward, String legend);


void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);


void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);


void drawFastHLine(int16_t x0, int16_t y0, int length, uint16_t color);


void drawFastVLine(int16_t x0, int16_t y0, int length, uint16_t color);

// Fonts

// fonts
#include "opensans8b.h"
#include "opensans10.h"
#include "opensans10b.h"
#include "opensans12.h"
#include "opensans12b.h"
#include "OpenSans16.h"
#include "OpenSans16b.h"
#include "opensans18.h"
#include "opensans18b.h"
#include "opensans24b.h"
#endif
#endif