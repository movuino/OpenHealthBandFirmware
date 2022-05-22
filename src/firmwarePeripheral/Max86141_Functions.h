
/*
   Intensity LED  : 0 = 0mA | 255 = 124mA (max). Be careful, data from PD can saturate at 524287 if Intensity LED is too high
   PGG Sample Rate : 0x00 = 24.995 samples per second | 0x13 = 4096 samples per second (max)
   Sample Average : 2, 4, 8, 16, 32, 64, 128 samples (max)
   Led Sequence Control : LED1 (1), LED2 (2), LED3 (3), LED1 and LED2 pulsed simultaneously (4),
   LED1 and LED3 pulsed simultaneously (5), LED2 and LED3 pulsed simultaneously (6), LED1, LED2 and LED3 pulsed simultaneously (7),
   Pilot on LED1 (8), DIRECT AMBIENT (9), LED4 [external mux control] (10), LED5 [external mux control] (11), LED6 [external mux control] (12)
   DIRECT AMBIENT : DA (i.e. normal photodiode measurements)
   Sequence Control is up to the configuration you wish (page 14-15 datasheet)
   PD: PhotoDiode
   1 LED is RGB or 1 color
*/

#include <MAX86141.h>
#include <SPI.h>

/* PPG Sensor Configurations */

// Sensor composed with 1 PD and 2 LEDs //
//#define PDLEDs

// Sensor composed with 2 PDs and 1 LED //
//#define PDsLED

// Sensor composed with 2 PDs and 3 LEDs //
#define PDsLEDs

/* PPG Max 86140 - 86141 Service & Characteristics*/
BLEService PPG86Service = BLEService(0x1300);

#ifdef PDLEDs
////// PDLEDs (1 PD - 2 LEDs) //////
BLECharacteristic PDLEDs_PD1_SNR1Characteristic = BLECharacteristic(0x1301);

//////////// Pointers used to send timestamp, 4 samples PD1 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_SNR1_1[24];

void getDataPDLEDs();
#endif

#ifdef PDsLED
///// PDsLED (2 PDs - 1 LED) //////
BLECharacteristic PDsLED_PD1_PD2_SNR1_SNR2Characteristic = BLECharacteristic(0x1302);

//////////// Pointers used to send timestamp, 2 samples PD1, 2 samples PD2 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[28];

void getDataPDsLED();
#endif

#ifdef PDsLEDs
////// PDsLEDs (2 PDs - 3 LEDs) //////
BLECharacteristic PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic = BLECharacteristic(0x1303);

//////////// Pointers used to send timestamp, 2 samples PD1, 2 samples PD2 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[28];

void getDataPDsLEDs();
#endif


/* Inculde LED configuration */
int ledMode[10];

#include "LEDsConfiguration_Sensor.h"


/* Pin Definitions */
// #define MISO_PIN              19
// #define MOSI_PIN              18
// #define SCK_PIN               5
//#define SS_PIN                19 //(Adafruit)
#define SS_PIN                10 //(Movuino)


/* Global Variables */
static int spiClk = 1000000; // 8 MHz Maximum
int cpt1 = 0, cpt2 = 0;
int interruptPin = 36;
int sequences_size = 0;
int fifo_size = 8;
float snr_pd1, snr_pd2;

MAX86141 pulseOx1;


void configurePPG86(void) {
  Serial.println("####  PPG MAX86141 CONFIG ####");
  // Configure IO.
  pinMode(SS_PIN, OUTPUT);
  digitalWrite(SS_PIN, HIGH);

  //initialise SPI
  pulseOx1.spi = new SPIClass(SPI);
  pulseOx1.SS = SS_PIN;
  Serial.println("Init Device");
  pulseOx1.spi->begin();

  delay(100);
  pulseOx1.setDebug(true);

#ifdef PDLEDs
  sequences_size = config(rgbLED_G/*Green LED (Sequence 1A, 0-3)*/ | DA /*Direct Ambient (Sequence 1B, 4-9)*/);
  pulseOx1.initialisation(1/*nb_pds*/, ledMode/*LedMode*/, sequences_size/*Number of sequences*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDsLED
  sequences_size = config(rgbLED_G /*Green LED selected (Sequence 1A, 0-3)*/ | DA /*Direct Ambient (Sequence 2B, 4-9)*/);
  pulseOx1.initialisation(2/*nb_pds*/, ledMode/*LedMode*/, sequences_size/*Number of sequences*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0xE/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDsLEDs
  sequences_size = config(rgbLED_G/*Green LED (Sequence 1A, 0-3)*/ | DA /*Direct Ambient (Sequence 1B, 4-9)*/);
  pulseOx1.initialisation(2/*nb_ppg*/, ledMode/*LedMode*/, sequences_size/*Number of sequences*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

  Serial.println("--Read Register-- System Control");
  Serial.println(pulseOx1.read_reg(0x0D));

  Serial.println("--Read Register-- PART_ID");
  int part_ID = pulseOx1.read_reg(0xFF);
  Serial.println(part_ID);

  Serial.println("--Read Temp-- 0x40");
  pulseOx1.write_reg(0x40, 0xFF);

  Serial.println(pulseOx1.read_reg(0x41));

  if (part_ID == 36) {
    Serial.println("MAX86140 connection succeeded !");
    errorPPG86 = false;
  }
  else if (part_ID == 37) {
    Serial.println("MAX86141 connection succeeded !");
    errorPPG86 = false;
  }
  else {
    Serial.println("Connection failed !");
  }

  bufError[1] = errorPPG86;

  delay(1000);

  pulseOx1.setDebug(false);
}


void updatePPG86(void) {

#ifdef BleTest
  if ( Bluefruit.connected()) {

    ssCommand = StartCharacteristic.read8();

    if (ssCommand == 1) { //Received 1 from Central to start sending data

      if (shutdown_or_restartPPG == 1) { // the sensor was shutdown
        //Init PPG 86140 - 86141/
        configurePPG86();

        if (!errorPPG86) {
          uint8_t intStatus;
          //Read Status
          intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
          bool flagA_full = (intStatus & 0x80) >> 7;

          /////// if there is 8 data in the FIFO ///////
          if (flagA_full) {
            pulseOx1.device_data_read1();

#ifdef PDsLEDs
            getDataPDsLEDs();
#endif

#ifdef PDsLED
            getDataPDsLED();
#endif

#ifdef PDLEDs
            getDataPDLEDs();
#endif
          }
        }
        shutdown_or_restartPPG = 0;
      }

      else if (shutdown_or_restartPPG == 3) { // first Start when when central notify
        // Init PPG 86140 - 86141 //
        configurePPG86();
        shutdown_or_restartPPG = 0;
        firstStartPPG = 1;
      }

      if (!errorPPG86) {
        uint8_t intStatus;
        //Read Status
        intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
        bool flagA_full = (intStatus & 0x80) >> 7;

        /////// if there is 8 data in the FIFO ///////
        if (flagA_full) {
          pulseOx1.device_data_read1();

#ifdef PDsLEDs
          getDataPDsLEDs();
#endif

#ifdef PDsLED
          getDataPDsLED();
#endif

#ifdef PDLEDs
          getDataPDLEDs();
#endif
        }

#ifdef PDsLED
        CHECK_NOTIFICATION(PDsLED_PD1_PD2_SNR1_SNR2Characteristic.notify(pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2, 28))
#endif

#ifdef PDLEDs
        CHECK_NOTIFICATION(PDLEDs_PD1_SNR1Characteristic.notify(pt_ledSeq1A_PD1_SNR1_1, 24))
#endif

#ifdef PDsLEDs
        CHECK_NOTIFICATION(PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.notify(pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3, 28))
#endif
      }

      if ((intensityLedsCharacteristic.read8() != 0)  && (smplRateCharacteristic.read8() != 0)) {
        /// Change Intensity leds, sample rate and sample avearge of the Max86141 ///
        pulseOx1.setIntensityLed(intensityLedsCharacteristic.read8(), ledMode);
        pulseOx1.setSample(smplAvgCharacteristic.read8(), smplRateCharacteristic.read8());
        intensityLedsCharacteristic.write8(0);
        smplRateCharacteristic.write8(0);
        smplAvgCharacteristic.write8(0);
      }
    }
  }
#endif
}

#ifdef PDsLEDs
void getDataPDsLEDs() {

#ifdef SerialTest
  Serial.println("----- PPG data ----- :");
  Serial.println("Reading all data from PD1: ");
  for (int i = 0; i < fifo_size / 4; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
  }

  Serial.println("Reading all data from PD2: ");
  for (int i = 0; i < fifo_size / 4; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
  }

  free(pulseOx1.tab_ledSeq1A_PD1);
  free(pulseOx1.tab_ledSeq1A_PD2);
#endif

#ifdef BleTest
  ///////// See if direct ambient is affecting the output of ADC (OverFlow) /////////
  uint8_t InterruptStatus_without_AFull_DataReady = pulseOx1.read_reg(REG_INT_STAT_1) << 2;
  bool InterruptStatus_with_ALC = pulseOx1.read_reg(REG_INT_STAT_1) >> 8;
  //Serial.println("############## Reading ALC bit ############ " + String(InterruptStatus_with_ALC));

  if (InterruptStatus_with_ALC == 0) {
    ///////////// Pointer to send only 2 samples by Bluetooth (PD1) ////////////
    if ((pulseOx1.tab_ledSeq1A_PD1[0] != 0) && (pulseOx1.tab_ledSeq1A_PD1[1] != 0)) {

      ///////////// Addition data of PD1 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD1[cpt1] = pulseOx1.tab_ledSeq1A_PD1[0];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = pulseOx1.tab_ledSeq1A_PD1[1];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD1) ////////////
      uint32_t timestamp1 = millis();

      uint8_t* ptTimeStamp = (uint8_t*)&timestamp1;

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[0] = ptTimeStamp[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[1] = ptTimeStamp[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[2] = ptTimeStamp[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[3] = ptTimeStamp[3];

      uint8_t* ptTab_ledSeq1A_PD1_1 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[0];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[4] = ptTab_ledSeq1A_PD1_1[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[5] = ptTab_ledSeq1A_PD1_1[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[6] = ptTab_ledSeq1A_PD1_1[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[7] = ptTab_ledSeq1A_PD1_1[3];

      uint8_t* ptTab_ledSeq1A_PD1_2 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[1];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[8] = ptTab_ledSeq1A_PD1_2[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[9] = ptTab_ledSeq1A_PD1_2[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[10] = ptTab_ledSeq1A_PD1_2[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[11]  = ptTab_ledSeq1A_PD1_2[3];

      cpt1 += 2;
      if (cpt1 == SIZE) {
        //Serial.println("################################SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          uint8_t* ptVar = (uint8_t*)&a;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[12] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[13] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[14] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[15] = ptVar[3];
        }
        else {
          uint8_t* ptVar = (uint8_t*)&var;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[12] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[13] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[14] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[15] = ptVar[3];
        }
        cpt1 = 0;
      }

    }

    if ((pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0)) {

      ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD2) ////////////
      uint8_t* ptTab_ledSeq1A_PD2_1 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD2[0];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[16] = ptTab_ledSeq1A_PD2_1[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[17] = ptTab_ledSeq1A_PD2_1[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[18] = ptTab_ledSeq1A_PD2_1[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[19] = ptTab_ledSeq1A_PD2_1[3];

      uint8_t* ptTab_ledSeq1A_PD2_2 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD2[1];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[20] = ptTab_ledSeq1A_PD2_2[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[21] = ptTab_ledSeq1A_PD2_2[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[22] = ptTab_ledSeq1A_PD2_2[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[23] = ptTab_ledSeq1A_PD2_2[3];

      cpt2 += 2;
      if (cpt2 == SIZE) {
        //Serial.println("################################SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
        snr_pd2 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE));
        if (var < 0) {
          int a = -100 * var;
          uint8_t* ptVar = (uint8_t*)&a;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[24] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[25] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[26] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[27] = ptVar[3];
        }
        else {
          uint8_t* ptVar = (uint8_t*)&var;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[24] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[25] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[26] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_3[27] = ptVar[3];
        }
        cpt2 = 0;
      }
    }

    ////////// Detect saturation and with SNR set the led intensity to correct the measurement //////////
    if ((snr_pd1 > 80.0) || (snr_pd2 > 80.0)) {
      //Serial.println("############################ SATURATION #########################");
      /// Regulate led instensity to 20 ///
      pulseOx1.setIntensityLed(20, ledMode);
    }

  }
  free(pulseOx1.tab_ledSeq1A_PD1);
  free(pulseOx1.tab_ledSeq1A_PD2);
#endif
}
#endif

#ifdef PDLEDs
void getDataPDLEDs() {

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
  Serial.println("----- PPG data ----- :");
  Serial.println("Reading all data from PD1: ");
  for (int i = 0; i < fifo_size / 2; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
  }

  free(pulseOx1.tab_ledSeq1A_PD1);
#endif

  //---------------------------- Bluetooth Communication ----------------------------------//
#ifdef BleTest
  ///////// See if direct ambient is affecting the output of ADC (OverFlow) /////////
  uint8_t InterruptStatus_without_AFull_DataReady = pulseOx1.read_reg(REG_INT_STAT_1) << 2;
  bool InterruptStatus_with_ALC = pulseOx1.read_reg(REG_INT_STAT_1) >> 8;
  //Serial.println("############## Reading ALC bit ############ " + String(InterruptStatus_with_ALC));

  if (InterruptStatus_with_ALC == 0) {
    ///////////// Pointer to send only 4 samples by Bluetooth (PD1) ////////////
    if ((pulseOx1.tab_ledSeq1A_PD1[0] != 0) && (pulseOx1.tab_ledSeq1A_PD1[1] != 0) && (pulseOx1.tab_ledSeq1A_PD1[2] != 0) && (pulseOx1.tab_ledSeq1A_PD1[3] != 0)) {

      ///////////// Addition data of PD1 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD1[cpt1] = pulseOx1.tab_ledSeq1A_PD1[0];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = pulseOx1.tab_ledSeq1A_PD1[1];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 2] = pulseOx1.tab_ledSeq1A_PD1[2];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 3] = pulseOx1.tab_ledSeq1A_PD1[3];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD1) ////////////
      uint32_t timestamp1 = millis();

      uint8_t* ptTimeStamp = (uint8_t*)&timestamp1;

      pt_ledSeq1A_PD1_SNR1_1[0] = ptTimeStamp[0];
      pt_ledSeq1A_PD1_SNR1_1[1] = ptTimeStamp[1];
      pt_ledSeq1A_PD1_SNR1_1[2] = ptTimeStamp[2];
      pt_ledSeq1A_PD1_SNR1_1[3] = ptTimeStamp[3];

      uint8_t* ptTab_ledSeq1A_PD1_1 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[0];

      pt_ledSeq1A_PD1_SNR1_1[4] = ptTab_ledSeq1A_PD1_1[0];
      pt_ledSeq1A_PD1_SNR1_1[5] = ptTab_ledSeq1A_PD1_1[1];
      pt_ledSeq1A_PD1_SNR1_1[6] = ptTab_ledSeq1A_PD1_1[2];
      pt_ledSeq1A_PD1_SNR1_1[7] = ptTab_ledSeq1A_PD1_1[3];

      uint8_t* ptTab_ledSeq1A_PD1_2 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[1];

      pt_ledSeq1A_PD1_SNR1_1[8]  = ptTab_ledSeq1A_PD1_2[0];
      pt_ledSeq1A_PD1_SNR1_1[9]  = ptTab_ledSeq1A_PD1_2[1];
      pt_ledSeq1A_PD1_SNR1_1[10] = ptTab_ledSeq1A_PD1_2[2];
      pt_ledSeq1A_PD1_SNR1_1[11] = ptTab_ledSeq1A_PD1_2[3];

      uint8_t* ptTab_ledSeq1A_PD1_3 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[2];

      pt_ledSeq1A_PD1_SNR1_1[12] = ptTab_ledSeq1A_PD1_3[0];
      pt_ledSeq1A_PD1_SNR1_1[13] = ptTab_ledSeq1A_PD1_3[1];
      pt_ledSeq1A_PD1_SNR1_1[14] = ptTab_ledSeq1A_PD1_3[2];
      pt_ledSeq1A_PD1_SNR1_1[15] = ptTab_ledSeq1A_PD1_3[3];

      uint8_t* ptTab_ledSeq1A_PD1_4 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[3];

      pt_ledSeq1A_PD1_SNR1_1[16] = ptTab_ledSeq1A_PD1_4[0];
      pt_ledSeq1A_PD1_SNR1_1[17] = ptTab_ledSeq1A_PD1_4[1];
      pt_ledSeq1A_PD1_SNR1_1[18] = ptTab_ledSeq1A_PD1_4[2];
      pt_ledSeq1A_PD1_SNR1_1[19] = ptTab_ledSeq1A_PD1_4[3];

      cpt1 += 4;
      if (cpt1 == SIZE) {
        //Serial.println("################################SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          uint8_t* ptVar = (uint8_t*)&a;

          pt_ledSeq1A_PD1_SNR1_1[20] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_1[21] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_1[22] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_1[23] = ptVar[3];
        }
        else {
          uint8_t* ptVar = (uint8_t*)&var;

          pt_ledSeq1A_PD1_SNR1_1[20] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_1[21] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_1[22] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_1[23] = ptVar[3];
        }
        cpt1 = 0;
      }
    }

    ////////// Detect saturation and with SNR set the led intensity to correct the measurement //////////
    if ((snr_pd1 > 80.0)) {
      //Serial.println("############################ SATURATION #########################");
      /// Regulate led instensity to 20 ///
      pulseOx1.setIntensityLed(20, ledMode);
    }

  }
  free(pulseOx1.tab_ledSeq1A_PD1);
#endif
}
#endif

#ifdef PDsLED
void getDataPDsLED() {
  //---------------------------- Serial Communication -------------------------------------//

#ifdef SerialTest
  Serial.println("----- PPG data ----- :");
  Serial.println("Reading all data from PD1: ");
  for (int i = 0; i < fifo_size / 4; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
  }

  Serial.println("Reading all data from PD2: ");
  for (int i = 0; i < fifo_size / 4; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
  }

  free(pulseOx1.tab_ledSeq1A_PD1);
  free(pulseOx1.tab_ledSeq1A_PD2);
#endif


  //---------------------------- Bluetooth Communication ----------------------------------//
#ifdef BleTest
  ///////// See if direct ambient is affecting the output of ADC (OverFlow) /////////
  uint8_t InterruptStatus_without_AFull_DataReady = pulseOx1.read_reg(REG_INT_STAT_1) << 2;
  bool InterruptStatus_with_ALC = pulseOx1.read_reg(REG_INT_STAT_1) >> 8;
  //Serial.println("############## Reading ALC bit ############ " + String(InterruptStatus_with_ALC));

  if (InterruptStatus_with_ALC == 0) {
    ///////////// Pointer to send only 2 samples by Bluetooth (PD1) ////////////
    if ((pulseOx1.tab_ledSeq1A_PD1[0] != 0) && (pulseOx1.tab_ledSeq1A_PD1[1] != 0)) {

      ///////////// Addition data of PD1 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD1[cpt1] = pulseOx1.tab_ledSeq1A_PD1[0];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = pulseOx1.tab_ledSeq1A_PD1[1];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD1) ////////////
      uint32_t timestamp1 = millis();

      uint8_t* ptTimeStamp = (uint8_t*)&timestamp1;

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[0] = ptTimeStamp[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[1] = ptTimeStamp[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[2] = ptTimeStamp[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[3] = ptTimeStamp[3];

      uint8_t* ptTab_ledSeq1A_PD1_1 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[0];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[4] = ptTab_ledSeq1A_PD1_1[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[5] = ptTab_ledSeq1A_PD1_1[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[6] = ptTab_ledSeq1A_PD1_1[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[7] = ptTab_ledSeq1A_PD1_1[3];

      uint8_t* ptTab_ledSeq1A_PD1_2 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD1[1];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[8] = ptTab_ledSeq1A_PD1_2[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[9] = ptTab_ledSeq1A_PD1_2[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[10]  = ptTab_ledSeq1A_PD1_2[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[11]  = ptTab_ledSeq1A_PD1_2[3];

      cpt1 += 2;
      if (cpt1 == SIZE) {
        //Serial.println("################################SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          uint8_t* ptVar = (uint8_t*)&a;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[12] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[13] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[14] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[15] = ptVar[3];
        }
        else {
          uint8_t* ptVar = (uint8_t*)&var;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[12] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[13] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[14] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[15] = ptVar[3];
        }
        cpt1 = 0;
      }

    }

    if ((pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0)) {

      ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];

      ///////////// Pointer to send only 2 samples by Bluetooth (PD2) ////////////
      uint8_t* ptTab_ledSeq1A_PD2_1 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD2[0];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[16] = ptTab_ledSeq1A_PD2_1[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[17] = ptTab_ledSeq1A_PD2_1[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[18] = ptTab_ledSeq1A_PD2_1[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[19] = ptTab_ledSeq1A_PD2_1[3];

      uint8_t* ptTab_ledSeq1A_PD2_2 = (uint8_t*)&pulseOx1.tab_ledSeq1A_PD2[1];

      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[20] = ptTab_ledSeq1A_PD2_2[0];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[21] = ptTab_ledSeq1A_PD2_2[1];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[22] = ptTab_ledSeq1A_PD2_2[2];
      pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[23] = ptTab_ledSeq1A_PD2_2[3];

      cpt2 += 2;
      if (cpt2 == SIZE) {
        //Serial.println("################################SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
        snr_pd2 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE));
        if (var < 0) {
          int a = -100 * var;
          uint8_t* ptVar = (uint8_t*)&a;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[24] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[25] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[26] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[27] = ptVar[3];
        }
        else {
          uint8_t* ptVar = (uint8_t*)&var;

          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[24] = ptVar[0];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[25] = ptVar[1];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[26] = ptVar[2];
          pt_ledSeq1A_PD1_SNR1_PD2_SNR2_2[27] = ptVar[3];
        }
        cpt2 = 0;
      }
    }

    ////////// Detect saturation and with SNR set the led intensity to correct the measurement //////////
    if ((snr_pd1 > 80.0) || (snr_pd2 > 80.0)) {
      //Serial.println("############################ SATURATION #########################");
      /// Regulate led instensity to 20 ///
      pulseOx1.setIntensityLed(20, ledMode);
    }

  }
  free(pulseOx1.tab_ledSeq1A_PD1);
  free(pulseOx1.tab_ledSeq1A_PD2);
#endif
}
#endif
