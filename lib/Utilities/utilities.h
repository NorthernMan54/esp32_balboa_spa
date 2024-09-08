#ifndef ESP_UTILITIES_H
#define ESP_UTILITIES_H
#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

/**
 * Calculate uptime and take into account the millis() rollover
 * returns: unsigned long uptime in seconds
 */
unsigned long uptime();
unsigned long getTime();
String msgToString(uint8_t *data, uint8_t len);
String msgToString(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

void append_request(unsigned char *byte_array, int *offset, unsigned char *request, int request_size);
String formatNumberWithCommas(unsigned long num);
String formatNumberWithCommas(uint32_t num);
void printPrefix(Print *_logOutput, int logLevel);

#define hasDayChanged(lastCheckedTime) ( \
  { \
    time_t currentTime = now(); \
    bool dayChanged = (hour(currentTime) == 0 && hour(lastCheckedTime) != 0); \
    lastCheckedTime = currentTime; \
    dayChanged; \
  } \
)

#endif