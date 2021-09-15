#include <bluefruit.h>
#include <Wire.h>

BLEConnection* connection ;

#include "Max86141_Functions.h"
#include "IMU_Functions.h"
#include "Temperature_Functions.h"
#include "TensorFlowLite_Functions.h"

uint8_t bufError[4];
long startTimer = 0;

/*send or stop bluetooth communication*/
String start_stop_Sending;

/*Shutdown or restart the sensor*/
bool shutdown_or_restart;

/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);

BLEService        hrms = BLEService(UUID16_SVC_HEART_RATE);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);

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


  /*Init PPG 86140 - 86141*/
  configurePPG86();
  /*pinMode(interruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interruptPin), updatePPG86, ISR_DEFERRED | HIGH);
    //attachInterrupt(digitalPinToInterrupt(interruptPin), blink, ISR_DEFERRED | CHANGE);
    pulseOx1.startTime1 = millis();*/

  /*init IMU*/
  configureIMU();

  /*init TEMP*/
  //configureTemp();

  // Setup services
  //setupHRM();

  setupErrorService();
  bufError[0] = errorIMU;
  bufError[1] = errorPPG86;
  //bufError[1] = errorPPG;
  bufError[2] = errorTemp;
  bufError[3] = errorTens;
  // Setup the advertising packet(s)

  ErrorCharacteristic.write(bufError, 4);

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

  setupIMUService();
  //AccCharacteristic.write(bufAcc_Gy_Ma, 32);
  AccCharacteristic.write(bufAcc, 11);
  GyroCharacteristic.write(bufGyro, 11);
  MagCharacteristic.write(bufMag, 10);

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");
}

void loop() {

  if (!errorPPG86) {
    updatePPG86();
  }
  if (!errorIMU) {
    updateAcc();
    updateGyro();
    updateMag();
  }

  if ( Bluefruit.connected()) {

    if (start_stop_Sending == "send") {

      if (shutdown_or_restart == 1) { // the sensor was shutdown
        /*Init PPG 86140 - 86141*/
        configurePPG86();

        if (!errorPPG86) {
          updatePPG86();
        }
        shutdown_or_restart = 0;
      }
      
      if (dataReady)  {

        if ( ErrorCharacteristic.notify(bufError, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

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
        /*if ( ledSeq1A_PPG1Characteristic2.notify( pt_ledSeq1A_PD1_2, 8) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
          } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
          }

          if ( ledSeq1A_PPG2Characteristic2.notify( pt_ledSeq1A_PD2_2, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
          } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
          }*/

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
        dataReady = false;
      }
      else delay(1);
    }

    if (start_stop_Sending == "stop") {
      Serial.println("Device disconnected, data not sent");
      /*Shutdown PPG*/
      pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
      shutdown_or_restart = 1;
    }
  }
  /*Put a delay to send to the feather*/
  //delay(50);

}
