#ifndef RS485_H
#define RS485_H

#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

#define RS485_WRITE_QUEUE 10

void rs485Setup();
void rs485Loop();
void rs485ClearToSend();

// void rs485Send(uint8_t *data, int length, boolean addCrc, boolean force = false);
// void rs485Send(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data, boolean addCrc, boolean force = false);

extern uint8_t id; // spa id

// Analytics

struct Rs485Stats
{
  uint32_t messagesToday;
  uint32_t messagesYesterday;
  uint32_t crcToday;
  uint32_t crcYesterday;
  uint32_t magicNumber;
};

extern Rs485Stats rs485Stats;

struct rs485WriteQueueMessage
{
  char message[BALBOA_MESSAGE_SIZE];
  int length;
};

#endif