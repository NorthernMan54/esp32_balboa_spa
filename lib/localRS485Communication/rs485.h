#ifndef RS485_H
#define RS485_H

#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

#define RS485_WRITE_QUEUE 10

void rs485Setup();
void rs485Loop();
void rs485ClearToSend();

void rs485Send(uint8_t *data, int length, boolean addCrc, boolean force = false);
void rs485Send(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data, boolean addCrc, boolean force = false);

extern uint8_t id; // spa id

struct rs485WriteQueueMessage
{
  char message[BALBOA_MESSAGE_SIZE];
  int length;
};

#endif