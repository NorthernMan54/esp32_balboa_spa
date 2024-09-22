#include "spaCommunication.h"
#include <Arduino.h>
#include <ArduinoLog.h>
#include <WiFi.h>
#include <CircularBuffer.hpp>
#include <AsyncTCP.h>
#include <esp_task_wdt.h>

#include  <spaUtilities.h>
#include "../../src/main.h"

#define BALBOA_PORT 4257

CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> Q_in;

void spaCommunicationSetup()
{

};

// SPA Communication State's

AsyncClient client;

void dataAvailable(void *r, AsyncClient *c, void *buf, size_t len)
{
  // Log.verbose(F("[Comm]: Data Available %x, %d" CR), String((char *)buf).substring(0, len).c_str(), len);

  SpaReadQueueMessage *messageToSend = new SpaReadQueueMessage;
  messageToSend->length = (len < BALBOA_MESSAGE_SIZE ? len : BALBOA_MESSAGE_SIZE);
  for (int i = 0; i < messageToSend->length; i++)
  {
    messageToSend->message[i] = *((uint8_t *)buf + i);
  }

  if (xQueueSend(spaReadQueue, &messageToSend, 0) != pdTRUE)
  {
    Log.error(F("[Comm]: SPA Read Queue full, dropped %s" CR), msgToString(messageToSend->message, messageToSend->length).c_str());
  }
  else
  {
    // Log.verbose(F("[Comm]: Data added to Read Queue [%d]%s" CR),messageToSend->length, msgToString(messageToSend->message, messageToSend->length).c_str());
  }
}

unsigned long lastConnectTime = 0;

bool spaCommunicationLoop(IPAddress spaIP)
{
  if (!client.connected() && lastConnectTime < millis())
  {
    Log.verbose(F("[Comm]: Connecting to Spa %p" CR), spaIP);
    client.connect(spaIP, BALBOA_PORT);
    lastConnectTime = millis() + 5000;

    esp_task_wdt_init(RUNNING_WDT_TIMEOUT, true);
    client.onData(dataAvailable);
    client.onDisconnect([](void *r, AsyncClient *c)
                        { Log.verbose(F("[Comm]: Disconnected from Spa" CR)); });
    client.onConnect([](void *r, AsyncClient *c)
                     { Log.verbose(F("[Comm]: Connected to Spa %p" CR), c->remoteIP()); });
    client.onTimeout([](void *r, AsyncClient *c, uint32_t time)
                     { Log.verbose(F("[Comm]: Timeout from Spa" CR)); });
    client.onError([](void *r, AsyncClient *c, int8_t error)
                   { Log.verbose(F("[Comm]: Error from Spa %d" CR), error); });
    yield();
  }
  else if (client.connected() && uxQueueMessagesWaiting(spaWriteQueue) > 0)
  {
    SpaWriteQueueMessage *messageToSend;
    if (xQueueReceive(spaWriteQueue, &messageToSend, 0) == pdTRUE)
    {
      Log.verbose(F("[Comm]: Sending data to Spa %s" CR), msgToString(messageToSend->message, messageToSend->length).c_str());
      client.add(reinterpret_cast<const char *>(messageToSend->message), messageToSend->length);
      client.send();
      // client.write(messageToSend.message, messageToSend.length);
    }
    delete messageToSend;
  }
  else
  {
    // Log.verbose(F("[Comm]: No messages in Write Queue, and client not connected. %d" CR), client.connected());
  }

  // lastConnectTime is 5000,  millis is 4000 - return true
  // lastConnectTime is 5000,  millis is 6000 - return client.connected()
  // lastConnectTime is 0,  millis is 6000 - return client.connected()

  return (lastConnectTime > millis() ? true : client.connected());
};

void spaCommunicationEnd()
{
  Log.verbose(F("[Comm]: spaCommunicationEnd()" CR));
  if (client.connected())
  {
    client.close(true);
    client.free();
  }
};