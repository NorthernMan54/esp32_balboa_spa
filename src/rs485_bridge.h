#ifndef rs485_bridge_h
#define rs485_bridge_h
#include "esp32_spa.h"
#include <WiFiUdp.h>

#define BALBOA_PORT 4257
#define BALBOA_UDP_DISCOVERY_PORT 30303

void bridgeSetup();
void bridgeLoop();
void bridgeSend(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

#endif