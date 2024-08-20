#ifndef balboa_helper_h
#define balboa_helper_h
#include "esp32_spa.h"
#include "Analytics.h"

#define BALBOA_MESSAGE_SIZE 50
#define WIFI_MODULE_ID 0x0a

bool isMessageValid(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void ID_request();
void ID_ack();

void balboaSetup();
void decodeFault();
void decodeFilterSettings();
void decodeConfig();
void decodeStatus();
void decodePreferences(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void decodeInformation(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

void sendExistingClientResponse(uint8_t id);
void panelButtonPress(uint8_t button);
void setTemperature(String temp);
void setTime(uint8_t hour, uint8_t minute);
void requestConfig();
void requestFaultLog();
void requestFilterSettings();
void requestPreferences();

#define STRON String("ON").c_str()
#define STROFF String("OFF").c_str()
#define TwoBit(value, bit) (((value) >> (bit)) & 0x03)

#define DeDuplicate (last_state_crc != Q_in[Q_in[1]])
#define New_Client_Clear_to_Send (Q_in[2] == 0xFE && Q_in[4] == New_Client_Clear_to_Send_Type)
#define Channel_Assignment_Response (Q_in[2] == 0xFE && Q_in[4] == Channel_Assignment_Response_Type)
#define Existing_WiFi_Client_Request (Q_in[2] == 0x0a && Q_in[4] == Existing_Client_Request_Type) // 7e 05 0a bf 04 77 7e
#define Clear_to_Send (Q_in[2] == id && Q_in[4] == Clear_to_Send_Type)

// Message Types

#define New_Client_Clear_to_Send_Type 0x00
#define Channel_Assignment_Request_Type 0x01
#define Channel_Assignment_Response_Type 0x02
#define Channel_Assignment_Acknowledgement_Type 0x03
#define Existing_Client_Request_Type 0x04
#define Existing_Client_Response_Type 0x05
#define Clear_to_Send_Type 0x06
#define Nothing_to_Send_Type 0x07
#define Toggle_Item_Request_Type 0x11
#define Status_Message_Type 0x13
#define Set_Temperature_Type 0x20
#define Set_Time_Type 0x21
#define Settings_Request_Type 0x22
#define Filter_Cycles_Type 0x23
#define Information_Response_Type 0x24
#define Settings_0x04_Response_Type 0x25
#define Preferences_Type 0x26
#define Set_Preference_Request_Type 0x27
#define Fault_Log_Type 0x28
#define Lock_Settings_Type 0x2D
#define Configuration_Type 0x2E

#define Status_Update (Q_in[2] == 0xFF && Q_in[4] == Status_Message_Type)
#define Filter_Cycles_Message (Q_in[2] == id && Q_in[4] == Filter_Cycles_Type)
#define Fault_Log_Response (Q_in[2] == id && Q_in[4] == Fault_Log_Type)
#define Configuration_Response (Q_in[2] == id && Q_in[4] == Configuration_Type)
#define Preferences_Response (Q_in[2] == id && Q_in[4] == Preferences_Type)
#define Information_Response (Q_in[2] == id && Q_in[4] == Information_Response_Type)

#define Bridge_Message (id > 0 && (Q_in[2] == id || Q_in[2] == 0xFF))

#define STATUS_TIME_VALID (data[8] < 24 && data[9] < 60)
#define STATUS_TEMP_VALID (data[7] < 110 || data[7] == 0xFF)
#define STATUS_TARGET_TEMP_VALID (data[25] < 110 || data[25] == 0xFF)

#define WiFi_Module_Configuration_Response \
  Q_out.push(0x7E);                        \
  Q_out.push(0x1D);                        \
  Q_out.push(0x0A);                        \
  Q_out.push(0xBF);                        \
  Q_out.push(0x94);                        \
  Q_out.push(0x02);                        \
  Q_out.push(0x14);                        \
  Q_out.push(0x80);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x15);                        \
  Q_out.push(0x27);                        \
  Q_out.push(0x3F);                        \
  Q_out.push(0x9B);                        \
  Q_out.push(0x95);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x00);                        \
  Q_out.push(0x15);                        \
  Q_out.push(0x27);                        \
  Q_out.push(0xFF);                        \
  Q_out.push(0xFF);                        \
  Q_out.push(0x3F);                        \
  Q_out.push(0x9B);                        \
  Q_out.push(0x95);                        \
  Q_out.push(0x27);                        \
  Q_out.push(0x7E);

// Button Toggles command 0x11

#define BUTTON_NORMAL 0x01
#define BUTTON_CLEAR 0x03
#define BUTTON_PUMP1 0x04
#define BUTTON_PUMP2 0x05
#define BUTTON_PUMP3 0x06
#define BUTTON_PUMP4 0x07
#define BUTTON_PUMP5 0x08
#define BUTTON_PUMP6 0x09
#define BUTTON_BLOWER 0x0C
#define BUTTON_MISTER 0x0E
#define BUTTON_LIGHT1 0x11
#define BUTTON_LIGHT2 0x12
#define BUTTON_AUX1 0x16
#define BUTTON_AUX2 0x17
#define BUTTON_SOAK 0x1D
#define BUTTON_HOLD 0x3C
#define BUTTON_TEMP_RANGE 0x50 // Temperature Range (Low/High)
#define BUTTON_HEAT_MODE 0x51  // Heat Mode (Ready/Rest)

struct
{
  uint8_t jet1 : 2;
  uint8_t jet2 : 2;
  uint8_t blower : 1;
  uint8_t light : 1;
  uint8_t restmode : 2;
  uint8_t temprange : 1;
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