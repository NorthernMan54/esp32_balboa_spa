#include "utilities.h"
#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

/**
 * Calculate uptime and take into account the millis() rollover
 * returns: unsigned long uptime in seconds
 */
unsigned long uptime() {
  static unsigned long lastUptime = 0;
  static unsigned long uptimeAdd = 0;
  unsigned long uptime = millis() / 1000 + uptimeAdd;
  if (uptime < lastUptime) {
    uptime += 4294967;
    uptimeAdd += 4294967;
  }
  lastUptime = uptime;
  return uptime;
}

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return(0);
  }
  time(&now);
  return now;
}

String msgToString(uint8_t *data, uint8_t len) {
  String s;
  for (int i = 0; i < len; i++) {
    if (data[i] < 0x10)
      s += "0";
    s += String(data[i], HEX);
    s += " ";
  }
  return s;
}

String msgToString(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data) {
  String s;
  for (int i = 0; i < data.size(); i++) {
    if (data[i] < 0x10)
      s += "0";
    s += String(data[i], HEX);
    s += " ";
  }
  return s;
}

void append_request(unsigned char *byte_array, int *offset, unsigned char *request, int request_size) {
    memcpy(byte_array + *offset, request, request_size);
    *offset += request_size;
}

String formatNumberWithCommas(uint32_t num) {
  return formatNumberWithCommas((unsigned long)num);
}

String formatNumberWithCommas(unsigned long num) {
    String numStr = String(num);
    int insertPosition = numStr.length() - 3;
    
    while (insertPosition > 0) {
        numStr = numStr.substring(0, insertPosition) + ',' + numStr.substring(insertPosition);
        insertPosition -= 3;
    }

    return numStr;
}

void _printTimestamp(Print* _logOutput) {

  // Division constants
  const unsigned long MSECS_PER_SEC       = 1000;
  const unsigned long SECS_PER_MIN        = 60;
  const unsigned long SECS_PER_HOUR       = 3600;
  const unsigned long SECS_PER_DAY        = 86400;

  // Total time
  const unsigned long msecs               =  millis();
  const unsigned long secs                =  msecs / MSECS_PER_SEC;

  // Time in components
  const unsigned long MilliSeconds        =  msecs % MSECS_PER_SEC;
  const unsigned long Seconds             =  secs  % SECS_PER_MIN ;
  const unsigned long Minutes             = (secs  / SECS_PER_MIN) % SECS_PER_MIN;
  const unsigned long Hours               = (secs  % SECS_PER_DAY) / SECS_PER_HOUR;

  // Time as string
  char timestamp[20];
  sprintf(timestamp, "%02d:%02d:%02d.%03d ", Hours, Minutes, Seconds, MilliSeconds);
  _logOutput->print(timestamp);
}

void _printLogLevel(Print* _logOutput, int logLevel) {
    /// Show log description based on log level
    switch (logLevel)
    {
        default:
        case 0:_logOutput->print("SILENT " ); break;
        case 1:_logOutput->print("FATAL "  ); break;
        case 2:_logOutput->print("ERROR "  ); break;
        case 3:_logOutput->print("WARNING "); break;
        case 4:_logOutput->print("INFO "   ); break;
        case 5:_logOutput->print("TRACE "  ); break;
        case 6:_logOutput->print("VERBOSE "); break;
    }   
}

void printPrefix(Print* _logOutput, int logLevel) {
    _printTimestamp(_logOutput);
 //   printLogLevel (_logOutput, logLevel);
}