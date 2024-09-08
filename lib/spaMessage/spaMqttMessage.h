#ifndef SPA_MQTT_MESSAGE_H
#define SPA_MQTT_MESSAGE_H


void publishSpaStatusData();
void publishSpaConfigurationData();
void publishSpaFilterSettingsData();
void publishSpaFaultLogData();
void publishSpaPreferencesData();
void publishSpaInformationData();
void publishWiFiModuleConfigurationData();
void publishSpaSettings0x04Data();

#endif