#ifndef RESTART_REASON_H
#define RESTART_REASON_H
#include <Arduino.h>

String getLastRestartReason();
void setLastRestartReason(String description);
String getLastRestartReasonDescription();

#define RR_MAGIC_NUMBER 0x5A5A5A5A
#define RR_MAXIMUM_DESCRIPTION_LENGTH 30

#endif