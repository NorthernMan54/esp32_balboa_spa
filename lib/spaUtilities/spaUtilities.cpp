#include "spaUtilities.h"
#include <Arduino.h>
#include <CircularBuffer.hpp>

/**
 * Calculate uptime and take into account the millis() rollover
 * returns: unsigned long uptime in seconds
 */
time_t uptime()
{
  static time_t lastUptime = 0;
  static time_t uptimeAdd = 0;
  time_t uptime = millis() / 1000 + uptimeAdd;
  if (uptime < lastUptime)
  {
    uptime += 4294967;
    uptimeAdd += 4294967;
  }
  lastUptime = uptime;
  return uptime;
}

time_t getTime()
{
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo,5))
  {
    // Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

int getHour()
{ // the hour now
  return getHour(getTime());
}

int getHour(time_t time)
{ // the hour for the given time
  struct tm *timeInfo = localtime(&time);

  // Get the hour
  return timeInfo->tm_hour;
}

String msgToString(uint8_t *data, uint8_t len)
{
  String s;
  for (int i = 0; i < len; i++)
  {
    if (data[i] < 0x10)
      s += "0";
    s += String(data[i], HEX);
    s += " ";
  }
  return s;
}

String msgToString(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  String s;
  for (int i = 0; i < data.size(); i++)
  {
    if (data[i] < 0x10)
      s += "0";
    s += String(data[i], HEX);
    s += " ";
  }
  return s;
}

void append_request(unsigned char *byte_array, int *offset, unsigned char *request, int request_size)
{
  memcpy(byte_array + *offset, request, request_size);
  *offset += request_size;
}

String formatNumberWithCommas(time_t num)
{
  return formatNumberWithCommas((unsigned long)num);
}

/*
  Format a number with commas
  num: number to format
  returns: formatted number as a string
*/
String formatNumberWithCommas(int num)
{
  return formatNumberWithCommas((unsigned long)num);
}

/*
  Format a number with commas
  num: number to format
  returns: formatted number as a string
*/
String formatNumberWithCommas(uint32_t num)
{
  return formatNumberWithCommas((unsigned long)num);
}

/*
  Format a number with commas
  num: number to format
  returns: formatted number as a string
*/
String formatNumberWithCommas(unsigned long num)
{
  String numStr = String(num);
  int insertPosition = numStr.length() - 3;

  while (insertPosition > 0)
  {
    numStr = numStr.substring(0, insertPosition) + ',' + numStr.substring(insertPosition);
    insertPosition -= 3;
  }

  return numStr;
}

void _printTimestamp(Print *_logOutput)
{

  // Division constants
  const unsigned long MSECS_PER_SEC = 1000;
  const unsigned long SECS_PER_MIN = 60;
  const unsigned long SECS_PER_HOUR = 3600;
  const unsigned long SECS_PER_DAY = 86400;

  // Total time
  const unsigned long msecs = millis();
  const unsigned long secs = msecs / MSECS_PER_SEC;

  // Time in components
  const unsigned long MilliSeconds = msecs % MSECS_PER_SEC;
  const unsigned long Seconds = secs % SECS_PER_MIN;
  const unsigned long Minutes = (secs / SECS_PER_MIN) % SECS_PER_MIN;
  const unsigned long Hours = (secs % SECS_PER_DAY) / SECS_PER_HOUR;

  // Time as string
  char timestamp[20];
  sprintf(timestamp, "%02d:%02d:%02d.%03d ", Hours, Minutes, Seconds, MilliSeconds);
  _logOutput->print(timestamp);
}

void _printLogLevel(Print *_logOutput, int logLevel)
{
  /// Show log description based on log level
  switch (logLevel)
  {
  default:
  case 0:
    _logOutput->print("SILENT ");
    break;
  case 1:
    _logOutput->print("FATAL ");
    break;
  case 2:
    _logOutput->print("ERROR ");
    break;
  case 3:
    _logOutput->print("WARNING ");
    break;
  case 4:
    _logOutput->print("INFO ");
    break;
  case 5:
    _logOutput->print("TRACE ");
    break;
  case 6:
    _logOutput->print("VERBOSE ");
    break;
  }
}

void logPrintPrefix(Print *_logOutput, int logLevel)
{
  _printTimestamp(_logOutput);
  //   printLogLevel (_logOutput, logLevel);
}

String weekday_D[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String month_M[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

/*
history - array of floats to convert to string, length is GRAPH_MAX_READINGS
*/
String historyToString(float *history)
{
  String tempHistory = "";
  for (int i = 0; i < GRAPH_MAX_READINGS; i++)
  {
    tempHistory += String(history[i]) + ", ";
  }
  return tempHistory;
}

/*
  Format a time as HH:MM
  hour: hour to format
  minute: minute to format
  returns: formatted time as a string
*/
String formatAsHourMinute(uint8_t hour, uint8_t minute)
{
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", hour, minute);
  return String(timeStr);
}