#ifndef rs485_driver_h
#define rs485_driver_h
#include "esp32_spa.h"

#define RS485_WRITE_QUEUE 10
void rs485Setup();
void rs485Loop();
void rs485ClearToSend();

void rs485Send(uint8_t *data, int length, boolean addCrc, boolean force = false);
void rs485Send(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data, boolean addCrc, boolean force = false);

void rs485_send();

struct rs485WriteQueueMessage
{
  char message[BALBOA_MESSAGE_SIZE];
  int length;
} rs485WriteQueueMessage_t;

#endif