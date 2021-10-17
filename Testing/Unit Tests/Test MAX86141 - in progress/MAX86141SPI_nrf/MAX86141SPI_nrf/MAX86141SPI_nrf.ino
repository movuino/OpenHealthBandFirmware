#include <SPI.h>
#include "MAX86141.h"

static int spiClk = 1000000; // 8 MHz Maximum

int fifo_size = 0;

bool RED_ON = false;
bool IR_ON = false;
bool AMBIENT = true;

float RED_AVG, IR_AVG, AMBIENT_AVG;

//
// Pin Definitions.
//*6
// #define MISO_PIN              19
// #define MOSI_PIN              18
// #define SCK_PIN               5
#define SS_PIN                12


MAX86141 pulseOx1;

void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);

  // Configure IO.
  pinMode(SS_PIN, OUTPUT);

  digitalWrite(SS_PIN, HIGH);

  //initialise SPI
  pulseOx1.spi = new SPIClass(SPI);
  // SPI.begin();

  pulseOx1.SS = SS_PIN;

  Serial.println("Init Device");
  pulseOx1.spi->begin();
  delay(100);

  pulseOx1.setDebug(true);
  pulseOx1.init(spiClk);

  Serial.println(pulseOx1.read_reg(0x0D));

  Serial.println("--Read Register-- PART_ID");
  Serial.println(pulseOx1.read_reg(0xFF));

  Serial.println("--Read Temp-- 0x40");
  pulseOx1.write_reg(0x40, 0xFF);

  Serial.println(pulseOx1.read_reg(0x41));
  Serial.println(pulseOx1.read_reg(0x41));
  delay(1000);

  pulseOx1.setDebug(false);
}


void loop() {
  fifo_size = pulseOx1.read_reg(REG_FIFO_DATA_COUNT);

  if(fifo_size >= 6){
    pulseOx1.device_data_read();
    int led1A = pulseOx1.led1A[0];
    int led2A = pulseOx1.led2A[0];
    int led1B = pulseOx1.led1B[0];
    int led2B = pulseOx1.led2B[0];
    int led3A = pulseOx1.led3A[0];
    int led3B = pulseOx1.led3B[0];

    Serial.print("LED 1A: ");
    Serial.print(led1A);
    Serial.print("\t");

    Serial.print("2A: ");
    Serial.print(led2A);
    Serial.print("\t");

    Serial.print("3A: ");
    Serial.print(led3A);
    Serial.print("\t");

    Serial.print("1B: ");
    Serial.print(led1B);
    Serial.print("\t");

    Serial.print("2B: ");
    Serial.print(led2B);
    Serial.print("\t");

    Serial.print("3B: ");
    Serial.print(led3B);
    Serial.println("");

    /*
    float AVG = 0.16666666*(led1A + led1B + led2A + led2B + pulseOx1.ambA[0] + pulseOx1.ambB[0]);
    //Serial.println(AVG);
    if(RED_ON) RED_AVG = AVG;
    if(IR_ON) IR_AVG = AVG;
    if(AMBIENT) AMBIENT_AVG = AVG;

    if((IR_AVG != 0) && (RED_AVG != 0) && (AMBIENT_AVG != 0)){
      RED_AVG = RED_AVG - AMBIENT_AVG;
      IR_AVG = IR_AVG - AMBIENT_AVG;
      Serial.print("Red: ");
      Serial.print(RED_AVG);
      Serial.print("\t IR:");
      Serial.print(IR_AVG);
      Serial.print("\t Ratio: ");
      Serial.println(RED_AVG/IR_AVG);
      RED_AVG = 0;
      IR_AVG = 0;
      AMBIENT_AVG = 0;
    }
    */

    // AMBIENT_AVG = (pulseOx1.led3A[0] + pulseOx1.led3B[0])*0.5;

    // RED_AVG = (led1A + led1B)*0.5;
    // Serial.print("Red: ");
    // Serial.print(RED_AVG);

    // IR_AVG = (led2A + led2B)*0.5;
    // Serial.print("\t");
    // Serial.print("IR: ");
    // Serial.print(IR_AVG);

    // Serial.print("\t");
    // Serial.print("Ambient: ");
    // Serial.println(AMBIENT_AVG);

    // float ratio = RED_AVG/IR_AVG;
    //Serial.print("\t");
    //Serial.print("Ratio: ");
    //Serial.println(ratio*4);

    delayMicroseconds(2500);
  }
}
