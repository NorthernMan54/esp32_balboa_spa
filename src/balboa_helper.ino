#include "esp32_spa.h"
#include "balboa_helper.h"

#include "Analytics.h"
#include "rs485_driver.h"
#include "CRC8.h"
#include "CRC.h"

Analytics heaton;
Analytics filteron;

CRC8 crc;
uint8_t validateCRC8(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  if (data.size() > 3)
  {

    byte array[70];
    for (int i = 0; i < data.size() - 2; i++)
    {
      array[i] = data[i + 1];
    }
    // polynome, initial, xorOut, reverseIn, reverseOut)
    //  print_msg((uint8_t *)array, data.size() - 3);
    return calcCRC8((uint8_t *)array, data.size() - 3, 0x07, 0x02, 0x02);
  }
  else
  {
    return 0;
  }
}

void decodeFault()
{
  SpaFaultLog.totEntry = Q_in[5];
  SpaFaultLog.currEntry = Q_in[6];
  SpaFaultLog.faultCode = Q_in[7];
  switch (SpaFaultLog.faultCode)
  { // this is a inelegant way to do it, a lookup
    // table would be better
  case 15:
    SpaFaultLog.faultMessage = "Sensors are out of sync";
    break;
  case 16:
    SpaFaultLog.faultMessage = "The water flow is low";
    break;
  case 17:
    SpaFaultLog.faultMessage = "The water flow has failed";
    break;
  case 18:
    SpaFaultLog.faultMessage = "The settings have been reset";
    break;
  case 19:
    SpaFaultLog.faultMessage = "Priming Mode";
    break;
  case 20:
    SpaFaultLog.faultMessage = "The clock has failed";
    break;
  case 21:
    SpaFaultLog.faultMessage = "The settings have been reset";
    break;
  case 22:
    SpaFaultLog.faultMessage = "Program memory failure";
    break;
  case 26:
    SpaFaultLog.faultMessage = "Sensors are out of sync -- Call for service";
    break;
  case 27:
    SpaFaultLog.faultMessage = "The heater is dry";
    break;
  case 28:
    SpaFaultLog.faultMessage = "The heater may be dry";
    break;
  case 29:
    SpaFaultLog.faultMessage = "The water is too hot";
    break;
  case 30:
    SpaFaultLog.faultMessage = "The heater is too hot";
    break;
  case 31:
    SpaFaultLog.faultMessage = "Sensor A Fault";
    break;
  case 32:
    SpaFaultLog.faultMessage = "Sensor B Fault";
    break;
  case 34:
    SpaFaultLog.faultMessage = "A pump may be stuck on";
    break;
  case 35:
    SpaFaultLog.faultMessage = "Hot fault";
    break;
  case 36:
    SpaFaultLog.faultMessage = "The GFCI test failed";
    break;
  case 37:
    SpaFaultLog.faultMessage = "Standby Mode (Hold Mode)";
    break;
  default:
    SpaFaultLog.faultMessage = "Unknown error";
    break;
  }
  SpaFaultLog.daysAgo = Q_in[8];
  SpaFaultLog.hour = Q_in[9];
  SpaFaultLog.minutes = Q_in[10];
  mqtt.publish((mqttTopic + "fault/Entries").c_str(), String(SpaFaultLog.totEntry).c_str());
  mqtt.publish((mqttTopic + "fault/Entry").c_str(), String(SpaFaultLog.currEntry).c_str());
  mqtt.publish((mqttTopic + "fault/Code").c_str(), String(SpaFaultLog.faultCode).c_str());
  mqtt.publish((mqttTopic + "fault/Message").c_str(), SpaFaultLog.faultMessage.c_str());
  mqtt.publish((mqttTopic + "fault/DaysAgo").c_str(), String(SpaFaultLog.daysAgo).c_str());
  mqtt.publish((mqttTopic + "fault/Hours").c_str(), String(SpaFaultLog.hour).c_str());
  mqtt.publish((mqttTopic + "fault/Minutes").c_str(), String(SpaFaultLog.minutes).c_str());
  have_faultlog = 2;
  // mqtt.publish((mqttTopic + "debug/have_faultlog").c_str(), "have the
  // faultlog, #2");
  mqtt.publish((mqttTopic + "debug/message").c_str(), "Fault Log Received");
}

void decodeFilterSettings()
{
  String s;
  String d;
  String payld;

  SpaFilterSettings.filt1Hour = Q_in[5];
  SpaFilterSettings.filt1Minute = Q_in[6];
  SpaFilterSettings.filt1DurationHour = Q_in[7];
  SpaFilterSettings.filt1DurationMinute = Q_in[8];
  SpaFilterSettings.filt2Enable = bitRead(Q_in[9], 7); // check
  SpaFilterSettings.filt2Hour =
      Q_in[9] ^ (SpaFilterSettings.filt2Enable << 7); // check
  SpaFilterSettings.filt2Minute = Q_in[10];
  SpaFilterSettings.filt2DurationHour = Q_in[11];
  SpaFilterSettings.filt2DurationMinute = Q_in[12];
  // MQTT stuff
  /*mqtt.publish((mqttTopic + "config/filt1Hour").c_str(),
  String(SpaFilterSettings.filt1Hour).c_str()); mqtt.publish((mqttTopic +
  "config/filt1Minute").c_str(), String(SpaFilterSettings.filt1Minute).c_str());
  mqtt.publish((mqttTopic + "config/filt1DurationHour").c_str(),
  String(SpaFilterSettings.filt1DurationHour).c_str()); mqtt.publish((mqttTopic
  + "config/filt1DurationMinute").c_str(),
  String(SpaFilterSettings.filt1DurationMinute).c_str());
  mqtt.publish((mqttTopic + "config/filt2Hour").c_str(),
  String(SpaFilterSettings.filt2Hour).c_str()); mqtt.publish((mqttTopic +
  "config/filt2Minute").c_str(), String(SpaFilterSettings.filt2Minute).c_str());
  mqtt.publish((mqttTopic + "config/filt2DurationHour").c_str(),
  String(SpaFilterSettings.filt2DurationHour).c_str()); mqtt.publish((mqttTopic
  + "config/filt2DurationMinute").c_str(),
  String(SpaFilterSettings.filt2DurationMinute).c_str());
  mqtt.publish((mqttTopic + "config/filt2Enable").c_str(),
  String(SpaFilterSettings.filt2Enable).c_str());*/

  // Filter 1 time conversion
  if (SpaFilterSettings.filt1Hour < 10)
    s = "0";
  else
    s = "";
  s = String(SpaFilterSettings.filt1Hour) + ":";
  if (SpaFilterSettings.filt1Minute < 10)
    s += "0";
  s += String(SpaFilterSettings.filt1Minute);

  if (SpaFilterSettings.filt1DurationHour < 10)
    d = "0";
  else
    d = "";
  d = String(SpaFilterSettings.filt1DurationHour) + ":";
  if (SpaFilterSettings.filt1DurationMinute < 10)
    d += "0";
  d += String(SpaFilterSettings.filt1DurationMinute);

  payld = "{\"start\":\"" + s + "\",\"duration\":\"" + d + "\"}";
  mqtt.publish((mqttTopic + "filter/filter1").c_str(), payld.c_str());

  // Filter 2 time conversion
  if (SpaFilterSettings.filt2Hour < 10)
    s = "0";
  else
    s = "";
  s += String(SpaFilterSettings.filt2Hour) + ":";
  if (SpaFilterSettings.filt2Minute < 10)
    s += "0";
  s += String(SpaFilterSettings.filt2Minute);

  if (SpaFilterSettings.filt2DurationHour < 10)
    d = "0";
  else
    d = "";
  d += String(SpaFilterSettings.filt2DurationHour) + ":";
  if (SpaFilterSettings.filt2DurationMinute < 10)
    d += "0";
  d += String(SpaFilterSettings.filt2DurationMinute);
  if ((int)(SpaFilterSettings.filt2Enable) == 1)
    mqtt.publish((mqttTopic + "filter/filter2_enabled").c_str(), STRON);
  else
    mqtt.publish((mqttTopic + "filter/filter2_enabled").c_str(), STROFF);

  payld = "{\"start\":\"" + s + "\",\"duration\":\"" + d + "\"}";
  mqtt.publish((mqttTopic + "filter/filter2").c_str(), payld.c_str());

  mqtt.publish((mqttTopic + "debug/message").c_str(), "Filter Settings Received");
  have_filtersettings = 2;
}

void decodeConfig()
{
  // mqtt.publish((mqttTopic + "config/status").c_str(), "Got config");
  SpaConfig.pump1 = Q_in[5] & 0x03;
  SpaConfig.pump2 = (Q_in[5] & 0x0C) >> 2;
  SpaConfig.pump3 = (Q_in[5] & 0x30) >> 4;
  SpaConfig.pump4 = (Q_in[5] & 0xC0) >> 6;
  SpaConfig.pump5 = (Q_in[6] & 0x03);
  SpaConfig.pump6 = (Q_in[6] & 0xC0) >> 6;
  SpaConfig.light1 = (Q_in[7] & 0x03);
  SpaConfig.light2 = (Q_in[7] >> 2) & 0x03;
  SpaConfig.circ = ((Q_in[8] & 0x80) != 0);
  SpaConfig.blower = ((Q_in[8] & 0x03) != 0);
  SpaConfig.mister = ((Q_in[9] & 0x30) != 0);
  SpaConfig.aux1 = ((Q_in[9] & 0x01) != 0);
  SpaConfig.aux2 = ((Q_in[9] & 0x02) != 0);
  SpaConfig.temp_scale =
      Q_in[3] & 0x01; // Read temperature scale - 0 -> Farenheit, 1-> Celcius
  mqtt.publish((mqttTopic + "config/pumps1").c_str(), String(SpaConfig.pump1).c_str());
  mqtt.publish((mqttTopic + "config/pumps2").c_str(), String(SpaConfig.pump2).c_str());
  mqtt.publish((mqttTopic + "config/pumps3").c_str(), String(SpaConfig.pump3).c_str());
  mqtt.publish((mqttTopic + "config/pumps4").c_str(), String(SpaConfig.pump4).c_str());
  mqtt.publish((mqttTopic + "config/pumps5").c_str(), String(SpaConfig.pump5).c_str());
  mqtt.publish((mqttTopic + "config/pumps6").c_str(), String(SpaConfig.pump6).c_str());
  mqtt.publish((mqttTopic + "config/light1").c_str(), String(SpaConfig.light1).c_str());
  mqtt.publish((mqttTopic + "config/light2").c_str(), String(SpaConfig.light2).c_str());
  mqtt.publish((mqttTopic + "config/circ").c_str(), String(SpaConfig.circ).c_str());
  mqtt.publish((mqttTopic + "config/blower").c_str(), String(SpaConfig.blower).c_str());
  mqtt.publish((mqttTopic + "config/mister").c_str(), String(SpaConfig.mister).c_str());
  mqtt.publish((mqttTopic + "config/aux1").c_str(), String(SpaConfig.aux1).c_str());
  mqtt.publish((mqttTopic + "config/aux2").c_str(), String(SpaConfig.aux2).c_str());
  mqtt.publish((mqttTopic + "config/temp_scale").c_str(), String(SpaConfig.temp_scale).c_str());
  have_config = 2;
  mqtt.publish((mqttTopic + "debug/message").c_str(), "SPA Config Received");
}

void decodeStatus()
{
  String s;
  double d = 0.0;
  double c = 0.0;

  // DEBUG for finding meaning:
  // print_msg(Q_in);

  // 25:Flag Byte 20 - Set Temperature
  if ((Q_in[14] & 0x01) == 0)
  {
    d = Q_in[25];
  }
  else if ((Q_in[14] & 0x01) == 1)
  {
    d = Q_in[25] / 2;
    if (Q_in[25] % 2 == 1)
      d += 0.5;
  }

  mqtt.publish((mqttTopic + "status/target_temp").c_str(), String(d, 2).c_str());

  // 7:Flag Byte 2 - Actual temperature
  if (Q_in[7] != 0xFF)
  {
    if ((Q_in[14] & 0x01) == 0)
    {
      d = Q_in[7];
    }
    else if ((Q_in[14] & 0x01) == 1)
    {
      d = Q_in[7] / 2;
      if (Q_in[7] % 2 == 1)
        d += 0.5;
    }

    if (c > 0)
    {
      if ((d > c * 1.2) || (d < c * 0.8))
        d = c; // remove spurious readings greater or less than 20% away from// previous read
    }

    mqtt.publish((mqttTopic + "status/temperature").c_str(), String(d, 2).c_str());
    c = d;
  }
  else
  {
    d = 0;
  }
  // REMARK Move upper publish to HERE to get 0 for unknown temperature

  // 5:Flag Byte 0 - ?Spa State? 0x00=Running, 0x01=Initializing, 0x05=Hold Mode, ?0x14=A/B Temps ON?, 0x17=Test Mode
  mqtt.publish((mqttTopic + "status/spa_state").c_str(), String(Q_in[5]).c_str());
  // 6:Flag Byte 1 - ?Initialization Mode? 0x00=Idle, 0x01=Priming Mode, 0x02=?Fault?, 0x03=Reminder, 0x04=?Stage 1?, 0x05=?Stage 3?, 0x42=?Stage 2?
  mqtt.publish((mqttTopic + "status/spa_mode").c_str(), String(Q_in[6]).c_str());

  // 8:Flag Byte 3 Hour & 9:Flag Byte 4 Minute => Time
  if (Q_in[8] < 10)
    s = "0";
  else
    s = "";
  SpaState.hour = Q_in[8];
  s += String(Q_in[8]) + ":";
  if (Q_in[9] < 10)
    s += "0";
  s += String(Q_in[9]);
  SpaState.minutes = Q_in[9];
  mqtt.publish((mqttTopic + "status/time").c_str(), s.c_str());

  // 10:Flag Byte 5 - Heating Mode
  switch (Q_in[10])
  {
  case 0:
    mqtt.publish((mqttTopic + "status/heat_mode").c_str(), "Rest"); // Ready
    SpaState.restmode = 0;
    break;
  case 3:                                                                    // Ready-in-Rest
    mqtt.publish((mqttTopic + "status/heat_mode").c_str(), "Ready-in-Rest"); // Ready
    SpaState.restmode = 3;
    break;
  case 1:
    mqtt.publish((mqttTopic + "status/heat_mode").c_str(), "off"); // Rest
    SpaState.restmode = 1;
    break;
  }

  // 14:Flag Byte 9 - Temperature Scale, Clock Mode, Filter Mode
  // 0  Temperature Scale	0=1°F, 1=0.5°C
  SpaConfig.temp_scale = Q_in[14] & 0x01;
  // 1	Clock Mode	0=12-hour, 1=24-hour
  // 2		??
  // 3-4	Filter Mode	0=OFF, 1=Cycle 1, 2=Cycle 2, 3=Cycle 1 and 2  ( Docs said bits 3-4, but I found mine is 2-3 )
  switch (TwoBit(Q_in[14], 2))
  {
  case 0:
    mqtt.publish((mqttTopic + "status/filterMode").c_str(), STROFF); // Ready
    filteron.off();
    break;
  case 1:                                                               // Ready-in-Rest
    mqtt.publish((mqttTopic + "status/filterMode").c_str(), "Cycle 1"); // Ready
    filteron.on();
    break;
  case 2:
    mqtt.publish((mqttTopic + "status/filterMode").c_str(), "Cycle 2"); // Ready
    filteron.on();
    break;
  case 3:                                                                   // Ready-in-Rest
    mqtt.publish((mqttTopic + "status/filterMode").c_str(), "Cycle 1 & 2"); // Ready
    filteron.on();
    break;
  }
  mqtt.publish((mqttTopic + "status/filterOnTimeToday").c_str(), String(filteron.today()).c_str());
  mqtt.publish((mqttTopic + "status/filterOnTimeYesterday").c_str(), String(filteron.yesterday()).c_str());
  // 5	Panel Locked	0=No, 1=Yes
  // 6	??	0
  // 7	??	0

  // 15:Flags Byte 10 / Heat status - 0=OFF, 1=Heating, 2=Heat Waiting, Temp Range
  d = TwoBit(Q_in[15], 4);
  if (d == 0)
  {
    mqtt.publish((mqttTopic + "status/heatstate").c_str(), STROFF);
    heaton.off();
  }
  else if (d == 1 || d == 2)
  {
    mqtt.publish((mqttTopic + "status/heatstate").c_str(), STRON);
    heaton.on();
  }
  mqtt.publish((mqttTopic + "status/heaterOnTimeToday").c_str(), String(heaton.today()).c_str());
  mqtt.publish((mqttTopic + "status/heaterOnTimeYesterday").c_str(), String(heaton.yesterday()).c_str());

  d = bitRead(Q_in[15], 2);
  if (d == 0)
  {
    mqtt.publish((mqttTopic + "status/temprange").c_str(), STROFF); // LOW
    SpaState.temprange = 0;
  }
  else if (d == 1)
  {
    mqtt.publish((mqttTopic + "status/temprange").c_str(), STRON); // HIGH
    SpaState.temprange = 1;
  }

  d = bitRead(Q_in[15], 3);
  if (d == 0)
  {
    mqtt.publish((mqttTopic + "status/needHeat").c_str(), STROFF); // LOW
  }
  else if (d == 1)
  {
    mqtt.publish((mqttTopic + "status/needHeat").c_str(), STRON); // HIGH
  }

  // 16:Flags Byte 11
  if (TwoBit(Q_in[16], 0) == 2)
  {
    mqtt.publish((mqttTopic + "status/jet_1").c_str(), "High");
    SpaState.jet1 = 2;
  }
  else if (TwoBit(Q_in[16], 0) == 1)
  {
    mqtt.publish((mqttTopic + "status/jet_1").c_str(), STRON);
    SpaState.jet1 = 1;
  }
  else
  {
    mqtt.publish((mqttTopic + "status/jet_1").c_str(), STROFF);
    SpaState.jet1 = 0;
  }

  if (TwoBit(Q_in[16], 2) == 2)
  {
    mqtt.publish((mqttTopic + "status/jet_2").c_str(), "High");
    SpaState.jet1 = 2;
  }
  else if (TwoBit(Q_in[16], 2) == 1)
  {
    mqtt.publish((mqttTopic + "status/jet_2").c_str(), STRON);
    SpaState.jet1 = 1;
  }
  else
  {
    mqtt.publish((mqttTopic + "status/jet_2").c_str(), STROFF);
    SpaState.jet1 = 0;
  }

  // 18:Flags Byte 13
  if (bitRead(Q_in[18], 1) == 1)
    mqtt.publish((mqttTopic + "status/circ").c_str(), STRON);
  else
    mqtt.publish((mqttTopic + "status/circ").c_str(), STROFF);

  if (TwoBit(Q_in[18], 2) > 0)
  {
    mqtt.publish((mqttTopic + "status/blower").c_str(), STRON);
    SpaState.blower = 1;
  }
  else
  {
    mqtt.publish((mqttTopic + "status/blower").c_str(), STROFF);
    SpaState.blower = 0;
  }
  // 19:Flags Byte 14
  if (Q_in[19] == 0x03)
  {
    mqtt.publish((mqttTopic + "status/light").c_str(), STRON);
    SpaState.light = 1;
  }
  else
  {
    mqtt.publish((mqttTopic + "status/light").c_str(), STROFF);
    SpaState.light = 0;
  }

  last_state_crc = Q_in[Q_in[1]];

#ifdef RLY1
  // Publish own relay states
  s = "OFF";
  if (digitalRead(RLY1) == LOW)
    s = "ON";
  mqtt.publish((mqttTopic + "status/relay_1").c_str(), s.c_str());
#endif
#ifdef RLY2
  s = "OFF";
  if (digitalRead(RLY2) == LOW)
    s = "ON";
  mqtt.publish((mqttTopic + "status/relay_2").c_str(), s.c_str());
#endif
}

void sendExistingClientResponse(uint8_t id)
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x05);
  dataBuffer.push(0x04);
  dataBuffer.push(0x37);
  dataBuffer.push(0x00); // 08 10 BF 05 04 08 00 - Config request doesn't seem to work

  rs485Send(dataBuffer, true);
  publishDebug(("Publish Existing Module Response to: " + String(id, 16)).c_str());
}

inline void ID_request()
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(0xFE);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x01);
  dataBuffer.push(0x02);
  dataBuffer.push(0xF1);
  dataBuffer.push(0x73);

  rs485Send(dataBuffer, true);
}

inline void ID_ack()
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x03);

  rs485Send(dataBuffer, true);
}

inline void panelButtonPress(uint8_t button)
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x0A);
  dataBuffer.push(0x01);
  dataBuffer.push(button);

  rs485Send(dataBuffer, true);
}

void setTemperature(String temp)
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  double d = temp.toDouble();
  if (d > 0)
    d *= 2; // Convert to internal representation
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x20);
  dataBuffer.push(d);

  rs485Send(dataBuffer, true);
}

void requestConfig()
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x22);
  dataBuffer.push(0x00);
  dataBuffer.push(0x00);
  dataBuffer.push(0x01);
  publishDebug("Requesting SPA Configuration");

  rs485Send(dataBuffer, true);
}

void requestFaultLog()
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x22);
  dataBuffer.push(0x20);
  dataBuffer.push(0xFF);
  dataBuffer.push(0x00);
  publishDebug("Requesting SPA Fault Log");
  rs485Send(dataBuffer, true);
}

void requestFilterSettings()
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x22);
  dataBuffer.push(0x01);
  dataBuffer.push(0x00);
  dataBuffer.push(0x00);
  publishDebug("Requesting SPA Filter Settings");
  rs485Send(dataBuffer, true);
}