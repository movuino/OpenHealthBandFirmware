#include <bluefruit.h>

BLEClientService        timeStampService(0x1165);
BLEClientService        IMUService(0x1101);

BLEClientCharacteristic timeStampCharacteristic(0x1166);
BLEClientCharacteristic IMUCharacteristic(0x1102);


long myTimer=0;
long receivedSamplesNb=0;

void setup() {
  Serial.begin(115200);
//  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println(F("Custom Service and Characteristic Example Rx"));
  Serial.println(F("--------------------------------------\n"));

  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);

  Bluefruit.setName("OHB Receiver");

  // Initialize Custom Service Client
  IMUService.begin();
  timeStampService.begin();
 
  
  // set up callback for receiving measurement
  IMUCharacteristic.setNotifyCallback(IMUCharacteristic_notify_callback);
  IMUCharacteristic.begin();
  timeStampCharacteristic.setNotifyCallback(timeStampCharacteristic_notify_callback);
  timeStampCharacteristic.begin();

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(timeStampService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
