# hanging-station
The hanging station is a ballometer addon box that measures atmospheric data on a rope below the hot-air balloon.

The addon has the following physical sensors: SHT31 (temperature and humidity), TSL2591 (solar irradiation), LSM303DLHC (magnetic compass and accelerometer). Futhermore it has a 433 MHz receiver to record signals from FlyTec TT34 envelope temperature sensors. The sensors are read out by an ESP32 DEVKIT V1 microcontroller and the measurement results are broadcasted via bluetooth low energy. The main ballometer box in the hot-air balloon receives the bluetooth signals and forwards them to https://ballometer.io.

## motivation

While the main ballometer box already reads temperature and solar irradiation it has certain drawbacks. First the main box is attached to the basket. This means it does not always have clear view of the sun and can therefore not always measure solar irradiation. The hanging station on the otherhand is on a rope below the basket and should get plenty of unobstructed sun access. Another problem in the basket is the temperature measurement. The proximity to the burner and the mouth of the balloon means an increased local temperature. This is nice for the people in the basked when it is cold outside, but it gives an inaccurate reading of the ambient temperature. With a sensor further down under the basket of the balloon this problem can be mitigated. 

Finally it turns out that the 433 MHz receiver is susceptible to power fluctuations. In the main ballometer box these are omnipresent because the Raspberry Pi 3B+ produces spikes in the load due to switching hardware components like the wifi module. On the ESP32 on the otherhand the power consumption is much lower and in turn the supply voltage more stable.

## installation

Install the arduino IDE. In the IDE do

```
Tools -> Board -> Board Manager -> search esp32 -> install
```

Choose board ```DOIT ESP32...```

Get the adafruit libraries for the sensors:

```
Sketch -> Include Library -> Manage Libraries...
-> adafruit sht31 -> install
-> adafruit tsl2591 -> install
-> adafruit lsm303dlhc -> install
```
