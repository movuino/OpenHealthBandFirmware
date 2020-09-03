/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <Wire.h>
#include "MAX30105.h"


#include <bluefruit.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>

//700-122179

// BLE Service
BLEDfu  bledfu;  // OTA DFU service
BLEDis  bledis;  // device information
BLEUart bleuart; // uart over ble
BLEBas  blebas;  // battery
int counter=0;
int counterG;
uint8_t buf[16];
bool connected=false;
unsigned int timer0;
uint16_t bufData[8];
bool dataReady=false;

int samplingRateHz = 100 ;
int samplingRateMs = 1000/10 ;


//ppg
MAX30105 particleSensor;
long samplesTaken = 0; //Counter for calculating the Hz or read rate
/*
timestamp uint32 
r g b uint32
*/


/*
b[3] = (uint8_t)test32;
b[2] = (uint8_t)(test32>>=8);
b[1] = (uint8_t)(test32>>=8);
b[0] = (uint8_t)(test32>>=8);
u = b[0];
u = (u  << 8) + b[1];
u = (u  << 8) + b[2];
u = (u  << 8) + b[3];*/



void setup()
{
  Serial.begin(115200);
  //init sensor
  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  /*ppg sensor setup*/
  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  //Setup parameters
  byte ledBrightness = 0x3F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 3; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 400; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 411; //Options: 69, 118, 215, 411
  int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
  particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
  
#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) yield();
#endif
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");

  
  // Setup the BLE LED to be enabled on CONNECT
  // Note: This is actually the default behaviour, but provided
  // here in case you want to control this LED manually via PIN 19
  Bluefruit.autoConnLed(true);

  // Config the peripheral connection with maximum bandwidth 
  // more SRAM required by SoftDevice
  // Note: All config***() function must be called before begin()
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52");
  //Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // To be consistent OTA DFU should be added first if it exists
  bledfu.begin();

  // Configure and Start Device Information Service
  bledis.setManufacturer("Adafruit Industries");
  bledis.setModel("Bluefruit Feather52");
  bledis.begin();

  // Configure and Start BLE Uart Service
  bleuart.begin();

  // Start BLE Battery Service
  blebas.begin();
  blebas.write(100);

  // Set up and start advertising
  startAdv();

  Serial.println("Please use Adafruit's Bluefruit LE app to connect in UART mode");
  Serial.println("Once connected, enter character(s) that you wish to send");
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include bleuart 128-bit uuid
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();
  
  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   * 
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
}

void loop()
{

  
  // Forward data from HW Serial to BLEUART
  /*while (Serial.available())
  {
    // Delay to wait for enough input, since we have a limited transmission buffer
    delay(2);
    uint8_t buf[64];
    //int count = Serial.readBytes(buf, sizeof(buf));
    bleuart.write( buf, count );
  }  */
 if(connected){
    //data gathering here
  //if(millis()-timer0>=samplingRateMs){
  //    Serial.print("timestamp ");
      /*fill buf 0-3 timestamp*/
  /*    uint32_t timestamp=millis();
      buf[3] = (uint8_t)timestamp;
      buf[2] = (uint8_t)(timestamp>>=8);
      buf[1] = (uint8_t)(timestamp>>=8);
      buf[0] = (uint8_t)(timestamp>>=8);
      //for(int x=0;x<4;x++) Serial.print(String(buf[x])+" ");
      /*uint32_t timestamp2= 0;
      timestamp2 = buf[0];
      timestamp2 = (timestamp2  << 8) + buf[1];
      timestamp2 = (timestamp2 << 8) + buf[2];
      timestamp2 = (timestamp2 << 8) + buf[3];  
      Serial.print("timestamp: "+String(timestamp2));*/
   /*   Serial.print(" - red: ");
      /*fill buf 4-7 ppgred 126666*/
   /*   uint32_t redPPG= particleSensor.getRed();
      buf[7] = (uint8_t)redPPG;
      buf[6] = (uint8_t)(redPPG>>=8);
      buf[5] = (uint8_t)(redPPG>>=8);
      buf[4] = (uint8_t)(redPPG>>=8);
      // for(int x=4;x<8;x++) Serial.print(String(buf[x])+" ");
      Serial.print(" - IR: ");
      /*fill buf 4-7 ppgred 126666*/
   /*   uint32_t irPPG= particleSensor.getIR();
      buf[11] = (uint8_t)irPPG;
      buf[10] = (uint8_t)(irPPG>>=8);
      buf[9] = (uint8_t)(irPPG>>=8);
      buf[8] = (uint8_t)(irPPG>>=8);
      // for(int x=4;x<8;x++) Serial.print(String(buf[x])+" ");
      Serial.print(" - Green: ");
      /*fill buf 4-7 ppgred 126666*/
   /*   uint32_t greenPPG= particleSensor.getGreen();
      buf[15] = (uint8_t)greenPPG;
      buf[14] = (uint8_t)(greenPPG>>=8);
      buf[13] = (uint8_t)(greenPPG>>=8);
      buf[12] = (uint8_t)(greenPPG>>=8);
      // for(int x=4;x<8;x++) Serial.print(String(buf[x])+" ");
      timer0=millis();
      Serial.println("");
      dataReady=true;    
      counterG++;
  }*/
  /*Get data from ppg*/
  particleSensor.check(); //Check the sensor, read up to 3 samples
  //Serial.print(String(millis()-timer0)+" ");
  while (particleSensor.available()){
    samplesTaken++;
    uint32_t timestamp=millis();
    buf[3] = (uint8_t)timestamp;
    buf[2] = (uint8_t)(timestamp>>=8);
    buf[1] = (uint8_t)(timestamp>>=8);
    buf[0] = (uint8_t)(timestamp>>=8);
    Serial.print(" R[");
    uint32_t redPPG= particleSensor.getFIFORed();
    Serial.print(redPPG);
    buf[7] = (uint8_t)redPPG;
    buf[6] = (uint8_t)(redPPG>>=8);
    buf[5] = (uint8_t)(redPPG>>=8);
    buf[4] = (uint8_t)(redPPG>>=8);
    Serial.print("] IR[");
    uint32_t irPPG= particleSensor.getIR();
    Serial.print(irPPG);
    buf[11] = (uint8_t)irPPG;
    buf[10] = (uint8_t)(irPPG>>=8);
    buf[9] = (uint8_t)(irPPG>>=8);
    buf[8] = (uint8_t)(irPPG>>=8);
    Serial.print("] G[");
    uint32_t greenPPG= particleSensor.getGreen();
    Serial.print(greenPPG);
    buf[15] = (uint8_t)greenPPG;
    buf[14] = (uint8_t)(greenPPG>>=8);
    buf[13] = (uint8_t)(greenPPG>>=8);
    buf[12] = (uint8_t)(greenPPG>>=8);
    Serial.print("] Hz[");
    Serial.print((float)samplesTaken / ((millis() - timer0) / 1000.0), 2);
    Serial.print("]");
    Serial.print(samplesTaken);
    Serial.println();
    dataReady=true;
    particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }  
  
  if(dataReady)  {
    bleuart.write(buf, 16  );
    dataReady=false;    
  }
 }
 
  
  // Forward from BLEUART to HW Serial
  while ( bleuart.available() )
  {
    uint8_t ch;
    ch = (uint8_t) bleuart.read();
    Serial.write(ch);
  }
}

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));

  Serial.print("Connected to ");
  Serial.println(central_name);
  connected=true;
  timer0=millis();
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.println();
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
  connected=false;
}
