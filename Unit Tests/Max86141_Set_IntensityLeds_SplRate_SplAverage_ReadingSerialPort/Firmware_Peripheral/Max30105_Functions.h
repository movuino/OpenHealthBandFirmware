
#include "MAX30105.h"

/*PPG Service & characteristic*/
BLEService PPGService = BLEService(0x1165);
BLECharacteristic rawPPGCharacteristic = BLECharacteristic(0x1166);

MAX30105 particleSensor;
long samplesTaken = 0; //Counter for calculating the Hz or read rate
byte ledMode = 3;

int counter = 0;
int counterG;
uint8_t buf[16];
//uint8_t bufIMU[16];
uint8_t bufPPG[16];



int buflen = 16;
bool connected = false;
unsigned int timer0;
uint16_t bufData[8];

int samplingRateHz = 100 ;
int samplingRateMs = 1000 / 10;

int mtu = 0;
int phy = 0;
int interval = 0;
uint32_t irPPG = 0;
uint32_t irPPG2 = 0;
uint32_t redPPG = 0;
uint32_t redPPG2 = 0;
uint32_t greenPPG = 0;
uint32_t greenPPG2 = 0;

bool errorPPG = false;

void configurePPG() {

  /*ppg sensor setup*/
  //particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  //Setup parameters
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  //ledMode = 3;
  ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  buflen = 4 + 4 * (ledMode);
  byte sampleAverage = 1;
  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

}

void updatePPG() {
  /*Get data from ppg*/
  particleSensor.check(); //Check the sensor, read up to 3 samples
  //Serial.print(String(millis()-timer0)+" ");
  while (particleSensor.available()) {
    samplesTaken++;
    counterG++;
    uint32_t timestamp = millis();
    uint32_t timestamp2 = timestamp;
    buf[3] = (uint8_t)timestamp;
    buf[2] = (uint8_t)(timestamp >>= 8);
    buf[1] = (uint8_t)(timestamp >>= 8);
    buf[0] = (uint8_t)(timestamp >>= 8);
    //those getters cannot be separated
    redPPG = particleSensor.getFIFORed();
    irPPG = particleSensor.getFIFOIR();
    greenPPG = particleSensor.getFIFOGreen();
    redPPG2 = redPPG;
    buf[7] = (uint8_t)redPPG2;
    buf[6] = (uint8_t)(redPPG2 >>= 8);
    buf[5] = (uint8_t)(redPPG2 >>= 8);
    buf[4] = (uint8_t)(redPPG2 >>= 8);
    if (ledMode >= 1) {
      irPPG2 = irPPG;
      buf[11] = (uint8_t)irPPG2;
      buf[10] = (uint8_t)(irPPG2 >>= 8);
      buf[9] = (uint8_t)(irPPG2 >>= 8);
      buf[8] = (uint8_t)(irPPG2 >>= 8);
    }
    if (ledMode >= 2) {
      greenPPG2 = greenPPG;
      buf[15] = (uint8_t)greenPPG2;
      buf[14] = (uint8_t)(greenPPG2 >>= 8);
      buf[13] = (uint8_t)(greenPPG2 >>= 8);
      buf[12] = (uint8_t)(greenPPG2 >>= 8);
    }
#ifdef debugPPG
    //not working it has to be fixed !!
    Serial.print(timestamp2);
    Serial.print(" R[");
    Serial.print(redPPG);
    Serial.print("] IR[");
    Serial.print(irPPG);
    Serial.print("]");
    Serial.print("G[");
    Serial.print(greenPPG);
    Serial.print("]");
    Serial.print("Hz[");
    Serial.print((float)samplesTaken / ((millis() - timer0) / 1000.0), 2);
    Serial.print("]");
    Serial.print(samplesTaken);
    Serial.println();
#endif
    dataReady = true;
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
  }
}
