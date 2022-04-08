#include <MAX86141.h>
//#include <SPI.h>
//
#define SPICLK 1000000 // 8 MHz Maximum
#define SS_PIN 10


#include <SPI.h>

void setup() {
  pinMode(SS_PIN, OUTPUT); // set the SS pin as an output
  SPI.begin();         // initialize the SPI library
  Serial.begin(115200);
}

uint8_t ret;

void loop() {

  
    SPI.beginTransaction(SPISettings(SPICLK, MSBFIRST, SPI_MODE0));
    digitalWrite(SS_PIN, LOW); 
    
    SPI.transfer(0xFF); // ID register
    SPI.transfer(0xFF); // read operation
    ret = SPI.transfer(0x00); // store value
    Serial.println(ret);
    
    SPI.endTransaction();
    digitalWrite(SS_PIN, HIGH);
    delay(10);

}
