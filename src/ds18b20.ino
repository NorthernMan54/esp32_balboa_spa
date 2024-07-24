#include <OneWire.h>
#include <DallasTemperature.h>
#include "esp32_spa.h"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(DS18B20_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int count = 0;

void ds18b20Setup(void)
{
  // Start up the library
  sensors.begin();
  if (sensors.getDS18Count())
  {
    mqtt.publish((mqttTopic + "debug/info").c_str(), (String(sensors.getDS18Count()) + " DS18B20 devices found").c_str(), true);
  }
  else
  {
    mqtt.publish((mqttTopic + "debug/error").c_str(), "No DS18B20 devices found");
  }
  mqtt.publish((mqttTopic + "node/ds18Count").c_str(), String(sensors.getDS18Count()).c_str());
}

void ds18b20loop(void)
{
  if (sensors.getDS18Count())
  {
    // call sensors.requestTemperatures() to issue a global temperature
    // request to all devices on the bus
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");

    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(sensors.getTempCByIndex(0));

    count++;
    sensors.setUserDataByIndex(0, count);
    int x = sensors.getUserDataByIndex(0);
    Serial.println(count);
  }
  else
  {
    mqtt.publish((mqttTopic + "debug/error").c_str(), "No DS18B20 devices found");
  }
}
