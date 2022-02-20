#include <bluefruit.h>
#include <Wire.h>

BLEConnection* connection ;

/* Bool Errors */
bool errorIMU = true;
bool errorPPG86 = true;
bool errorTemp = true;
bool errorTens = true;

#define PPG_Max86141
#define IMU9250
//#define Temperature
//#define TensorFlow

/* Tests */
/* Print data on Serial Monitor when BLE is unenabled */
//#define SerialTest
#define BleTest


#ifdef PPG_Max86141
#include "Max86141_Functions.h"
#endif

#ifdef IMU9250
#include "IMU_Functions.h"
#endif

#ifdef Temperature
#include "Temperature_Functions.h"
#endif

#ifdef TensorFlow
#include "TensorFlowLite_Functions.h"
#endif

uint8_t bufError[4];


/*Shutdown or restart the Max86141 sensor*/
bool shutdown_or_restart = 0;

/*Start or Stop command received from Central*/
bool ssCommand = 0;

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
////// PDLEDs (1 PD - 2 LEDs) ////////////
/*BLECharacteristic ledSeq1A_PPG1Characteristic1 = BLECharacteristic(0x1301);
  BLECharacteristic tagSeq1A_PPG1Characteristic1 = BLECharacteristic(0x1302);
  BLECharacteristic ledSeq1B_PPG1Characteristic1 = BLECharacteristic(0x1303);
  BLECharacteristic tagSeq1B_PPG1Characteristic1 = BLECharacteristic(0x1304);*/

///// PDsLED (2 PDs - 1 LED) ///////////
BLECharacteristic ledSeq1A_PPG1Characteristic2 = BLECharacteristic(0x1305);
//BLECharacteristic tagSeq1A_PPG1Characteristic2 = BLECharacteristic(0x1306);
BLECharacteristic ledSeq1A_PPG2Characteristic2 = BLECharacteristic(0x1307);
/*BLECharacteristic tagSeq1A_PPG2Characteristic2 = BLECharacteristic(0x1308);
  BLECharacteristic ledSeq1B_PPG1Characteristic2 = BLECharacteristic(0x1309);
  BLECharacteristic tagSeq1B_PPG1Characteristic2 = BLECharacteristic(0x1310);
  BLECharacteristic ledSeq1B_PPG2Characteristic2 = BLECharacteristic(0x1311);
  BLECharacteristic tagSeq1B_PPG2Characteristic2 = BLECharacteristic(0x1312);*/
BLECharacteristic SNR1_2PPG1Characteristic2 = BLECharacteristic(0x1313);
BLECharacteristic SNR2_2PPG2Characteristic2 = BLECharacteristic(0x1314);

/*TEMP Service & characteristics*/
BLEService TempService = BLEService(UUID16_SVC_HEALTH_THERMOMETER );
BLECharacteristic  TempCharacteristic = BLECharacteristic(UUID16_CHR_TEMPERATURE_MEASUREMENT);
BLECharacteristic  TempCharacteristicType = BLECharacteristic(UUID16_CHR_TEMPERATURE_TYPE);

/*TensorFlow Service & characteristics*/
BLEService        TensorFlowService = BLEService(0x1500);
BLECharacteristic PossCharacteristic = BLECharacteristic(0x1501);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

BLEService Start_StopService = BLEService(0x1400);
BLECharacteristic StartCharacteristic = BLECharacteristic(0x1401);
BLECharacteristic intensityLedsCharacteristic = BLECharacteristic(0x1402);
BLECharacteristic smplRateCharacteristic = BLECharacteristic(0x1403);
BLECharacteristic smplAvgCharacteristic = BLECharacteristic(0x1404);


void setup() {

  Serial.begin(115200);
  while ( !Serial )
    delay(10);   // for nrf52840 with native usb

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Serial.println("-----------------------\n");

  // Initialise the Bluefruit module
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.setTxPower(13);
  Bluefruit.begin();
   
  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.setName("Movuino OHB - 000");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

////////////////////////////////////
 // pinMode(19, OUTPUT);
 // delay(5000);
 // digitalWrite(19, LOW); 
 // delay(5000);
 // digitalWrite(19, HIGH); 
////////////////////////

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

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

#ifdef Temperature
  /*init TEMP*/
  configureTemp();
#endif

#ifdef TensorFlow
  /*init TensorFlow*/
  configureTensorFlow();
#endif

  /* Setup services */
  setupErrorService();

  bufError[0] = errorIMU;
  bufError[1] = errorPPG86;
  bufError[2] = errorTemp;
  bufError[3] = errorTens;

  // Setup the advertising packet(s)
  ErrorCharacteristic.write(bufError, 4);

#ifdef PPG_Max86141
  setupPPGMax86();
  /*ledSeq1A_PPG1Characteristic1.write(pt_ledSeq1A_PD1_1, 4);
    tagSeq1A_PPG1Characteristic1.write(pt_tagSeq1A_PD1_1, 1);
    ledSeq1B_PPG1Characteristic1.write(pt_ledSeq1B_PD1_1, 4);
    tagSeq1B_PPG1Characteristic1.write(pt_tagSeq1B_PD1_1, 1);*/

  ledSeq1A_PPG1Characteristic2.write(ptledSeq1APD1, 20);
  //ledSeq1A_PPG1Characteristic2.write(pt_ledSeq1A_PD1_2, 8);
  //tagSeq1A_PPG1Characteristic2.write(pt_tagSeq1A_PD1_2, 1);
  //ledSeq1A_PPG2Characteristic2.write(pt_ledSeq1A_PD2_2, 4);
  ledSeq1A_PPG2Characteristic2.write(pt_ledSeq1A_PD2_2, 20);
  /*tagSeq1A_PPG2Characteristic2.write(pt_tagSeq1A_PD2_2, 1);
    ledSeq1B_PPG1Characteristic2.write(pt_ledSeq1B_PD1_2, 4);
    tagSeq1B_PPG1Characteristic2.write(pt_tagSeq1B_PD1_2, 1);
    ledSeq1B_PPG2Characteristic2.write(pt_ledSeq1B_PD2_2, 4);
    tagSeq1B_PPG2Characteristic2.write(pt_tagSeq1B_PD2_2, 1);*/
  SNR1_2PPG1Characteristic2.write(SNR1_2, 4);
  SNR2_2PPG2Characteristic2.write(SNR2_2, 4);
#endif

#ifdef IMU9250
  setupIMUService();
  //AccCharacteristic.write(bufAcc_Gy_Ma, 32);
  AccCharacteristic.write(bufAcc, 11);
  GyroCharacteristic.write(bufGyro, 11);
  MagCharacteristic.write(bufMag, 10);
#endif

#ifdef TensorFlow
  setupTensorFlowService();
  PossCharacteristic.write(p, 12);
#endif

#ifdef Temperature
  setupTempService();
  TempCharacteristic.write(buffData, 8);
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

}

void loop() {
  /* Update Sensors for new values */
#ifdef SerialTest

#ifdef PPG_Max86141
  if (!errorPPG86) {
    updatePPG86();
  }
#endif

#ifdef IMU9250
  if (!errorIMU) {
    updateAcc();
    updateGyro();
    updateMag();
  }
#endif

#ifdef Temperature
  if (!errorTemp) {
    updateTemp();
  }
#endif

#ifdef TensorFlow
  if (!errorTens) {
    updateTensorFlow();
  }
#endif

#endif

  /* Sending data by Bluetooth */
  if ( Bluefruit.connected()) {

    ssCommand = StartCharacteristic.read8();

    if (ssCommand == 1) { //Received 1 from Central to start sending data
 
 /* Update Sensors for new values */
#ifdef PPG_Max86141
  if (!errorPPG86) {
    updatePPG86();
  }
#endif

#ifdef IMU9250
  if (!errorIMU) {
    updateAcc();
    updateGyro();
    updateMag();
  }
#endif

#ifdef Temperature
  if (!errorTemp) {
    updateTemp();
  }
#endif

#ifdef TensorFlow
  if (!errorTens) {
    updateTensorFlow();
  }
#endif

      if (shutdown_or_restart == 1) { // the sensor was shutdown
#ifdef PPG_Max86141
        /*Init PPG 86140 - 86141*/
        configurePPG86();

        if (!errorPPG86) {
          updatePPG86();
        }
#endif
        shutdown_or_restart = 0;
      }

      if ((intensityLedsCharacteristic.read8() != 0)  && (smplRateCharacteristic.read8() != 0)) {
        /// Change Intensity leds, sample rate and sample avearge of the Max86141 ///
        pulseOx1.setIntensityLed(intensityLedsCharacteristic.read8(), LedMode);
        pulseOx1.setSample(smplAvgCharacteristic.read8(), smplRateCharacteristic.read8());
        intensityLedsCharacteristic.write8(0);
        smplRateCharacteristic.write8(0);
        smplAvgCharacteristic.write8(0);
      }


      if ( ErrorCharacteristic.notify(bufError, 4) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }

#ifdef IMU9250
      if ( AccCharacteristic.notify(bufAcc, 11) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      if ( GyroCharacteristic.notify(bufGyro, 11) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      if ( MagCharacteristic.notify(bufMag, 10) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
#endif

#ifdef PPG_Max86141
      if ( ledSeq1A_PPG1Characteristic2.notify( ptledSeq1APD1, 20) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }

      if ( ledSeq1A_PPG2Characteristic2.notify( ptledSeq1APD2, 20) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }

      if (  SNR1_2PPG1Characteristic2.notify( SNR1_2, 4) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
      if (  SNR2_2PPG2Characteristic2.notify( SNR2_2, 4) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
#endif

#ifdef TensorFlow
      if ( PossCharacteristic.notify(p, 12) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
#endif

#ifdef Temperature
      if ( TempCharacteristic.notify(buffData, 8) ) {
        //Serial.print("IMUCharacteristic updated to: ");
        //Serial.println(timeStampValue);
      } else {
        Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }
#endif

    }

    if ( StartCharacteristic.read8() == 2) { //Received 2 from Central to stop sending data
      Serial.println("Device disconnected, data not sent");

#ifdef PPG_Max86141
      /*Shutdown PPG*/
      pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
#endif

      shutdown_or_restart = 1;
      StartCharacteristic.write8(0);

    }
  }

  /*Put a delay to send to the feather*/
  //delay(50);

}
