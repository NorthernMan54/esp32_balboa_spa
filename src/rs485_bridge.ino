#include "esp32_spa.h"
#include "rs485_bridge.h"
#include "rs485_driver.h"

CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> Q_out;

uint8_t message[BALBOA_MESSAGE_SIZE];

void print_msg(String topic, uint8_t *Q_in, int length)
{
  String s;
  // for (i = 0; i < (Q_in[1] + 2); i++) {
  for (int i = 0; i < length; i++)
  {
    int x = Q_in[i];
    if (x < 0x10)
      s += "0";
    s += String(x, HEX);
    s += " ";
  }
  mqtt.publish((mqttTopic + topic).c_str(), s.c_str());
}

void print_msg(String topic, CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
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
  mqtt.publish((mqttTopic + topic).c_str(), s.c_str());
}

WiFiServer bridge(BALBOA_PORT);         // Port number for the server
WiFiClient clients[MAX_BRIDGE_CLIENTS]; // Array to hold the clients
WiFiUDP Discovery;

char packetBuffer[255];                                 // buffer to hold incoming packet
char ReplyBuffer[] = "BWGSPA\r\n00-15-27-00-00-01\r\n"; // a string to send back Q_out.push(0x3F); Q_out.push(0x9B); Q_out.push(0x95)

void bridgeSetup()
{
  bridge.begin();
#ifdef LOCAL_CONNECT
  Discovery.begin(BALBOA_UDP_DISCOVERY_PORT);
#endif
  //  bridge.setNoDelay(true);
};

void bridgeLoop()
{

  WiFiClient newClient = bridge.available();
  if (newClient)
  {
    // Check if there's space for a new client
    bool added = false;
    for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
    {
      if (!clients[i])
      {
        clients[i] = newClient;
        mqtt.publish((mqttTopic + "bridge/msg").c_str(), ("Bridge Connected (" + String(i) + ") " + clients[i].remoteIP().toString()).c_str());
        added = true;
        break;
      }
    }

    if (!added)
    {
      mqtt.publish((mqttTopic + "bridge/msg").c_str(), ("Bridge not Connected - no slots " + newClient.remoteIP().toString()).c_str());
      newClient.stop();
    }
  }

  // Handle data from connected clients
  for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
  {
    if (clients[i] && clients[i].connected())
    {
      if (clients[i].available())
      {
        int length = 0;
        // Need to handle "7e 08 0a bf 22 00 00 01 58 7e 7e 08 0a bf 22 04 00 00 f4 7e 7e 08 0a bf 22 01 00 00 34 7e 7e 08 0a bf 22 02 00 00 89 7e "
        // Which is config request (2e), followed by 04 (25), Filter Cycles Message (23), and Information Response ( 24 )
        length = clients[i].read(message, BALBOA_MESSAGE_SIZE);
        if (length > 0)
        {
          print_msg("bridge/in", message, length);
          if (message[2] == 0x0A && message[4] == 0x04)
          {
            Q_out.clear();
            WiFi_Module_Configuration_Response
                bridgeSend(Q_out);
            Q_out.clear();
          }
          else
          {
            //  P_in.clear();
            mqtt.publish((mqttTopic + "bridge/msg").c_str(), (String(length) + " Msg Received").c_str());
            rs485Send(message, length, false);
          }
        }
        else
        {
          mqtt.publish((mqttTopic + "bridge/msg").c_str(), "No Msg Received");
        }
      }
      else if (!clients[i].connected())
      {
        // Client disconnected
        mqtt.publish((mqttTopic + "bridge/msg").c_str(), ("Client Disconnected (" + String(i) + ") " + clients[i].remoteIP().toString()).c_str());
        clients[i].stop();
        clients[i] = WiFiClient();
      }
    }
  }

#ifdef LOCAL_CONNECT
  // Check for UDP Discovery
  int packetSize = Discovery.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Discovery.read(packetBuffer, 255);
    Discovery.beginPacket(Discovery.remoteIP(), Discovery.remotePort());
    Discovery.write((const uint8_t *)ReplyBuffer, strlen(ReplyBuffer));
    Discovery.endPacket();
    mqtt.publish((mqttTopic + "bridge/udpMsg").c_str(), packetBuffer);
  }
#endif
};

void bridgeSend(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  bool sent = false;
  for (int i = 0; i < MAX_BRIDGE_CLIENTS; i++)
  {
    if (clients[i] && clients[i].connected())
    {
      sent = true;

      uint8_t output[BALBOA_MESSAGE_SIZE];
      data.copyToArray(output);
      clients[i].write(output, data.size());
    }
    else
    {
    }
  }
  if (sent)
  {
    print_msg("bridge/out", data);
  }
  else
  {
    mqtt.publish((mqttTopic + "bridge/msg").c_str(), "Client not connected");
  }
};