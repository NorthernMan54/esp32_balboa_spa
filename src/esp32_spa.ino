// https://github.com/ccutrer/balboa_worldwide_app/blob/master/doc/protocol.md
// Reference:https://github.com/ccutrer/balboa_worldwide_app/wiki

// Please install the needed dependencies:
// CircularBuffer
// PubSubClient

// TODO:
// HomeAssistant autodiscover - DONE
// Configuration handling -> DONE
// Proper states (rather than just ON/OFF) -> NOT SURE HOW TO SOLVE THIS
// OTA update from Firebase -> TO DO
// ARDUINOOTA -> DOESN'T WORK YET -> SOMETHING WRONG WITH MDNS
// STA Mode to configure wifi -> WIP

// +12V RED
// GND  BLACK
// A    YELLOW
// B    WHITE
#include "balboa_helper.h"
#include "esp32_spa.h"
#include "rs485_bridge.h"
#include "rs485_driver.h"

#include <TickTwo.h>
#include <config.h>

uint8_t x, i, j;

void _yield()
{
  yield();
  mqtt.loop();
  httpServer.handleClient();
  // MDNS.update();
  ArduinoOTA.handle();
}

void print_msg(CircularBuffer<uint8_t, BALBOA_MESSAGE_SIZE> &data)
{
  String s;
  // for (i = 0; i < (Q_in[1] + 2); i++) {
  for (i = 0; i < data.size(); i++)
  {
    x = data[i];
    if (x < 0x10)
      s += "0";
    s += String(x, HEX);
    s += " ";
  }
  mqtt.publish((mqttTopic + "node/msg").c_str(), s.c_str());
  //  mqtt.publish((mqttTopic + "node/send").c_str(), String(send, 16).c_str());
  _yield();
}

///////////////////////////////////////////////////////////////////////////////

void hardReset()
{
  // ESP.wdtDisable();
  ESP.restart();
  while (1)
  {
  };
}

void mqttPubSub()
{
  // ONLY DO THE FOLLOWING IF have_config == true otherwise it will not work

  mqttDiscovery();

  mqtt.subscribe((mqttTopic + "target_temp/set").c_str());
  mqtt.subscribe((mqttTopic + "heatingmode/set").c_str());
  mqtt.subscribe((mqttTopic + "heat_mode/set").c_str());
  mqtt.subscribe((mqttTopic + "temprange/set").c_str());

  // OPTIONAL ELEMENTS
  if (SpaConfig.pump1 != 0)
  {
    mqtt.subscribe((mqttTopic + "jet_1/set").c_str());
  }
  if (SpaConfig.pump2 != 0)
  {
    mqtt.subscribe((mqttTopic + "jet_2/set").c_str());
  }
  if (SpaConfig.blower)
  {
    mqtt.subscribe((mqttTopic + "blower/set").c_str());
  }
  if (SpaConfig.light1)
  {
    mqtt.subscribe((mqttTopic + "light/set").c_str());
  }

#if defined(RLY1) || defined(RLY2)
  mqtt.subscribe((mqttTopic + "relay_1/set").c_str());
  mqtt.subscribe((mqttTopic + "relay_2/set").c_str());
#endif

  // not sure what this is
  last_state_crc = 0x00;

  // done with config
  have_config = 3;
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
    mqtt.connect(gateway_name, BROKER_LOGIN.c_str(), BROKER_PASS.c_str(), (mqttTopic + "node/state").c_str(), 1, true, "OFF");

    // time to connect
    delay(1000);

    if (mqtt.connected())
    {
      publishError("MQTT Timeout - Reconnect Successfully Run");
      mqtt.publish((mqttTopic + "node/state").c_str(), "ON");
      // have_config = 2;
      if (have_config >= 2)
      {
        // have_config = 2; we have disconnected, let's republish our configuration
        mqttPubSub();
      }
    }
  }
  mqtt.setBufferSize(512); // increase pubsubclient buffer size
}

// function called when a MQTT message arrived
void mqttCommand(char *p_topic, byte *p_payload, unsigned int p_length)
{
  // concat the payload into a string
  String payload;
  for (uint8_t i = 0; i < p_length; i++)
  {
    payload.concat((char)p_payload[i]);
  }
  String topic = String(p_topic);

  mqtt.publish((mqttTopic + "node/command").c_str(), (topic + " -> " + payload).c_str());
  _yield();

  // handle message topic
  if (topic.startsWith((mqttTopic + "relay_").c_str()))
  {
#if defined(RLY1) || defined(RLY2)
    bool newstate = 0;

    if (payload.equals("ON"))
      newstate = LOW;
    else if (payload.equals("OFF"))
      newstate = HIGH;

    if (topic.charAt(10) == '1')
    {
      pinMode(RLY1, INPUT);
      delay(25);
      pinMode(RLY1, OUTPUT);
      digitalWrite(RLY1, newstate);
    }
    else if (topic.charAt(10) == '2')
    {
      pinMode(RLY2, INPUT);
      delay(25);
      pinMode(RLY2, OUTPUT);
      digitalWrite(RLY2, newstate);
    }
#else
    publishError("Relays not defined");
#endif
  }
  else if (topic.equals((mqttTopic + "command").c_str()))
  {
    if (payload.equals("reset"))
      setLastRestartReason("MQTT Reset Command");
    _yield();
    hardReset();
  }
  else if (topic.equals((mqttTopic + "heatingmode/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.restmode == 1)
      panelButtonPress(BUTTON_HEAT_MODE); // ON = Ready; OFF = Rest
    else if (payload.equals("OFF") && SpaState.restmode == 0)
      panelButtonPress(BUTTON_HEAT_MODE);
  }
  else if (topic.equals((mqttTopic + "heat_mode/set").c_str()))
  {
    if (payload.equals("heat") && SpaState.restmode == 1)
      panelButtonPress(BUTTON_HEAT_MODE); // ON = Ready; OFF = Rest
    else if (payload.equals("off") && SpaState.restmode == 0)
      panelButtonPress(BUTTON_HEAT_MODE);
  }
  else if (topic.equals((mqttTopic + "light/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.light == 0)
      panelButtonPress(BUTTON_LIGHT1);
    else if (payload.equals("OFF") && SpaState.light == 1)
      panelButtonPress(BUTTON_LIGHT1);
  }
  else if (topic.equals((mqttTopic + "jet_1/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.jet1 == 0)
      panelButtonPress(BUTTON_PUMP1);
    else if (payload.equals("OFF") && SpaState.jet1 == 1)
      panelButtonPress(BUTTON_PUMP1);
  }
  else if (topic.equals((mqttTopic + "jet_2/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.jet2 == 0)
      panelButtonPress(BUTTON_PUMP2);
    else if (payload.equals("OFF") && SpaState.jet2 == 1)
      panelButtonPress(BUTTON_PUMP2);
  }
  else if (topic.equals((mqttTopic + "blower/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.blower == 0)
      panelButtonPress(BUTTON_BLOWER);
    else if (payload.equals("OFF") && SpaState.blower == 1)
      panelButtonPress(BUTTON_BLOWER);
  }
  else if (topic.equals((mqttTopic + "temprange/set").c_str()))
  {
    if (payload.equals("ON") && SpaState.temprange == 0)
      panelButtonPress(BUTTON_TEMP_RANGE); // ON = High, OFF = Low
    else if (payload.equals("OFF") && SpaState.temprange == 1)
      panelButtonPress(BUTTON_TEMP_RANGE);
  }
  else if (topic.equals((mqttTopic + "target_temp/set").c_str()))
  {
    setTemperature(payload);
  }
  else
  {
    publishError(("Unknown MQTT Topic: " + topic).c_str());
  }
}

void resetConfigStatus()
{
  if (have_config == 1)
  {
    have_config = 0;
    mqtt.publish((mqttTopic + "node/have_config").c_str(), String(have_config, 16).c_str());
  }
}
TickTwo resetConfig(resetConfigStatus, 1 * 60 * 1000); // 1 minutes

void resetFaultLog() { have_faultlog = 0; }
TickTwo faultlogTimer(resetFaultLog, 5 * 60 * 1000); // 5 minutes

void resetFilterStatus() { have_filtersettings = 0; }
TickTwo filterStatusTimer(resetFilterStatus, 5 * 60 * 1000); // 5 minutes

int getTime()
{
  time_t now;
  struct tm *now_tm;
  int hour;

  now = time(NULL);
  now_tm = localtime(&now);
  hour = now_tm->tm_hour;

  return hour;
}

void nodeStatusReport()
{
  mqtt.publish((mqttTopic + "node/uptime").c_str(), String(millis() / 1000).c_str());
  mqtt.publish((mqttTopic + "node/getTime").c_str(), String(getTime()).c_str());
  mqtt.publish((mqttTopic + "node/state").c_str(), "ON");
  mqtt.publish((mqttTopic + "node/version").c_str(), VERSION);
  mqtt.publish((mqttTopic + "node/flashsize").c_str(), String(ESP.getFlashChipSize()).c_str());
  mqtt.publish((mqttTopic + "node/chipid").c_str(), String(ESP.getChipModel()).c_str());
  mqtt.publish((mqttTopic + "node/speed").c_str(), String(ESP.getCpuFreqMHz()).c_str());
  mqtt.publish((mqttTopic + "node/heap").c_str(), String(ESP.getFreeHeap()).c_str());
  mqtt.publish((mqttTopic + "node/stack").c_str(), String(uxTaskGetStackHighWaterMark(NULL)).c_str());

  String release = String(__DATE__) + " - " + String(__TIME__);
  mqtt.publish((mqttTopic + "node/release").c_str(), release.c_str());

  // ... and resubscribe
  mqtt.subscribe((mqttTopic + "command").c_str());
  mqtt.publish((mqttTopic + "node/have_config").c_str(), String(have_config, 16).c_str());
  mqtt.publish((mqttTopic + "node/have_faultlog").c_str(), String(have_faultlog, 16).c_str());
  mqtt.publish((mqttTopic + "node/have_filtersettings").c_str(), String(have_filtersettings, 16).c_str());
}
TickTwo nodeStatusTimer(nodeStatusReport, 1 * 60 * 1000); // 1 minutes

#ifdef DS18B20_PIN
TickTwo ds18b20Timer(ds18b20loop, 5 * 60 * 1000); // 5 minutes
#endif

///////////////////////////////////////////////////////////////////////////////

void setup()
{
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
  restartReasonSetup();

  // Begin RS485 in listening mode -> no longer required with new RS485 chip

#if defined(RLY1) || defined(RLY2)
  pinMode(RLY1, OUTPUT);
  digitalWrite(RLY1, HIGH);
  pinMode(RLY2, OUTPUT);
  digitalWrite(RLY2, HIGH);
#endif

  // Serial log
  Serial.begin(115200);

  rs485Setup();

  // Setup gateway name and mqtt topic

  String s = WiFi.macAddress();
  sprintf(gateway_name, "spa-%.2s%.2s%.2s%.2s%.2s%.2s", s.c_str(),
          s.c_str() + 3, s.c_str() + 6, s.c_str() + 9, s.c_str() + 12,
          s.c_str() + 15);
  mqttTopic = mqttTopic + String(gateway_name) + "/";

  WiFi.setTxPower(WIFI_POWER_19_5dBm); // this sets wifi to highest power
  WiFi.begin(WIFI_SSID.c_str(), WIFI_PASSWORD.c_str());
  unsigned long timeout = millis() + 10000;

  while (WiFi.status() != WL_CONNECTED && millis() < timeout)
  {
    yield();
  }

  // Reset because of no connection
  if (WiFi.status() != WL_CONNECTED)
  {
    // STA LOGIC HERE
    wifi_settings = 1;

    // SAVE WIFI SETTINGS TO FILESYSTEM
    setLastRestartReason("WIFI Not Connected during startup");
    hardReset();
  }

  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
  bridgeSetup();
  balboaSetup();
  //  httpUpdater.setup(&httpServer, "admin", "");
  httpServer.begin();

  mqtt.setServer(BROKER.c_str(), 1883);
  mqtt.setCallback(mqttCommand);
  mqtt.setKeepAlive(10);
  mqtt.setSocketTimeout(20);

  if (!mqtt.connected())
    reconnect();

  MDNS.begin("spa");
  MDNS.addService("http", "tcp", 80);

  otaSetup();

  getLastRestartReason();
  setLastRestartReason("Unknown (No Info Stored)");

  nodeStatusReport();
  faultlogTimer.start();
  filterStatusTimer.start();
  nodeStatusTimer.start();
  resetConfig.start();

#ifdef DS18B20_PIN
  ds18b20Timer.start();
  ds18b20Setup();
#endif
  esp_task_wdt_init(INITIAL_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                       // add current thread to WDT watch

  // give Spa time to wake up after POST
  for (uint8_t i = 0; i < 5; i++)
  {
    delay(1000);
    yield();
  }

  Q_in.clear();
  publishDebug("Awaiting SPA Connection");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    setLastRestartReason("Wifi Failed");
    ESP.restart();
  }
  if (!mqtt.connected())
    reconnect();
  if (have_config == 2)
    mqttPubSub(); // do mqtt stuff after we're connected and if we have got the config elements
  // httpServer.handleClient(); needed?
  _yield();

  rs485Loop();
  faultlogTimer.update();
  filterStatusTimer.update();
  nodeStatusTimer.update();
  resetConfig.update();

  if (Serial2.available())
  {
    x = Serial2.read();
    Q_in.push(x);

    // Drop until SOF is seen
    if (Q_in.first() != 0x7E)
      Q_in.clear();

    esp_task_wdt_reset();
  }

  // Double SOF-marker, drop last one
  if (Q_in[1] == 0x7E && Q_in.size() > 1)
    Q_in.pop();

  if (x == 0x7E && Q_in.size() > 2 && Q_in.size() == Q_in[1] + 2 && validateCRC8(Q_in) == Q_in[Q_in[1]])
  {
    //  print_msg(Q_in);
    //  publishDebug(("Q_in.size()=" + String(Q_in.size(), 16) + ", Q_in[1]=" + String(Q_in[1], 16) + ", validateCRC8(Q_in)=" + String(validateCRC8(Q_in), 16) + ", Q_in[Q_in[1]]=" + String(Q_in[Q_in[1]], 16)).c_str());
    //  if (x == 0x7E && Q_in.size() > 2 && validateCRC8(Q_in) == Q_in[(Q_in[1] < Q_in.size() ? Q_in[Q_in[1]] : 0)])

#ifndef PRODUCTION
    print_msg(Q_in);
#endif
    if (Bridge_Message) // Send to bridge if it's a message for the bridge
      if (!Clear_to_Send)
      {
        bridgeSend(Q_in);
      }
    // Unregistered or yet in progress
    //    mqtt.publish((mqttTopic + "node/send").c_str(), String(send, 16).c_str());
    if (id == 0)
    {
      if (Status_Update) // This is hacky, but it appears to work
      {
        id = WIFI_MODULE_ID;
        sendExistingClientResponse(id);
        mqtt.publish((mqttTopic + "node/id").c_str(), String(id, 16).c_str());
        publishDebug("Set SPA id 0x0A");
        esp_task_wdt_init(RUNNING_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
        Q_in.clear();
      }
      if (Channel_Assignment_Response)
      {
        id = Q_in[5];
        if (id > 0x2F)
          id = 0x2F;

        ID_ack();
        mqtt.publish((mqttTopic + "node/id").c_str(), String(id, 16).c_str());
        publishDebug("Received SPA id");
        esp_task_wdt_init(RUNNING_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
      }

      // FE BF 00:Any new clients?
      if (New_Client_Clear_to_Send)
      {
        ID_request();
      }
    }
    else if (Clear_to_Send) // CTS
    {                       // we have an ID, do clever stuff
      if (have_config == 0)
      { // Get configuration of the hot tub
        requestConfig();
        have_config = 1;
      }
      else if (have_faultlog == 0 && have_config == 3)
      { // Get the fault log
        requestFaultLog();
        have_faultlog = 1;
      }
      else if ((have_filtersettings == 0) &&
               (have_faultlog ==
                2))
      { // Get the filter cycles log once we have the faultlog
        requestFilterSettings();
        have_filtersettings = 1;
      }
      else
      {
        rs485ClearToSend();
      }
    }
    else if (Configuration_Response)
    {
      // {"topic":"node/msg","payload":"7e b 19 bf 2e a 00 01 10 00 00 37 7e ","command":"2e","config":true}
      decodeConfig();
    }
    else if (Fault_Log_Response)
    {
      decodeFault();
    }
    else if (Status_Update && DeDuplicate)
    { // FF AF 13:Status Update - Packet index offset 5
      decodeStatus();
    }
    else if (Filter_Cycles_Message)
    {
      decodeFilterSettings();
      requestPreferences();
    }
    else if (Preferences_Response)
    {
      decodePreferences(Q_in);
    } // 0x26
    else if (Information_Response)
    {
      decodeInformation(Q_in);
    } // 0x24
    else
    {
#ifndef PRODUCTION
//      if (Q_in[2] & 0xFE || Q_in[2] == id)
//        print_msg(Q_in);
#endif
    }

    // Clean up queue
    _yield();
    Q_in.clear();
  }
  else if (Q_in.isFull() || (Q_in.size() > 2 && Q_in[1] > BALBOA_MESSAGE_SIZE))
  {
    Q_in.clear();
  }
  else
  {
  }

#ifdef DS18B20_PIN
  ds18b20Timer.update();
#endif
  bridgeLoop();
}
