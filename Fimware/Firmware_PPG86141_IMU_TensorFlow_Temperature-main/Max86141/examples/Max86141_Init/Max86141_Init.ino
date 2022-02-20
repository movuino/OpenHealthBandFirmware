
/*
   * Intensity LED  : 0 = 0mA | 255 = 124mA (max)
   * PGG Sample Rate : 0x00 = 24.995 samples per second | 0x13 = 4096 samples per second (max)
   * Sample Average : 2, 4, 8, 16, 32, 64, 128 samples (max)
   * Led Sequence Control : LED1 (1) // GREEN LED, LED2 (2) //IR LED, LED3 (3) //RED LED, LED1 and LED2 pulsed simultaneously (4),
   LED1 and LED3 pulsed simultaneously (5), LED2 and LED3 pulsed simultaneously (6), LED1, LED2 and LED3 pulsed simultaneously (7),
   Pilot on LED1 (8), DIRECT AMBIENT (9), LED4 [external mux control] (10), LED5 [external mux control] (11), LED6 [external mux control] (12)
   * DIRECT AMBIENT (i.e. normal photodiode measurements)
   * Sequence Control is up to the configuration you wish (page 14-15 datasheet)
   * PD: PhotoDiode
*/

#include <MAX86141.h>
#include <SPI.h>

/* 2 PDs - 1 LED */
#define PDsLED

/* 1 PD - 2 LEDs */
//#define PDLEDs

/* 2 PDs - 3 LEDs */
//#define PDsLEDs

/* Sample Rate taken */
#define Sample_Rate

static int spiClk = 1000000; // 8 MHz Maximum

/* Pin Definitions */
//#define MISO_PIN              19
//#define MOSI_PIN              18
//#define SCK_PIN               5
//#define SS_PIN                19 //(Adafruit)
#define SS_PIN                10 //(Movuino)

MAX86141 pulseOx1;

int cpt1 = 0, cpt2 = 0;

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

void setup() {

  Serial.begin(115200);
  while ( !Serial ) delay(10);

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

#ifdef PDsLED
  pulseOx1.initialisation(2/*nb_pds*/, LedMode/*LedMode*/, 1/*Number of sequences*/, 1/*Number of LEDs*/, 10/*intensity_LEDs*/, 0x00/*sample_average*/, 0xE/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDLEDs
  pulseOx1.initialisation(1/*nb_pds*/, LedMode/*LedMode*/, 2/*Number of sequences*/, 2/*Number of LEDs*/, 10/*intensity_LEDs*/, 1/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x3/*ADC Range= 16uA*/, spiClk);
#endif

#ifdef PDsLEDs
  pulseOx1.initialisation(2/*nb_ppg*/, LedMode/*LedMode*/, 4/*Number of sequences*/, 3/*Number of LEDs*/, 4/*intensity_LEDs*/, 0x3/*sample_average*/, 0x0E/*sample_rate*/, 0x3/*pulse width*/, 0x2/*ADC Range= 16uA*/, spiClk);
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
  }
  else if (part_ID == 37) {
    Serial.println("MAX86141 connection succeeded !");
  }
  else {
    Serial.println("Connection failed !");
  }

  delay(1000);

  pulseOx1.setDebug(false);

  startTime = millis();
}


void loop() {

  uint8_t intStatus;
  //Read Status
  intStatus = pulseOx1.read_reg(REG_INT_STAT_1);
  bool flagA_full = (intStatus & 0x80) >> 7;

  /////// if there is 8 samples in the FIFO ///////
  if (flagA_full) {
    samplesTaken += 4;
    int fifo_size = pulseOx1.device_data_read1();

#ifdef PDsLED
    Serial.println("LED 1 Seq 1 A - PD1 channel : ");
    int ledSeq1A_PD1 = pulseOx1.get_ledSeq1A_PD1();
    Serial.println(ledSeq1A_PD1);
    Serial.println("TAG of LED 1 Seq 1 A - PD1 channel : ");
    int tagSeq1A_PD1 = pulseOx1.get_tagSeq1A_PD1();
    Serial.println(tagSeq1A_PD1);
    Serial.println("LED 1 Seq 1 A - PD2 channel : ");
    int ledSeq1A_PD2 = pulseOx1.get_ledSeq1A_PD2();
    Serial.println(ledSeq1A_PD2);
    Serial.println("TAG of LED 1 Seq 1 A - PD2  channel : ");
    int tagSeq1A_PD2 = pulseOx1.get_tagSeq1A_PD2();
    Serial.println(tagSeq1A_PD2);
    Serial.println("LED AVERAGE");
    int led_avg = (ledSeq1A_PD1 + ledSeq1A_PD2) * 0.5;
    Serial.println(led_avg);
    
  ///////////// Addition data of PD1 in buffer to measure SNR (Signal Noise Ratio) //////////
    pulseOx1.signalData_ledSeq1A_PD1[cpt1] = ledSeq1A_PD1;
    cpt1 += 1;
    if (cpt1 == SIZE) {
      Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD1, SIZE)));
      cpt1 = 0;
    }

  ///////////// Addition data of PD2 in buffer to measure SNR (Signal Noise Ratio) //////////
    pulseOx1.signalData_ledSeq1A_PD2[cpt2] = ledSeq1A_PD2;
    cpt2 += 1;
    if (cpt2 == SIZE) {
      Serial.println("SNR (dB): " + String(pulseOx1.signaltonoise(pulseOx1.signalData_ledSeq1A_PD2, SIZE)));
      cpt2 = 0;
    }


#ifdef Sample_Rate
    Serial.print("Sample Rate : Hz[");
    Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
    Serial.print("]");
    Serial.println();
    Serial.println();
#endif


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
  }
  else {
    //Serial.println("FIFO not A_full");
  }
}
