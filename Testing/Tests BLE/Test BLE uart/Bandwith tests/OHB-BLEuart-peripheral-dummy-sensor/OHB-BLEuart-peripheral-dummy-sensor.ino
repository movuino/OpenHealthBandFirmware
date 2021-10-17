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
unsigned int dataPackets=0;
unsigned int sentPackets=0;
uint8_t buf[16];
uint8_t c[16];
uint32_t test32=0;
bool connected=false;
unsigned int timer0;
unsigned int myTime=0;
uint16_t bufData[8];
bool dataReady=false;

int samplingRateHz = 100 ;
int samplingRateMs = 1000/100 ;

MAX30105 particleSensor;
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
  
#if CFG_DEBUG
  // Blocking wait for connection when debug mode is enabled via IDE
  while ( !Serial ) yield();
#endif
  Serial.println("Bluefruit52 BLEUART Example");
  Serial.println("---------------------------\n");
// Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
  
  
  
  
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
 if(connected){
  //data gathering here
  myTime=millis()-timer0;
  if(myTime>=10){
      
      timer0=millis();
      //uint32_t redPPG= particleSensor.getRed();
      //Serial.print("time "+String(myTime));
      dataPackets++;
      /*dummy buffer*/  
      memset(c, '1', 16);    
      buf[0]=1;
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      /*dummy buffer*/      
      c[3] = (uint8_t)test32;
      c[2] = (uint8_t)(test32>>=8);
      c[1] = (uint8_t)(test32>>=8);
      c[0] = (uint8_t)(test32>>=8);
      
      //Serial.println(String(myTime));
      dataReady=true;
      if(dataPackets%1000 ==0) {
      Serial.print(dataPackets);
      Serial.print(" ");
      Serial.println(myTime);
     // dataPackets =0;
      }
     // Serial.print("d "+String(dataPackets));
      }
  if(dataReady) {
    bleuart.write(c, 16  );
    sentPackets++;
    dataReady=!dataReady;
    }
  //Serial.println("s "+String(sentPackets));
 }
 
  // Forward from BLEUART to HW Serial
  while ( bleuart.available() ){
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
