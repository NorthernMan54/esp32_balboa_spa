#include "balboa_helper.h"
#include <LittleFS.h>
#include <ArduinoJson.h>

void restartReasonSetup()
{
  DynamicJsonDocument jsonSettings(1024);
  // jsonSettings["WIFI_SSID"] = "";
  // jsonSettings["WIFI_PASSWORD"] = "";
  // jsonSettings["BROKER"] = "";
  // jsonSettings["BROKER_LOGIN"] = "";
  // jsonSettings["BROKER_PASS"] = "";
  String error_msg = "";
  if (LittleFS.begin())
  {

    if (SAVE_CONN == true)
    {
      File f = LittleFS.open("/ip.txt", "w");
      if (!f)
      {
        error_msg = "failed to create file";
      }

      jsonSettings["WIFI_SSID"] = WIFI_SSID;
      jsonSettings["WIFI_PASSWORD"] = WIFI_PASSWORD;
      jsonSettings["BROKER"] = BROKER;
      jsonSettings["BROKER_LOGIN"] = BROKER_LOGIN;
      jsonSettings["BROKER_PASS"] = BROKER_PASS;

      if (serializeJson(jsonSettings, f) == 0)
      {
        error_msg = "failed to write file";
      }

      f.close();
    }

    jsonSettings["WIFI_SSID"] = "";
    jsonSettings["WIFI_PASSWORD"] = "";
    jsonSettings["BROKER"] = "";
    jsonSettings["BROKER_LOGIN"] = "";
    jsonSettings["BROKER_PASS"] = "";

    if (ip_settings == 0)
    {
      ip_settings = 1;
      // read the settings from filesystem, if empty, put in AP mode

      File file = LittleFS.open("/ip.txt", "r");
      if (!file)
      {
        error_msg = "could not open file for reading";
        // STA LOGIC HERE
        wifi_settings = 1;
      }
      else
      {
        deserializeJson(jsonSettings, file);
        // Filesystem methods assuming it all went well

        // now I have them - NOTE: PROBABLY NEED TO CHECK THEM!!!!!
        ip_settings = 2;
        WIFI_SSID = jsonSettings["WIFI_SSID"].as<String>();
        WIFI_PASSWORD = jsonSettings["WIFI_PASSWORD"].as<String>();
        BROKER = jsonSettings["BROKER"].as<String>();
        BROKER_LOGIN = jsonSettings["BROKER_LOGIN"].as<String>();
        BROKER_PASS = jsonSettings["BROKER_PASS"].as<String>();
        error_msg = "Successfully read the configuration";
      }
      file.close();
    }
  }
  else
  {
    error_msg = "Could not mount fs";
  }

  LittleFS.end();
}

void getLastRestartReason()
{

  String espResetReason = "";
  esp_reset_reason_t reason = esp_reset_reason();

  //  Serial.print("Reset/Boot Reason was: ");
  //  Serial.println(reason);

  switch (reason)
  {
  case ESP_RST_UNKNOWN:
    espResetReason = "Reset reason can not be determined";
    break;

  case ESP_RST_POWERON:
    espResetReason = "Reset due to power-on event";
    break;

  case ESP_RST_EXT:
    espResetReason = "Reset by external pin (not applicable for ESP32)";
    break;

  case ESP_RST_SW:
    espResetReason = "Software reset via esp_restart";
    break;

  case ESP_RST_PANIC:
    espResetReason = "Software reset due to exception/panic";
    break;

  case ESP_RST_INT_WDT:
    espResetReason = "Reset (software or hardware) due to interrupt watchdog";
    break;

  case ESP_RST_TASK_WDT:
    espResetReason = "Reset due to task watchdog";
    break;

  case ESP_RST_WDT:
    espResetReason = "Reset due to other watchdogs";
    break;

  case ESP_RST_DEEPSLEEP:
    espResetReason = "Reset after exiting deep sleep mode";
    break;

  case ESP_RST_BROWNOUT:
    espResetReason = "Brownout reset (software or hardware)";
    break;

  case ESP_RST_SDIO:
    espResetReason = "Reset over SDIO";
    break;

  default:
    break;
  }

  mqtt.publish((mqttTopic + "debug/espResetReason").c_str(), (espResetReason).c_str());
  if (!LittleFS.begin())
  {
    publishError("LittleFS Error");
    return; // Stop if file system can't be initialized
  }

  File file =
      LittleFS.open("/restartReason.txt", "r"); // Open the file for reading
  if (!file)
  {
    publishError("Failed to open restartReason.txt file for reading");
  }
  else
  {
    String startReason = file.readStringUntil('\n'); // Read the restart reason
    mqtt.publish((mqttTopic + "debug/restartReason").c_str(), ("Last Restart Reason = " + startReason).c_str());
    file.close();
  }

  LittleFS.end(); // Close the file system
}

void setLastRestartReason(const String &reason)
{
  if (!LittleFS.begin())
  {
    publishError("LittleFS Error");
    return; // Stop if file system can't be initialized
  }

  File file =
      LittleFS.open("/restartReason.txt", "w"); // Open the file for writing
  if (!file)
  {
    publishError("Failed to open restartReason.txt file for writing");
  }
  else
  {
    file.print(reason); // Write the new reason
    file.close();
  }
  publishDebug((String("Restart due to - ") + reason).c_str());

  LittleFS.end(); // Close the file system
}