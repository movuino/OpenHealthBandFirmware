#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);
/*PPG Service & characteristic*/
BLEClientService PPGService(0x1165);
BLEClientCharacteristic rawPPGCharacteristic(0x1166);
/*IMU Service & characteristics*/
BLEClientService IMUService(0x1101);
BLEClientCharacteristic AccCharacteristic(0x1102);
BLEClientCharacteristic GyroCharacteristic(0x1103);
BLEClientCharacteristic MagCharacteristic(0x1104);

uint16_t conn_handle0;
long myTimer=0;
long receivedSamplesNb=0;
int errorFlag=0;

void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println(F("Custom Service and Characteristic Example Rx"));
  Serial.println(F("--------------------------------------\n"));
  // Initialize Bluefruit with maximum connections as Peripheral = 0, Central = 1
  // SRAM usage required by SoftDevice will increase dramatically with number of connections
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);
  Bluefruit.setName("OHB Receiver");
  // Initialize Custom Service Client
  ErrorService.begin();
  // set up callback for receiving measurement
  ErrorCharacteristic.setNotifyCallback(ErrorCharacteristic_notify_callback);
  ErrorCharacteristic.begin();
  PPGService.begin();
  // set up callback for receiving measurement
  rawPPGCharacteristic.setNotifyCallback(rawPPGCharacteristic_notify_callback);
  rawPPGCharacteristic.begin();
  IMUService.begin();
  // set up callback for receiving measurement
  AccCharacteristic.setNotifyCallback(AccCharacteristic_notify_callback);
  AccCharacteristic.begin();
  GyroCharacteristic.setNotifyCallback(GyroCharacteristic_notify_callback);
  GyroCharacteristic.begin();
  MagCharacteristic.setNotifyCallback(MagCharacteristic_notify_callback);
  MagCharacteristic.begin();
  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(ErrorService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
