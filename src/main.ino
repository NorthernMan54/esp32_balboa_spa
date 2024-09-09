#include <Arduino.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ArduinoLog.h>
#include <esp_task_wdt.h>
#include <TelnetStream.h>

// Local Libraries
#include <restartReason.h>
#include <wifiModule.h>
#include <findSpa.h>
#include <spaCommunication.h>
#include <spaMessage.h>
#include <spaWebServer.h>
#include <utilities.h>
#include <mqttModule.h>
#include <rs485.h>
#include <bridge.h>

#include "main.h"

void setup()
{
  // Launch serial for debugging purposes
  Serial.begin(SERIAL_BAUD);
  Log.setPrefix(printPrefix);
  Log.begin(LOG_LEVEL, &Serial);
  esp_task_wdt_init(INITIAL_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                       // add current thread to WDT watch
  logSection("WELCOME TO esp32_balboa_spa");
  Log.notice(F("Version: %s" CR), VERSION);
  Log.notice(F("Build: %s" CR), BUILD);

  logSection("Build Definitions");
  #ifdef LOCAL_CONNECT
  Log.notice(F("LOCAL_CONNECT Enabled" CR));
  #else
  Log.notice(F("LOCAL_CONNECT Disabled" CR));
  #endif
  #ifdef LOCAL_CLIENT
  Log.notice(F("LOCAL_CLIENT Enabled" CR));
  #else
  Log.notice(F("LOCAL_CLIENT Disabled" CR));
  #endif
  #ifdef REMOTE_CLIENT
  Log.notice(F("REMOTE_CLIENT Enabled" CR));
  #else
  Log.notice(F("REMOTE_CLIENT Disabled" CR));
  #endif
  #ifdef TELNET_LOG
  Log.notice(F("TELNET_LOG Enabled" CR));
  #else
  Log.notice(F("TELNET_LOG Disabled" CR));
  #endif

  logSection("ESP Information");
  Log.notice(F("Last restart reason: %s" CR), getLastRestartReason().c_str());
  Log.verbose(F("Free heap: %d bytes" CR), ESP.getFreeHeap());
  Log.verbose(F("Free sketch space: %d bytes" CR), ESP.getFreeSketchSpace());
  Log.verbose(F("Chip ID: %x" CR), ESP.getEfuseMac());

  Log.verbose(F("Flash chip size: %d bytes" CR), ESP.getFlashChipSize());
//  Log.verbose(F("Flash chip speed: %d Hz" CR), ESP.getFlashChipSpeed());
  Log.verbose(F("CPU frequency: %d Hz" CR), ESP.getCpuFreqMHz());
  Log.verbose(F("SDK version: %s" CR), ESP.getSdkVersion());

  logSection("Wifi Module Setup");
  wifiModuleSetup();
  logSection("MQTT Module Setup");
  mqttModuleSetup();
#ifdef LOCAL_CLIENT
  logSection("RS485 Module Setup");
  rs485Setup();
#endif
#ifdef REMOTE_CLIENT
  logSection("Find Remote Spa Setup");
  findSpaSetup();
  logSection("Spa Remote Communications Setup");
  spaCommunicationSetup();
#endif
  logSection("Spa Message Setup");
  spaMessageSetup();
  logSection("Web Server Setup");
  spaWebServerSetup();
#ifdef LOCAL_CONNECT
  logSection("Bridge Setup");
  bridgeSetup();
#endif
  logSection("Setup Complete");
}

void loop()
{
#ifdef LOCAL_CLIENT
  rs485Loop();
#endif
  wifiModuleLoop();

  if (WiFi.status() == WL_CONNECTED)
  {
    mqttModuleLoop();
#ifdef REMOTE_CLIENT
    if (findSpaLoop())
    {
      if (!spaCommunicationLoop(getSpaIP()))
      {
        Log.verbose(F("[Main]: spaCommunicationLoop failed, client disconnected" CR));
        resetSpaCount();
      }
    }
#endif
    spaMessageLoop();
    spaWebServerLoop();
#ifdef LOCAL_CONNECT
    bridgeLoop();
#endif
  }
}