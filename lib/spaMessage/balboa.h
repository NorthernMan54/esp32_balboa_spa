#ifndef BALBOA_H
#define BALBOA_H
#include <Arduino.h>
#include "../../src/main.h"
#include <map>

// Message Types

#define STALE_TIME 60 * 60
#define RETRY_TIME 10 * 60

#define staleData(key) (key.lastUpdate == 0 || key.lastUpdate + STALE_TIME < getTime())
#define retryRequest(key) (key.lastRequest == 0 || key.lastRequest + RETRY_TIME < getTime())

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
#define WiFi_Module_Configuration_Type 0x94

// Configuration, Settings 0x04, Filter Cycles, and Information Messages
#define CONFIGURATION_REQUEST   {0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x00, 0x00, 0x01, 0x58, 0x7e}
#define SETTINGS_0X04_REQUEST   {0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x04, 0x00, 0x00, 0xf4, 0x7e}
#define FILTER_SETTINGS_REQUEST {0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x01, 0x00, 0x00, 0x34, 0x7e}
#define INFORMATION_REQUEST     {0x7e, 0x08, 0x0a, 0xbf, 0x22, 0x02, 0x00, 0x00, 0x89, 0x7e}

/*
Settings Code	Name	Subsequent Arguments	Response
0x00	Configuration	0x00, 0x01	Configuration Response
0x01	Filter Cycles	0x00, 0x00	Filter Cycles Message
0x02	Information	0x00, 0x00	Information Response
0x04	??	0x00, 0x00	Settings 0x04 Response
0x08	Preferences	0x00, 0x00	Preferences Response
0x10	??	0x00,0x00	(None)
0x20	Fault Log	Entry Number (0-23, 0xFF is last fault), 0x00	Fault Log Response
0x40	??	0x00, 0x00	Settings 0x40 Response
0x80	GFCI Test	0x00, 0x00	GFCI Test Response
*/

#define Configuration_Req_Type 0x00
#define Filter_Cycles_Req_Type 0x01
#define Information_Req_Type 0x02
#define Settings_0x04_Req_Type 0x04
#define Preferences_Req_Type 0x08
#define Fault_Log_Req_Type 0x20
#define Settings_0x40_Req_Type 0x40
#define GFCI_Test_Req_Type 0x80

#define WIFI_MODULE_ID 0x0a

#define Status_Update(Q_in) (Q_in[2] == 0xFF && Q_in[4] == Status_Message_Type)
#define Filter_Cycles_Message(Q_in) (Q_in[2] == id && Q_in[4] == Filter_Cycles_Type)
#define Fault_Log_Response(Q_in) (Q_in[2] == id && Q_in[4] == Fault_Log_Type)
#define Configuration_Response(Q_in) (Q_in[2] == id && Q_in[4] == Configuration_Type)
#define Preferences_Response(Q_in) (Q_in[2] == id && Q_in[4] == Preferences_Type)
#define Information_Response(Q_in) (Q_in[2] == id && Q_in[4] == Information_Response_Type)

#define DeDuplicate(Q_in) (last_state_crc != Q_in[Q_in[1]])
#define New_Client_Clear_to_Send(Q_in) (Q_in[2] == 0xFE && Q_in[4] == New_Client_Clear_to_Send_Type)
#define Channel_Assignment_Response(Q_in) (Q_in[2] == 0xFE && Q_in[4] == Channel_Assignment_Response_Type)
#define Existing_WiFi_Client_Request(Q_in) (Q_in[2] == 0x0a && Q_in[4] == Existing_Client_Request_Type) // 7e 05 0a bf 04 77 7e
#define Clear_to_Send(Q_in) (Q_in[2] == id && Q_in[4] == Clear_to_Send_Type)

#define For_Us_Message(Q_in) (Q_in[2] == id || Q_in[2] == 0xFF)  

#define WiFi_Module_Configuration_Response(Q_out) \
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

struct SpaStatusData
{
  uint8_t crc;
  unsigned long lastUpdate;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  uint8_t spaState;
  uint8_t initMode;
  float currentTemp;
  char time[6];
  uint8_t heatingMode;
  uint8_t reminderType;
  uint8_t sensorA;
  uint8_t sensorB;
  uint8_t tempScale;
  uint8_t clockMode;
  uint8_t filterMode;
  bool panelLocked;
  uint8_t tempRange;
  bool needsHeat;
  uint8_t heatingState;

  uint8_t pump1;
  uint8_t pump2;
  uint8_t pump3;
  uint8_t pump4;
  uint8_t pump5;
  uint8_t pump6;

  uint8_t circ;
  uint8_t blower;

  bool light1;
  bool light2;

  bool mister;

  float setTemp;
  float lowSetTemp;
  float highSetTemp;
  uint8_t notification;
  uint8_t flags19;
  bool settingsLock;
  uint8_t m8CycleTime;

// Analytics

  unsigned long heaterOnTimeToday;
  unsigned long heaterOnTimeYesterday;
  unsigned long filterOnTimeToday;
  unsigned long filterOnTimeYesterday;

};

struct SpaConfigurationData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  uint8_t pump1;
  uint8_t pump2;
  uint8_t pump3;
  uint8_t pump4;
  uint8_t pump5;
  uint8_t pump6;
  uint8_t light1;
  uint8_t light2;
  bool circulationPump;
  bool blower;
  bool mister;
  bool aux1;
  bool aux2;
  bool temp_scale;
};

struct SpaFilterSettingsData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  uint8_t filt1Hour;
  uint8_t filt1Minute;
  uint8_t filt1DurationHour;
  uint8_t filt1DurationMinute;
  bool filt2Enable;
  uint8_t filt2Hour;
  uint8_t filt2Minute;
  uint8_t filt2DurationHour;
  uint8_t filt2DurationMinute;
};

struct SpaFaultLogData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  uint8_t totEntry;
  uint8_t currEntry;
  uint8_t faultCode;
  String faultMessage;
  uint8_t daysAgo;
  uint8_t hour;
  uint8_t minutes;
};

struct SpaInformationData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  char softwareID[10];
  char model[10];
  uint8_t setupNumber;
  uint8_t voltage;
  uint8_t heaterType;
  char dipSwitch[3];
};

struct WiFiModuleConfigurationData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;

  char macAddress[11];
};

struct SpaSettings0x04Data
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;
};

struct SpaPreferencesData
{
  uint8_t crc;
  unsigned long lastUpdate;
  unsigned long lastRequest;
  u_int32_t magicNumber;
  uint8_t rawData[BALBOA_MESSAGE_SIZE];
  uint8_t rawDataLength;

  uint8_t reminders;
  uint8_t tempScale;
  uint8_t clockMode;
  uint8_t cleanupCycle;
  uint8_t dolphinAddress;
  uint8_t m8AI;
};

#define SPA_STATE_MAP { \
  {0, "Running"}, \
  {1, "Initializing"}, \
  {5, "Hold Mode"}, \
  {0x14, "A/B Temps ON"}, \
  {0x17, "Test Mode"} \
}

#define SPA_MODE_MAP {                         \
  {0, "Idle"},                                 \
  {1, "Priming Mode"},                         \
  {2, "Fault"},                                \
  {3, "Reminder"},                             \
  {4, "Stage 1"},                              \
  {5, "Stage 3"},                              \
  {0x42, "Stage 2"}                            \
}

#define FILTER_MODE_MAP {                                 \
  {0, "Off"},                                            \
  {1, "Cycle 1"},                                         \
  {2, "Cycle 2"},                                         \
  {3, "Cycle 1 & 2"}                                      \
}

#define TEMP_RANGE_MAP {                          \
  {0, "Low Range"},                               \
  {1, "High Range"}                               \
}

#define PUMP_STATE_MAP {                            \
  {0, "Off"},                                      \
  {1, "Low"},                                       \
  {2, "High"}                                       \
}

#define LOCKED_MAP {                          \
  {0, "Unlocked"},                            \
  {1, "Locked"}                               \
}

#define ON_OFF_MAP {                          \
  {0, "Off"},                                 \
  {1, "On"}                                   \
}

#endif