#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic warning "-Wextra"
# pragma GCC diagnostic warning "-Wall"
#endif

#include <Arduino.h>

#include <MAX86141.h>
#include <SPI.h>
#include <bluefruit.h>
#include <Wire.h>

#include "BleHandler.hpp"
#include "PpgHandler.hpp"
#include "ImuHandler.hpp"

#include "version.h"



/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);

/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);

/* PPG Max 86140 - 86141 Service & Characteristics*/
BLEService PPG86Service = BLEService(0x1300);


/* PDsLED (2 PDs - 1 LED) */
BLECharacteristic ledSeq1A_PPG1Characteristic2 = BLECharacteristic(0x1305);
BLECharacteristic ledSeq1A_PPG2Characteristic2 = BLECharacteristic(0x1307);

/* SNR (Signal Noise Ratio) */
BLECharacteristic SNR1_2PPG1Characteristic2 = BLECharacteristic(0x1313);
BLECharacteristic SNR2_2PPG2Characteristic2 = BLECharacteristic(0x1314);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

/* Utils */
BLEService Start_StopService = BLEService(0x1400);
BLECharacteristic StartCharacteristic = BLECharacteristic(0x1401);
BLECharacteristic intensityLedsCharacteristic = BLECharacteristic(0x1402);
BLECharacteristic smplRateCharacteristic = BLECharacteristic(0x1403);
BLECharacteristic smplAvgCharacteristic = BLECharacteristic(0x1404);
BLECharacteristic calibrationCharacteristic = BLECharacteristic(0x1405);

PpgHandler PPG;
ImuHandler IMU;

bool isAcquisitionStarted = false;
uint8_t globalTab[65];


void overdriveCallback(int newIntensity)
{
  char payload[1];
  Serial.println("Overdrive");
  payload[0] = newIntensity;
  intensityLedsCharacteristic.write(payload, 1);
  Serial.println(intensityLedsCharacteristic.notify(payload, 1));
}

void setup()
{
  Serial.begin(115200);
  delay(1000); // Wait for Serial without blocking

  Serial.printf("VERSION %s\n", FIRMWARE_VERSION);
  Serial.println(F("Initialise the OHB module"));
  Serial.println(F("-----------------------"));
  Serial.println();

  Serial.println(F("Setting Device Name to 'Open Health Band'"));
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.setTxPower(13);
  Bluefruit.begin();
  Bluefruit.setName("Movuino OHB - 000");

  /* Set BLE callbacks */
  Bluefruit.Periph.setConnectCallback(connection_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnection_callback);

  Serial.println(F("Configuring the Device Information Service"));
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  /* Configure the battery service */
  Serial.println(F("Configuring the Battery Service"));
  blebas.begin();
  blebas.write(100);

  /* Configure MAX86141 */
  PPG.configure();
  PPG.setCorrectionCallback(overdriveCallback);

  /* Configure MPU */
  IMU.configure();

  setupErrorService();
  setupIMUService();
  setupPPGService();
  setupUtilsService();

  const int errorPaylaod[] = {IMU.getError(), PPG.getError(), 0, 0};
  ErrorCharacteristic.write(errorPaylaod, 4);

  Bluefruit.Advertising.addService(ErrorService, PPG86Service, IMUService, Start_StopService);
  startAvertising();
  Serial.println(F("Setup done"));
}

void loop()
{
  if (!Bluefruit.connected()) return;

  // If we're connected
  switch (StartCharacteristic.read8())
  {
    case 1:
      Serial.println(F("Starting Acquisition"));
      StartCharacteristic.write8(0);
      isAcquisitionStarted = true;
      break;
    case 2:
      Serial.println(F("Stoping Acquisition"));
      StartCharacteristic.write8(0);
      isAcquisitionStarted = false;
      break;
    default:
      break;
  }

  // Update PPG configuration
  if (intensityLedsCharacteristic.read8())
  {
    PPG.setIntensity(intensityLedsCharacteristic.read8());
    intensityLedsCharacteristic.write8(0);
  }
  if (smplRateCharacteristic.read8())
  {
    PPG.setSampleRate(smplRateCharacteristic.read8());
    smplRateCharacteristic.write8(0);
  }
  if (smplAvgCharacteristic.read8())
  {
    PPG.setSampleAvg(smplAvgCharacteristic.read8());
    smplAvgCharacteristic.write8(0);
  }
  if (calibrationCharacteristic.read8())
  {
    PPG.calibrate();
    calibrationCharacteristic.write8(0);
  }

  if (!isAcquisitionStarted) return;
  // If acquisition started...

  if (true) {
    IMU.update();
    AccCharacteristic.notify(IMU.getAccTab(), IMU_ACC_TAB_SIZE);
    GyroCharacteristic.notify(IMU.getGyrTab(), IMU_GYR_TAB_SIZE);
    MagCharacteristic.notify(IMU.getMagTab(), IMU_MAG_TAB_SIZE);
  }

  if (true) {
    PPG.update();
    ledSeq1A_PPG1Characteristic2.notify(PPG.getPD1Tab(), PPG_VAL_TAB_SIZE);
    ledSeq1A_PPG2Characteristic2.notify(PPG.getPD2Tab(), PPG_VAL_TAB_SIZE);
    SNR1_2PPG1Characteristic2.notify(PPG.getSNR1Tab(), PPG_SNR_TAB_SIZE);
    SNR2_2PPG2Characteristic2.notify(PPG.getSNR2Tab(), PPG_SNR_TAB_SIZE);
  }

  delay(25);
}
