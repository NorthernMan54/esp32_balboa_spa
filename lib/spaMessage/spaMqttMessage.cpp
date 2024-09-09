#include "spaMqttMessage.h"
#include "spaMessage.h"
#include "balboa.h"
#include <Arduino.h>
#include <ArduinoLog.h>
#include <mqttModule.h>

#define BUFFER_SIZE BALBOA_MESSAGE_SIZE * 3 + 1
// Macro to simplify publishing data
#define PUBLISH_STATUS_ELEMENT(element, format)                      \
  do                                                                 \
  {                                                                  \
    char buffer[BUFFER_SIZE];                                                 \
    snprintf(buffer, sizeof(buffer), format, spaStatusData.element); \
    publishElement(#element, "status", buffer);                      \
  } while (0)

#define PUBLISH_STATUS_MAP(element, suppliedMap, format)                  \
  do                                                                      \
  {                                                                       \
    std::map<uint8_t, const char *> map = suppliedMap;                    \
    char buffer[BUFFER_SIZE];                                                      \
    snprintf(buffer, sizeof(buffer), format, map[spaStatusData.element]); \
    publishElement(#element, "status", buffer);                           \
  } while (0)

void publishElement(const char *element, const char *group, const char *value)
{
  char topic[128];
  snprintf(topic, sizeof(topic), "%s%s/%s", mqttTopic.c_str(), group, element);
  // Log.verbose("Publishing %s: %s\n", topic, value);
  mqtt.publish(topic, value);
}

#define convertRawDataToHex(structure, outputBuffer)                             \
  do                                                                             \
  {                                                                              \
    char *ptr = outputBuffer;                                                    \
    for (int i = 0; i < structure.rawDataLength && i < BALBOA_MESSAGE_SIZE; i++) \
    {                                                                            \
      ptr += sprintf(ptr, "%02X ", structure.rawData[i]);                        \
    }                                                                            \
    *(ptr - 1) = '\0';                                                           \
  } while (0)

void publishSpaStatusData()
{
  PUBLISH_STATUS_ELEMENT(crc, "%u");
  PUBLISH_STATUS_ELEMENT(lastUpdate, "%lu");
  PUBLISH_STATUS_ELEMENT(magicNumber, "%u");
  PUBLISH_STATUS_ELEMENT(rawDataLength, "%u");
  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaStatusData, rawDataHex);
  publishElement("rawData", "status", rawDataHex);

  // PUBLISH_STATUS_ELEMENT(spaState, "%u");
  PUBLISH_STATUS_MAP(spaState, SPA_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(initMode, SPA_MODE_MAP, "%s");
  // PUBLISH_STATUS_ELEMENT(initMode, "%u");
  PUBLISH_STATUS_ELEMENT(currentTemp, "%.2f");
  PUBLISH_STATUS_ELEMENT(time, "%s");
  PUBLISH_STATUS_ELEMENT(heatingMode, "%u");

  PUBLISH_STATUS_ELEMENT(reminderType, "%u");
  PUBLISH_STATUS_ELEMENT(sensorA, "%u");
  PUBLISH_STATUS_ELEMENT(sensorB, "%u");
  PUBLISH_STATUS_ELEMENT(tempScale, "%u");
  PUBLISH_STATUS_ELEMENT(clockMode, "%u");
  PUBLISH_STATUS_MAP(filterMode, FILTER_MODE_MAP, "%s");
  PUBLISH_STATUS_MAP(panelLocked, LOCKED_MAP, "%s");
  PUBLISH_STATUS_MAP(tempRange, TEMP_RANGE_MAP, "%s");
  PUBLISH_STATUS_ELEMENT(needsHeat, "%u");
  PUBLISH_STATUS_ELEMENT(heatingState, "%u");
  PUBLISH_STATUS_MAP(pump1, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(pump2, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(pump3, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(pump4, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(pump5, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(pump6, PUMP_STATE_MAP, "%s");
  PUBLISH_STATUS_MAP(light1, ON_OFF_MAP, "%s");
  PUBLISH_STATUS_MAP(blower, ON_OFF_MAP, "%s");
  PUBLISH_STATUS_MAP(light1, ON_OFF_MAP, "%s");
  PUBLISH_STATUS_MAP(light2, ON_OFF_MAP, "%s");
  PUBLISH_STATUS_MAP(mister, ON_OFF_MAP, "%s");

  PUBLISH_STATUS_ELEMENT(mister, "%u");
  PUBLISH_STATUS_ELEMENT(setTemp, "%.2f");
  PUBLISH_STATUS_ELEMENT(lowSetTemp, "%.2f");
  PUBLISH_STATUS_ELEMENT(highSetTemp, "%.2f");
  PUBLISH_STATUS_ELEMENT(notification, "%u");
  PUBLISH_STATUS_ELEMENT(flags19, "%u");
  PUBLISH_STATUS_ELEMENT(settingsLock, "%u");
  PUBLISH_STATUS_MAP(settingsLock, LOCKED_MAP, "%s");
  PUBLISH_STATUS_ELEMENT(m8CycleTime, "%u");

  PUBLISH_STATUS_ELEMENT(heaterOnTimeToday, "%u");
  PUBLISH_STATUS_ELEMENT(heaterOnTimeYesterday, "%u");
  PUBLISH_STATUS_ELEMENT(filterOnTimeToday, "%u");
  PUBLISH_STATUS_ELEMENT(filterOnTimeYesterday, "%u");
}

#define PUBLISH_CONFIG_ELEMENT(element, format)                             \
  do                                                                        \
  {                                                                         \
    char buffer[BUFFER_SIZE];                                                        \
    snprintf(buffer, sizeof(buffer), format, spaConfigurationData.element); \
    publishElement(#element, "config", buffer);                             \
  } while (0)

void publishSpaConfigurationData()
{
  PUBLISH_CONFIG_ELEMENT(crc, "%u");
  PUBLISH_CONFIG_ELEMENT(lastUpdate, "%lu");
  PUBLISH_CONFIG_ELEMENT(lastRequest, "%lu");
  PUBLISH_CONFIG_ELEMENT(magicNumber, "%u");
  PUBLISH_CONFIG_ELEMENT(rawDataLength, "%u");

  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaConfigurationData, rawDataHex);
  publishElement("rawData", "config", rawDataHex);

  PUBLISH_CONFIG_ELEMENT(pump1, "%u");
  PUBLISH_CONFIG_ELEMENT(pump2, "%u");
  PUBLISH_CONFIG_ELEMENT(pump3, "%u");
  PUBLISH_CONFIG_ELEMENT(pump4, "%u");
  PUBLISH_CONFIG_ELEMENT(pump5, "%u");
  PUBLISH_CONFIG_ELEMENT(pump6, "%u");
  PUBLISH_CONFIG_ELEMENT(light1, "%u");
  PUBLISH_CONFIG_ELEMENT(light2, "%u");
  PUBLISH_CONFIG_ELEMENT(circulationPump, "%u");
  PUBLISH_CONFIG_ELEMENT(blower, "%u");
  PUBLISH_CONFIG_ELEMENT(mister, "%u");
  PUBLISH_CONFIG_ELEMENT(aux1, "%u");
  PUBLISH_CONFIG_ELEMENT(aux2, "%u");
  PUBLISH_CONFIG_ELEMENT(temp_scale, "%u");
}

#define PUBLISH_FILTER_SETTINGS_ELEMENT(element, format)                     \
  do                                                                         \
  {                                                                          \
    char buffer[BUFFER_SIZE];                                                         \
    snprintf(buffer, sizeof(buffer), format, spaFilterSettingsData.element); \
    publishElement(#element, "filterSettings", buffer);                      \
  } while (0)

void publishSpaFilterSettingsData()
{
  PUBLISH_FILTER_SETTINGS_ELEMENT(crc, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(lastUpdate, "%lu");
  PUBLISH_FILTER_SETTINGS_ELEMENT(lastRequest, "%lu");
  PUBLISH_FILTER_SETTINGS_ELEMENT(magicNumber, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(rawDataLength, "%u");

  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaFilterSettingsData, rawDataHex);
  publishElement("rawData", "filterSettings", rawDataHex);

  PUBLISH_FILTER_SETTINGS_ELEMENT(filt1Hour, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt1Minute, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt1DurationHour, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt1DurationMinute, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt2Enable, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt2Hour, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt2Minute, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt2DurationHour, "%u");
  PUBLISH_FILTER_SETTINGS_ELEMENT(filt2DurationMinute, "%u");
}

#define PUBLISH_FAULT_LOG_ELEMENT(element, format)                     \
  do                                                                   \
  {                                                                    \
    char buffer[64];                                                   \
    snprintf(buffer, sizeof(buffer), format, spaFaultLogData.element); \
    publishElement(#element, "faultLog", buffer);                      \
  } while (0)

void publishSpaFaultLogData()
{
  PUBLISH_FAULT_LOG_ELEMENT(crc, "%u");
  PUBLISH_FAULT_LOG_ELEMENT(lastUpdate, "%lu");
  PUBLISH_FAULT_LOG_ELEMENT(lastRequest, "%lu");
  PUBLISH_FAULT_LOG_ELEMENT(magicNumber, "%u");

  PUBLISH_FAULT_LOG_ELEMENT(rawDataLength, "%u");
  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaFaultLogData, rawDataHex);
  publishElement("rawData", "faultLog", rawDataHex);

  PUBLISH_FAULT_LOG_ELEMENT(totEntry, "%u");
  PUBLISH_FAULT_LOG_ELEMENT(currEntry, "%u");
  PUBLISH_FAULT_LOG_ELEMENT(faultCode, "%u");

  // Handle publishing of String type
  publishElement("faultMessage", "faultLog", spaFaultLogData.faultMessage.c_str());

  PUBLISH_FAULT_LOG_ELEMENT(daysAgo, "%u");
  PUBLISH_FAULT_LOG_ELEMENT(hour, "%u");
  PUBLISH_FAULT_LOG_ELEMENT(minutes, "%u");
}

#define PUBLISH_PREFERENCES_ELEMENT(element, format)                      \
  do                                                                      \
  {                                                                       \
    char buffer[BUFFER_SIZE];                                                      \
    snprintf(buffer, sizeof(buffer), format, spaPreferencesData.element); \
    publishElement(#element, "preferences", buffer);                      \
  } while (0)

void publishSpaPreferencesData()
{
  PUBLISH_PREFERENCES_ELEMENT(crc, "%u");
  PUBLISH_PREFERENCES_ELEMENT(lastUpdate, "%lu");
  PUBLISH_PREFERENCES_ELEMENT(lastRequest, "%lu");
  PUBLISH_PREFERENCES_ELEMENT(magicNumber, "%u");

  PUBLISH_PREFERENCES_ELEMENT(rawDataLength, "%u");
  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaPreferencesData, rawDataHex);
  publishElement("rawData", "preferences", rawDataHex);

  PUBLISH_PREFERENCES_ELEMENT(reminders, "%u");
  PUBLISH_PREFERENCES_ELEMENT(tempScale, "%u");
  PUBLISH_PREFERENCES_ELEMENT(clockMode, "%u");
  PUBLISH_PREFERENCES_ELEMENT(cleanupCycle, "%u");
  PUBLISH_PREFERENCES_ELEMENT(dolphinAddress, "%u");
  PUBLISH_PREFERENCES_ELEMENT(m8AI, "%u");
}

#define PUBLISH_INFORMATION_ELEMENT(element, format)                      \
  do                                                                      \
  {                                                                       \
    char buffer[BUFFER_SIZE];                                                      \
    snprintf(buffer, sizeof(buffer), format, spaInformationData.element); \
    publishElement(#element, "information", buffer);                      \
  } while (0)

void publishSpaInformationData()
{
  PUBLISH_INFORMATION_ELEMENT(crc, "%u");
  PUBLISH_INFORMATION_ELEMENT(lastUpdate, "%lu");
  PUBLISH_INFORMATION_ELEMENT(lastRequest, "%lu");
  PUBLISH_INFORMATION_ELEMENT(magicNumber, "%u");

  PUBLISH_INFORMATION_ELEMENT(rawDataLength, "%u");
  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaInformationData, rawDataHex);
  publishElement("rawData", "information", rawDataHex);

  // Handle publishing of char arrays
  publishElement("softwareID", "information", spaInformationData.softwareID);
  publishElement("model", "information", spaInformationData.model);
  PUBLISH_INFORMATION_ELEMENT(setupNumber, "%u");
  PUBLISH_INFORMATION_ELEMENT(voltage, "%u");
  PUBLISH_INFORMATION_ELEMENT(heaterType, "%u");
  publishElement("dipSwitch", "information", spaInformationData.dipSwitch);
}

#define PUBLISH_SETTINGS_0x04_ELEMENT(element, format)                     \
  do                                                                       \
  {                                                                        \
    char buffer[BUFFER_SIZE];                                                       \
    snprintf(buffer, sizeof(buffer), format, spaSettings0x04Data.element); \
    publishElement(#element, "settings0x04", buffer);                      \
  } while (0)

void publishSpaSettings0x04Data()
{
  PUBLISH_SETTINGS_0x04_ELEMENT(crc, "%u");
  PUBLISH_SETTINGS_0x04_ELEMENT(lastUpdate, "%lu");
  PUBLISH_SETTINGS_0x04_ELEMENT(lastRequest, "%lu");
  PUBLISH_SETTINGS_0x04_ELEMENT(magicNumber, "%u");

  PUBLISH_SETTINGS_0x04_ELEMENT(rawDataLength, "%u");
  // Convert rawData to a space-separated hexadecimal string
  char rawDataHex[3 * BALBOA_MESSAGE_SIZE + 1]; // 2 characters per byte + space + null terminator
  convertRawDataToHex(spaSettings0x04Data, rawDataHex);
  publishElement("rawData", "settings0x04", rawDataHex);

}

#define PUBLISH_WIFI_MODULE_CONFIG_ELEMENT(element, format)                        \
  do                                                                               \
  {                                                                                \
    char buffer[BUFFER_SIZE];                                                               \
    snprintf(buffer, sizeof(buffer), format, wiFiModuleConfigurationData.element); \
    publishElement(#element, "wifiModuleConfig", buffer);                          \
  } while (0)

void publishWiFiModuleConfigurationData()
{
  PUBLISH_WIFI_MODULE_CONFIG_ELEMENT(crc, "%u");
  PUBLISH_WIFI_MODULE_CONFIG_ELEMENT(lastUpdate, "%lu");
  PUBLISH_WIFI_MODULE_CONFIG_ELEMENT(lastRequest, "%lu");
  PUBLISH_WIFI_MODULE_CONFIG_ELEMENT(magicNumber, "%u");

  // Handle publishing of char arrays
  publishElement("macAddress", "wifiModuleConfig", wiFiModuleConfigurationData.macAddress);
}
