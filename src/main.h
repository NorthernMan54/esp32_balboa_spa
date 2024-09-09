#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_VERBOSE
#endif

#define VERSION "0.0.1"
#define BUILD (String(__DATE__) + " - " + String(__TIME__)).c_str()

#define INITIAL_WDT_TIMEOUT 300 // Reset ESP32 if wifi is not connected within 5 minutes
#define RUNNING_WDT_TIMEOUT 120  // Reset ESP32 if no SPA messages are received for 60 seconds

#define logSection(section)                                                  \
  Log.setShowLevel(false);                                                   \
  Log.notice(F("************* " section " **************" CR)); \
  Log.setShowLevel(true);

// Global Message Queues

#define BALBOA_MESSAGE_SIZE 50

extern QueueHandle_t spaWriteQueue;
extern QueueHandle_t spaReadQueue;

struct SpaReadQueueMessage
{
  u_int8_t message[BALBOA_MESSAGE_SIZE];
  int length;
};

struct SpaWriteQueueMessage
{
  u_int8_t message[BALBOA_MESSAGE_SIZE];
  int length;
};

#ifdef LOCAL_CLIENT
#ifdef REMOTE_CLIENT
#error "Cannot define both LOCAL_CLIENT and REMOTE_CLIENT"
#endif
#endif

#ifndef LOCAL_CLIENT
#ifndef REMOTE_CLIENT
#error "Define either LOCAL_CLIENT or REMOTE_CLIENT"
#endif
#endif

#endif