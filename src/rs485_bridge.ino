#include "esp32_spa.h"
#include "rs485_bridge.h"

#define BALBOA_PORT 4257

CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> P_in;
CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> P_out;

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

void print_msg(String topic, CircularBuffer<uint8_t, 35> &data)
{
  String s;
  // for (i = 0; i < (Q_in[1] + 2); i++) {
  for (int i = 0; i < data.size(); i++)
  {
    int x = Q_in[i];
    if (x < 0x10)
      s += "0";
    s += String(x, HEX);
    s += " ";
  }
  mqtt.publish((mqttTopic + topic).c_str(), s.c_str());
}

WiFiServer bridge(BALBOA_PORT); // Port number for the server
WiFiClient client;

void bridgeSetup()
{
  bridge.begin();
  //  bridge.setNoDelay(true);
};

void bridgeLoop()
{

  // Check for new client connections
  if (!client || !client.connected())
  {
    client = bridge.available(); // Accept new client connection
    if (client)
    {
      mqtt.publish((mqttTopic + "bridge/msg").c_str(), "Bridge Connected");
      Serial.println("New client connected");
    }
  }

  // If a client is connected, bridge data
  if (client && client.connected())
  {
    //  mqtt.publish((mqttTopic + "bridge/msg").c_str(), "Client Connected");
    if (client.available())
    {
      int length = 0;
      mqtt.publish((mqttTopic + "bridge/msg").c_str(), String(client.available()).c_str());
      while (client.available())
      {
        length = client.read(message, BALBOA_MESSAGE_SIZE);
      }
      if (length > 0)
      {
        print_msg("bridge/in", message, length);
        //  P_in.clear();
        mqtt.publish((mqttTopic + "bridge/msg").c_str(), (String(length) + " Msg Received").c_str());
        send = 0xfe;
        for (int i = 2; i < length - 2; i++)
        {
          Q_out.push(message[i]);
        }
      }
      else
      {
        mqtt.publish((mqttTopic + "bridge/msg").c_str(), "No Msg Received");
      }
    }
  }

  // Close the connection if the client has disconnected
  if (client && !client.connected())
  {
    mqtt.publish((mqttTopic + "bridge/msg").c_str(), "Client Disconnected");
    client.stop();
  }
};

void bridgeSend(CircularBuffer<uint8_t, 35> &data)
{
  if (client && client.connected())
  {
    print_msg("bridge/out", data);
    uint8_t output[BALBOA_MESSAGE_SIZE];
    data.copyToArray(output);
    client.write(output, data.size());
  }
  else
  {
    mqtt.publish((mqttTopic + "bridge/msg").c_str(), "Client not connected");
  }
};