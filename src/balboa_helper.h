#ifndef balboa_helper_h
#define balboa_helper_h
#include "esp32_spa.h"

uint8_t validateCRC8(CircularBuffer<uint8_t, 35> &data);
void ID_request();
void ID_ack();
void rs485_send();

void decodeFault();
void decodeFilterSettings();
void decodeConfig();
void decodeStatus();

#define STRON String("ON").c_str()
#define STROFF String("OFF").c_str()
#define BALBOA_MESSAGE_SIZE 35

struct
{
  uint8_t jet1 : 2;
  uint8_t jet2 : 2;
  uint8_t blower : 1;
  uint8_t light : 1;
  uint8_t restmode : 1;
  uint8_t highrange : 1;
  uint8_t padding : 2;
  uint8_t hour : 5;
  uint8_t minutes : 6;
} SpaState;

struct
{
  uint8_t pump1 : 2; // this could be 1=1 speed; 2=2 speeds
  uint8_t pump2 : 2;
  uint8_t pump3 : 2;
  uint8_t pump4 : 2;
  uint8_t pump5 : 2;
  uint8_t pump6 : 2;
  uint8_t light1 : 1;
  uint8_t light2 : 1;
  uint8_t circ : 1;
  uint8_t blower : 1;
  uint8_t mister : 1;
  uint8_t aux1 : 1;
  uint8_t aux2 : 1;
  uint8_t temp_scale : 1; // 0 -> Farenheit, 1-> Celcius
} SpaConfig;

struct
{
  uint8_t totEntry : 5;
  uint8_t currEntry : 5;
  uint8_t faultCode : 6;
  String faultMessage;
  uint8_t daysAgo : 8;
  uint8_t hour : 5;
  uint8_t minutes : 6;
} SpaFaultLog;

struct
{
  uint8_t filt1Hour : 5;
  uint8_t filt1Minute : 6;
  uint8_t filt1DurationHour : 5;
  uint8_t filt1DurationMinute : 6;
  uint8_t filt2Enable : 1;
  uint8_t filt2Hour : 5;
  uint8_t filt2Minute : 6;
  uint8_t filt2DurationHour : 5;
  uint8_t filt2DurationMinute : 6;

} SpaFilterSettings;

#endif