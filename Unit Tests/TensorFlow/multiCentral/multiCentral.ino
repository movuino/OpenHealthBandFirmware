#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);

/*IMU Service & characteristics*/
BLEClientService IMUService(0x1101);
BLEClientCharacteristic AccCharacteristic(0x1102);
BLEClientCharacteristic GyroCharacteristic(0x1103);
BLEClientCharacteristic MagCharacteristic(0x1104);

/*TensorFlow Service & characteristics*/
BLEClientService        TensorFlowService(0x1300);
BLEClientCharacteristic PossCharacteristic(0x1301);

long timeStamp;
uint16_t conn_handle0;
long myTimer=0;
long receivedSamplesNb=0;
int errorFlag=0;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float accelRange=16.0;
float gyroRange=2000.0;
double Gb1, P_O1, P_G1, P_R1, P_T1;
float sensor_Temp;
uint32_t sixRAM,nineRAM;
int aTensorFlow, bTensorFlow, cTensorFlow;
void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb
 
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);
  Bluefruit.setName("OHB Receiver");
  // Initialize Custom Service Client
  ErrorService.begin();
  // set up callback for receiving measurement
  ErrorCharacteristic.setNotifyCallback(ErrorCharacteristic_notify_callback);
  ErrorCharacteristic.begin();
  
  IMUService.begin();
  // set up callback for receiving measurement
  AccCharacteristic.setNotifyCallback(AccCharacteristic_notify_callback);
  AccCharacteristic.begin();
  GyroCharacteristic.setNotifyCallback(GyroCharacteristic_notify_callback);
  GyroCharacteristic.begin();
  MagCharacteristic.setNotifyCallback(MagCharacteristic_notify_callback);
  MagCharacteristic.begin();
  

 TensorFlowService.begin();
// set up callback for receiving measurement
 PossCharacteristic.setNotifyCallback(PossCharacteristic_notify_callback);
 PossCharacteristic.begin();

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
