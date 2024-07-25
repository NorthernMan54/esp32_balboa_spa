#ifndef esp32_spa_h
#define esp32_spa_h

// Hardware configuration

#define AUTO_TX true   // if your chip needs to pull D1 high/low set this to false
#define TX485_Rx 16
#define TX485_Tx 17
#define RLY1 26
#define RLY2 27

// DS18B20 configuration
#define DS18B20_PIN 4 // uncommment to enable

// ArduinoLog configuration - Not implemented yet

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_NOTICE
#endif

// #define PRODUCTION true  // Enables debug messages etc

#define VERSION "0.37.4"
#define SAVE_CONN true // save the ip details above to local filesystem

// Home Assistant Auto Discovery - uncommment to enable

// #define HASSIO true
// #define DISCOVERY_TOPIC "homeAssistant/" // MQTT Discovery topic

// No need to edit anything below this line

#include "balboa_helper.h"
#include "config.h" // MQTT and WiFi configuration

#include <CircularBuffer.hpp>
#include <ArduinoLog.h>
#include <ArduinoOTA.h>
#include <HTTPUpdate.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <PubSubClient.h> // MQTT client
#include <HardwareSerial.h>
#include <esp_task_wdt.h>

// MQTT Debug Messages

#define publishDebug(...) mqtt.publish((mqttTopic + "debug/message").c_str(), __VA_ARGS__);
#define publishError(...) mqtt.publish((mqttTopic + "debug/error").c_str(), __VA_ARGS__);

// Leverage ESP32 WDT, to reset the device if the spa is not connected within 5 minutes, and if after connection messages stop coming in for 10 seconds

#define INITIAL_WDT_TIMEOUT 300 // watchdog timeout in seconds
#define RUNNING_WDT_TIMEOUT 10 // watchdog timeout in seconds

// global variables

CircularBuffer<uint8_t, 35> Q_in;
CircularBuffer<uint8_t, 35> Q_out;
uint8_t id = 0x00;  // spa id

uint8_t last_state_crc = 0x00;   // Used the reduce the number of status updates messages processed ( ie if the CRC doesn't change, don't process the message)
char have_config = 0;            // stages: 0-> want it; 1-> requested it; 2-> got it; 3->further processed it
char have_faultlog = 0;          // stages: 0-> want it; 1-> requested it; 2-> got it;3-> further processed it
char have_filtersettings = 0;    // stages: 0-> want it; 1-> requested it; 2-> gotit; 3-> further processed it
char ip_settings = 0;            // stages: 0-> want it; 1-> requested it; 2-> got it; 3->further processed it
char wifi_settings = 0;          // stages: 0-> want it; 1-> requested it; 2-> got it;3-> further processed it

// WiFi and MQTT Configuration - values defined in config.h

String WIFI_SSID = _WIFI_SSID;
String WIFI_PASSWORD = _WIFI_PASSWORD;
String BROKER = _BROKER;
String BROKER_LOGIN = _BROKER_LOGIN;
String BROKER_PASS = _BROKER_PASS;

// HomeAssistant autodiscover, defaults to a different topic to avoid creating entities in HomeAssistant

#ifndef HASSIO
#define HASSIO false
#endif
void mqttDiscovery();
#ifndef DISCOVERY_TOPIC
#define DISCOVERY_TOPIC "testAssistant/" // MQTT Discovery topic
#endif
String discoveryTopic = DISCOVERY_TOPIC;

// restartReason
void getLastRestartReason();
void setLastRestartReason(const String &reason);
void restartReasonSetup();

// OTA

void otaSetup();

// ds18b20

void ds18b20Setup(void);
void ds18b20loop(void);

// Global functions

WebServer httpServer(80);
HTTPUpdate httpUpdater;
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
String mqttTopic = "Spa/";  // root topic, gets appeanded with node mac address
char gateway_name[20];

#endif