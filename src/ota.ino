#include "esp32_spa.h"

void otaSetup()
{

  ArduinoOTA.setHostname(gateway_name);
  ArduinoOTA.begin();
  ArduinoOTA.onStart(notifyOfUpdateStarted);
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                        { esp_task_wdt_reset(); });
  ArduinoOTA.onEnd(notifyOfUpdateEnded);
  ArduinoOTA.onError([](ota_error_t error)
                     {
    publishError(("OTA Failed: " + String(error)).c_str());
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed"); });
}

void notifyOfUpdateStarted()
{
  publishDebug("Arduino OTA Update Start");
  _yield();
}

void notifyOfUpdateEnded()
{
  publishDebug("Arduino OTA Update Complete");
  setLastRestartReason("OTA Update");
  _yield();
}
