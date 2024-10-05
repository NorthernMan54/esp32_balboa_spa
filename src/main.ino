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
#include <spaUtilities.h>
#include <mqttModule.h>
#include <rs485.h>
#include <bridge.h>
#include <spaEpaper.h>
#include <spaLvgl.h>

#include "main.h"

String buildDefinitionString = "";
#define addBuildDefinition(name) buildDefinitionString += #name " ";

void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
  delay(5000);
#endif
  // Launch serial for debugging purposes
  Serial.begin(SERIAL_BAUD);
  Log.setPrefix(logPrintPrefix);
  Log.begin(LOG_LEVEL, &Serial);
  esp_task_wdt_init(INITIAL_WDT_TIMEOUT, true); // enable panic so ESP32 restarts
  esp_task_wdt_add(NULL);                       // add current thread to WDT watch
  logSection("WELCOME TO esp32_balboa_spa");
#ifdef spaEpaper
  logSection("EPaper Setup");
  spaEpaperSetup();
#endif
#ifdef SPALVGL
  logSection("SPA LVGL Setup");
  spaLvglSetup();
#endif
  logSection("Build Definitions");
  Log.notice(F("Version: %s" CR), VERSION);
  Log.notice(F("Build: %s" CR), BUILD);

#ifdef ESP32S3
  Log.notice(F("Build for ESP32S3" CR));
#else
  Log.notice(F("Build for ESP32" CR));
#endif

#ifdef ARDUINO_ESP32S3_DEV
  Log.notice(F("Build for ARDUINO_ESP32S3_DEV" CR));
#endif

#ifdef LOCAL_CONNECT
  addBuildDefinition("LOCAL_CONNECT");
#endif

#ifdef LOCAL_CLIENT
  addBuildDefinition("LOCAL_CLIENT");
#endif

#ifdef REMOTE_CLIENT
  addBuildDefinition("REMOTE_CLIENT");
#endif

#ifdef TELNET_LOG
  addBuildDefinition("TELNET_LOG");
#endif

#ifdef BRIDGE
  addBuildDefinition("BRIDGE");
#endif

#ifdef spaEpaper
  addBuildDefinition("spaEpaper");
#endif

#ifdef SPALVGL
  addBuildDefinition("SPALVGL");
#endif

  Log.notice(F("Build Definitions: %s" CR), buildDefinitionString.c_str());

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
  logSection("Web Server Setup");
  spaWebServerSetup();
  logSection("Spa Message Setup");
  spaMessageSetup();

#if defined(LOCAL_CONNECT) || defined(BRIDGE)
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
#ifdef spaEpaper
  spaEpaperLoop();
#endif
#ifdef SPALVGL
  spaLvglLoop();
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
#if defined(LOCAL_CONNECT) || defined(BRIDGE)
    bridgeLoop();
#endif
  }
}