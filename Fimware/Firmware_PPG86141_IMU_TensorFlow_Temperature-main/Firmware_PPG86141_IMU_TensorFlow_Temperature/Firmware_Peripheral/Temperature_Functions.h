#include "Protocentral_MLX90632.h"

#define debugTemp1

Protocentral_MLX90632 ReadSensor;
uint32_t tempBuff32[7];
double Gb1, P_O1, P_G1, P_R1, P_T1;
float sensor_Temp;

uint32_t bufTemp[9];
uint64_t bufSix_Nine_RAM;
//unsigned char* buffData;
uint8_t* buffData;

void configureTemp() {
    Serial.println("####  TEMPERATURE CONFIG ###");
  Serial.println("Protocentral MLX90632 Read Data");
  Wire.begin();
  ReadSensor.begin();
  for (int i = 0; i < 5; i++) {
    tempBuff32[i] = ReadSensor.buff[i];
  }
  Gb1 = (double)tempBuff32[4] * pow(2, -10);
  P_O1 = (double)tempBuff32[3] * pow(2, -8);
  P_G1 = (double)tempBuff32[1] * pow(2, -20);
  P_R1 = (double)tempBuff32[0] * pow(2, -8);
  P_T1 = (double)tempBuff32[2] * pow(2, -44);


  if (ReadSensor.begin() == true) {

    Serial.println("Communication success with the sensor");
  }
  else {
    Serial.println("Communication failure with the sensor");
    errorTemp = true;
  }
}

void updateTemp() {
  //sensor_Temp = ReadSensor.getSensorTemp(); //Get the temperature of the sensor
  //
  if (ReadSensor.dataAvailable() == true) {
    ReadSensor.tempBuff();
    tempBuff32[5] = ReadSensor.buff[5];
    tempBuff32[6] = ReadSensor.buff[6];

    uint32_t timestamp = millis();
    bufTemp[2] = (uint8_t)timestamp;
    bufTemp[1] = (uint8_t)(timestamp >>= 8);
    bufTemp[0] = (uint8_t)(timestamp >>= 8);
    for (int i = 3; i <= 9; i++) {
      bufTemp[i] = tempBuff32[i - 3];

#ifdef debugTemp
      Serial.println(String(tempBuff32[i - 3]));
#endif
    }

  }
  else {
    Serial.println("Data not available");
  }
#ifdef debugTemp1
  uint32_t sixRAM = tempBuff32[5];
  uint32_t nineRAM = tempBuff32[6];
  double VRta =  nineRAM + Gb1 * ( sixRAM / 12.0);
  double AMB = (sixRAM / 12.0) / VRta * pow(2, 19);
  float sensor_Temp = P_O1 + (AMB - P_R1) / P_G1 + P_T1 * pow((AMB - P_R1), 2);
  Serial.print(" Sensor temperature: ");
  Serial.print(sensor_Temp, 2);
  Serial.print(" C");
  Serial.println();
#endif


  //buffData= (unsigned char*)&bufTemp[8];

  //buffData= (uint8_t*)&bufTemp[8];
  bufSix_Nine_RAM = bufTemp[8];
  bufSix_Nine_RAM = (bufSix_Nine_RAM  << 32) + bufTemp[9];
  buffData = (uint8_t*)&bufSix_Nine_RAM;
  /*uint32_t tp1;
    tp1 = buffData[3];
    tp1 = (tp1  << 8) + buffData[2];
    tp1 = (tp1  << 8) + buffData[1];
    tp1 = (tp1  << 8) + buffData[0];
     Serial.println("--- "+String(tp1));
     uint32_t tp2;
    tp2 = buffData[7];
    tp2 = (tp2  << 8) + buffData[6];
    tp2 = (tp2  << 8) + buffData[5];
    tp2 = (tp2  << 8) + buffData[4];
     Serial.println("--- "+String(tp2));*/
  /* uint32_t tp;
    tp = buffData[3];
    tp = (tp  << 8) + buffData[2];
    tp = (tp << 8) + buffData[1];
    tp = (tp << 8) + buffData[0];
      Serial.println("--- "+String(tp));*/
  /*Serial.println(" Gb1 "+String(Gb1));
    Serial.println(" P_O1 "+String(P_O1));
    Serial.println(" P_G1 "+String(P_G1));
    Serial.println(" P_R1 "+String(P_R1));
    Serial.println(" P_T1 "+String(P_T1));*/
}
