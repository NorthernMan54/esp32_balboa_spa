#ifndef SPA_COMMUNICATION_H
#define SPA_COMMUNICATION_H
#include <Arduino.h>

void spaCommunicationSetup();
bool spaCommunicationLoop(IPAddress);
void spaCommunicationEnd();

#endif