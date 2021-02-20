#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#include <Adafruit_LSM303_U.h>

#include "MySimpleBLE.h"
#include "TT34.h"

Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

MySimpleBLE ble;

const unsigned int advertisingDuration = 100;   // ms
const unsigned int gpioDeepSleepDuration = 2; // s

const unsigned int tt34Pin = 25;
const unsigned int timeout = 5000000; // us

TT34 tt34(tt34Pin, timeout);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello");

  sht31.begin(0x44);
  
  tsl.begin();
  tsl.setGain(TSL2591_GAIN_LOW);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);

  mag.enableAutoRange(true);
  mag.begin();
}

void loop()
{
  const float tt34Temperature = tt34.getTemperature(); // deg C
  const float shtTemperature = sht31.readTemperature(); // deg C
  const float shtHumidity = sht31.readHumidity(); // percent
  const unsigned int tslVisible = tsl.getLuminosity(TSL2591_VISIBLE); // a.u.
  const unsigned int tslInfrared = tsl.getLuminosity(TSL2591_INFRARED); // a.u.
  sensors_event_t event;
  mag.getEvent(&event);
  const float magX = event.magnetic.x; // uT
  const float magY = event.magnetic.y; // uT
  const float magZ = event.magnetic.z; // uT

  advertise("tt34", String(tt34Temperature));
  advertise("shtT", String(shtTemperature));
  advertise("shtH", String(shtHumidity));
  advertise("tslV", String(tslVisible));
  advertise("tslI", String(tslInfrared));
  advertise("magX", String(magX));
  advertise("magY", String(magY));
  advertise("magZ", String(magZ));

  Serial.println("enter deep sleep...");
  esp_deep_sleep(1000000LL * gpioDeepSleepDuration);
}

void advertise(String name, String value) {
  String msg = name + "_" + value;
  Serial.println(msg);
  ble.begin(msg);
  delay(advertisingDuration);
}
