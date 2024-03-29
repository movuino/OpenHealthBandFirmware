//////////////////////////////////////////////////////////////////////////////////////////
//
// Protocentral Arduino Library for  MLX90632 Non-contact temperature sensor
// If you have bought the breakout the connection with the Arduino board is as follows:
//
//|MLX pin label| Arduino Connection   |Pin Function      |
//|----------------- |:--------------------:|-----------------:|
//| SDA              |    A4               |  Serial Data      |
//| SCL              | A5                  |  Serial Clock     |
//| Vin              | 5V                  |  Power            |
//| GND              | Gnd                 |  Gnd              |
//
//
// Based on the original MLX90632 librry by Nathan Seidle, Sparkfun Electronics
/////////////////////////////////////////////////////////////////////////////////////////

#include "Protocentral_MLX90632.h"
#include <Wire.h>

Protocentral_MLX90632 ReadSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Protocentral MLX90632 Read Data");

  Wire.begin();

  ReadSensor.begin();
}

void loop()
{
 /* float object_Temp = ReadSensor.getObjectTempF(); //Get the temperature of the object
  Serial.print("Object temperature: ");
  Serial.print(object_Temp, 2);
  Serial.print(" F");*/
  float sensor_Temp = ReadSensor.getSensorTemp(); //Get the temperature of the sensor
  Serial.print(" Sensor temperature: ");
  Serial.print(sensor_Temp, 2);
  Serial.print(" C");
  Serial.println();
}
