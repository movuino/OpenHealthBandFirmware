#include <bluefruit.h>
#include <Wire.h>

BLEConnection* connection;

/* Enable the data measure */
#define PPG_Max86141
#define IMU9250

#include "Common.h"

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
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms

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
  // Init PPG 86140 - 86141 //
  configurePPG86();
#endif

#ifdef IMU9250
  // init IMU //
  configureIMU();
#endif

  // Setup services //
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
  if (!errorPPG86) {
    updatePPG86();
  }
#endif

#ifdef IMU9250
  if (!errorIMU) {
    updateIMU();
  }
#endif

  if (Bluefruit.connected()) {
    //Serial.println("No connected device");
    if ( StartCharacteristic.read8() == 2) { //Received 2 from Central to stop sending data
      Serial.println("Device disconnected, data IMU & PPG not sent");

      /* sleep device */
      mpu.write_byte(0x69, PWR_MGMT_1, 0x40);  // Set sleep mode bit (6), unenable all sensors
      delay(100);                                  // Wait for all registers to reset

      /* Shutdown PPG */
      pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])

      shutdown_or_restartIMU = 1;
      shutdown_or_restartPPG = 1;

      StartCharacteristic.write8(0);
    }
  }
#endif
}
