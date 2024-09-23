#include <OneWire.h>
#include <DallasTemperature.h>

#include <Arduino.h>
#include "../../src/config.h"

const int oneWireBus = TEMP_SENSOR_GPIO;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void tempProbeSetup(){
 sensors.begin();
};

void tempProbeLoop() {

};