#include "wifiModule.h"
#include <WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <ArduinoLog.h>
#include <esp_task_wdt.h>
#include <spaCommunication.h>
#include <TelnetStream.h>

#include <time.h>
#include "../restartReason/restartReason.h"

WiFiManager wifiManager;
char gatewayName[20];

void wifiModuleSetup()
{
  String s = WiFi.macAddress();
  sprintf(gatewayName, "spa-%.2s%.2s%.2s%.2s%.2s%.2s", s.c_str(),
          s.c_str() + 3, s.c_str() + 6, s.c_str() + 9, s.c_str() + 12,
          s.c_str() + 15);

  WiFi.setTxPower(WIFI_POWER_19_5dBm); // this sets wifi to highest power
  WiFi.setHostname(gatewayName);
  ArduinoOTA.setHostname(gatewayName);
  Log.notice(F("[WiFi]: Hostname: %s" CR), WiFi.getHostname());
  Log.notice(F("[WiFi]: OTA Hostname: %s" CR), ArduinoOTA.getHostname().c_str());
}

void wifiModuleLoop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    wifiConnect();
  }
  else
  {
    ArduinoOTA.handle();
#ifdef TELNET_LOG
    switch (TelnetStream.read())
    {
    case 'C':
      TelnetStream.println("bye bye");
      TelnetStream.flush();
      TelnetStream.stop();
      break;
    }
#endif
  }
}

/* Functions */

void wifiConnect()
{
  Log.notice(F("[WiFi]: Connecting to %s" CR), WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  unsigned long timeout = millis() + WIFI_CONNECT_TIMEOUT;

  while (WiFi.status() != WL_CONNECTED && millis() < timeout)
  {
    yield();
  }
  if (WiFi.status() != WL_CONNECTED)
  {
    Log.error(F("[WiFi]: Connect failed to %s" CR), WIFI_SSID);
  }
  else
  {
    configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org");
    Log.notice(F("[WiFi]: Connected, IP Address: %s" CR), WiFi.localIP().toString().c_str());
    Log.notice(F("[WiFi]: Time: %s" CR), getStringTime().c_str());
    otaSetup();
#ifdef TELNET_LOG
    Log.notice(F("[WiFi]: Switching to telnet %p" CR), WiFi.localIP());
    TelnetStream.begin();
    Log.begin(LOG_LEVEL, &TelnetStream);
#endif
  }
}

String getStringTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Log.error(F("[WiFi]: Obtaining Time failed" CR));
    return String("Failed to obtain time");
  }
  char timeCharArray[64];
  strftime(timeCharArray, sizeof(timeCharArray), "%Y-%m-%d %H:%M:%S", &timeinfo);

  return String(timeCharArray);
}

void otaSetup()
{

  ArduinoOTA.begin();
  ArduinoOTA.onStart(notifyOfUpdateStarted);
  ArduinoOTA.onEnd(notifyOfUpdateEnded);
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { Log.noticeln(F("[WiFi]: OTA Progress: %u%%\r"), (progress / (total / 100))); 
                          esp_task_wdt_reset(); });
  ArduinoOTA.onError([](ota_error_t error)
                     {
    if (error == OTA_AUTH_ERROR)
      Log.error(F("[WiFi]: OTA Auth Filed" CR));
    else if (error == OTA_BEGIN_ERROR)
      Log.error(F("[WiFi]: Begin Failed" CR));
    else if (error == OTA_CONNECT_ERROR)
      Log.error(F("[WiFi]: OTA Connect Failed" CR));
    else if (error == OTA_RECEIVE_ERROR)
      Log.error(F("[WiFi]: OTA Receive Failed" CR));
    else if (error == OTA_END_ERROR)
      Log.error(F("[WiFi]: OTA End Failed" CR)); });
  Log.notice(F("[WiFi]: Arduino OTA Enabled" CR));
}

void notifyOfUpdateStarted()
{
  Log.notice(F("[WiFi]: Arduino OTA Update Start" CR));
  spaCommunicationEnd();
}

void notifyOfUpdateEnded()
{
  Log.notice(F("[WiFi]: Arduino OTA Update Complete" CR));
  setLastRestartReason("OTA Update");
}