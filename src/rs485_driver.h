#ifndef rs485_driver_h
#define rs485_driver_h
#include "esp32_spa.h"

void rs485_send();

struct rs485QueueMessage {
  char message[BALBOA_MESSAGE_SIZE];
} rs485QueueMessage_t;

#endif