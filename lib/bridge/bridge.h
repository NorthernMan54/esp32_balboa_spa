#ifndef bridge_h
#define bridge_h
#include <Arduino.h>
#include <CircularBuffer.hpp>
#include "../../src/main.h"

#define BALBOA_PORT 4257
#define BALBOA_UDP_DISCOVERY_PORT 30303
#define MAX_BRIDGE_CLIENTS 10

void bridgeSetup();
void bridgeLoop();
void bridgeSend(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void bridgeSend(u_int8_t *message, int length);

#endif