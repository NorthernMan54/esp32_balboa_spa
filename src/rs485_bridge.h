#ifndef rs485_bridge_h
#define rs485_bridge_h
#include "esp32_spa.h"

void bridgeSetup();
void bridgeLoop();
void bridgeSend(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

#endif