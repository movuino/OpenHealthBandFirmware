#include <bluefruit.h>
#include "MAX30105.h"

//#define debugPPG

BLEService timeStampService = BLEService(0x1165);

BLECharacteristic timeStampCharacteristic = BLECharacteristic(0x1166);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas  blebas;  // battery
BLEDfu  bledfu;  // OTA DFU service
BLEConnection* connection ;
uint32_t timeStampValue = 0;
long startTimer=0;

//ppg
MAX30105 particleSensor;
long samplesTaken = 0; //Counter for calculating the Hz or read rate
byte ledMode;

int counter=0;
int counterG;
uint8_t buf[16];
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
uint32_t irPPG=0;
uint32_t irPPG2=0;
uint32_t redPPG=0;
uint32_t redPPG2=0;
uint32_t greenPPG=0;
uint32_t greenPPG2=0;



void configurePPG(){
 
  /*ppg sensor setup*/
  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  //Setup parameters
  byte ledBrightness = 0x3F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  buflen=4+4*(ledMode);
  int sampleRate = 200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
}

void updatePPG(){
  /*Get data from ppg*/
  particleSensor.check(); //Check the sensor, read up to 3 samples
  //Serial.print(String(millis()-timer0)+" ");
  while (particleSensor.available()){
    samplesTaken++;
    counterG++;
    uint32_t timestamp=millis();
    buf[3] = (uint8_t)timestamp;
    buf[2] = (uint8_t)(timestamp>>=8);
    buf[1] = (uint8_t)(timestamp>>=8);
    buf[0] = (uint8_t)(timestamp>>=8);
    redPPG= particleSensor.getFIFORed();
    redPPG2=redPPG;
    buf[7] = (uint8_t)redPPG;
    buf[6] = (uint8_t)(redPPG>>=8);
    buf[5] = (uint8_t)(redPPG>>=8);
    buf[4] = (uint8_t)(redPPG>>=8);
    
    if(ledMode >1){
      irPPG= particleSensor.getFIFOIR();
      irPPG2=irPPG;
      buf[11] = (uint8_t)irPPG;
      buf[10] = (uint8_t)(irPPG>>=8);
      buf[9] = (uint8_t)(irPPG>>=8);
      buf[8] = (uint8_t)(irPPG>>=8);
     
      }
    if(ledMode >2){
      
      greenPPG= particleSensor.getFIFOGreen();
     greenPPG2=greenPPG;
      buf[15] = (uint8_t)greenPPG;
      buf[14] = (uint8_t)(greenPPG>>=8);
      buf[13] = (uint8_t)(greenPPG>>=8);
      buf[12] = (uint8_t)(greenPPG>>=8);
    
      } 
    #ifdef debugPPG
    //not working it has to be fixed !!
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
    dataReady=true;
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }
}
void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);
  /*not strating until ppg sensor is started */
  /*todo : error codes*/
  if (particleSensor.begin() == false){
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
    }
  configurePPG();
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
  setupTimeStampService();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();
}

void loop() {
  //timeStampValue = millis()-startTimer;
  //startTimer=millis();
  digitalToggle(LED_RED);
  updatePPG();
  if ( Bluefruit.connected() ) {
      if(dataReady)  {
        // Note: We use .notify instead of .write!
        // If it is connected but CCCD is not enabled
        // The characteristic's value is still updated although notification is not sent
        if ( timeStampCharacteristic.notify(buf,16) ){
            //Serial.print("timeStampCharacteristic updated to: ");
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
