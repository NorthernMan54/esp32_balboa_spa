#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <TickTwo.h>
#include <ArduinoLog.h>

// Local Libraries

#include <wifiModule.h>
#include <utilities.h>
#include <restartReason.h>
#include "mqttModule.h"
#include <rs485.h>

// Local Functions
void reconnect();
void mqttMessage(char *p_topic, byte *p_payload, unsigned int p_length);
void nodeStatusReport();

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
String mqttTopic = "Spa/"; // root topic, gets appeanded with node mac address

TickTwo sendStatus(nodeStatusReport, 1.5 * 60 * 1000); // 5 minutes

void mqttModuleSetup()
{
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttMessage);
  mqtt.setKeepAlive(10);
  mqtt.setSocketTimeout(20);
  mqttTopic = mqttTopic + String(gatewayName) + "/";
  Log.notice("MQTT Server: %s:%d\n", MQTT_SERVER, MQTT_PORT);
  Log.notice("MQTT Topic: %s\n", mqttTopic.c_str());
  sendStatus.start();
}

void mqttModuleLoop()
{
  // TODO: Implement MQTT module loop
  if (!mqtt.connected())
  {
    reconnect();
  }
  sendStatus.update();
  mqtt.loop();
}

void reconnect()
{
  // int oldstate = mqtt.state();
  // boolean connection = false;
  //  Loop until we're reconnected
  if (!mqtt.connected())
  {
    // Attempt to connect

    // connection =
    mqtt.connect(gatewayName, BROKER_LOGIN, BROKER_PASS, (mqttTopic + "node/state").c_str(), 1, true, "OFF");

    // time to connect
    delay(1000);

    if (mqtt.connected())
    {
      publishError("MQTT Timeout - Reconnect Successfully Run");
      mqtt.subscribe((mqttTopic + "command").c_str());
      nodeStatusReport();
    }
  }
  mqtt.setBufferSize(512); // increase pubsubclient buffer size
}

void mqttMessage(char *p_topic, byte *p_payload, unsigned int p_length)
{
  mqtt.publish(p_topic, p_payload, p_length);
}

void nodeStatusReport()
{
  if (mqtt.connected())
  {
    publishNodeStatus("ip", WiFi.localIP().toString().c_str());
    publishNodeStatus("mac", WiFi.macAddress().c_str());
    publishNodeStatus("gateway", gatewayName);
    publishNodeStatus("restartReason", getLastRestartReason().c_str());
    publishNodeStatus("uptime", String(millis() / 1000).c_str());
    publishNodeStatus("getTime", String(getTime()).c_str());
    publishNodeStatus("state", "ON");
    publishNodeStatus("flashsize", String(ESP.getFlashChipSize()).c_str());
    publishNodeStatus("chipid", String(ESP.getChipModel()).c_str());
    publishNodeStatus("speed", String(ESP.getCpuFreqMHz()).c_str());
    publishNodeStatus("heap", String(ESP.getFreeHeap()).c_str());
    publishNodeStatus("stack", String(uxTaskGetStackHighWaterMark(NULL)).c_str());
#ifdef LOCAL_CLIENT
    publishNodeStatus("rs485 messagesToday", String(rs485Stats.messagesToday).c_str());
    publishNodeStatus("rs485 crcToday", String(rs485Stats.crcToday).c_str());
    publishNodeStatus("rs485 messagesYesterday", String(rs485Stats.messagesYesterday).c_str());
    publishNodeStatus("rs485 crcYesterday", String(rs485Stats.crcYesterday).c_str());
    publishNodeStatus("rs485 badFormatToday", String(rs485Stats.badFormatToday).c_str());
    publishNodeStatus("rs485 badFormatYesterday", String(rs485Stats.badFormatYesterday).c_str());
#endif

    String release = String(__DATE__) + " - " + String(__TIME__);
    publishNodeStatus("release", release.c_str());
  }
}