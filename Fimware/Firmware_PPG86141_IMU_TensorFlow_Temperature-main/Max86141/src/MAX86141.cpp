#include "MAX86141.h"

        /*write to register function*/
void MAX86141::write_reg(uint8_t address, uint8_t data_in) {

      /**< A buffer with data to transfer. */
      m_tx_buf[0] = address;  //Target Register
      m_tx_buf[1] = WRITE_EN; //Set Write mode
      m_tx_buf[2] = data_in;  //Byte to Write

      if(debug == true) {
         /* Serial.print("W_TX ");
          Serial.print(m_tx_buf[0], HEX);
          Serial.print("|");
          Serial.print(m_tx_buf[1], HEX);
          Serial.print("|");
          Serial.println(m_tx_buf[2], HEX);*/
      }

      /**< A buffer for incoming data. */

      m_rx_buf[0] = 0;
      m_rx_buf[1] = 0;
      m_rx_buf[2] = 0;

      // digitalWrite(SS, HIGH);
      spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
      digitalWrite(SS, LOW);

      m_rx_buf[0] = spi->transfer(address);
      m_rx_buf[1] = spi->transfer(WRITE_EN);
      m_rx_buf[2] = spi->transfer(data_in);

      digitalWrite(SS, HIGH);
      spi->endTransaction();

      if(debug == true) {
          /*Serial.print("W_RX ");
          Serial.print(m_rx_buf[0], HEX);
          Serial.print("|");
          Serial.print(m_rx_buf[1], HEX);
          Serial.print("|");
          Serial.println(m_rx_buf[2], HEX);*/
      }
          }

/*read register function*/
uint8_t MAX86141::read_reg(uint8_t address) {

  /**< A buffer with data to transfer. */
  m_tx_buf[0] = address;  //Target Address
  m_tx_buf[1] = READ_EN;  //Set Read mode
  m_tx_buf[2] = 0x00;     //

  /**< A buffer for incoming data. */
  m_rx_buf[0] = 0;
  m_rx_buf[1] = 0;
  m_rx_buf[2] = 0;

  // if(debug == true) {
  //     Serial.print("R_TX ");
  //     Serial.print(m_tx_buf[0], HEX);
  //     Serial.print("|");
  //     Serial.print(m_tx_buf[1], HEX);
  //     Serial.print("|");
  //     Serial.println(m_tx_buf[2], HEX);
  // }

  // digitalWrite(SS, HIGH);
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  digitalWrite(SS, LOW);

  spi->transfer(m_tx_buf,3);

  digitalWrite(SS, HIGH);

  spi->endTransaction();

  m_rx_buf[0] = m_tx_buf[0];
  m_rx_buf[1] = m_tx_buf[1];
  m_rx_buf[2] = m_tx_buf[2];

  if(debug == true){
     /* Serial.print("R_RX ");
      Serial.print(m_rx_buf[0], HEX);
      Serial.print("|");
      Serial.print(m_rx_buf[1], HEX);
      Serial.print("|");
      Serial.println(m_rx_buf[2], HEX);*/
  }

  return m_rx_buf[2];
}

//////////////// GETs ////////////////////
int MAX86141::getledModeSize(){
  return ledModeSize;
}
int MAX86141::getNbPD(){
  return nb_pd;
}
int* MAX86141::getLedMode(){
  return ledMode;
}
int MAX86141::getNbLeds(){
  return number_leds;
}
int MAX86141::getIntensityLed(){
  return intensity_led;
}
int MAX86141::getSample_average(){
  return sample_average;
}
int MAX86141::getSample_rate(){
  return sample_rate;
}
int MAX86141::getPulse_width(){
  return pulse_width;
}
int MAX86141::getADCRange(){
  return adcRange;
}

int MAX86141::get_ledSeq1A_PD1(){
  return ledSeq1A_PD1;
}
int MAX86141::get_tagSeq1A_PD1(){
  return tagSeq1A_PD1;
}

int MAX86141::get_ledSeq1B_PD1(){
 return ledSeq1B_PD1;
 }
 int MAX86141::get_tagSeq1B_PD1(){
  return tagSeq1B_PD1;
}
int MAX86141::get_ledSeq2A_PD1(){
  return ledSeq2A_PD1;
}
int MAX86141::get_tagSeq2A_PD1(){
  return tagSeq2A_PD1;
}
int MAX86141::get_ledSeq2B_PD1(){
  return ledSeq2B_PD1;
} 
int MAX86141::get_tagSeq2B_PD1(){
  return tagSeq2B_PD1;
}
int MAX86141::get_ledSeq3A_PD1(){
  return ledSeq3A_PD1;
}
int MAX86141::get_tagSeq3A_PD1(){
  return tagSeq3A_PD1;
}
int MAX86141::get_ledSeq3B_PD1(){
  return ledSeq3B_PD1;
}
int MAX86141::get_tagSeq3B_PD1(){
  return tagSeq3B_PD1;
}

int MAX86141::get_ledSeq1A_PD2(){
  return ledSeq1A_PD2;
} 
int MAX86141::get_tagSeq1A_PD2(){
  return tagSeq1A_PD2;
}
int MAX86141::get_ledSeq1B_PD2(){
  return ledSeq1B_PD2;
}
int MAX86141::get_tagSeq1B_PD2(){
  return tagSeq1B_PD2;
}
int MAX86141::get_ledSeq2A_PD2(){
  return ledSeq2A_PD2;
}
int MAX86141::get_tagSeq2A_PD2(){
  return tagSeq2A_PD2;
}
int MAX86141::get_ledSeq2B_PD2(){
  return ledSeq2B_PD2;
} 
int MAX86141::get_tagSeq2B_PD2(){
  return tagSeq2B_PD2;
}
int MAX86141::get_ledSeq3A_PD2(){
  return ledSeq3A_PD2;
} 
int MAX86141::get_tagSeq3A_PD2(){
  return tagSeq3A_PD2;
}
int MAX86141::get_ledSeq3B_PD2(){
  return ledSeq3B_PD2;
}
int MAX86141::get_tagSeq3B_PD2(){
  return tagSeq3B_PD2;
}
////////////////////////////////////////

////////////// SETs ///////////////////////////
void MAX86141::setNumbPD(int pd){
  nb_pd= pd;
  if(nb_pd==1){
          //write_reg(REG_MODE_CONFIG, 0xA);
    write_reg(REG_MODE_CONFIG, 0xB);
  }
  else
          //write_reg(REG_MODE_CONFIG, 0x2);
    write_reg(REG_MODE_CONFIG, 0x3);
}

void MAX86141::setLedModeSize( int size_led){
  ledModeSize= size_led;
}

void MAX86141::setLedMode(int *ledMd){
  ledMode = ledMd;
  if(ledModeSize==1){
    Serial.print("1 LED CONTROL SEQ1");
    Serial.println(ledMode[0],BIN);
    write_reg(REG_LED_SEQ_1, ledMode[0]);
  }
  else if(ledModeSize==2){
    ledMode[1]= ledMode[1]<<4; 
    int SEQ1= ledMode[1]+ ledMode[0];
    Serial.print("2 LED CONTROL SEQ1");
    Serial.println(SEQ1,BIN);
    write_reg(REG_LED_SEQ_1, SEQ1);
  }
  else if(ledModeSize==3){
    ledMode[1]= ledMode[1]<<4; 
    int SEQ1= ledMode[1]+ ledMode[0];
    Serial.println("3 LED CONTROL ");
    write_reg(REG_LED_SEQ_1, SEQ1);
    write_reg(REG_LED_SEQ_2, ledMode[2]);
  }
  else if(ledModeSize==4){
    Serial.print("4 LED CONTROL ");

    ledMode[1]= ledMode[1]<<4; 
    int SEQ1= ledMode[1]+ ledMode[0];
                  //Serial.println(SEQ1,BIN);
    write_reg(REG_LED_SEQ_1, SEQ1);
    ledMode[3]= ledMode[3]<<4;
    int SEQ2= ledMode[3]+ ledMode[2];
                  //Serial.println(SEQ2,BIN);
    write_reg(REG_LED_SEQ_2, SEQ2);
  }
  else if(ledModeSize==5){
   Serial.print("5 LED CONTROL ");

   ledMode[1]= ledMode[1]<<4;
   int SEQ1= ledMode[1]+ ledMode[0];
   write_reg(REG_LED_SEQ_1, SEQ1);
   ledMode[3]= ledMode[3]<<4;
   int SEQ2= ledMode[3]+ ledMode[2];
   write_reg(REG_LED_SEQ_2, SEQ2);
   write_reg(REG_LED_SEQ_3, ledMode[4]);
 }
 else {
  ledMode[1]= ledMode[1]<<4;
  int SEQ1= ledMode[1]+ ledMode[0];
  write_reg(REG_LED_SEQ_1, SEQ1);
  ledMode[3]= ledMode[3]<<4;
  int SEQ2= ledMode[3] + ledMode[2];
  write_reg(REG_LED_SEQ_2, SEQ2);
  ledMode[5]= ledMode[5]<<4;
  int SEQ3= ledMode[5]+ ledMode[4];
  write_reg(REG_LED_SEQ_3, SEQ3);
}
}

void MAX86141::setNumLeds(int nb_leds){
    number_leds = nb_leds;
}

void MAX86141::setIntensityLed(int intens_led, int* LedMode1){
    intensity_led= intens_led;
    ledMode = LedMode1;

    if( ledModeSize == 1){
      if(ledMode[0] == 1){
        write_reg(REG_LED_RANGE_1, 0b00000011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led);
      }
      if(ledMode[0] == 2){
        write_reg(REG_LED_RANGE_1, 0b00001100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED2_PA, intensity_led);
      }
      if(ledMode[0] == 3){
        write_reg(REG_LED_RANGE_1, 0b00110000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED3_PA, intensity_led);
      }
      if(ledMode[0] == 4){
        write_reg(REG_LED_RANGE_1, 0b00001111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
        write_reg(REG_LED2_PA, intensity_led);
      }
      if(ledMode[0] == 5){
        write_reg(REG_LED_RANGE_1, 0b00110011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
      }
      if(ledMode[0] == 6){
        write_reg(REG_LED_RANGE_1, 0b00111100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED2_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
      }
      if(ledMode[0] == 7){
        write_reg(REG_LED_RANGE_1, 0b00111111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
        write_reg(REG_LED2_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
      }
      if(ledMode[0] == 10){
        write_reg(REG_LED_RANGE_1, 0b00000000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED_RANGE_2, 0b00001111); // xx,LED6,LED5,LED4. 00,01,10,11 low to high
        write_reg(REG_LED4_PA, intensity_led);
      }
        }

    if( ledModeSize == 2){

      for(int i=0; i<ledModeSize; i++) {

        if(ledMode[i] == 1){
        write_reg(REG_LED_RANGE_1, 0b00000011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led);
        }
        if(ledMode[i] == 2){
        write_reg(REG_LED_RANGE_1, 0b00001100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 3){
        write_reg(REG_LED_RANGE_1, 0b00110000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 4){
        write_reg(REG_LED_RANGE_1, 0b00001111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
        write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 5){
        write_reg(REG_LED_RANGE_1, 0b00110011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 6){
        write_reg(REG_LED_RANGE_1, 0b00111100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED2_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 7){
        write_reg(REG_LED_RANGE_1, 0b00111111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
        write_reg(REG_LED2_PA, intensity_led);
        write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 10){
        write_reg(REG_LED_RANGE_1, 0b00000000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
        write_reg(REG_LED_RANGE_2, 0b00001111); // xx,LED6,LED5,LED4. 00,01,10,11 low to high
        write_reg(REG_LED4_PA, intensity_led);
        }
      }
  }

    if( ledModeSize == 3) {
      for(int i=0; i<ledModeSize; i++) {

        if(ledMode[i] == 1){
          write_reg(REG_LED_RANGE_1, 0b00000011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led);
        }
        if(ledMode[i] == 2){
          write_reg(REG_LED_RANGE_1, 0b00001100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 3){
          write_reg(REG_LED_RANGE_1, 0b00110000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 4){
          write_reg(REG_LED_RANGE_1, 0b00001111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
          write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 5){
          write_reg(REG_LED_RANGE_1, 0b00110011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 6){
          write_reg(REG_LED_RANGE_1, 0b00111100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED2_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 7){
          write_reg(REG_LED_RANGE_1, 0b00111111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
          write_reg(REG_LED2_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 10){
          write_reg(REG_LED_RANGE_1, 0b00000000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED_RANGE_2, 0b00001111); // xx,LED6,LED5,LED4. 00,01,10,11 low to high
          write_reg(REG_LED4_PA, intensity_led);
        }
      }
    }

    if( ledModeSize == 4) {
      for(int i=0; i<ledModeSize; i++) {

        if(ledMode[i] == 1){
          write_reg(REG_LED_RANGE_1, 0b00000011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led);
        }
        if(ledMode[i] == 2){
          write_reg(REG_LED_RANGE_1, 0b00001100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 3){
          write_reg(REG_LED_RANGE_1, 0b00110000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 4){
          write_reg(REG_LED_RANGE_1, 0b00001111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
          write_reg(REG_LED2_PA, intensity_led);
        }
        if(ledMode[i] == 5){
          write_reg(REG_LED_RANGE_1, 0b00110011); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 6){
          write_reg(REG_LED_RANGE_1, 0b00111100); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED2_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 7){
          write_reg(REG_LED_RANGE_1, 0b00111111); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED1_PA, intensity_led); // 0 = 0mA, 255 = Max mA
          write_reg(REG_LED2_PA, intensity_led);
          write_reg(REG_LED3_PA, intensity_led);
        }
        if(ledMode[i] == 10){
          write_reg(REG_LED_RANGE_1, 0b00000000); // xx,LED3,LED2,LED1. 00,01,10,11 low to high
          write_reg(REG_LED_RANGE_2, 0b00001111); // xx,LED6,LED5,LED4. 00,01,10,11 low to high
          write_reg(REG_LED4_PA, intensity_led);
        }
      }
    }  
}

void MAX86141::setPulseWidth(int pulse){
  pulse_width= pulse;
}

void MAX86141::setADCrange(int adc){
  adcRange= adc;
}

void MAX86141::setSample(int smpl_avr, int smpl_rate){
  sample_average= smpl_avr;
  sample_rate= smpl_rate;
//Serial.println("sample_rate"+String(sample_rate));
  int sample1= sample_rate<<3;
    //Serial.println(sample1,BIN);
  int sample2= sample_average>>2;
    //Serial.println(sample2,BIN);
  int sample= sample1 + sample2;
    //Serial.print("Samlple");
      //    Serial.println(sample,BIN);
  write_reg(REG_PPG_CONFIG_2, sample); //SPS (0-5), SMP_AVE (6-8)
  }
///////////////////////////////////////////////

/*inspired by pseudo-code available on MAX86141 datasheet for initialisation*/
void MAX86141::initialisation(int pd, int *ledMd, int size_led, int nb_leds, int intens_led, int smpl_avr, int smpl_rate, int pulse, int adc, int newSpiClk=10000)
  {
    setSpiClk(newSpiClk);
    uint8_t temp;

    write_reg(REG_MODE_CONFIG, 0b00000001); //Single_PPG (Register 0x0D[3]),LP_Mode,Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])
    delay(1);

    //write_reg(REG_PICKET_FENCE,0b11000000);
    //write_reg(REG_PICKET_FENCE,0b00000000);

    /* Clear interrupts. */
    read_reg(REG_INT_STAT_1);
    read_reg(REG_INT_STAT_2);

    write_reg(REG_MODE_CONFIG, 0b00000010); //Low Power mode disabled Shutdown (Register 0x0D[1]),Soft Reset (Register 0x0D[0])

    setNumbPD(pd);
    setNumLeds(nb_leds);
    setLedModeSize(size_led);
    
    /* PPG1 & 2 & 3 */
    setPulseWidth( pulse);
    setADCrange( adc);
    if(nb_pd==1){

     adcRange= adcRange<<2;
     int8_t adcRange_pulseWidth=  adcRange + pulse_width;

     write_reg(REG_PPG_CONFIG_1, adcRange_pulseWidth); //ALC_DIS,ADD_OFF,PPG2_RGE,PPG1_RGE,PPG_TINT
   }
    else{
     int8_t a = adcRange<<4;
     int8_t b = adcRange<<2;

     adcRange = a+b;
     int8_t adcRange_pulseWidth= adcRange + pulse_width;

     write_reg(REG_PPG_CONFIG_1, adcRange_pulseWidth); //ALC_DIS,ADD_OFF,PPG2_RGE,PPG1_RGE,PPG_TINT
    }

    setSample(smpl_avr, smpl_rate);

    write_reg(REG_PPG_CONFIG_3, 0b11000000); //LED_SETLNG, DIG_FILT_SEL, BURST_EN
    write_reg(REG_PD_BIAS, 0b00010001);

    setIntensityLed(intens_led, ledMd);

    if(nb_pd==1){
        write_reg(REG_MODE_CONFIG, 0b00001110);//Low Power Mode enabled (Register 0x0D[2]), Shutdown (Register 0x0D[1]), Reset (Register 0x0D[0])
                }
    else{
        write_reg(REG_MODE_CONFIG, 0b00000110);//Low Power Mode enabled (Register 0x0D[2]), Shutdown (Register 0x0D[1]), Reset (Register 0x0D[0])
    }

  /* Configure FIFO. */

     //write_reg(REG_FIFO_CONFIG_1, 128-16); //(A_FULL = 112)
    write_reg(REG_FIFO_CONFIG_1, 0b01111000); //(A_FULL = 120)
    write_reg(REG_FIFO_CONFIG_2, 0b00010010);

  /* Configure interrupt. */
    //write_reg(REG_INT_EN_1, 0x84);
    write_reg(REG_INT_EN_1, 0xC0);

    setLedMode(ledMd);

  /* exit shutdown mode. */
    if(nb_pd==1){
        write_reg(REG_MODE_CONFIG, 0b00001100); //Low Power Mode enabled (Register 0x0D[2]), Shutdown (Register 0x0D[1]), Reset (Register 0x0D[0])
      }
    else{
        write_reg(REG_MODE_CONFIG, 0b00000100); //Low Power Mode enabled (Register 0x0D[2]), Shutdown (Register 0x0D[1]), Reset (Register 0x0D[0])
      }

    }


/*read FIFO*/
void MAX86141::read_fifo(uint8_t data_buffer[], int count)
{
  data_buffer[0] = REG_FIFO_DATA;
  data_buffer[1] = READ_EN;
  digitalWrite(SS, HIGH);
  spi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE3));
  digitalWrite(SS, LOW);
  spi->transfer(data_buffer, 2);
  spi->transfer(data_buffer, count * 3);
  digitalWrite(SS, HIGH);
  spi->endTransaction();
  digitalWrite(SS, LOW);

  if(debug == true){
//Serial.println("Data buffer");
//Serial.println(data_buffer[0]);
  }
}

void MAX86141::clearInt() {
  uint16_t intr = 0x00;
  intr = read_reg(REG_INT_STAT_1) << 8;
  intr |= read_reg(REG_INT_STAT_2);
}

/* inspired by pseudo-code available on MAX86141 datasheet */
int  MAX86141::device_data_read1()
{
  int sample_count;
  uint8_t reg_val;

//delay(2);
//sample_count =  read_reg(REG_FIFO_DATA_COUNT);
  sample_count=8;
  //Serial.println("sample_count :"+String(read_reg(REG_FIFO_DATA_COUNT)));


/////// Allocation dynamique /////
  uint8_t *dataBuf = (uint8_t*)malloc(sample_count*3*sizeof(uint8_t));
  memset(dataBuf, 0, sample_count*3*sizeof(uint8_t));   // Flush buffer
///////////////////////////////

 //uint8_t dataBuf[FIFO_SAMPLES*3];

//read_fifo(dataBuf,sample_count);
  read_fifo(dataBuf, FIFO_SAMPLES);

/*
for(int i=0; i<sample_count*3;i++){
  Serial.print(dataBuf[i]);
  Serial.print(",");
 }
   Serial.println();
 */

//refer to MAX86140-MAX86141 datasheet - page 15
  if(nb_pd==1)
  {
//suitable formatting of data for 1 LED control
   if(ledModeSize==1){
/* Reading of 1 sample for Serial Test */
    tagSeq1A_PD1 = (dataBuf[0*3+0] >> 3) & 0x1f;
    ledSeq1A_PD1 = ((dataBuf[0*3+0] << 16) | (dataBuf[0*3+1] << 8) | (dataBuf[0*3+2])) & 0x7ffff;

/* Reading of 8 samples for BLE */
    int i = 0;
    int *tagSeq1A_PD11 = (int*)malloc((sample_count)*sizeof(int)),*ledSeq1A_PD11 = (int*)malloc((sample_count)*sizeof(int));    
    for (i = 0; i < sample_count; i++)
    {
      tagSeq1A_PD11[i] = (dataBuf[i*3+0] >> 3) & 0x1f;
      ledSeq1A_PD11[i] = ((dataBuf[i*3+0] << 16) | (dataBuf[i*3+1] << 8) | (dataBuf[i*3+2])) & 0x7ffff;
    }

    free(dataBuf);
    free(tagSeq1A_PD11);
    free(ledSeq1A_PD11);
  }

    //suitable formatting of data for 2 LEDs control
    if(ledModeSize==2){

  /* Reading of 1 sample for Serial Test */
      tagSeq1A_PD1 = (dataBuf[0*6+0] >> 3) & 0x1f;
      ledSeq1A_PD1 = ((dataBuf[0*6+0] << 16) | (dataBuf[0*6+1] << 8) | (dataBuf[0*6+2])) & 0x7ffff;
      tagSeq1B_PD1 = (dataBuf[0*6+3] >> 3) & 0x1f;
      ledSeq1B_PD1 = ((dataBuf[0*6+3] << 16) | (dataBuf[0*6+4] << 8) | (dataBuf[0*6+5])) & 0x7ffff;

  /* Reading of 8 samples for BLE */
      int i = 0;
      int *tagSeq1A_PD11 = (int*)malloc((sample_count/2)*sizeof(int)),*ledSeq1A_PD11 = (int*)malloc((sample_count/2)*sizeof(int));
      int *tagSeq1B_PD11 = (int*)malloc((sample_count/2)*sizeof(int)),*ledSeq1B_PD11 = (int*)malloc((sample_count/2)*sizeof(int));

      for (i = 0; i < sample_count/2; i++)
      {
        tagSeq1A_PD11[i] = (dataBuf[i*6+0] >> 3) & 0x1f;
        ledSeq1A_PD11[i] = ((dataBuf[i*6+0] << 16) | (dataBuf[i*6+1] << 8) | (dataBuf[i*6+2])) & 0x7ffff;

        tagSeq1B_PD11[i] = (dataBuf[i*6+3] >> 3) & 0x1f;
        ledSeq1B_PD11[i] = ((dataBuf[i*6+3] << 16) | (dataBuf[i*6+4] << 8) | (dataBuf[i*6+5])) & 0x7ffff;
      }

      free(dataBuf);
      free(tagSeq1A_PD11);
      free(ledSeq1A_PD11);
      free(tagSeq1B_PD11);
      free(ledSeq1B_PD11);

    }
  }

    else {
     if(ledModeSize==1){

/* Reading of 1 sample for Serial Test */
      tagSeq1A_PD1 = (dataBuf[0*6+0] >> 3) & 0x1f;
      ledSeq1A_PD1 = ((dataBuf[0*6+0] << 16) | (dataBuf[0*6+1] << 8) | (dataBuf[0*6+2])) & 0x7ffff;
      tagSeq1A_PD2 = (dataBuf[0*6+3] >> 3) & 0x1f;
      ledSeq1A_PD2 = ((dataBuf[0*6+3] << 16) | (dataBuf[0*6+4] << 8) | (dataBuf[0*6+5])) & 0x7ffff;

/* Reading of 8 samples for BLE */
      int i = 0;
      int *tagSeq1A_PD11 = (int*)malloc((sample_count/2)*sizeof(int)),*ledSeq1A_PD11 = (int*)malloc((sample_count/2)*sizeof(int));
      int *tagSeq1A_PD22 = (int*)malloc((sample_count/2)*sizeof(int)),*ledSeq1A_PD22 = (int*)malloc((sample_count/2)*sizeof(int));

      tab_ledSeq1A_PD1= (int*)malloc((sample_count/2)*sizeof(int));
      tab_ledSeq1A_PD2= (int*)malloc((sample_count/2)*sizeof(int));

/* Checking if data tags are corrects before collecting data */

      if( (dataBuf[0]>>3 ==1) && (dataBuf[3]>>3 ==7) && (dataBuf[6]>>3 ==1) && (dataBuf[9]>>3 ==7) && (dataBuf[12]>>3 ==1)
        && (dataBuf[15]>>3 ==7) && (dataBuf[18]>>3 ==1) &&(dataBuf[21]>>3 ==7)
        ){

        for (i = 0; i < sample_count/2; i++)
        {
          tagSeq1A_PD11[i] = (dataBuf[i*6+0] >> 3) & 0x1f;
          ledSeq1A_PD11[i] = ((dataBuf[i*6+0] << 16) | (dataBuf[i*6+1] << 8) | (dataBuf[i*6+2])) & 0x7ffff;

          tagSeq1A_PD22[i] = (dataBuf[i*6+3] >> 3) & 0x1f;
          ledSeq1A_PD22[i] = ((dataBuf[i*6+3] << 16) | (dataBuf[i*6+4] << 8) | (dataBuf[i*6+5])) & 0x7ffff;

          tab_ledSeq1A_PD1[i] = ledSeq1A_PD11[i];
          tab_ledSeq1A_PD2[i] = ledSeq1A_PD22[i]; 
        }
      }

      if( (dataBuf[0]>>3 ==7) && (dataBuf[3]>>3 ==1) && (dataBuf[6]>>3 ==7) && (dataBuf[9]>>3 ==1) && (dataBuf[12]>>3 ==7)
        && (dataBuf[15]>>3 ==1) && (dataBuf[18]>>3 ==7) &&(dataBuf[21]>>3 ==1)
        ){
        for (i = 0; i < sample_count/2; i++)
        {
          tagSeq1A_PD22[i] = (dataBuf[i*6+0] >> 3) & 0x1f;
          ledSeq1A_PD22[i] = ((dataBuf[i*6+0] << 16) | (dataBuf[i*6+1] << 8) | (dataBuf[i*6+2])) & 0x7ffff;

          tagSeq1A_PD11[i] = (dataBuf[i*6+3] >> 3) & 0x1f;
          ledSeq1A_PD11[i] = ((dataBuf[i*6+3] << 16) | (dataBuf[i*6+4] << 8) | (dataBuf[i*6+5])) & 0x7ffff;

          tab_ledSeq1A_PD1[i] = ledSeq1A_PD11[i];
          tab_ledSeq1A_PD2[i] = ledSeq1A_PD22[i]; 
        }
      }

      free(dataBuf);
      free(tagSeq1A_PD11);
      free(ledSeq1A_PD11);
      free(tagSeq1A_PD22);
      free(ledSeq1A_PD22);

    }

    //suitable formatting of data for 2 LEDs control

    if(ledModeSize==2){

    /* Reading of 1 sample for Serial Test */
      tagSeq1A_PD1 = (dataBuf[0*12+0] >> 3) & 0x1f;
      ledSeq1A_PD1 = ((dataBuf[0*12+0] << 16) | (dataBuf[0*12+1] << 8) | (dataBuf[0*12+2])) & 0x7ffff;

      tagSeq1A_PD2 = (dataBuf[0*12+3] >> 3) & 0x1f;
      ledSeq1A_PD2 = ((dataBuf[0*12+3] << 16) | (dataBuf[0*12+4] << 8) | (dataBuf[0*12+5])) & 0x7ffff;

      tagSeq1B_PD1 = (dataBuf[0*12+6] >> 3) & 0x1f;
      ledSeq1B_PD1 = ((dataBuf[0*12+6] << 16) | (dataBuf[0*12+7] << 8) | (dataBuf[0*12+8])) & 0x7ffff;

      tagSeq1B_PD2 = (dataBuf[0*12+9] >> 3) & 0x1f;
      ledSeq1B_PD2 = ((dataBuf[0*12+9] << 16) | (dataBuf[0*12+10] << 8)| (dataBuf[0*12+11])) & 0x7ffff;

    /* Reading of 8 samples for BLE */
      int i = 0;
      int *tagSeq1A_PD11 = (int*)malloc((sample_count/4)*sizeof(int)),*ledSeq1A_PD11 = (int*)malloc((sample_count/4)*sizeof(int)),*tagSeq1B_PD11 = (int*)malloc((sample_count/4)*sizeof(int)),*ledSeq1B_PD11 = (int*)malloc((sample_count/4)*sizeof(int));
      int *tagSeq1A_PD22 = (int*)malloc((sample_count/4)*sizeof(int)),*ledSeq1A_PD22 = (int*)malloc((sample_count/4)*sizeof(int)),*tagSeq1B_PD22 = (int*)malloc((sample_count/4)*sizeof(int)),*ledSeq1B_PD22 = (int*)malloc((sample_count/4)*sizeof(int));

      for (i = 0; i < sample_count/4; i++)
      {   
        tagSeq1A_PD11[i] = (dataBuf[i*12+0] >> 3) & 0x1f;
        ledSeq1A_PD11[i] = ((dataBuf[i*12+0] << 16) | (dataBuf[i*12+1] << 8) | (dataBuf[i*12+2])) & 0x7ffff;

        tagSeq1A_PD22[i] = (dataBuf[i*12+3] >> 3) & 0x1f;
        ledSeq1A_PD22[i] = ((dataBuf[i*12+3] << 16) | (dataBuf[i*12+4] << 8) | (dataBuf[i*12+5])) & 0x7ffff;

        tagSeq1B_PD11[i] = (dataBuf[i*12+6] >> 3) & 0x1f;
        ledSeq1B_PD11[i] = ((dataBuf[i*12+6] << 16) | (dataBuf[i*12+7] << 8) | (dataBuf[i*12+8])) & 0x7ffff;

        tagSeq1B_PD22[i] = (dataBuf[i*12+9] >> 3) & 0x1f;
        ledSeq1B_PD22[i] = ((dataBuf[i*12+9] << 16) | (dataBuf[i*12+10] << 8)| (dataBuf[i*12+11])) & 0x7ffff;
      }

      free(dataBuf);
      free(tagSeq1A_PD11);
      free(ledSeq1A_PD11);
      free(tagSeq1A_PD22);
      free(ledSeq1A_PD22);
      free(tagSeq1B_PD11);
      free(ledSeq1B_PD11);
      free(tagSeq1B_PD22);
      free(ledSeq1B_PD22);

    }
      }

    clearInt();
    return sample_count;
    }

/*void MAX86141::fifo_intr()
{
    uint8_t count;
    count = read_reg(REG_FIFO_DATA_COUNT);

    if (count == 0x80) //indicates full FIFO
    {
        device_data_read();
    }
 }*/


void MAX86141::setSS(int pin){
  SS = pin;
}

void MAX86141::setSPI(SPIClass * newspi){
  spi = newspi;
}

void MAX86141::setSpiClk(int newSpiClk) {
  spiClk = newSpiClk;
}

void MAX86141::setDebug(bool setdebug) {
  debug = setdebug;
}


//axis=0, freedom degre=0
float MAX86141::signaltonoise(int* signalBuff, int len){
  // STEP 1, FIND THE MEAN.
   long sum = 0L ;  // sum will be larger than an item, long for safety.
  /*for (int i : signalBuff) {
    sum += i ;}*/
   for(int i=0; i<len; i++){
    sum += signalBuff[i] ;
  }
  float mean = sum/(float)len;

   // STEP 2, sum the squares of the differences from the mean
  float sqDevSum = 0.0;
  for(int i = 0; i < len; i++) {
    // pow(x, 2) is x squared.
    sqDevSum += pow((mean - float(signalBuff[i])), 2);
  }

   // STEP 3, FIND THE MEAN OF THAT
  // STEP 4, TAKE THE SQUARE ROOT OF THAT
  float stDev = sqrt(sqDevSum/float(len));

  return (float)20*log10(abs(mean/stDev));
}
