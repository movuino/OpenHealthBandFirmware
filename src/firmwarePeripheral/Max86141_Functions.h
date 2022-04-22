
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
BLECharacteristic ledSeq1A_PPG1Characteristic1 = BLECharacteristic(0x1301);
//BLECharacteristic tagSeq1A_PPG1Characteristic1 = BLECharacteristic(0x1302);
////// SNR (Signal Noise Ratio) //////
BLECharacteristic SNR1_1PPG1Characteristic1 = BLECharacteristic(0x1315);

//////////// Pointers used to send timestamp, 4 samples PD1 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_1[20];
uint8_t SNR1_1[4];

void getDataPDLEDs();
#endif

#ifdef PDsLED
///// PDsLED (2 PDs - 1 LED) //////
BLECharacteristic ledSeq1A_PPG1Characteristic2 = BLECharacteristic(0x1305);
//BLECharacteristic tagSeq1A_PPG1Characteristic2 = BLECharacteristic(0x1306);
BLECharacteristic ledSeq1A_PPG2Characteristic2 = BLECharacteristic(0x1307);
//BLECharacteristic tagSeq1A_PPG2Characteristic2 = BLECharacteristic(0x1308);
////// SNR (Signal Noise Ratio) //////
BLECharacteristic SNR1_2PPG1Characteristic2 = BLECharacteristic(0x1313);
BLECharacteristic SNR2_2PPG2Characteristic2 = BLECharacteristic(0x1314);

//////////// Pointers used to send timestamp, 2 samples PD1, 2 samples PD2 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_2[12];
uint8_t pt_ledSeq1A_PD2_2[12];
uint8_t SNR1_2[4], SNR2_2[4];

void getDataPDsLED();
#endif

#ifdef PDsLEDs
////// PDsLEDs (2 PDs - 3 LEDs) //////
BLECharacteristic ledSeq1A_PPG1Characteristic3 = BLECharacteristic(0x1309);
//BLECharacteristic tagSeq1A_PPG1Characteristic3 = BLECharacteristic(0x1310);
BLECharacteristic ledSeq1A_PPG2Characteristic3 = BLECharacteristic(0x1311);
//BLECharacteristic tagSeq1B_PPG2Characteristic3 = BLECharacteristic(0x1312);
////// SNR (Signal Noise Ratio) //////
BLECharacteristic SNR1_3PPG1Characteristic3 = BLECharacteristic(0x1317);
BLECharacteristic SNR2_3PPG2Characteristic3 = BLECharacteristic(0x1318);

//////////// Pointers used to send timestamp, 2 samples PD1, 2 samples PD2 and SNR by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_3[12];
uint8_t pt_ledSeq1A_PD2_3[12];
uint8_t SNR1_3[4], SNR2_3[4];

void getDataPDsLEDs();
#endif

/*Functions*/
void testingSampleRatePPG();

/* Inculde LED configuration */
int ledMode[10];

#include "LEDsConfiguration_Sensor.h"

/* Sample Rate taken */
//#define Sample_Rate
#define SampleRatePPG

/* Pin Definitions  */
// #define MISO_PIN              19
// #define MOSI_PIN              18
// #define SCK_PIN               5
//#define SS_PIN                19 //(Adafruit)
#define SS_PIN                10 //(Movuino)


/* Global Variables */
static int spiClk = 1000000; // 8 MHz Maximum
int cpt1 = 0, cpt2 = 0;
int interruptPin = 36;
bool dataReady = false;
long startTime;
long samplesTaken = 0;
int sequences_size = 0;
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

  delay(1000);

  pulseOx1.setDebug(false);

  startTime = millis();

  Serial.println();

}


void updatePPG86(void) {

  uint8_t intStatus;
  //Read Status
  intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
  bool flagA_full = (intStatus & 0x80) >> 7;

  /////// if there is 8 data in the FIFO ///////
  if (flagA_full) {
    int fifo_size = pulseOx1.device_data_read1();

#ifdef PDsLED
    getDataPDsLED();
#endif

#ifdef PDLEDs
    getDataPDLEDs();
#endif

#ifdef PDsLEDs
    getDataPDsLEDs();
#endif

#ifdef BleTest
    if ( Bluefruit.connected()) {
      if (start_stop_Sending == "send") {

        if (shutdown_or_restart == 1) { // the sensor was shutdown
          //Init PPG 86140 - 86141/
          samplesTaken = 0;
          configurePPG86();

          if (!errorPPG86) {
            updatePPG86();
          }
          shutdown_or_restart = 0;
        }

#ifdef PDsLED
        if ( ledSeq1A_PPG1Characteristic2.notify( pt_ledSeq1A_PD1_2, 12) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( ledSeq1A_PPG2Characteristic2.notify( pt_ledSeq1A_PD2_2, 12) ) {
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

#ifdef PDLEDs
        if ( ledSeq1A_PPG1Characteristic1.notify( pt_ledSeq1A_PD1_1, 20) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if (  SNR1_1PPG1Characteristic1.notify( SNR1_1, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
#endif

#ifdef PDsLEDs
        if ( ledSeq1A_PPG1Characteristic3.notify( pt_ledSeq1A_PD1_3, 12) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( ledSeq1A_PPG2Characteristic3.notify( pt_ledSeq1A_PD2_3, 12) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if (  SNR1_3PPG1Characteristic3.notify( SNR1_3, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if (  SNR2_3PPG2Characteristic3.notify( SNR2_3, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
#endif

      }
    }
    else {
      //Serial.println("No connected device");
      if ( start_stop_Sending == "stop" ) {
        Serial.println("Device disconnected, data not sent");

        //#ifdef PPG_Max86141
        //Shutdown PPG/
        pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
        //#endif

        shutdown_or_restart = 1;
        start_stop_Sending = "send";
      }
    }
#endif
  }
  Serial.println();
}

#ifdef PDsLEDs
void getDataPDsLEDs() {
  samplesTaken = samplesTaken + 2;

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
      pt_ledSeq1A_PD1_3[3] = (uint8_t)timestamp1;
      pt_ledSeq1A_PD1_3[2] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_3[1] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_3[0] = (uint8_t)(timestamp1 >>= 8);


      pt_ledSeq1A_PD1_3[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[0];
      pt_ledSeq1A_PD1_3[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_3[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_3[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);

      pt_ledSeq1A_PD1_3[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[1];
      pt_ledSeq1A_PD1_3[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_3[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_3[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);

      cpt1 += 2;
      if (cpt1 == SIZE) {
        //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          SNR1_3[3] = (uint8_t)a;
          SNR1_3[2] = (uint8_t)(a >>= 8);
          SNR1_3[1] = (uint8_t)(a >>= 8);
          SNR1_3[0] = (uint8_t)(a >>= 8);
        }
        else {
          SNR1_3[3] = (uint8_t)var;
          SNR1_3[2] = (uint8_t)(var >>= 8);
          SNR1_3[1] = (uint8_t)(var >>= 8);
          SNR1_3[0] = (uint8_t)(var >>= 8);
        }
        cpt1 = 0;
      }

    }

    if ((pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0)) {

      ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD2) ////////////
      uint32_t timestamp2 = millis();
      pt_ledSeq1A_PD2_3[3] = (uint8_t)timestamp2;
      pt_ledSeq1A_PD2_3[2] = (uint8_t)(timestamp2 >>= 8);
      pt_ledSeq1A_PD2_3[1] = (uint8_t)(timestamp2 >>= 8);
      pt_ledSeq1A_PD2_3[0] = (uint8_t)(timestamp2 >>= 8);

      pt_ledSeq1A_PD2_3[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[0];
      pt_ledSeq1A_PD2_3[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
      pt_ledSeq1A_PD2_3[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
      pt_ledSeq1A_PD2_3[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);

      pt_ledSeq1A_PD2_3[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[1];
      pt_ledSeq1A_PD2_3[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
      pt_ledSeq1A_PD2_3[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
      pt_ledSeq1A_PD2_3[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);

      cpt2 += 2;
      if (cpt2 == SIZE) {
        //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
        snr_pd2 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE));
        if (var < 0) {
          int a = -100 * var;
          SNR2_3[3] = (uint8_t)a;
          SNR2_3[2] = (uint8_t)(a >>= 8);
          SNR2_3[1] = (uint8_t)(a >>= 8);
          SNR2_3[0] = (uint8_t)(a >>= 8);
        }
        else {
          SNR2_3[3] = (uint8_t)var;
          SNR2_3[2] = (uint8_t)(var >>= 8);
          SNR2_3[1] = (uint8_t)(var >>= 8);
          SNR2_3[0] = (uint8_t)(var >>= 8);
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

#ifdef Sample_Rate
  Serial.print("Sample Rate : Hz[");
  Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.println();
#endif
}
#endif

#ifdef PDLEDs
void getDataPDLEDs() {

  samplesTaken = samplesTaken + 4;

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
  samplesTaken = samplesTaken + 4;
  Serial.println("----- PPG data ----- :");
  Serial.println("Reading all data from PD1: ");
  for (int i = 0; i < fifo_size / 2; i++) {
    Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
  }

  free(pulseOx1.tab_ledSeq1A_PD1);
#endif

#ifdef Sample_Rate
  Serial.print("Sample Rate : Hz[");
  Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.println();
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
      pt_ledSeq1A_PD1_1[3] = (uint8_t)timestamp1;
      pt_ledSeq1A_PD1_1[2] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_1[1] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_1[0] = (uint8_t)(timestamp1 >>= 8);


      pt_ledSeq1A_PD1_1[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[0];
      pt_ledSeq1A_PD1_1[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_1[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_1[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);

      pt_ledSeq1A_PD1_1[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[1];
      pt_ledSeq1A_PD1_1[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_1[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_1[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);

      pt_ledSeq1A_PD1_1[15] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[2];
      pt_ledSeq1A_PD1_1[14] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
      pt_ledSeq1A_PD1_1[13] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
      pt_ledSeq1A_PD1_1[12] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);

      pt_ledSeq1A_PD1_1[19] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[3];
      pt_ledSeq1A_PD1_1[18] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
      pt_ledSeq1A_PD1_1[17] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
      pt_ledSeq1A_PD1_1[16] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);

      cpt1 += 4;
      if (cpt1 == SIZE) {
        //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          SNR1_1[3] = (uint8_t)a;
          SNR1_1[2] = (uint8_t)(a >>= 8);
          SNR1_1[1] = (uint8_t)(a >>= 8);
          SNR1_1[0] = (uint8_t)(a >>= 8);
        }
        else {
          SNR1_1[3] = (uint8_t)var;
          SNR1_1[2] = (uint8_t)(var >>= 8);
          SNR1_1[1] = (uint8_t)(var >>= 8);
          SNR1_1[0] = (uint8_t)(var >>= 8);
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
  samplesTaken = samplesTaken + 2;
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

#ifdef Sample_Rate
  Serial.print("Sample Rate : Hz[");
  Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.println();
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
      pt_ledSeq1A_PD1_2[3] = (uint8_t)timestamp1;
      pt_ledSeq1A_PD1_2[2] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_2[1] = (uint8_t)(timestamp1 >>= 8);
      pt_ledSeq1A_PD1_2[0] = (uint8_t)(timestamp1 >>= 8);


      pt_ledSeq1A_PD1_2[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[0];
      pt_ledSeq1A_PD1_2[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_2[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
      pt_ledSeq1A_PD1_2[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);

      pt_ledSeq1A_PD1_2[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[1];
      pt_ledSeq1A_PD1_2[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_2[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
      pt_ledSeq1A_PD1_2[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);

      cpt1 += 2;
      if (cpt1 == SIZE) {
        //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
        snr_pd1 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
        if (var < 0) {
          int a = -100 * var;
          SNR1_2[3] = (uint8_t)a;
          SNR1_2[2] = (uint8_t)(a >>= 8);
          SNR1_2[1] = (uint8_t)(a >>= 8);
          SNR1_2[0] = (uint8_t)(a >>= 8);
        }
        else {
          SNR1_2[3] = (uint8_t)var;
          SNR1_2[2] = (uint8_t)(var >>= 8);
          SNR1_2[1] = (uint8_t)(var >>= 8);
          SNR1_2[0] = (uint8_t)(var >>= 8);
        }
        cpt1 = 0;
      }

    }

    if ((pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0)) {

      ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];

      ///////////// Pointer to send only 4 samples by Bluetooth (PD2) ////////////
      uint32_t timestamp2 = millis();
      pt_ledSeq1A_PD2_2[3] = (uint8_t)timestamp2;
      pt_ledSeq1A_PD2_2[2] = (uint8_t)(timestamp2 >>= 8);
      pt_ledSeq1A_PD2_2[1] = (uint8_t)(timestamp2 >>= 8);
      pt_ledSeq1A_PD2_2[0] = (uint8_t)(timestamp2 >>= 8);

      pt_ledSeq1A_PD2_2[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[0];
      pt_ledSeq1A_PD2_2[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
      pt_ledSeq1A_PD2_2[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
      pt_ledSeq1A_PD2_2[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);

      pt_ledSeq1A_PD2_2[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[1];
      pt_ledSeq1A_PD2_2[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
      pt_ledSeq1A_PD2_2[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
      pt_ledSeq1A_PD2_2[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);

      cpt2 += 2;
      if (cpt2 == SIZE) {
        //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
        snr_pd2 = pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE);
        int var = 0;
        var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE));
        if (var < 0) {
          int a = -100 * var;
          SNR2_2[3] = (uint8_t)a;
          SNR2_2[2] = (uint8_t)(a >>= 8);
          SNR2_2[1] = (uint8_t)(a >>= 8);
          SNR2_2[0] = (uint8_t)(a >>= 8);
        }
        else {
          SNR2_2[3] = (uint8_t)var;
          SNR2_2[2] = (uint8_t)(var >>= 8);
          SNR2_2[1] = (uint8_t)(var >>= 8);
          SNR2_2[0] = (uint8_t)(var >>= 8);
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

void testingSampleRatePPG() {

  long startTimePPG = micros();
  long samplesTakenPPG = 0;

  while (samplesTakenPPG < 125) {
    uint8_t intStatus;
    //Read Status
    intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
    bool flagA_full = (intStatus & 0x80) >> 7;

    /////// if there is 8 data in the FIFO ///////
    if (flagA_full) {
      int fifo_size = pulseOx1.device_data_read1();

#ifdef PDsLEDs
      samplesTakenPPG += 2;
      getDataPDsLEDs();
#endif
    }
    /*
      }

      long endTimePPG = micros();
      Serial.print("PPG samples[");
      Serial.print(samplesTakenPPG);
      Serial.print("]");
      Serial.println();
      Serial.print("PPG Sample Rate : Hz[");
      Serial.print((float)(samplesTakenPPG) / ((endTimePPG - startTimePPG) / 1000000.0), 2);
      Serial.print("]");
      Serial.println();
      Serial.print("Elapsed Time : Us[");
      Serial.print(endTimePPG - startTimePPG);
      Serial.print("]");
      Serial.println();
      Serial.println();
    */

#ifdef BleTest
    if ( Bluefruit.connected()) {
      if (start_stop_Sending == "send") {

        if (shutdown_or_restart == 1) { // the sensor was shutdown
          //Init PPG 86140 - 86141/
          samplesTaken = 0;
          configurePPG86();

          if (!errorPPG86) {
            updatePPG86();
          }
          shutdown_or_restart = 0;
        }
/*
        if ( ErrorCharacteristic.notify(bufError, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
*/
        if ( ledSeq1A_PPG1Characteristic3.notify( pt_ledSeq1A_PD1_3, 12) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( ledSeq1A_PPG2Characteristic3.notify( pt_ledSeq1A_PD2_3, 12) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if (  SNR1_3PPG1Characteristic3.notify( SNR1_3, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if (  SNR2_3PPG2Characteristic3.notify( SNR2_3, 4) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          // Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
      }
    }

    else {
      //Serial.println("No connected device");
      if ( start_stop_Sending == "stop" ) {
        Serial.println("Device disconnected, data not sent");

        //#ifdef PPG_Max86141
        //Shutdown PPG/
        pulseOx1.write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
        //#endif

        shutdown_or_restart = 1;
        start_stop_Sending = "send";
      }
    }
#endif
  }

  long endTimePPG = micros();
  Serial.print("PPG samples avec BLE[");
  Serial.print(samplesTakenPPG);
  Serial.print("]");
  Serial.println();
  Serial.print("PPG Sample Rate : Hz[");
  Serial.print((float)(samplesTakenPPG) / ((endTimePPG - startTimePPG) / 1000000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.print("Elapsed Time : Us[");
  Serial.print(endTimePPG - startTimePPG);
  Serial.print("]");
  Serial.println();
  Serial.println();

}
