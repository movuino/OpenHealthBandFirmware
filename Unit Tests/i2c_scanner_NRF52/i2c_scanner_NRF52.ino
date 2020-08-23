#include <Wire.h>

/*
 * Modification had to be made for NRF52 I2C scanner  otherwise I2C is stuck
 * add  Wire.write("0");
 * 
 */

void setup()
  {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
//  pinMode(RED_LED,OUTPUT);
  }

void loop()
{
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
 // digitalWrite(RED_LED,LOW);
  for(address = 1; address < 127; address++ )
  {
   // Serial.print(address,HEX);
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    Wire.write("0");
    error = Wire.endTransmission(); //if it doesnot work try Wire.endTransmission(true); 
   // Serial.print(" ");
    //Serial.println(error);
    if (error == 0 || error == 3 )
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknow error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }   
   else 
      {
      // Serial.println("error");
      delay(1);  
      }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
  {
    //Serial.println("done\n");
}
//digitalWrite(RED_LED,HIGH);
delay(500);           // wait 5 seconds for next scan
}
