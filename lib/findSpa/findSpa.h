#ifndef FINDSPA_H
#define FINDSPA_H

#include <Arduino.h>

void findSpaSetup();
bool findSpaLoop();
void resetSpaCount();
bool spaFound();
IPAddress getSpaIP();

bool spaAlreadyDiscovered(IPAddress ip);

#endif