#include <bluefruit.h>
#include "MAX30105.h"
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mySensor(0x69);
//float aX, aY, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
uint32_t  aX, aY, aZ;
float aX2,aY2,aZ2;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t   accScale = 16;
uint8_t   gyroScale = 2000;
//error codes
/*uint8_t IMUerror=0x00;
  uint8_t PPGerror=0x00;*/
bool errorIMU = false;
bool errorPPG = false;
uint8_t bufError[2];
float accelRange=16.0;
//todo : getter and setter for scales
/*Debug Modes*/
#define debugPPG
//#define debugAcc1;
//#define debugMag;


/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);
/*PPG Service & characteristic*/
BLEService PPGService = BLEService(0x1165);
BLECharacteristic rawPPGCharacteristic = BLECharacteristic(0x1166);
BLECharacteristic snrPPGCharacteristic = BLECharacteristic(0x1167);

/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);



BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas  blebas;  // battery
BLEDfu  bledfu;  // OTA DFU service
BLEConnection* connection ;
uint32_t timeStampValue = 0;
long startTimer = 0;

//ppg
MAX30105 particleSensor;
long samplesTaken = 0; //Counter for calculating the Hz or read rate
long startTime;

byte ledMode=3;

int counter = 0;
int counterG;
uint8_t buf[16];
uint8_t bufPPG[16];
//uint8_t bufIMU[16];
uint8_t bufAcc[7];
uint8_t bufGyro[7];
uint8_t bufMag[8];
uint8_t snr[4];
int bufSNR[21];
int cpt1 = 0;

int buflen = 16;
bool connected = false;
unsigned int timer0;
uint16_t bufData[8];
bool dataReady = false;

int samplingRateHz = 100 ;
int samplingRateMs = 1000 / 10 ;

int mtu = 0;
int phy = 0;
int interval = 0;
uint32_t irPPG = 0;
uint32_t irPPG2 = 0;
uint32_t redPPG = 0;
uint32_t redPPG2 = 0;
uint32_t greenPPG = 0;
uint32_t greenPPG2 = 0;


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
  //particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    //particleSensor.setup(ledBrightness, 3, ledMode, 100, pulseWidth, adcRange); //Configure sensor with these settings
    particleSensor.setup(ledBrightness, 7, ledMode, 1000, pulseWidth, adcRange); //Configure sensor with these settings

}
//axis=0, freedom degre=0
float signaltonoise(int* signalBuff, int len){
  // STEP 1, FIND THE MEAN.
   long sum = 0L ;  // sum will be larger than an item, long for safety.
  /*for (int i : signalBuff) {
    sum += i ;}*/
   for(int i=0; i<len; i++){
    sum += signalBuff[i] ;
   }
    float mean = sum/(float)len;

   // STEP 2, sum the squares of the differences from the mean
   float sqDevSum = 0.0;
  for(int i = 0; i < len; i++) {
    // pow(x, 2) is x squared.
    sqDevSum += pow((mean - float(signalBuff[i])), 2);
  }

   // STEP 3, FIND THE MEAN OF THAT
  // STEP 4, TAKE THE SQUARE ROOT OF THAT
   float stDev = sqrt(sqDevSum/float(len));
   
   return (float)20*log10(abs(mean/stDev));
}

void updatePPG() {
  /*Get data from ppg*/
  particleSensor.check(); //Check the sensor, read up to 3 samples
   Serial.println("Check samples: "+String(particleSensor.check()));
  //Serial.print(String(millis()-timer0)+" ");
  while (particleSensor.available()) {
    Serial.println("Available samples: "+String(particleSensor.available()));
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
    irPPG2 =irPPG;
    buf[11] = (uint8_t)irPPG2;
    buf[10] = (uint8_t)(irPPG2 >>= 8);
    buf[9] = (uint8_t)(irPPG2 >>= 8);
    buf[8] = (uint8_t)(irPPG2 >>= 8);
    }
   if(ledMode>=2){
    greenPPG2 = greenPPG;
    buf[15] = (uint8_t)greenPPG2;
    buf[14] = (uint8_t)(greenPPG2 >>= 8);
    buf[13] = (uint8_t)(greenPPG2>>= 8);
    buf[12] = (uint8_t)(greenPPG2 >>= 8);
///////////////////////////////////////////////
    bufSNR[cpt1]=greenPPG;
    cpt1 += 1;
    if (cpt1 == 21) {
      int newBufSNR[20];
      for(int i=0; i<20; i++){
        newBufSNR[i]=bufSNR[i+1];
      }
      int var = 0;
      var = 100 * (signaltonoise(newBufSNR, 20));
      if (var < 0) {
        int a = -100 * var;
        // Serial.println("SNR2 : " + String(a));
        //SNR1_2 = (uint8_t*)&a;
        snr[3] = (uint8_t)a;
        snr[2] = (uint8_t)(a >>= 8);
        snr[1] = (uint8_t)(a >>= 8);
        snr[0] = (uint8_t)(a >>= 8);
      }
      else {
        //Serial.println("SNR2 : " + String(var));
        //SNR1_2 = (uint8_t*)&var;
        snr[3] = (uint8_t)var;
        snr[2] = (uint8_t)(var >>= 8);
        snr[1] = (uint8_t)(var >>= 8);
        snr[0] = (uint8_t)(var >>= 8);
      }
      cpt1 = 0;
    }

///////////////////////////////////////////////
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
    Serial.print((float)samplesTaken / ((millis() - startTime) / 1000.0), 2);
    Serial.print("]");
    Serial.println("samplesTaken "+String(samplesTaken));
    Serial.println();
#endif
    dataReady = true;
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
  }
}
void configureIMU() {
  /*Read sensorId*/
  uint8_t sensorId;

  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  }
  else {
    Serial.println("Error cannot read sensor ID");
    errorIMU = true;
  }
  /*Begin IMU*/
  if (!errorIMU) {
    mySensor.beginAccel();
    mySensor.beginGyro();
    mySensor.beginMag();
  }
}
void updateAcc() {
  if (mySensor.accelUpdate() == 0) {
    //read sensor
    accelBuf = mySensor.accelBuff;
    //read timestamp
    uint32_t timestamp = millis();
    bufAcc[3] = (uint8_t)timestamp;
    bufAcc[2] = (uint8_t)(timestamp >>= 8);
    bufAcc[1] = (uint8_t)(timestamp >>= 8);
    bufAcc[0] = (uint8_t)(timestamp >>= 8);
    bufAcc[4] = 16;
    for (int i = 5; i <= 10; i++) {
      bufAcc[i] = accelBuf[i - 5];
      #ifdef debugAcc2
      Serial.print(String(i));
      Serial.print(":");
      Serial.print(String(bufAcc[i]));
      Serial.print(" ");
      #endif
    }
    #ifdef debugAcc1
    Serial.println(" ");
    
    int16_t v = ((int16_t) accelBuf[0]) << 8 | accelBuf[1];
    aX2=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
    v = ((int16_t) accelBuf[2]) << 8 | accelBuf[3];
    aY2=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
    v = ((int16_t) accelBuf[4]) << 8 | accelBuf[5];
    aZ2=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
    Serial.print(String(aX2));
    Serial.print(" ");   
    Serial.print(String(aY2));   
    Serial.print(" ");   
    Serial.print(String(aZ2));   
    Serial.println(" ");        
    #endif
    dataReady = true;
  } else {
    Serial.println("Cannot read accel values");
  }
}
void updateGyro() {
  if (mySensor.gyroUpdate() == 0) {
    gyroBuf = mySensor.gyroBuff;
    uint32_t timestamp = millis();
    bufGyro[3] = (uint8_t)timestamp;
    bufGyro[2] = (uint8_t)(timestamp >>= 8);
    bufGyro[1] = (uint8_t)(timestamp >>= 8);
    bufGyro[0] = (uint8_t)(timestamp >>= 8);
    bufGyro[4] = 2000;
    for (int i = 5; i <= 10; i++) {
      bufGyro[i] = gyroBuf[i - 5];
    }

  } else {
    Serial.println("Cannod read accel values");
  }
}
void updateMag() {
  if (mySensor.magUpdate() == 0) {
    magBuf = mySensor.magBuff;
    uint32_t timestamp = millis();
    bufMag[3] = (uint8_t)timestamp;
    bufMag[2] = (uint8_t)(timestamp >>= 8);
    bufMag[1] = (uint8_t)(timestamp >>= 8);
    bufMag[0] = (uint8_t)(timestamp >>= 8);
    for (int i = 4; i <= 9; i++) {
      bufMag[i] = magBuf[i-4];
      #ifdef debugMag
      Serial.println(String(bufMag[i]));
      #endif
    }

  } else {
    Serial.println("Cannod read accel values");
  }
}
void setup() {
  Serial.begin(115200);
  //delay(5000);
  /*init IMU*/
  configureIMU();
  /*Init PPG*/
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    errorPPG = true;
  }
  configurePPG();
  Serial.println(F("Custom TimeStamp Service and Characteristic"));
  Serial.println(F("-----------------------\n"));

  bufError[0] = errorIMU;
  bufError[1] = errorPPG;

  startTime = millis();

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.setTxPower(13);  //13 max - 0 min
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.setName("Movuino OHB - 000");
  //Bluefruit.setName("Palm");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms
  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.setSerialNum("ZZHH");
  bledis.begin();

  // Setup the Custom Service BLEService and BLECharacteristic
  Serial.println("Configuring the Custom Service");
  setupErrorService();
  setupPPGService();
  setupIMUService();

  // Setup the advertising packet(s)
  ErrorCharacteristic.write(bufError, 2);
  Serial.println("Setting up the advertising payload(s)");
  startAdv();
}

void loop() {
 // 
 // timeStampValue = millis()-startTimer;
  //startTimer=millis();
 // digitalToggle(LED_RED);
 if (!errorPPG) {
    updatePPG();
  }
  if (!errorIMU) {
    updateAcc();
    updateGyro();
    updateMag();
  }
  if ( Bluefruit.connected() ) {
    if (dataReady)  {
      // Note: We use .notify instead of .write!
      // If it is connected but CCCD is not enabled
      // The characteristic's value is still updated although notification is not sent
     if ( rawPPGCharacteristic.notify(buf, 16) ) {
        //Serial.print("rawPPGCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }

      if ( snrPPGCharacteristic.notify(snr, 4) ) {
        //Serial.print("rawPPGCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }

      
      if ( AccCharacteristic.notify(bufAcc, 11) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      if ( GyroCharacteristic.notify(bufGyro, 11) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      if ( MagCharacteristic.notify(bufMag, 11) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      dataReady = false;
    }
    else delay(1);

   if (counterG >= 1000) {
      printConnParams();
      counterG = 0;
    }

  }
}