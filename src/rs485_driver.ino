#include "rs485_driver.h"

QueueHandle_t rs485WriteQueue;

// Local functions
void print_outMsg(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void print_outQMsg(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void rs485Write(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);
void addCRC(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data);

void rs485Setup()
{
  if (!AUTO_TX)
  {
    pinMode(TX485_Tx, OUTPUT);
    digitalWrite(TX485_Tx, LOW);
  }
  // Spa communication, 115.200 baud 8N1
  Serial2.begin(115200, SERIAL_8N1, TX485_Rx, TX485_Tx);
  rs485WriteQueue = xQueueCreate(RS485_WRITE_QUEUE, sizeof(rs485WriteQueueMessage_t));
};

void rs485Loop() {
};

void rs485Send(uint8_t *data, int length, boolean addCrc, boolean force)
{
  if (addCrc)
  {
    CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
    for (int i = 0; i < length; i++)
    {
      dataBuffer.push(data[i]);
    }
    rs485Send(dataBuffer, true, force);
  }
  rs485WriteQueueMessage messageToSend;
  for (int i = 0; i < length; i++)
  {
    messageToSend.message[i] = data[i];
  }
  messageToSend.length = length;
  if (xQueueSend(rs485WriteQueue, &messageToSend, 0) != pdTRUE)
  {
    publishError("RS485 Queue full");
  }
  else
  {
    //  mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "Queue Send");
  }
}

void rs485Send(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data, boolean addCrc, boolean force)
{
  if (addCrc)
  {
    addCRC(data);
  }
  if (!force)
  {
    rs485WriteQueueMessage messageToSend;
    for (int i = 0; i < data.size(); i++)
    {
      messageToSend.message[i] = data[i];
    }
    messageToSend.length = data.size();
    if (xQueueSend(rs485WriteQueue, &messageToSend, 0) != pdTRUE)
    {
      publishError("RS485 Queue full");
    }
    else
    {
      //    mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "Queue Send");
    }
  }
  else
  {
    rs485Write(data);
  }
}

void rs485ClearToSend()
{
  //  mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "rs485ClearToSend");
  rs485WriteQueueMessage message;
  CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> dataBuffer;
  if (xQueueReceive(rs485WriteQueue, &message, 0) == pdTRUE)
  {
    for (int i = 0; i < message.length; i++)
    {
      dataBuffer.push(message.message[i]);
    }
    //   mqtt.publish((mqttTopic + "node/rs485Queue").c_str(), "Queue Receive");
    rs485Write(dataBuffer);
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
#ifndef PRODUCTION
  print_outQMsg(data);
#endif

  if (AUTO_TX)
  {
  }
  else
  {
    digitalWrite(TX485_Tx, LOW);
  }

  switch (data[4]) // Command byte
  {
  case Filter_Cycles_Type:
    have_filtersettings = 0; // Reset flag
    break;
  }

  data.clear();
}

void print_outMsg(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  String s;
  // for (i = 0; i < (Q_in[1] + 2); i++) {
  for (int i = 0; i < data.size(); i++)
  {
    int x = data[i];
    if (x < 0x10)
      s += "0";
    s += String(x, HEX);
    s += " ";
  }
  mqtt.publish((mqttTopic + "node/outMsg").c_str(), s.c_str());
  _yield();
}

void print_outQMsg(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  String s;
  // for (i = 0; i < (Q_in[1] + 2); i++) {
  for (int i = 0; i < data.size(); i++)
  {
    int x = data[i];
    if (x < 0x10)
      s += "0";
    s += String(x, HEX);
    s += " ";
  }
  mqtt.publish((mqttTopic + "node/outQMsg").c_str(), s.c_str());
  _yield();
}