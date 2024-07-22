#ifndef esp32_spa_h
#define esp32_spa_h

// Hardware configuration

#define AUTO_TX true   // if your chip needs to pull D1 high/low set this to false
#define TX485_Rx 16
#define TX485_Tx 17
#define RLY1 26
#define RLY2 27

// ArduinoLog configuration - Not implemented yet

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_NOTICE
#endif

#define PRODUCTION false  // Enables hard reset when no spa messages are received

#define VERSION "0.37.4"
#define SAVE_CONN true // save the ip details above to local filesystem

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

#define WDT_TIMEOUT 10 // watchdog timeout in seconds
// global variables

CircularBuffer<uint8_t, 35> Q_in;
CircularBuffer<uint8_t, 35> Q_out;
uint8_t id = 0x00;  // spa id

uint8_t last_state_crc = 0x00;
char have_config = 0;            // stages: 0-> want it; 1-> requested it; 2-> got it; 3->further processed it
char have_faultlog = 0;          // stages: 0-> want it; 1-> requested it; 2-> got it;3-> further processed it
char have_filtersettings = 0;    // stages: 0-> want it; 1-> requested it; 2-> gotit; 3-> further processed it
char ip_settings = 0;            // stages: 0-> want it; 1-> requested it; 2-> got it; 3->further processed it
char wifi_settings = 0;          // stages: 0-> want it; 1-> requested it; 2-> got it;3-> further processed it
char faultlog_minutes = 0;       // temp logic so we only get the fault log once per 5 minutes
char filtersettings_minutes = 0; // temp logic so we only get the filter settings once per 5 minutes

// WiFi and MQTT Configuration - values defined in config.h

String WIFI_SSID = _WIFI_SSID;
String WIFI_PASSWORD = _WIFI_PASSWORD;
String BROKER = _BROKER;
String BROKER_LOGIN = _BROKER_LOGIN;
String BROKER_PASS = _BROKER_PASS;

// HomeAssistant autodiscover
#define HASSIO false
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

// Global functions

WebServer httpServer(80);
HTTPUpdate httpUpdater;
WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);
String mqttTopic = "Spa/";
char gateway_name[20];

#endif