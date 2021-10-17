#include <bluefruit.h>
#include <Wire.h>

BLEConnection* connection ;

/* Bool Errors*/
bool errorIMU = false;
bool errorPPG86 = false;
bool errorTemp = false;
bool errorTens = false;

#define PPG_Max86141
#define IMU9250
#define Temperature
#define TensorFlow

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
long startTimer = 0;

/*send or stop bluetooth communication*/
String start_stop_Sending;

/*Shutdown or restart the Max86141 sensor*/
bool shutdown_or_restart;

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

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

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

  ledSeq1A_PPG1Characteristic2.write(ptledSeq1APD1, 16);
  //ledSeq1A_PPG1Characteristic2.write(pt_ledSeq1A_PD1_2, 8);
  //tagSeq1A_PPG1Characteristic2.write(pt_tagSeq1A_PD1_2, 1);
  //ledSeq1A_PPG2Characteristic2.write(pt_ledSeq1A_PD2_2, 4);
  ledSeq1A_PPG2Characteristic2.write(pt_ledSeq1A_PD2_2, 16);
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
  TempCharacteristic.write(buffData,8);
#endif
  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");


}

void loop() {

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

/* Sending data by Bluetooth */
  if ( Bluefruit.connected()) {

    if (start_stop_Sending == "send") {

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

//     if (dataReady)  {

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
        if ( ledSeq1A_PPG1Characteristic2.notify( ptledSeq1APD1, 16) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( ledSeq1A_PPG2Characteristic2.notify( ptledSeq1APD2, 16) ) {
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

       // dataReady = false;
      //}
      //else delay(1);
    }

    if (start_stop_Sending == "stop") {
      Serial.println("Device disconnected, data not sent");
      
#ifdef PPG_Max86141
      /*Shutdown PPG*/
      pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
#endif

      shutdown_or_restart = 1;
    }
  }


  /*Put a delay to send to the feather*/
  delay(50);

}
