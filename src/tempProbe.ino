#include "tempProbe.h"
#include <Arduino.h>
#include <ArduinoLog.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <TickTwo.h>

#include "config.h"

const int oneWireBus = TEMP_SENSOR_GPIO;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

TickTwo tempProbeTick(readTemperature, .2 * 60 * 1000); // 1 minutes
void tempProbeSetup(){
 sensors.begin();
 tempProbeTick.start();
};

void tempProbeLoop() {
  tempProbeTick.update();
  /*
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  delay(5000);
  */
};

void readTemperature(){
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Log.notice("[TEMP]: Temperature: %F ºC" CR, temperatureC);
}