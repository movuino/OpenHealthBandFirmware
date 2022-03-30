
/*
     Intensity LED  : 0 = 0mA | 255 = 124mA (max)
     PGG Sample Rate : 0x00 = 24.995 samples per second | 0x13 = 4096 samples per second (max)
     Sample Average : 2, 4, 8, 16, 32, 64, 128 samples (max)
     Led Sequence Control : LED1 (1) // GREEN LED, LED2 (2) //IR LED, LED3 (3) //RED LED, LED1 and LED2 pulsed simultaneously (4),
   LED1 and LED3 pulsed simultaneously (5), LED2 and LED3 pulsed simultaneously (6), LED1, LED2 and LED3 pulsed simultaneously (7),
   Pilot on LED1 (8), DIRECT AMBIENT (9), LED4 [external mux control] (10), LED5 [external mux control] (11), LED6 [external mux control] (12)
     DIRECT AMBIENT (i.e. normal photodiode measurements)
     Sequence Control is up to the configuration you wish (page 14-15 datasheet)
     PD: PhotoDiode
*/

#include <MAX86141.h>
#include <SPI.h>


//////////// Pointers for PDsLED use to send 4 samples by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_2[20];
uint8_t pt_ledSeq1A_PD2_2[20];
uint8_t SNR1_2[4], SNR2_2[4];

/* Sensor Characteristics */
/* 2 PDs - 1 LED */
#define PDsLED

/* 1 PD - 2 LEDs */
//#define PDLEDs

/* 2 PDs - 3 LEDs */
//#define PDsLEDs

/* Sample Rate taken */
#define Sample_Rate

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

#ifdef PDsLED
int LedMode[1] = {1/*LED1 (Sequence 1, 0-3)*/};
#endif

#ifdef PDLEDs
int LedMode[2] = {4/*LED1 and LED2 simultaneously (Sequence 1, 0-3)*/, 9/*DIRECT AMBIENT (Sequence 1, 4-7)*/};
#endif

#ifdef PDsLEDs
int LedMode[4] = {1/*LED1 (Sequence 1, 0-3)*/, 2/*LED2 (Sequence 1, 4-7)*/, 5/*LED1 and LED3 simultaneously (Sequence 2, 0-3)*/, 9/*DIRECT AMBIENT (Sequence 2, 4-7)*/};
#endif

#ifdef SerialTest
int LedMode_s[1] = {1/*LED1 (Sequence 1, 0-3)*/};
#endif

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

  ///////////////////// Serial Communication //////////////////
#ifdef SerialTest
#ifdef PDsLED
  pulseOx1.initialisation(2/*nb_pds*/, LedMode_s/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 31/*intensity_LEDs*/, 0x0/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif
#endif

  ///////////////////// Bluetooth Communication //////////////////
#ifdef BleTest
#ifdef PDsLED
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0xE/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDLEDs
  pulseOx1.initialisation(1/*nb_pds*/, LedMode/*LedMode*/, 2/*Number of sequences*/, 2/*Number of LEDs*/, 15/*intensity_LEDs*/, 1/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x3/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDsLEDs
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 4/*Number of sequences*/, 3/*Number of LEDs*/, 0x4/*intensity_LEDs*/, 8/*sample_average*/, 0x0E/*sample_rate*/, 3/*pulse width*/, 0x3/*ADC Range= 16uA*/, spiClk);
#endif
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
    samplesTaken = samplesTaken + 4;
    int fifo_size = pulseOx1.device_data_read1();

    //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef PDsLED
    Serial.println("----- PPG data ----- :");
    Serial.println("Reading all data from PD1: ");
    for (int i = 0; i < fifo_size / 2; i++) {
      Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
    }

    Serial.println("Reading all data from PD2: ");
    for (int i = 0; i < fifo_size / 2; i++) {
      Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
    }

    free(pulseOx1.tab_ledSeq1A_PD1);
    free(pulseOx1.tab_ledSeq1A_PD2);
    free(pulseOx1.tab_ledSeq1B_PD1);
    free(pulseOx1.tab_ledSeq1B_PD2);
#endif
#endif

    //---------------------------- Bluetooth Communication ----------------------------------//
#ifdef BleTest

#ifdef PDsLED
    Serial.println("----- PPG data ----- :");
    for (int i = 0; i < fifo_size / 2; i++) {
      Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
    }

    for (int i = 0; i < fifo_size / 2; i++) {
      Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
    }

#ifdef Sample_Rate
    Serial.print("Sample Rate : Hz[");
    Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
    Serial.print("]");
    Serial.println();
    Serial.println();
#endif


    if ((pulseOx1.tab_ledSeq1A_PD1[0] != 0) && (pulseOx1.tab_ledSeq1A_PD1[1] != 0) && (pulseOx1.tab_ledSeq1A_PD1[2] != 0)
        && (pulseOx1.tab_ledSeq1A_PD1[3] != 0)) {

      ///////////// Addition data of PD1 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD1[cpt1] = pulseOx1.tab_ledSeq1A_PD1[0];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = pulseOx1.tab_ledSeq1A_PD1[1];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 2] = pulseOx1.tab_ledSeq1A_PD1[2];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 3] = pulseOx1.tab_ledSeq1A_PD1[3];

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

      pt_ledSeq1A_PD1_2[15] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[2];
      pt_ledSeq1A_PD1_2[14] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
      pt_ledSeq1A_PD1_2[13] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
      pt_ledSeq1A_PD1_2[12] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);

      pt_ledSeq1A_PD1_2[19] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[3];
      pt_ledSeq1A_PD1_2[18] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
      pt_ledSeq1A_PD1_2[17] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
      pt_ledSeq1A_PD1_2[16] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);

      cpt1 += 4;
      if (cpt1 == SIZE) {
        Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
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


    if ((pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0) && (pulseOx1.tab_ledSeq1A_PD2[2] != 0)
        && (pulseOx1.tab_ledSeq1A_PD2[3] != 0)) {

      ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
      pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 2] = pulseOx1.tab_ledSeq1A_PD2[2];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 3] = pulseOx1.tab_ledSeq1A_PD2[3];

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

      pt_ledSeq1A_PD2_2[15] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[2];
      pt_ledSeq1A_PD2_2[14] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);
      pt_ledSeq1A_PD2_2[13] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);
      pt_ledSeq1A_PD2_2[12] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);

      pt_ledSeq1A_PD2_2[19] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[3];
      pt_ledSeq1A_PD2_2[18] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);
      pt_ledSeq1A_PD2_2[17] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);
      pt_ledSeq1A_PD2_2[16] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);


      cpt2 += 4;
      if (cpt2 == SIZE) {
        Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
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

    free(pulseOx1.tab_ledSeq1A_PD1);
    free(pulseOx1.tab_ledSeq1A_PD2);
    free(pulseOx1.tab_ledSeq1B_PD1);
    free(pulseOx1.tab_ledSeq1B_PD2);
#endif

#endif
  }
  Serial.println();

}
