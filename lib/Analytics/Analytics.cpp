#include <LittleFS.h>
#include <Analytics.h>
#include <spaUtilities.h>
#include <ArduinoLog.h>  // Ensure ArduinoLog is included
#include "../../src/main.h"
#include <cstring>  // For string handling functions like strcpy and strcat

Analytics::Analytics(AnalyticsData *data, const char *dataName)
{
  analyticsInstance = data;

  // Construct the filename using C-style strings
  strncpy(dataFileName, "/", sizeof(dataFileName) - 1);  // Start with "/"
  strncat(dataFileName, dataName, sizeof(dataFileName) - strlen(dataFileName) - 1);  // Append the dataName
  strncat(dataFileName, ".bin", sizeof(dataFileName) - strlen(dataFileName) - 1);  // Append ".bin"

  // Load data if the magic number is not set, meaning it's the first run
  if (analyticsInstance->magicNumber != ANALYTICS_MAGIC_NUMBER)
  {
    if (!loadData())  // Try to load saved data, reset if file doesn't exist
    {
      reset();  // If no saved data is found, reset the data
      saveData();  // Save the new reset state
    }
  }
  else
  {
    // Continue from previous session
    Log.notice(F("[Analytics]: Loaded existing %s data from RTC" CR), dataName);
  }

  analyticsInstance->lastCheckedTime = getTime();
}

Analytics::~Analytics() {}

void Analytics::add(uint8_t state)
{
  if (state)
  {
    on();
  }
  else
  {
    off();
  }
}

void Analytics::on()
{
  rollover(); // Check for day change before updating time
  analyticsInstance->onTimeToday += (millis() > analyticsInstance->previousReading ? millis() - analyticsInstance->previousReading : millis());
  analyticsInstance->previousReading = millis();
}

void Analytics::off()
{
  rollover(); // Check for day change before updating the reading time
  analyticsInstance->previousReading = millis();
}

unsigned long Analytics::today()
{
  return analyticsInstance->onTimeToday / 1000;
}

unsigned long Analytics::yesterday()
{
  return analyticsInstance->onTimeYesterday / 1000;
}

float *Analytics::history()
{
  return analyticsInstance->history;
}

void Analytics::reset()
{
  analyticsInstance->onTimeToday = 0;
  analyticsInstance->onTimeYesterday = 0;
  analyticsInstance->previousReading = millis();
  analyticsInstance->magicNumber = ANALYTICS_MAGIC_NUMBER;
  analyticsInstance->lastCheckedTime = getTime();

  for (int i = 0; i < GRAPH_MAX_READINGS; i++)
  {
    analyticsInstance->history[i] = 0.0;
  }

  saveData();  // Save the reset state
  Log.notice(F("[Analytics]: Analytics data has been reset and saved." CR));
}

void Analytics::rollover()
{
  if (hasDayChanged(analyticsInstance->lastCheckedTime))
  {
    analyticsInstance->onTimeYesterday = analyticsInstance->onTimeToday;
    analyticsInstance->onTimeToday = 0;

    // Shift history data for graph readings
    for (int x = GRAPH_MAX_READINGS; x > 1; x--)
    {
      analyticsInstance->history[x - 1] = analyticsInstance->history[x - 2];
    }

    // Store yesterday's onTime into history
    analyticsInstance->history[0] = analyticsInstance->onTimeYesterday / 1000;

    // Save updated data to persistent storage after rollover
    saveData();
    Log.notice(F("[Analytics]: Day rollover detected. Data updated and saved." CR));
  }
}

void Analytics::saveData()
{
  File file = LittleFS.open(dataFileName, "w");
  if (!file)
  {
    Log.error(F("[Analytics]: Failed to open file for writing: %s" CR), dataFileName);
    return;
  }

  // Write the entire analytics data structure to LittleFS
  if(!file.write((uint8_t*)analyticsInstance, sizeof(AnalyticsData))) {
    Log.error(F("[Analytics]: Failed to write data to file: %s" CR), dataFileName);
  } else {
    Log.notice(F("[Analytics]: Analytics data saved to %s" CR), dataFileName);
  }
  file.close();
}

bool Analytics::loadData()
{
  File file = LittleFS.open(dataFileName, "r");
  if (!file)
  {
    Log.warning(F("[Analytics]: No saved data found for %s. Resetting..." CR), dataFileName);
    return false;  // File not found, return false to trigger reset
  }

  // Read the saved data into the analyticsInstance structure
  if(!file.readBytes((char*)analyticsInstance, sizeof(AnalyticsData))) {
    Log.error(F("[Analytics]: Failed to read data from file: %s" CR), dataFileName);
    file.close();
    return false;
  }
  file.close();

  Log.notice(F("[Analytics]: Analytics data loaded from %s" CR), dataFileName);
  return true;  // Successfully loaded data
}
