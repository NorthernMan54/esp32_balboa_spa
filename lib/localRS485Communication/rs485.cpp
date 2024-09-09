#include <Arduino.h>
#include <CircularBuffer.hpp>
#include <ArduinoLog.h>
#include <esp_task_wdt.h>

#include <utilities.h>
#include <spaMessage.h>

#include "rs485.h"
#include "../../src/config.h"
#include "../../src/main.h"

// QueueHandle_t rs485WriteQueue;

// Local functions

void rs485Write(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void addCRC(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
bool isMessageValid(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void sendExistingClientResponse(uint8_t id);
// bool hasDayChanged();

RTC_NOINIT_ATTR Rs485Stats rs485Stats;

CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> spaMessage;
uint8_t id = 0x00; // spa id

#ifndef TX485_Tx
#define TX485_Tx 17
#endif

#ifndef TX485_Rx
#define TX485_Rx 16
#endif

#define RS_485_MAGIC_NUMBER 0x21345678

time_t lastCheckedTime;

void rs485Setup()
{
  if (!AUTO_TX)
  {
    pinMode(TX485_Tx, OUTPUT);
    digitalWrite(TX485_Tx, LOW);
  }
  // Spa communication, 115.200 baud 8N1
  Serial2.begin(115200, SERIAL_8N1, TX485_Rx, TX485_Tx);
  Log.verbose(F("[rs485]: RS485 setup, RX GPIO %d, TX GPIO %d" CR), TX485_Rx, TX485_Tx);

  lastCheckedTime = getTime();
  if (rs485Stats.magicNumber != RS_485_MAGIC_NUMBER)
  {
    Log.verbose(F("[Mess]: rs485Stats.magicNumber: %x" CR), rs485Stats.magicNumber);
    rs485Stats = {};
    rs485Stats.magicNumber = RS_485_MAGIC_NUMBER;
  }
};

/*

*/

uint8_t x;

void rs485Loop()
{
  if (Serial2.available())
  {
    x = Serial2.read();
    spaMessage.push(x);

    // Drop until SOF is seen
    if (spaMessage.first() != 0x7E)
      spaMessage.clear();
  }

  // Double SOF-marker, drop last one
  if (spaMessage[1] == 0x7E && spaMessage.size() > 1)
    spaMessage.pop();

  if (x == 0x7E && spaMessage.size() > 4 && spaMessage.size() == spaMessage[1] + 2 && isMessageValid(spaMessage))
  {
    // Log.verbose(F("[rs485]: Received: %d - %s" CR), id, msgToString(spaMessage).c_str());
    rs485Stats.messagesToday++;
    if (id == 0)
    {
      if (Status_Update(spaMessage)) // This is hacky, but it appears to work
      {
        id = WIFI_MODULE_ID;
        Log.verbose(F("[rs485]: Set SPA id 0x0A" CR));
        sendExistingClientResponse(id);
        esp_task_wdt_init(RUNNING_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
        spaMessage.clear();
      }

      // This method is used to assign a unique ID to the spa
      /*
          if (Channel_Assignment_Response(spaMessage))
          {
            id = spaMessage[5];
            if (id > 0x2F)
              id = 0x2F;

            ID_ack();
            mqtt.publish((mqttTopic + "node/id").c_str(), String(id, 16).c_str());
            publishDebug("Received SPA id");
            esp_task_wdt_init(RUNNING_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
          }

          // FE BF 00:Any new clients?
          if (New_Client_Clear_to_Send(spaMessage))
          {
            ID_request();
          }
          */
    }
    else if (Clear_to_Send(spaMessage))
    {
      rs485ClearToSend();
    }
    else if (For_Us_Message(spaMessage))
    {
      SpaReadQueueMessage *messageToSend = new SpaReadQueueMessage;
      messageToSend->length = (spaMessage.size() < BALBOA_MESSAGE_SIZE ? spaMessage.size() : BALBOA_MESSAGE_SIZE);
      for (int i = 0; i < messageToSend->length; i++)
      {
        messageToSend->message[i] = spaMessage[i];
      }

      if (xQueueSend(spaReadQueue, &messageToSend, 0) != pdTRUE)
      {
        Log.error(F("[rs485]: SPA Read Queue full, dropped %s" CR), msgToString(messageToSend->message, messageToSend->length).c_str());
      }
      else
      {
        // Log.verbose(F("[rs485]: Data added to Read Queue [%d]%s" CR),messageToSend->length, msgToString(messageToSend->message, messageToSend->length).c_str());
      }
    }
    spaMessage.clear();
  }

  if(hasDayChanged(lastCheckedTime)) {
    rs485Stats.messagesYesterday = rs485Stats.messagesToday;
    rs485Stats.crcYesterday = rs485Stats.crcToday;
    rs485Stats.messagesToday = 0;
    rs485Stats.crcToday = 0;
  }
};

void rs485ClearToSend()
{
  //  mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "rs485ClearToSend");
  rs485WriteQueueMessage *message;
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  if (xQueueReceive(spaWriteQueue, &message, 0) == pdTRUE)
  {
    for (int i = 0; i < message->length; i++)
    {
      dataBuffer.push(message->message[i]);
    }
    //   mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "Queue Receive");
    rs485Write(dataBuffer);
    delete message;
  }
  else
  {
    // A Nothing to Send message is sent by a client immediately after a
    // Clear to Send message if the client has no messages to send.
    dataBuffer.push(id);
    dataBuffer.push(0xBF);
    dataBuffer.push(0x07);
    addCRC(dataBuffer);
    //    mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "Clear to Send");
    rs485Write(dataBuffer);
  }
}

inline uint8_t crc8(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  unsigned long crc;
  int i, bit;
  uint8_t length = data.size();

  crc = 0x02;
  for (i = 0; i < length; i++)
  {
    crc ^= data[i];
    for (bit = 0; bit < 8; bit++)
    {
      if ((crc & 0x80) != 0)
      {
        crc <<= 1;
        crc ^= 0x7;
      }
      else
      {
        crc <<= 1;
      }
    }
  }

  return crc ^ 0x02;
}

inline uint8_t crc8(u_int8_t *data, int length)
{
  unsigned long crc;
  int i, bit;

  crc = 0x02;
  for (i = 0; i < length; i++)
  {
    crc ^= data[i];
    for (bit = 0; bit < 8; bit++)
    {
      if ((crc & 0x80) != 0)
      {
        crc <<= 1;
        crc ^= 0x7;
      }
      else
      {
        crc <<= 1;
      }
    }
  }

  return crc ^ 0x02;
}

void addCRC(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  // Add telegram length
  data.unshift(data.size() + 2);

  // Add CRC
  data.push(crc8(data));

  // Wrap telegram in SOF/EOF
  data.unshift(0x7E);
  data.push(0x7E);
}

void rs485Write(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  // The following is not required for the new RS485 chip
  if (AUTO_TX)
  {
  }
  else
  {
    digitalWrite(TX485_Tx, HIGH);
    delay(1);
  }
  for (int i = 0; i < data.size(); i++)
    Serial2.write(data[i]);

  Serial2.flush();

  if (AUTO_TX)
  {
  }
  else
  {
    digitalWrite(TX485_Tx, LOW);
  }

  // Log.verbose(F("[rs485]: Sent: %s" CR), msgToString(data).c_str());
  data.clear();
}

bool isMessageValid(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  if (data.size() < 2)
  {
    return false;
  }
  if (data[0] != 0x7E)
  {
    return false;
  }
  if (data[1] != data.size() - 2)
  {
    return false;
  }
  if (data.size() > BALBOA_MESSAGE_SIZE)
  {
    return false;
  }
  uint8_t message[BALBOA_MESSAGE_SIZE] = {0};
  for (int i = 1; i < data.size() - 2; i++)
  {
    message[i - 1] = data[i];
  }
  // Log.verbose(F("[rs485]: Data: %d - %s" CR), data.size(), msgToString(data).c_str());
  // Log.verbose(F("[rs485]: message: %s" CR), msgToString(message, data.size() - 3).c_str());
  // Log.verbose(F("[rs485]: CRC: %x, %x" CR), crc8(message, data.size() - 3), data[data[1]]);
  if (crc8(message, data.size() - 3) != data[data[1]])
  {
    rs485Stats.crcToday++;
    return false;
  }
  return true;
} // message[message[1]]

void sendExistingClientResponse(uint8_t id)
{
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  dataBuffer.push(id);
  dataBuffer.push(0xBF);
  dataBuffer.push(0x05);
  dataBuffer.push(0x04);
  dataBuffer.push(0x37);
  dataBuffer.push(0x00); // 08 10 BF 05 04 08 00 - Config request doesn't seem to work

  addCRC(dataBuffer);
  rs485Write(dataBuffer);
  Log.verbose(F("[rs485]: Sent Existing Client Response" CR), msgToString(dataBuffer).c_str());
}

/*
bool hasDayChanged() {
  time_t currentTime = now();  // Get the current time
  if (hour(currentTime) == 0 && hour(lastCheckedTime) != 0) {
    lastCheckedTime = currentTime;  // Update last checked time
    return true;  // Day has changed
  }
  lastCheckedTime = currentTime;  // Update last checked time
  return false;  // No day change
}
*/