#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <AsyncTCP.h>
#include <ArduinoLog.h>

#include <utilities.h>
#include <mqttModule.h>
#include <rs485.h>
#include <cacheRead.h>

#include "bridge.h"
#include "spaMessage.h"
#include "balboa.h"

#define publishBridge(...) mqtt.publish((mqttTopic + "bridge/msg").c_str(), __VA_ARGS__)

// Local functions

void clientDataAvailable(void *r, AsyncClient *c, void *buf, size_t len);

CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> Q_out;

uint8_t message[BALBOA_MESSAGE_SIZE];

AsyncServer bridge(BALBOA_PORT);          // Port number for the server
AsyncClient *clients[MAX_BRIDGE_CLIENTS]; // Array to hold the clients
WiFiUDP Discovery;

char packetBuffer[255]; // buffer to hold incoming packet
char replyBuffer[30];

bool bridgeStarted = false;

void bridgeSetup()
{
  String s = WiFi.macAddress();
  sprintf(replyBuffer, "BWGSPA\r\n00-15-27-%.2s-%.2s-%.2s\r\n", s.c_str() + 9, s.c_str() + 12, s.c_str() + 15);
  //  bridge.setNoDelay(true);
  Log.verbose(F("[Bridge]: Setup complete" CR));
};

void bridgeLoop()
{

  if (!bridgeStarted && WiFi.status() == WL_CONNECTED)
  {
    bridgeStarted = true;
    bridge.begin();
    Log.verbose(F("[Bridge]: Begin @ %p" CR), WiFi.localIP());
#ifdef LOCAL_CONNECT
    Discovery.begin(BALBOA_UDP_DISCOVERY_PORT);
#endif

    bridge.onClient([](void *arg, AsyncClient *client) { // On new client
      Log.verbose(F("[Bridge]: New Client Connected %p" CR), client->remoteIP());
      bool added = false;
      for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
      {
        if (clients[i] == nullptr || !clients[i]->connected())
        {
          clients[i] = client;
          publishDebug(("Bridge Client Connected (" + String(i) + ") " + clients[i]->remoteIP().toString()).c_str());
          added = true;
          clients[i]->onData(clientDataAvailable);
          clients[i]->onDisconnect([](void *r, AsyncClient *c)
                                   { Log.verbose(F("[Bridge]: Disconnected from Spa %p" CR), c->remoteIP()); });
          clients[i]->onConnect([](void *r, AsyncClient *c)
                                { Log.verbose(F("[Bridge]: Connected to Spa %p" CR), c->remoteIP()); });
          clients[i]->onTimeout([](void *r, AsyncClient *c, uint32_t time)
                                { Log.verbose(F("[Bridge]: Timeout from Spa" CR)); });
          clients[i]->onError([](void *r, AsyncClient *c, int8_t error)
                              { Log.verbose(F("[Bridge]: Error from Spa %d" CR), error); });
          break;
        }
      }

      if (!added)
      {
        publishError(("Bridge Client not Connected - no slots " + client->remoteIP().toString()).c_str());
        client->close();
      }
    },
                    NULL); // We don't need to pass any arguments to the callback
  }
  /*
  // Handle data from connected clients
  for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
  {
    if (clients[i]->connected())
    {
      if (!clients[i]->connected())
      {
        // Client disconnected
        publishDebug(("Client Disconnected (" + String(i) + ") " + clients[i]->remoteIP().toString()).c_str());
        clients[i]->stop();
      }
    }
  }*/

#ifdef LOCAL_CONNECT
  // Check for UDP Discovery
  int packetSize = Discovery.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Discovery.read(packetBuffer, 255);
    Discovery.beginPacket(Discovery.remoteIP(), Discovery.remotePort());
    Discovery.write((const uint8_t *)replyBuffer, strlen(replyBuffer));
    Discovery.endPacket();
    mqtt.publish((mqttTopic + "bridge/udpMsg").c_str(), packetBuffer);
  }
#endif
};

void clientDataAvailable(void *r, AsyncClient *client, void *buffer, size_t length)
{
  // Log.verbose(F("[Comm]: Data Available %x, %d" CR), String((char *)buf).substring(0, len).c_str(), len);

  if (length > 0 && length < BALBOA_MESSAGE_SIZE)
  {
    u_int8_t *message = (u_int8_t *)buffer;
    Log.verbose(F("[Bridge]: bridge/in %s" CR), msgToString(message, length).c_str());
    mqtt.publish((mqttTopic + "bridge/in").c_str(), msgToString(message, length).c_str());
    if (message[2] == id && message[4] == 0x04)
    {
      Q_out.clear();
      WiFi_Module_Configuration_Response(Q_out);
      bridgeSend(Q_out);
      Q_out.clear();
    }
    else
    {
      //  P_in.clear();
      publishBridge((String(length) + " Msg Received").c_str());
      cacheRead(message, length);
    }
  }
  else
  {
    publishBridge("No Msg Received");
  }
};

void bridgeSend(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  bool sent = false;
  for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
  {
    if (clients[i] && clients[i]->connected())
    {
      sent = true;

      Log.verbose(F("[Bridge]: before bridge/out %p - %s" CR), clients[i]->remoteIP(), msgToString(data).c_str());
      uint8_t output[BALBOA_MESSAGE_SIZE];
      data.copyToArray(output);
      clients[i]->add(reinterpret_cast<const char *>(output), data.size());
      if (!clients[i]->send())
      {
        Log.error(F("[Bridge]: Error sending to client %p - %s" CR), clients[i]->remoteIP(), msgToString(data).c_str());
      };

      // Log.verbose(F("[Bridge]: after bridge/out %p - %s" CR), clients[i]->remoteIP(), msgToString(data).c_str());
    }
    else
    {
    }
  }
  if (sent)
  {
    Log.verbose(F("[Bridge]: bridge/out %s" CR), msgToString(data).c_str());
    mqtt.publish((mqttTopic + "bridge/out").c_str(), msgToString(data).c_str());
  }
  else
  {
    publishBridge("Client not connected");
  }
  data.clear();
};

void bridgeSend(uint8_t *message, int length)
{
  bool sent = false;
  for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
  {
    if (clients[i] && clients[i]->connected())
    {
      sent = true;
      Log.verbose(F("[Bridge]: before bridge/out %p - %s" CR), clients[i]->remoteIP(), msgToString(message, length).c_str());
      clients[i]->add(reinterpret_cast<const char *>(message), length);
      if (!clients[i]->send())
      {
        Log.error(F("[Bridge]: Error sending to client %p - %s" CR), clients[i]->remoteIP(), msgToString(message, length).c_str());
      };
      // Log.verbose(F("[Bridge]: after bridge/out %p - %s" CR), clients[i]->remoteIP(), msgToString(message, length).c_str());
    }
  }
  if (sent)
  {
    // Log.verbose(F("[Bridge]: bridge/out %s" CR), msgToString(message, length).c_str());
    mqtt.publish((mqttTopic + "bridge/out").c_str(), msgToString(message, length).c_str());
  }
  else
  {
    publishBridge("Client not connected");
  }
};