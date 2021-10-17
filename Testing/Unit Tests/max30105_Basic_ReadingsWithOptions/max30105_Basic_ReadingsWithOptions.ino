/*
  MAX30105 Breakout: Output all the raw Red/IR/Green readings
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  Outputs all Red/IR/Green values.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

#include <Wire.h>
#include "MAX30105.h"

MAX30105 particleSensor;

#define debug Serial //Uncomment this line if you're using an Uno or ESP
int ledMode;
//#define debug SerialUSB //Uncomment this line if you're using a SAMD21
void configurePPG(){
 
  /*ppg sensor setup*/
 // particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  //Setup parameters
  byte ledBrightness = 0x3f; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  //buflen=4+4*(ledMode);
  int sampleRate = 50; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
}
void setup()
{
  debug.begin(115200);
  debug.println("MAX30105 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    debug.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  delay(100);
  configurePPG(); 
  delay(100);
}

void loop()
{
  debug.print(" R[");
  debug.print(particleSensor.getRed());
  debug.print("] IR[");
  debug.print(particleSensor.getIR());
  debug.print("] G[");
  debug.print(particleSensor.getGreen());
  debug.print("]");

  debug.println();
}
