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

/*
 * This sketch demonstrate the central API(). A additional bluefruit
 * that has bleuart as peripheral is required for the demo.
 */
#include <bluefruit.h>
//14000 228s = 61b
//sent 22802 3333  22s 3333 array  = 151*256 = 38000 B/s = 38kB
BLEClientBas  clientBas;  // battery client
BLEClientDis  clientDis;  // device information client
BLEClientUart clientUart; // bleuart client
unsigned int timer0,timer1;
int packetNb=0;
int readBytesNb=0;
byte buf[16];


void setup()
{
  Serial.begin(115200);
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println("Bluefruit52 Central BLEUART Example");
  Serial.println("-----------------------------------\n");
  //random for data generator 
  randomSeed(analogRead(0));
  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Bluefruit.begin(0, 1);
  
  Bluefruit.setName("Bluefruit52 Central");

  // Configure Battyer client
  clientBas.begin();  

  // Configure DIS client
  clientDis.begin();

  // Init BLE Central Uart Serivce
  clientUart.begin();
  clientUart.setRxCallback(bleuart_rx_callback);

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setConnectCallback(connect_callback);
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);

  /* Start Central Scanning
   * - Enable auto scan if disconnected
   * - Interval = 100 ms, window = 80 ms
   * - Don't use active scan
   * - Start(timeout) with timeout = 0 will scan forever (until connected)
   */
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);                   // // 0 = Don't stop scanning after n seconds
}

/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Check if advertising contain BleUart service
  if ( Bluefruit.Scanner.checkReportForService(report, clientUart) )
  {
    Serial.print("BLE UART service detected. Connecting ... ");

    // Connect to device with bleuart service in advertising
    Bluefruit.Central.connect(report);
  }else
  {      
    // For Softdevice v6: after received a report, scanner will be paused
    // We need to call Scanner resume() to continue scanning
    Bluefruit.Scanner.resume();
  }
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");

  Serial.print("Dicovering Device Information ... ");
  if ( clientDis.discover(conn_handle) )
  {
    Serial.println("Found it");
    char buffer[32+1];
    
    // read and print out Manufacturer
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getManufacturer(buffer, sizeof(buffer)) )
    {
      Serial.print("Manufacturer: ");
      Serial.println(buffer);
    }

    // read and print out Model Number
    memset(buffer, 0, sizeof(buffer));
    if ( clientDis.getModel(buffer, sizeof(buffer)) )
    {
      Serial.print("Model: ");
      Serial.println(buffer);
    }

    Serial.println();
  }else
  {
    Serial.println("Found NONE");
  }

  Serial.print("Dicovering Battery ... ");
  if ( clientBas.discover(conn_handle) )
  {
    Serial.println("Found it");
    Serial.print("Battery level: ");
    Serial.print(clientBas.read());
    Serial.println("%");
  }else
  {
    Serial.println("Found NONE");
  }

  Serial.print("Discovering BLE Uart Service ... ");
  if ( clientUart.discover(conn_handle) )
  {
    Serial.println("Found it");

    Serial.println("Enable TXD's notify");
    clientUart.enableTXD();

    Serial.println("Ready to receive from peripheral");
  }else
  {
    Serial.println("Found NONE");
    
    // disconnect since we couldn't find bleuart service
    Bluefruit.disconnect(conn_handle);
  }  
    timer0=millis();
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}

/**
 * Callback invoked when uart received data
 * @param uart_svc Reference object to the service where the data 
 * arrived. In this example it is clientUart
 */
void bleuart_rx_callback(BLEClientUart& uart_svc)
{
  //Serial.print("[RX]: ");
 // packetNb++;
  //Serial.print(String (millis()-timer0)+" "+String(packetNb));

 int i=0; 
 while ( uart_svc.available() )
  {
  buf[readBytesNb]=uart_svc.read();
  //Serial.print(String(buf[readBytesNb])+" ");
  //every 2 bytes received
  /*
  if(i==4) {
    
    uint16_t tempData= word( buf[i-1],  buf[i]);
    Serial.print(String(buf[i-1])+" ");
    Serial.print(String(buf[i])+" ");
    Serial.print(" [v]: ");
    Serial.print(tempData);
    Serial.print(" ");
    i=0;
  }
  else i++;*/ 

  readBytesNb++;
   // Serial.print( /*(char)*/ uart_svc.read() );
  }
  if(readBytesNb==16){
      //uint32_t timestamp= 0;
      int startByte=7;
      uint32_t timestamp = buf[0]; //0
      timestamp = (timestamp  << 8) + buf[1];
      timestamp = (timestamp << 8) + buf[2];
      timestamp = (timestamp << 8) + buf[3];  
      /*Serial.print("buf[0]: "+String(buf[0]));
      Serial.print(" buf[1]: "+String(buf[1]));
       Serial.print("buf[2]: "+String(buf[2]));
      Serial.print(" buf[3]: "+String(buf[3]));*/
      //Serial.print(" timestamp: "+String(timestamp));
      uint32_t redPPG= 0;
      redPPG = buf[4];
      redPPG = (redPPG  << 8) + buf[5];
      redPPG = (redPPG  << 8) + buf[6];
      redPPG = (redPPG  << 8) + buf[7];  
      //Serial.print("redPPG: "+String(redPPG));
      Serial.println(redPPG);
  }
  
  //Serial.print("Received bytes"+String(readBytesNb));
  readBytesNb=0;
  //Serial.println();
}

void loop()
{
  if ( Bluefruit.Central.connected() )
  {
    // Not discovered yet
    if ( clientUart.discovered() )
    {
      // Discovered means in working state
      // Get Serial input and send to Peripheral
      if ( Serial.available() )
      {
        delay(2); // delay a bit for all characters to arrive
        
        char str[20+1] = { 0 };
        Serial.readBytes(str, 20);
        
        clientUart.print( str );
      }
    }
  }
}
