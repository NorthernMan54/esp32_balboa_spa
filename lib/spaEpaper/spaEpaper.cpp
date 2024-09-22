#include "spaEpaper.h"
#include <Arduino.h>
#include <TickTwo.h>
#include <ArduinoLog.h>

#ifdef spaEpaper
#include "epd_driver.h" // https://github.com/Xinyuan-LilyGO/LilyGo-EPD47

#include "epd47.h"
#include <spaUtilities.h>

#include <spaMessage.h>

// Images / icons
#include "pumpOff.h"
#include "pumpLow.h"
#include "pumpHigh.h"

#include "lightOff.h"
#include "lightOn.h"

#include "filterOff.h"
#include "filterOn.h"

#include "lowHeatOff.h"
#include "lowHeatOn.h"
#include "highHeatOff.h"
#include "highHeatOn.h"

// Local functions

void displaySpaTime();
void displaySpaStatusData();
void epdStatusReport();
void iconDemo();
void displayPump(uint8_t pumpStatus, int pumpNumber);
void displayLight(uint8_t lightStatus, int lightNumber);
void displayFilter(uint8_t filterStatus);
void displayHeatState(int, int, uint8_t heatingMode, uint8_t heatingState);
void displayHeaterChart();
void displayFilterChart();
void displayTemperatureHistoryChart();
void displayTemperatures(int x, int y, float currentTemp, float highTemp, float lowTemp);

TickTwo epdStatus(epdStatusReport, 1 * 60 * 1000); // 5 minutes

#define TemperatureC "Temperature °C"

void spaEpaperSetup()
{
  Log.notice("[EPaper]: Setup epd Screen\n");
  epd47Setup();
  epdStatus.start();
  epd_poweron(); // Switch on EPD display
  epd_clear();   // Clear the screen

  iconDemo();
  setFont(OpenSans24B);
  drawString(400, 260, "Booting, please wait", CENTER);
  epd_update();       // Update the display to show the information
  epd_poweroff_all(); // Switch off all power to EPD
}

void spaEpaperLoop()
{
  epd47Loop();
  epdStatus.update();
}

int refreshCount = 0;

void epdStatusReport()
{
  Log.notice("[EPaper]: Update epd Screen - %d, %d\n", ++refreshCount, epdStatus.counter() % 30);
  memset(frameBuffer, 0xFF, EPD_WIDTH * EPD_HEIGHT / 2);
  epd_poweron(); // Switch on EPD display
  delay(50);
  (epdStatus.counter() % 30 ? epd_clear_area_cycles(epd_full_screen(), 1, 50) : epd_clear());
  delay(50);
  displaySpaTime();
  displaySpaStatusData();

  epd_update();       // Update the display to show the information
  epd_poweroff_all(); // Switch off all power to EPD

  // send status report
}

#define rowHeight(row) ((row * 45) + 36)
#define col1 10
#define col2 280

void displayFilterConfig(int x, int y)
{
  setFont(OpenSans12);
  drawString(col1, y, "Filter 1: ", LEFT);
  setFont(OpenSans12B);
  drawString(col1 + 10 * 9, y, formatAsHourMinute(spaFilterSettingsData.filt1DurationHour, spaFilterSettingsData.filt1DurationMinute) + "hr @ " + formatAsHourMinute(spaFilterSettingsData.filt1Hour, spaFilterSettingsData.filt1Minute), LEFT);
  setFont(OpenSans12);
  drawString(col1, y + 30 * 1, "Filter 2: ", LEFT);
  setFont(OpenSans12B);
  drawString(col1 + 10 * 9, y + 30 * 1, formatAsHourMinute(spaFilterSettingsData.filt2DurationHour, spaFilterSettingsData.filt2DurationMinute) + "hr @ " + formatAsHourMinute(spaFilterSettingsData.filt2Hour, spaFilterSettingsData.filt2Minute), LEFT);
}

void displaySpaStatusData()
{
  int x = 0;

  setFont(OpenSans18);
  drawString(col1, rowHeight(x), "Current Temp:", LEFT);
  setFont(OpenSans18B);
  drawString(col2, rowHeight(x++), String(spaStatusData.currentTemp, 2) + "°C", LEFT);

  setFont(OpenSans16);
  // Display set temperature
  drawString(col1, rowHeight(x), "High Set Temp:", LEFT);
  setFont(OpenSans16B);
  drawString(col2, rowHeight(x++), String(spaStatusData.highSetTemp, 2) + "°C", LEFT);

  setFont(OpenSans16);
  drawString(col1, rowHeight(x), "Low Set Temp:", LEFT);
  setFont(OpenSans16B);
  drawString(col2, rowHeight(x++), String(spaStatusData.lowSetTemp, 2) + "°C", LEFT);

  setFont(OpenSans16);
  drawString(col1, rowHeight(x), "Spa Status:", LEFT);
  setFont(OpenSans16B);
  drawString(col2, rowHeight(x++), getMapDescription(spaStatusData.spaState, spaStateMap), LEFT);

  setFont(OpenSans16);
  drawString(col1, rowHeight(x), "Spa Mode:", LEFT);
  setFont(OpenSans16B);
  drawString(col2, rowHeight(x++), getMapDescription(spaStatusData.initMode, initModeMap), LEFT);

  setFont(OpenSans16);
  drawString(col1, rowHeight(x), "Heat Mode:", LEFT);
  setFont(OpenSans16B);
  drawString(col2, rowHeight(x++), getMapDescription(spaStatusData.heatingMode, heatingModeMap), LEFT);

  displayFilterConfig(col1, rowHeight(x));

  displayTemperatures(580, 120, spaStatusData.currentTemp, spaStatusData.highSetTemp, spaStatusData.lowSetTemp);
  displayHeatState(530, 180, spaStatusData.tempRange, spaStatusData.heatingState);

  displayPump(spaStatusData.pump1, 1);
  displayPump(spaStatusData.pump2, 2);
  displayLight(spaStatusData.light1, 1);
  displayFilter(spaStatusData.filterMode);

  displayHeaterChart();
  displayFilterChart();
  displayTemperatureHistoryChart();
  //  drawString(10, rowHeight(9), "Pump 1: " + getMapDescription(spaStatusData.pump1, pumpMap), LEFT);
  //  drawString(col2, rowHeight(9), "Pump 2: " + getMapDescription(spaStatusData.pump2, pumpMap), LEFT);
  //  drawString(col2 + col2, rowHeight(9), "Light 1: " + getMapDescription(spaStatusData.light1, onOffMap), LEFT);

  //  setFont(OpenSans12B);
  //  drawString(10, rowHeight(7), "Heater On Time Today: " + String(spaStatusData.heaterOnTimeToday) + " sec", LEFT);
  //  drawString(10, rowHeight(7.5), "Heater On Time Yesterday: " + String(spaStatusData.heaterOnTimeYesterday) + " sec", LEFT);
  // Add more fields similarly if needed...
}

#define iconRow 370
void displayPump(uint8_t pumpStatus, int pumpNumber)
{
  Rect_t areap1 = {
      .x = (pumpNumber - 1) * 180 + 10,
      .y = iconRow,
      .width = pumpOff_width,
      .height = pumpOff_height};

  drawCenteredIconWithText(areap1, (pumpStatus ? (pumpStatus > 1 ? (uint8_t *)pumpHigh_data : (uint8_t *)pumpLow_data) : (uint8_t *)pumpOff_data), "Pump " + String(pumpNumber));
}

void displayLight(uint8_t lightStatus, int lightNumber)
{
  Rect_t areap1 = {
      .x = (lightNumber - 1) * 180 + 10 + 360,
      .y = iconRow,
      .width = lightOff_width,
      .height = lightOff_height};

  drawCenteredIconWithText(areap1, (lightStatus ? (uint8_t *)lightOn_data : (uint8_t *)lightOff_data), "Light " + String(lightNumber));
}

void displayFilter(uint8_t filterStatus)
{
  int filterNumber = 0;
  Rect_t areap1 = {
      .x = (filterNumber - 1) * 180 + 10 + 360 + 360,
      .y = iconRow,
      .width = filterOff_width,
      .height = filterOff_height};

  drawCenteredIconWithText(areap1, (filterStatus ? (uint8_t *)filterOn_data : (uint8_t *)filterOff_data), "Filter");
}

/*
Draw a temperature circle with the current, high and low temperatures

Parameters:
x – Center-point x coordinate
y – Center-point y coordinate
currentTemp - current temperature
highTemp - high set point
lowTemp - low set point temperatures
*/
void displayTemperatures(int x, int y, float currentTemp, float highTemp, float lowTemp)
{
  int radius = 55;
  epd_fill_circle(x, y, radius, LightGrey, frameBuffer);
  epd_fill_circle(x, y, 40, White, frameBuffer);
  epd_fill_rect(x - radius - 1, y, 112, radius + 1, White, frameBuffer);

  setFont(OpenSans10);
  drawString(x - radius - 20, y, "20", CENTER);
  drawString(x, y - radius - 5, "30", CENTER);
  drawString(x + radius + 20, y, "40", CENTER);

  arrowU(x, y, 30, (highTemp - 20) * 9 - 90, 18, 33, Grey);
  arrowU(x, y, 30, (lowTemp - 20) * 9 - 90, 18, 33, DarkGrey);
  arrowD(x, y, 60, (currentTemp - 20) * 9 - 88, 18, 33, Black);
  setFont(OpenSans10B);
  drawString(x, y + 30, TemperatureC, CENTER);
}

void displayHeatState(int x, int y, uint8_t tempRange, uint8_t heatingState)
{
  if (tempRange)
  {
    Rect_t areah1 = {
        .x = x,
        .y = y,
        .width = (heatingState ? highHeatOn_width : highHeatOff_width),
        .height = (heatingState ? highHeatOn_height : highHeatOff_height)};

    drawCenteredIconWithText(areah1, (heatingState ? (uint8_t *)highHeatOn_data : (uint8_t *)highHeatOff_data), "Heat");
  }
  else
  {
    Rect_t areal1 = {
        .x = x,
        .y = y,
        .width = (heatingState ? lowHeatOn_width : lowHeatOff_width),
        .height = (heatingState ? lowHeatOn_height : lowHeatOff_height)};

    drawCenteredIconWithText(areal1, (heatingState ? (uint8_t *)lowHeatOn_data : (uint8_t *)lowHeatOff_data), "Heat");
  }
}

int x(int tileNumber)
{
  int x, y;
  const int tileSize = 160;                 // Each tile is 120x120 units
  const int gridWidth = 960;                // Width of the grid
  const int numCols = gridWidth / tileSize; // Number of columns in the grid

  // Calculate the X and Y coordinates based on tile number
  x = (tileNumber % numCols) * tileSize + 10; // X-coordinate
  y = (tileNumber / numCols) * tileSize + 10; // Y-coordinate

  return x;
}

int y(int tileNumber)
{
  int x, y;
  const int tileSize = 140;                 // Each tile is 120x120 units
  const int gridWidth = 960;                // Width of the grid
  const int numCols = gridWidth / tileSize; // Number of columns in the grid

  // Calculate the X and Y coordinates based on tile number
  x = (tileNumber % numCols) * tileSize + 10; // X-coordinate
  y = (tileNumber / numCols) * 350 + 10;      // Y-coordinate
  // Log.notice("Tile number: %d, x: %d, y: %d\n", tileNumber, x, y);
  return y;
}

void displaySpaTime()
{
  struct tm timeinfo;
  char time_output[30], day_output[30];
  while (!getLocalTime(&timeinfo, 10))
  { // Wait for 5-sec for time to synchronise
    Log.error("[EPaper]: Failed to obtain time");
    return;
  }

  sprintf(day_output, "%s, %02u %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
  sprintf(time_output, "%s", spaStatusData.time); // Creates: '@ 14:05:49'

  setFont(OpenSans12B);
  drawString(610, 30, String(day_output) + "  @  " + String(time_output), LEFT);
}

int gwidth = 175, gheight = 100;
int gx = (SCREEN_WIDTH - gwidth * 4) / 5 + 8;   // (960 - 175 * 4) / 5 = 52
int gy = (SCREEN_HEIGHT - gheight * 3) / 4 + 8; // (540 - 100 * 3 )/ 4 = 60
int gapX = gwidth + gx;
int gapY = gheight + gy;

#define Y_MOVE 25
float graph_data[GRAPH_MAX_READINGS] = {0};

/*
Merge the current data with the history data, and convert from seconds to minutes
*/
float *mergeGraphData(float current, float *historyData, int rangeAdjust)
{
  for (int i = 0; i < GRAPH_MAX_READINGS - 1; i++)
  {
    graph_data[i + 1] = historyData[i] / rangeAdjust;
    graph_data[0] = current / rangeAdjust;
  }
  return graph_data;
}

void displayTemperatureHistoryChart()
{
  DrawGraph(gx + 3 * gapX, gy + 0 * gapY + Y_MOVE, gwidth, gheight, 20, 40, TemperatureC, spaStatusData.temperatureHistory, GRAPH_MAX_READINGS, autoscale_on, barchart_off, right_to_left, "h");
}
void displayHeaterChart()
{
  DrawGraph(gx + 3 * gapX, gy + 1 * gapY + Y_MOVE, gwidth, gheight, 900, 1050, "Heater (Min)", mergeGraphData(spaStatusData.heaterOnTimeToday, spaStatusData.heatOn->history(), 60), GRAPH_MAX_READINGS, autoscale_on, barchart_off, right_to_left, "d");
}

void displayFilterChart()
{
  DrawGraph(gx + 3 * gapX, gy + 2 * gapY + Y_MOVE, gwidth, gheight, 900, 1050, "Filter (Hr)", mergeGraphData(spaStatusData.filterOnTimeToday, spaStatusData.filterOn->history(), 60 * 60), GRAPH_MAX_READINGS, autoscale_on, barchart_off, right_to_left, "d");
}

void iconDemo()
{

  // Log.notice("iconDemo\n");
  int tile = 0;
  Rect_t areap1 = {
      .x = x(tile),
      .y = y(tile++),
      .width = pumpOff_width,
      .height = pumpOff_height};
  drawCenteredIconWithText(areap1, (uint8_t *)pumpOff_data, "Pump Off");
  Rect_t areap2 = {
      .x = x(tile),
      .y = y(tile++),
      .width = pumpLow_width,
      .height = pumpLow_height};
  drawCenteredIconWithText(areap2, (uint8_t *)pumpLow_data, "Pump Low");
  Rect_t areap3 = {
      .x = x(tile),
      .y = y(tile++),
      .width = pumpHigh_width,
      .height = pumpHigh_height};
  drawCenteredIconWithText(areap3, (uint8_t *)pumpHigh_data, "Pump High");

  Rect_t areaf1 = {
      .x = x(tile),
      .y = y(tile++),
      .width = filterOff_width,
      .height = filterOff_height};
  drawCenteredIconWithText(areaf1, (uint8_t *)filterOff_data, "Filter Off");
  Rect_t areaf2 = {
      .x = x(tile),
      .y = y(tile++),
      .width = filterOn_width,
      .height = filterOn_height};
  drawCenteredIconWithText(areaf2, (uint8_t *)filterOn_data, "Filter On");
  tile++; // Skip to next row
  Rect_t areal1 = {
      .x = x(tile),
      .y = y(tile++),
      .width = lightOff_width,
      .height = lightOff_height};
  drawCenteredIconWithText(areal1, (uint8_t *)lightOff_data, "Light Off");

  Rect_t areal2 = {
      .x = x(tile),
      .y = y(tile++),
      .width = lightOn_width,
      .height = lightOn_height};
  drawCenteredIconWithText(areal2, (uint8_t *)lightOn_data, "Light On");

  Rect_t areahh1 = {
      .x = x(tile),
      .y = y(tile++),
      .width = highHeatOff_width,
      .height = highHeatOff_height};
  drawCenteredIconWithText(areahh1, (uint8_t *)highHeatOff_data, "High / Off");

  Rect_t areahh2 = {
      .x = x(tile),
      .y = y(tile++),
      .width = highHeatOn_width,
      .height = highHeatOn_height};
  drawCenteredIconWithText(areahh2, (uint8_t *)highHeatOn_data, "High / On");

  Rect_t arealh1 = {
      .x = x(tile),
      .y = y(tile++),
      .width = lowHeatOff_width,
      .height = lowHeatOff_height};
  drawCenteredIconWithText(arealh1, (uint8_t *)lowHeatOff_data, "Low / Off");

  Rect_t arealh2 = {
      .x = x(tile),
      .y = y(tile++),
      .width = lowHeatOn_width,
      .height = lowHeatOn_height};
  drawCenteredIconWithText(arealh2, (uint8_t *)lowHeatOn_data, "Low / On");

  DrawGraph(gx + 3 * gapX, (510 - 160) / 2 + 50, gwidth, gheight, 900, 1050, TemperatureC, spaStatusData.temperatureHistory, GRAPH_MAX_READINGS, autoscale_on, barchart_off, right_to_left, "h");
  displayTemperatures(90, SCREEN_HEIGHT / 2 + 50, spaStatusData.currentTemp, spaStatusData.highSetTemp, spaStatusData.lowSetTemp);
}

#endif