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

/* Shutdown or restart the sensor */
uint8_t shutdown_or_restartPPG = 0, shutdown_or_restartIMU = 0;

/*Start or Stop command received from Central*/
uint8_t ssCommand = 0;
bool firstStartIMU = 0, firstStartPPG = 0;

//#define DEBUG
#ifdef DEBUG
# define CHECK_NOTIFICATION(condition) \
  if (!condition)                     \
    Serial.printf("Notification failed on line %d\n", __LINE__);
#else
# define CHECK_NOTIFICATION(condition) condition;
#endif

/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

BLEService        hrms = BLEService(UUID16_SVC_HEART_RATE);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);

BLEService Start_StopService = BLEService(0x1400);
BLECharacteristic StartCharacteristic = BLECharacteristic(0x1401);
BLECharacteristic intensityLedsCharacteristic = BLECharacteristic(0x1402);
BLECharacteristic smplRateCharacteristic = BLECharacteristic(0x1403);
BLECharacteristic smplAvgCharacteristic = BLECharacteristic(0x1404);

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
  //Bluefruit.autoConnLed(true);
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
  Bluefruit.Periph.setConnInterval(12, 12); //min : 15ms, max: multiple of 15 ms for iphone

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

  // Init Sensors //

#ifdef PPG_Max86141
  shutdown_or_restartPPG = 3;
#endif

#ifdef IMU9250
  shutdown_or_restartIMU = 3;
#endif

  // Setup services //
  setupErrorService();

  // Setup the advertising packet(s)
  ErrorCharacteristic.write(bufError, 2);

#ifdef PPG_Max86141
  setupPPGMax86();

#ifdef PDLEDs
  PDLEDs_PD1_SNR1Characteristic.write(pt_ledSeq1A_PD1_SNR1_1, 24);
#endif

#ifdef PDsLED
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.write(pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2, 28);
#endif

#ifdef PDsLEDs
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.write(pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3, 28);
#endif

#endif

#ifdef IMU9250
  setupIMUService();
  AccGyrMagCharacteristic.write(bufAccGyrMag, 22);
#endif

  setupStart_StopService();
  StartCharacteristic.write8(0);
  intensityLedsCharacteristic.write8(0);
  smplRateCharacteristic.write8(0);
  smplAvgCharacteristic.write8(0);

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
  Serial.println();
}

void loop() {
  //---------------------------- Bluetooth Communication ----------------------------------//
#ifdef BleTest

#ifdef PPG_Max86141
  updatePPG86();
#endif

#ifdef IMU9250
  updateIMU();
#endif

  if (Bluefruit.connected()) {

    ssCommand = StartCharacteristic.read8();

    if (ssCommand == 1) { // Received 1 from Central to start sending data
      CHECK_NOTIFICATION ( ErrorCharacteristic.notify(bufError, 2) )
    }

    if ( (StartCharacteristic.read8() == 2) ) { //Received 2 from Central to stop sending data
      Serial.println("Device disconnected, data IMU & PPG not sent");

      if (firstStartIMU == 1) {
#ifdef IMU9250
        // sleep device
        mpu.write_byte(0x69, PWR_MGMT_1, 0x40);  // Set sleep mode bit (6), unenable all sensors
        delay(100);                                  // Wait for all registers to reset
        shutdown_or_restartIMU = 1;
#endif
      }

      if (firstStartPPG == 1) {
#ifdef PPG_Max86141
        //Shutdown PPG/
        pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
        shutdown_or_restartPPG = 1;
#endif
      }
      StartCharacteristic.write8(0);
    }
  }
#endif
}
