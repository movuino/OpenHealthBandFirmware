#include <MAX86141.h>
#include <SPI.h>


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



///////////// Pointers for PDLEDs use to send 1 sample by Bluetooth /////////////
uint8_t* pt_ledSeq1A_PD1_1, *pt_tagSeq1A_PD1_1, *pt_ledSeq1B_PD1_1, *pt_tagSeq1B_PD1_1, *SNR1_1;

//////////// Pointers for PDsLED use to send 1 sample by Bluetooth /////////////
uint8_t pt_ledSeq1A_PD1_2[8];
uint8_t *pt_tagSeq1A_PD1_2, *pt_ledSeq1B_PD1_2, *pt_tagSeq1B_PD1_2;
uint8_t pt_ledSeq1A_PD2_2[4];
uint8_t *pt_tagSeq1A_PD2_2, *pt_ledSeq1B_PD2_2, *pt_tagSeq1B_PD2_2;
uint8_t SNR1_2[4], SNR2_2[4];

//////////// Pointers for PDsLED use to send 4 samples by Bluetooth /////////////
uint8_t ptledSeq1APD1[16]; // 4 samples * 4 bytes = 16
uint8_t ptledSeq1APD2[16]; // 4 samples * 4 bytes = 16

/* Sensor Characteristics */
//PD: PhotoDiode
# define PDsLED; // 2 PDs - 1 LED
//# define PDLEDs; // 1 PD - 2 LEDs
//# define PDsLEDs; // 2 PDs - 3 LEDs

/* Pin Definitions  */
// #define MISO_PIN              19
// #define MOSI_PIN              18
// #define SCK_PIN               5
//#define SS_PIN                19 //(Adafruit)
#define SS_PIN                10 //(Movuino)

/* Tests */
//# define SerialTest
#define BleTest

/* Global Variables */
static int spiClk = 1000000; // 8 MHz Maximum
int cpt1 = 0, cpt2 = 0;
int interruptPin = 36;
bool errorPPG86 = false;
bool dataReady = false;


MAX86141 pulseOx1;

void configurePPG86(void) {
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
  int LedMode[] = {1/*LED1 (Sequence 1, 0-3)*/};
  //pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 31/*intensity_LEDs*/, 0x0/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 10/*intensity_LEDs*/, 0x2/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif
#endif

  ///////////////////// Bluetooth Communication //////////////////
#ifdef BleTest
#ifdef PDsLED
  //int LedMode[] = {1/*LED1 (Sequence 1, 0-3)*/, 9/*DIRECT AMBIENT (Sequence 1, 4-7)*/};
  int LedMode[] = {1/*LED1 (Sequence 1, 0-3)*/};
  /*Test avec Web BLE*/
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0xE/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDLEDs
  int LedMode[] = {4/*LED1 and LED2 simultaneously (Sequence 1, 0-3)*/, 9/*DIRECT AMBIENT (Sequence 1, 4-7)*/};
  pulseOx1.initialisation(1/*nb_pds*/, LedMode/*LedMode*/, 2/*Number of sequences*/, 2/*Number of LEDs*/, 15/*intensity_LEDs*/, 1/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x3/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDsLEDs
  int LedMode[] = {1/*LED1 (Sequence 1, 0-3)*/, 2/*LED2 (Sequence 1, 4-7)*/, 5/*LED2 and LED3 simultaneously (Sequence 2, 0-3)*/, 9/*DIRECT AMBIENT (Sequence 2, 4-7)*/};
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 4/*Number of sequences*/, 3/*Number of LEDs*/, 0x4/*intensity_LEDs*/, 8/*sample_average*/, 0x0E/*sample_rate*/, 3/*pulse width*/, 0x3/*ADC Range= 16uA*/, spiClk);
#endif
#endif

  Serial.println("--Read Register-- System Control");
  Serial.println(pulseOx1.read_reg(0x0D));

  Serial.println("--Read Register-- PART_ID");
  Serial.println(pulseOx1.read_reg(0xFF));

  Serial.println("--Read Temp-- 0x40");
  pulseOx1.write_reg(0x40, 0xFF);

  Serial.println(pulseOx1.read_reg(0x41));
  delay(1000);

  pulseOx1.setDebug(false);
}


void updatePPG86(void) {

  uint8_t intStatus;
  //Read Status
  intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
  bool flagA_full = (intStatus & 0x80) >> 7;

  /////// if there is 8 data in the FIFO ///////
  if (flagA_full) {
    int fifo_size = pulseOx1.device_data_read1();

    //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef PDsLED
    //Serial.println("Reading all data from PD1: ");
    for (int i = 0; i < fifo_size / 2; i++) {
      Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
    }
    /*Serial.println("Reading all data from PD2: ");
      for(int i=0; i<fifo_size/2;i++){
      Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
      }*/

    free(pulseOx1.tab_ledSeq1A_PD1);
    free(pulseOx1.tab_ledSeq1A_PD2);
    free(pulseOx1.tab_ledSeq1B_PD1);
    free(pulseOx1.tab_ledSeq1B_PD2);
#endif
#endif

    //---------------------------- Bluetooth Communication ----------------------------------//
#ifdef BleTest

#ifdef PDsLED
    int ledSeq1A_PD1 = pulseOx1.get_ledSeq1A_PD1();
    //Serial.println(ledSeq1A_PD1);

    for (int i = 0; i < fifo_size / 2; i++) {
      if (pulseOx1.tab_ledSeq1A_PD1[i] != 0) {
        Serial.println(pulseOx1.tab_ledSeq1A_PD1[i]);
      }
    }
    //Serial.println("-----------------------");

    for (int i = 0; i < fifo_size / 2; i++) {
      if (pulseOx1.tab_ledSeq1A_PD2[i] != 0) {
        //Serial.println(pulseOx1.tab_ledSeq1A_PD2[i]);
      }
    }
    //Serial.println("##########################");


    ///////////// Add in buffer for SNR //////////
    //pulseOx1.signalData_ledSeq1A_PD1[cpt1] = ledSeq1A_PD1;
    if ( (pulseOx1.tab_ledSeq1A_PD1[0] != 0) && (pulseOx1.tab_ledSeq1A_PD1[1] != 0) && (pulseOx1.tab_ledSeq1A_PD1[2] != 0) && (pulseOx1.tab_ledSeq1A_PD1[3] != 0))
    { pulseOx1.signalData_ledSeq1A_PD1[cpt1] = pulseOx1.tab_ledSeq1A_PD1[0];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = pulseOx1.tab_ledSeq1A_PD1[1];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 2] = pulseOx1.tab_ledSeq1A_PD1[2];
      pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 3] = pulseOx1.tab_ledSeq1A_PD1[3];
    }

    uint32_t timestamp = millis();

    ///////////// Pointer to send only 1 sample by Bluetooth (PD1) ////////////
   
    pt_ledSeq1A_PD1_2[3] = (uint8_t)timestamp;
    pt_ledSeq1A_PD1_2[2] = (uint8_t)(timestamp >>= 8);
    pt_ledSeq1A_PD1_2[1] = (uint8_t)(timestamp >>= 8);
    pt_ledSeq1A_PD1_2[0] = (uint8_t)(timestamp >>= 8);

    pt_ledSeq1A_PD1_2[7] = (uint8_t)ledSeq1A_PD1;
    pt_ledSeq1A_PD1_2[6] = (uint8_t)(ledSeq1A_PD1 >>= 8);
    pt_ledSeq1A_PD1_2[5] = (uint8_t)(ledSeq1A_PD1 >>= 8);
    pt_ledSeq1A_PD1_2[4] = (uint8_t)(ledSeq1A_PD1 >>= 8);

    ///////////// Pointer to send only 4 samples by Bluetooth (PD1) ////////////
    
    ptledSeq1APD1[3] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[0];
    ptledSeq1APD1[2] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
    ptledSeq1APD1[1] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
    ptledSeq1APD1[0] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);

    ptledSeq1APD1[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[1];
    ptledSeq1APD1[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
    ptledSeq1APD1[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
    ptledSeq1APD1[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);

    ptledSeq1APD1[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[2];
    ptledSeq1APD1[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
    ptledSeq1APD1[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);
    ptledSeq1APD1[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[2] >>= 8);

    ptledSeq1APD1[15] = (uint8_t)pulseOx1.tab_ledSeq1A_PD1[3];
    ptledSeq1APD1[14] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
    ptledSeq1APD1[13] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);
    ptledSeq1APD1[12] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD1[3] >>= 8);

    cpt1 += 4;
    if (cpt1 == SIZE) {
      //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
      int var = 0;
      var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE));
      if (var < 0) {
        int a = -100 * var;
        // Serial.println("SNR2 : " + String(a));
        //SNR1_2 = (uint8_t*)&a;
        SNR1_2[3] = (uint8_t)a;
        SNR1_2[2] = (uint8_t)(a >>= 8);
        SNR1_2[1] = (uint8_t)(a >>= 8);
        SNR1_2[0] = (uint8_t)(a >>= 8);
      }
      else {
        //Serial.println("SNR2 : " + String(var));
        //SNR1_2 = (uint8_t*)&var;
        SNR1_2[3] = (uint8_t)var;
        SNR1_2[2] = (uint8_t)(var >>= 8);
        SNR1_2[1] = (uint8_t)(var >>= 8);
        SNR1_2[0] = (uint8_t)(var >>= 8);
      }
      cpt1 = 0;
    }

    int ledSeq1A_PD2 = pulseOx1.get_ledSeq1A_PD2();
    //Serial.println(ledSeq1A_PD2);

    ///////////// Add in buffer for SNR //////////
    //pulseOx1.signalData_ledSeq1A_PD2[cpt2] = ledSeq1A_PD2;
    if ( (pulseOx1.tab_ledSeq1A_PD2[0] != 0) && (pulseOx1.tab_ledSeq1A_PD2[1] != 0) && (pulseOx1.tab_ledSeq1A_PD2[2] != 0) && (pulseOx1.tab_ledSeq1A_PD2[3] != 0))
    { pulseOx1.signalData_ledSeq1A_PD2[cpt2] = pulseOx1.tab_ledSeq1A_PD2[0];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 1] = pulseOx1.tab_ledSeq1A_PD2[1];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 2] = pulseOx1.tab_ledSeq1A_PD2[2];
      pulseOx1.signalData_ledSeq1A_PD2[cpt2 + 3] = pulseOx1.tab_ledSeq1A_PD2[3];
    }


    ///////////// Pointer to send only 1 sample by Bluetooth (PD2) ////////////
    pt_ledSeq1A_PD2_2[3] = (uint8_t)ledSeq1A_PD2;
    pt_ledSeq1A_PD2_2[2] = (uint8_t)(ledSeq1A_PD2 >>= 8);
    pt_ledSeq1A_PD2_2[1] = (uint8_t)(ledSeq1A_PD2 >>= 8);
    pt_ledSeq1A_PD2_2[0] = (uint8_t)(ledSeq1A_PD2 >>= 8);

    ///////////// Pointer to send only 4 samples by Bluetooth (PD2) ////////////
    ptledSeq1APD2[3] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[0];
    ptledSeq1APD2[2] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
    ptledSeq1APD2[1] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
    ptledSeq1APD2[0] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);

    ptledSeq1APD2[7] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[1];
    ptledSeq1APD2[6] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
    ptledSeq1APD2[5] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
    ptledSeq1APD2[4] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);

    ptledSeq1APD2[11] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[2];
    ptledSeq1APD2[10] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);
    ptledSeq1APD2[9] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);
    ptledSeq1APD2[8] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[2] >>= 8);

    ptledSeq1APD2[15] = (uint8_t)pulseOx1.tab_ledSeq1A_PD2[3];
    ptledSeq1APD2[14] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);
    ptledSeq1APD2[13] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);
    ptledSeq1APD2[12] = (uint8_t)(pulseOx1.tab_ledSeq1A_PD2[3] >>= 8);


    cpt2 += 4;
    if (cpt2 == SIZE) {
      //Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
      int var = 0;
      var = 100 * (pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE));
      if (var < 0) {
        int a = -100 * var;
        //Serial.println("SNR2 : "+String(a));
        //SNR2_2 = (uint8_t*)&a;
        SNR2_2[3] = (uint8_t)a;
        SNR2_2[2] = (uint8_t)(a >>= 8);
        SNR2_2[1] = (uint8_t)(a >>= 8);
        SNR2_2[0] = (uint8_t)(a >>= 8);
      }
      else {
        //Serial.println("SNR2 : "+String(var));
        //SNR2_2 = (uint8_t*)&var;
        SNR2_2[3] = (uint8_t)var;
        SNR2_2[2] = (uint8_t)(var >>= 8);
        SNR2_2[1] = (uint8_t)(var >>= 8);
        SNR2_2[0] = (uint8_t)(var >>= 8);
      }
      cpt2 = 0;
    }

    free(pulseOx1.tab_ledSeq1A_PD1);
    free(pulseOx1.tab_ledSeq1A_PD2);
    free(pulseOx1.tab_ledSeq1B_PD1);
    free(pulseOx1.tab_ledSeq1B_PD2);
    /*Serial.println("LED 1 Seq 1 A - PD1 channel : ");
      Serial.println(ledSeq1A_PD1);
      Serial.println("TAG of LED 1 Seq 1 A - PD1 channel : ");
      int tagSeq1A_PD1 = pulseOx1.get_tagSeq1A_PD1();
      Serial.println(tagSeq1A_PD1);
      Serial.println("LED 1 Seq 1 A - PD2 channel : ");
      Serial.println(ledSeq1A_PD2);
      Serial.println("TAG of LED 1 Seq 1 A - PD2  channel : ");
      int tagSeq1A_PD2 = pulseOx1.get_tagSeq1A_PD2();
      Serial.println(tagSeq1A_PD2);
      Serial.println("LED AVERAGE");
      int led_avg = (ledSeq1A_PD1 + ledSeq1A_PD2) * 0.5;
      Serial.println(led_avg);*/
#endif

#ifdef PDLEDs

    Serial.println("LED 1 & LED 2 Seq 1 A - PD1 channel : ");
    int ledSeq1A_PD1 = pulseOx1.get_ledSeq1A_PD1();
    Serial.println(ledSeq1A_PD1);
    Serial.println("TAG of LED 1 & LED 2 Seq 1 A - PD1 channel : ");
    int tagSeq1A_PD1 = pulseOx1.get_tagSeq1A_PD1();
    Serial.println(tagSeq1A_PD1);
    Serial.println("Direct ambient - PD1 channel : ");
    int ledSeq1B_PD1 = pulseOx1.get_ledSeq1B_PD1();
    Serial.println(ledSeq1B_PD1);
    Serial.println("TAG Direct ambient - PD1 channel : ");
    int tagSeq1B_PD1 = pulseOx1.get_tagSeq1B_PD1();
    Serial.println(tagSeq1B_PD1);
#endif

#ifdef PDsLEDs

    Serial.println("LED 1 Seq 1 A - PD1 channel : ");
    int ledSeq1A_PD1 = pulseOx1.get_ledSeq1A_PD1();
    Serial.println(ledSeq1A_PD1);
    Serial.println("TAG Seq 1 A - PD1 channel : ");
    int tagSeq1A_PD1 = pulseOx1.get_tagSeq1A_PD1();
    Serial.println(tagSeq1A_PD1);
    Serial.println("LED 1 Seq 1 A - PD2 channel : ");
    int ledSeq1A_PD2 = pulseOx1.get_ledSeq1A_PD2();
    Serial.println(ledSeq1A_PD2);
    Serial.println("TAG Seq 1 A - PD2  channel : ");
    int tagSeq1A_PD2 = pulseOx1.get_tagSeq1A_PD2();
    Serial.println(tagSeq1A_PD2);

    Serial.println("LED 2 Seq 1 B - PD1 channel : ");
    int ledSeq1B_PD1 = pulseOx1.get_ledSeq1B_PD1();
    Serial.println(ledSeq1B_PD1);
    Serial.println("TAG of LED 2 Seq 1 B - PD1 channel : ");
    int tagSeq1B_PD1 = pulseOx1.get_tagSeq1B_PD1();
    Serial.println(tagSeq1B_PD1);
    Serial.println("LED 2 Seq 1 B - PD2 channel : ");
    int ledSeq1B_PD2 = pulseOx1.get_ledSeq1B_PD2();
    Serial.println(ledSeq1B_PD2);
    Serial.println("TAG of LED 2 Seq 1 B - PD2  channel : ");
    int tagSeq1B_PD2 = pulseOx1.get_tagSeq1A_PD2();
    Serial.println(tagSeq1A_PD2);

    Serial.println(" LED 3 Seq 2 A - PD1 channel : ");
    int ledSeq2A_PD1 = pulseOx1.get_ledSeq2A_PD1();
    Serial.println(ledSeq2A_PD1);
    Serial.println("TAG of LED 3 Seq 2 A - PD1 channel : ");
    int tagSeq2A_PD1 = pulseOx1.get_tagSeq2A_PD1();
    Serial.println(tagSeq2A_PD1);
    Serial.println(" LED 3 Seq 2 A - PD2 channel : ");
    int ledSeq2A_PD2 = pulseOx1.get_ledSeq2A_PD2();
    Serial.println(ledSeq2A_PD2);
    Serial.println("TAG of LED 3 Seq 2 A - PD2  channel : ");
    int tagSeq2A_PD2 = pulseOx1.get_tagSeq2A_PD2();
    Serial.println(tagSeq2A_PD2);

    Serial.println("Direct ambient Seq 2 B - PD1 channel : ");
    int ledSeq2B_PD1 = pulseOx1.get_ledSeq2B_PD1();
    Serial.println(ledSeq2B_PD1);
    Serial.println("TAG Direct ambient Seq 2 B - PPG1 channel : ");
    int tagSeq2B_PD1 = pulseOx1.get_tagSeq2B_PD1();
    Serial.println(tagSeq2B_PD1);
    Serial.println("Direct ambient Seq 2 B - PD2 channel : ");
    int ledSeq2B_PD2 = pulseOx1.get_ledSeq2B_PD2();
    Serial.println(ledSeq2B_PD2);
    Serial.println("TAG Direct ambient Seq 2 B - PD2 channel : ");
    int tagSeq2B_PD2 = pulseOx1.get_tagSeq2B_PD2();
    Serial.println(tagSeq2B_PD2);

    Serial.println("LED 1 AVERAGE");
    int led1_avg = (ledSeq1A_PD1 + ledSeq1A_PD2) * 0.5;
    Serial.println(led1_avg);
    Serial.println("LED 2 AVERAGE");
    int led2_avg = (ledSeq1B_PD1 + ledSeq1B_PD2) * 0.5;
    Serial.println(led2_avg);
    Serial.println("LED 3 AVERAGE");
    int led_avg = (ledSeq2A_PD1 + ledSeq2A_PD2) * 0.5;
    Serial.println(led_avg);
    Serial.println("Direct ambient AVERAGE");
    int ambient_avg = (ledSeq2B_PD1 + ledSeq2B_PD2) * 0.5;
    Serial.println(ambient_avg);

#endif
    dataReady = true;
#endif
  }
}
