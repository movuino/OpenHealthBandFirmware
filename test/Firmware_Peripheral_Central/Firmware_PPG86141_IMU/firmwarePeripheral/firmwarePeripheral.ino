#include <bluefruit.h>
#include <Wire.h>

BLEConnection* connection;

/* Bool Errors  */
bool errorIMU = true;
bool errorPPG86 = true;
uint8_t bufError[2];

/* Enable the data measure */
#define PPG_Max86141
#define IMU9250

/* Tests */
/* Print data on Serial Monitor when BLE is unenabled */
//#define SerialTest
/* Sending data when BLE is enabled */
#define BleTest


/* Send or stop bluetooth communication */
String start_stop_Sending;

/* Shutdown or restart the sensor */
bool shutdown_or_restart;


/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

BLEService        hrms = BLEService(UUID16_SVC_HEART_RATE);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);


#ifdef PPG_Max86141
#include "Max86141_Functions.h"
#endif

#ifdef IMU9250
#include "IMU_Functions.h"
#endif

#include "Advertise.h"
#include "Connection.h"
#include "Services.h"


void setup() {

  Serial.begin(115200);
  //while ( !Serial )
  delay(10);   // for nrf52840 with native usb

  // Initialise the Bluefruit module
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.setTxPower(13);

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Serial.println("-----------------------\n");

  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");

  Bluefruit.setName("Movuino OHB - 000");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

  // Setup the Heart Rate Monitor service using
  // BLEService and BLECharacteristic classes
  Serial.println("Configuring the Heart Rate Monitor Service");
  setupHRM();

  Serial.println();

  /*Init Sensors*/

#ifdef PPG_Max86141
  /*Init PPG 86140 - 86141*/
  configurePPG86();
#endif

#ifdef IMU9250
  /*init IMU*/
  configureIMU();
#endif

  /* Setup services */
  setupErrorService();

  bufError[0] = errorIMU;
  bufError[1] = errorPPG86;

  // Setup the advertising packet(s)
  ErrorCharacteristic.write(bufError, 2);

#ifdef PPG_Max86141
  setupPPGMax86();

#ifdef PDLEDs
  ledSeq1A_PPG1Characteristic1.write(pt_ledSeq1A_PD1_1, 20);
  SNR1_1PPG1Characteristic1.write(SNR1_1, 4);
#endif

#ifdef PDsLED
  ledSeq1A_PPG1Characteristic2.write(pt_ledSeq1A_PD1_2, 12);
  ledSeq1A_PPG2Characteristic2.write(pt_ledSeq1A_PD2_2, 12);
  SNR1_2PPG1Characteristic2.write(SNR1_2, 4);
  SNR2_2PPG2Characteristic2.write(SNR2_2, 4);
#endif

#ifdef PDsLEDs
  ledSeq1A_PPG1Characteristic3.write(pt_ledSeq1A_PD1_3, 12);
  ledSeq1A_PPG2Characteristic3.write(pt_ledSeq1A_PD2_3, 12);
  SNR1_3PPG1Characteristic3.write(SNR1_3, 4);
  SNR2_3PPG2Characteristic3.write(SNR2_3, 4);
#endif
#endif

#ifdef IMU9250
  setupIMUService();
  AccCharacteristic.write(bufAcc, 11);
  GyroCharacteristic.write(bufGyro, 11);
  MagCharacteristic.write(bufMag, 10);
#endif


  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
  Serial.println();
}

void loop() {
  /* Update Sensors for new values */

  //---------------------------- Serial Communication -------------------------------------//

#ifdef SerialTest

#ifdef PPG_Max86141
  if (!errorPPG86) {
    updatePPG86();
  }
#endif

#ifdef IMU9250
  if (!errorIMU) {
    updateIMU();
  }
#endif

#endif

  //---------------------------- Bluetooth Communication ----------------------------------//

#ifdef BleTest

#ifdef PPG_Max86141
  if (!errorPPG86) {

#ifdef SampleRatePPG
    testingSampleRatePPG();
#endif

  }
#endif

#ifdef IMU9250
  if (!errorIMU) {

#ifdef SampleRateIMU
    testingSampleRateIMU();
#endif

  }
#endif

#endif
}
