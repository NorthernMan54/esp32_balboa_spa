#ifndef SPA_UTILITIES_H
#define SPA_UTILITIES_H
#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

/**
 * Calculate uptime and take into account the millis() rollover
 * returns: unsigned long uptime in seconds
 */
time_t uptime();
time_t getTime();
int getHour(time_t time);
int getHour();
String msgToString(uint8_t *data, uint8_t len);
String msgToString(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

void append_request(unsigned char *byte_array, int *offset, unsigned char *request, int request_size);
String formatNumberWithCommas(unsigned long num);
String formatNumberWithCommas(uint32_t num);
String formatNumberWithCommas(int num);
String formatNumberWithCommas(time_t num);
void logPrintPrefix(Print *_logOutput, int logLevel);
String historyToString(float *history);
String formatAsHourMinute(uint8_t hour, uint8_t minute);

#define hasDayChanged(lastCheckedTime) (                                              \
    {                                                                                 \
      time_t currentTime = getTime();                                                 \
      bool dayChanged = (getHour(currentTime) == 0 && getHour(lastCheckedTime) != 0); \
      lastCheckedTime = currentTime;                                                  \
      dayChanged;                                                                     \
    })

extern String weekday_D[];
extern String month_M[];

#endif