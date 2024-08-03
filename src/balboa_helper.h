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

#define DeDuplicate (last_state_crc != Q_in[Q_in[1]])
#define New_Client_Clear_to_Send (Q_in[2] == 0xFE && Q_in[4] == 0x00 )
#define Channel_Assignment_Response (Q_in[2] == 0xFE && Q_in[4] == 0x02 )
#define Existing_WiFi_Client_Request (Q_in[2] == 0x0a && Q_in[4] == 0x04)  // 7e 05 0a bf 04 77 7e
#define Clear_to_Send (Q_in[2] == id && Q_in[4] == 0x06 )

#define Status_Update (Q_in[2] == 0xFF && Q_in[4] == 0x13 && DeDuplicate)
#define Filter_Cycles_Message Q_in[2] == id && Q_in[4] == 0x23 && DeDuplicate
#define Fault_Log_Response Q_in[2] == id && Q_in[4] == 0x28 && DeDuplicate
#define Configuration_Response Q_in[2] == id && Q_in[4] == 0x2E && DeDuplicate

#define Bridge_Message (id > 0 && (Q_in[2] == id || Q_in[2] == 0xFF))
#define Existing_Client_Response Q_out.push(0x0A); Q_out.push(0xBF); Q_out.push(0x05); Q_out.push(0x04); Q_out.push(0x37); Q_out.push(0x00);  // 08 10 BF 05 04 08 00 - Config request doesn't seem to work
// #define WiFi_Module_Configuration_Request Q_out.push(0x0A); Q_out.push(0xBF); Q_out.push(0x05); Q_out.push(0x04); Q_out.push(0x08); Q_out.push(0x00);
#define WiFi_Module_Configuration_Response Q_out.push(0x7E); Q_out.push(0x1D); Q_out.push(0x0A); Q_out.push(0xBF); Q_out.push(0x94); Q_out.push(0x02); Q_out.push(0x14); Q_out.push(0x80); Q_out.push(0x00); Q_out.push(0x15); Q_out.push(0x27); Q_out.push(0x3F); Q_out.push(0x9B); Q_out.push(0x95); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x00); Q_out.push(0x15); Q_out.push(0x27); Q_out.push(0xFF); Q_out.push(0xFF); Q_out.push(0x3F); Q_out.push(0x9B); Q_out.push(0x95); Q_out.push(0x27); Q_out.push(0x7E);

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