#include <bluefruit.h>
#include "MAX30105.h"
#include <MPU9250_asukiaaa.h>


MPU9250_asukiaaa mySensor(0x69);
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float accelRange=16.0;
uint8_t* accelBuf;
uint8_t* gyroBuf;
uint8_t* magBuf;
//#define debugPPG

BLEService IMUService = BLEService(0x1101);

BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);

BLEService timeStampService = BLEService(0x1165);
BLECharacteristic timeStampCharacteristic = BLECharacteristic(0x1166);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas  blebas;  // battery
BLEDfu  bledfu;  // OTA DFU service
BLEConnection* connection ;
uint32_t timeStampValue = 0;
long startTimer=0;


long samplesTaken = 0; //Counter for calculating the Hz or read rate
byte ledMode;

int counter=0;
int counterG;
uint8_t buf[16];
uint8_t bufAcc[6];
uint8_t bufGyro[6];
uint8_t bufMag[7];

int buflen=16;
bool connected=false;
unsigned int timer0;
uint16_t bufData[8];
bool dataReady=false;

int samplingRateHz = 100 ;
int samplingRateMs = 1000/10 ;

int mtu=0;
int phy=0;
int interval=0;

void configureIMU(){
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void updateAcc(){
if (mySensor.accelUpdate() == 0) {
    accelBuf=mySensor.accelBuff;
    for(int i=0;i<=6;i++){
      bufAcc[i] = accelBuf[i];
     //Serial.println("Cannod read accel values");
    }
    dataReady=true; 
  } else {
    Serial.println("Cannod read accel values");
  }
}
void updateGyro(){
if (mySensor.gyroUpdate() == 0) {
     gyroBuf=mySensor.gyroBuff;
    for(int i=0;i<=6;i++){
      bufGyro[i] = gyroBuf[i];
    }
    
  } else {
    Serial.println("Cannod read accel values");
  }
}
void updateMag(){
if (mySensor.magUpdate() == 0) {
     magBuf=mySensor.magBuff;
    for(int i=0;i<=7;i++){
      bufMag[i] = magBuf[i];
    }
    
  } else {
    Serial.println("Cannod read accel values");
  }
}
void updatePPG(){
  uint32_t timestamp=millis();
    buf[3] = (uint8_t)timestamp;
    buf[2] = (uint8_t)(timestamp>>=8);
    buf[1] = (uint8_t)(timestamp>>=8);
    buf[0] = (uint8_t)(timestamp>>=8);
}
void setup() {
  Serial.begin(115200);

  configureIMU();

  Serial.println(F("Custom TimeStamp Service and Characteristic"));
  Serial.println(F("-----------------------\n"));

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);  
  Bluefruit.setTxPower(13);  //13 max - 0 min
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.setName("Open Health Band");

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
  setupIMUService();
  setupTimeStampService();
  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();
}

void loop() {
  //timeStampValue = millis()-startTimer;
  //startTimer=millis();
  digitalToggle(LED_BLUE);
  updateAcc();
  updateGyro();
  updateMag();
  updatePPG();
  if ( Bluefruit.connected() ) {
      if(dataReady)  {
        // Note: We use .notify instead of .write!
        // If it is connected but CCCD is not enabled
        // The characteristic's value is still updated although notification is not sent
        if ( timeStampCharacteristic.notify(bufAcc,16) ){
            //Serial.print("IMUCharacteristic updated to: ");
            //Serial.println(timeStampValue);             
            }else{
            Serial.println("ERROR: Notify not set in the CCCD or not connected!");
            }
        if ( AccCharacteristic.notify(bufAcc,6) ){
            //Serial.print("IMUCharacteristic updated to: ");
            //Serial.println(timeStampValue);             
            }else{
            Serial.println("ERROR: Notify not set in the CCCD or not connected!");
            }
        if ( GyroCharacteristic.notify(bufGyro,6) ){
            //Serial.print("IMUCharacteristic updated to: ");
            //Serial.println(timeStampValue);             
            }else{
            Serial.println("ERROR: Notify not set in the CCCD or not connected!");
            }
         if ( MagCharacteristic.notify(bufMag,7) ){
            //Serial.print("IMUCharacteristic updated to: ");
            //Serial.println(timeStampValue);             
            }else{
            Serial.println("ERROR: Notify not set in the CCCD or not connected!");
            }    
        dataReady=false; 
        }
       else delay(1);
      if(counterG >=1000) {
        printConnParams();
        counterG=0;
       }
     
  }
}
