void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  /*Serial.print("Characteristic 1 Measurement: ");*/
  if (data[0] == 1) {
    Serial.println("IMU not detected");
    errorFlag++;
  }
  else Serial.println("IMU Detected");
  if (data[1] == 1) {
    Serial.println("PPG not detected");
    errorFlag++;
  }
  else Serial.println("PPG Detected");

  if (data[2] == 1) {
    Serial.println("Temperature not detected");
    errorFlag++;
  }
  else Serial.println("Temperature Detected");

  if (data[3] == 1) {
    Serial.println("TensorFlow not detected");
    errorFlag++;
  }
  else Serial.println("TensorFlow Detected");

  if (errorFlag == 0) {
    Serial.println("no error detected disabling notify");
    ErrorCharacteristic.disableNotify();
    // Bluefruit.disconnect(conn_handle0);
  }
  else {
    Serial.println("error detected ");
  }
}

void AccCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
uint8_t  timeStamp = data[0];
    timeStamp = (timeStamp  << 8) + data[1];
    timeStamp = (timeStamp << 8) + data[2];
    timeStamp = (timeStamp << 8) + data[3]; 
    Serial.print(timeStamp); 
  //Serial.println("AccCharacteristic updated");
  int16_t v = ((int16_t) data[5]) << 8 | data[6];
  aX=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  v = ((int16_t) data[7]) << 8 | data[8];
  aY=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  v = ((int16_t) data[9]) << 8 | data[10];
  aZ=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  //Serial.print(receivedSamplesNb);
  /*Serial.print("accelX: " + String( aX));
  Serial.print("accelY: " + String( aY));
  Serial.println("accelZ: " + String( aZ));*/
  Serial.println(" A "+String( aX)+" "+String( aY)+" "+String( aZ)+"  ");
}

void  GyroCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
    uint8_t timeStamp = data[0];
    timeStamp = (timeStamp  << 8) + data[1];
    timeStamp = (timeStamp << 8) + data[2];
    timeStamp = (timeStamp << 8) + data[3]; 
    Serial.print(timeStamp);  
    //Serial.println("GyroCharacteristic updated");
  int16_t v = ((int16_t) data[5]) << 8 | data[6];
  gX= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[7]) << 8 | data[8];
  gY= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[9]) << 8 | data[10];
  gZ= ((float) -v) * gyroRange / (float) 0x8000;
  /*Serial.print("gyrolX: " + String( gX));
  Serial.print("GyroY: " + String( gY));
  Serial.println("gyroZ: " + String( gZ));*/
  Serial.print(" G "+String( gX)+" "+String( gY)+" "+String( gZ));
  Serial.println();
}

void  MagCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  //Serial.println("MagCharacteristic updated");
 /*for(int i=5;i<=9;i++)
 {
  Serial.println(data[i]);
 }
 Serial.println("-----------------");
  mX=(((int16_t) data[1]) << 8) | data[0];
  mY=(((int16_t) data[3]) << 8) | data[2];
  mZ=(((int16_t) data[5]) << 8) | data[4];*/
  mX=(((int16_t) data[5]) << 8) | data[4];
  mY=(((int16_t) data[7]) << 8) | data[6];
  mZ=(((int16_t) data[9]) << 8) | data[8];
  Serial.print("mX: " + String( mX));
  Serial.print("mY: " + String( mY));
  Serial.println("mZ: " + String( mZ));
}

void PPGMax86_ledSeq1A_PPG1_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("tp1 : "+String(tp1));
  
  uint32_t  tp2 = data[4];
  tp2 = (tp2  << 8) + data[5];
  tp2 = (tp2  << 8) + data[6];
  tp2 = (tp2  << 8) + data[7];
  //Serial.println("tp2 : "+String(tp2));

  uint32_t tp3 = data[8];
  tp3 = (tp3  << 8) + data[9];
  tp3 = (tp3  << 8) + data[10];
  tp3 = (tp3  << 8) + data[11];
  //Serial.println("tp3 : "+String(tp3));

  uint32_t tp4 = data[12];
  tp4 = (tp4  << 8) + data[13];
  tp4 = (tp4  << 8) + data[14];
  tp4 = (tp4  << 8) + data[15];
  //Serial.println("tp4 : "+String(tp4));
  
  uint32_t tab[4] = {tp1, tp2, tp3, tp4};
  
  for (uint32_t i = 0; i < 4; i++) {
    if (tab[i] != 0) {
      Serial.println(tab[i]);
    }
  }
}

void PPGMax86_ledSeq1A_PPG2_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len){
     uint32_t tp1;

    tp1 = data[0];
    tp1 = (tp1  << 8) + data[1];
    tp1 = (tp1  << 8) + data[2];
    tp1 = (tp1  << 8) + data[3];
      //Serial.println("--- "+String(tp1));

    uint32_t tp2 = data[4];
    tp2 = (tp2  << 8) + data[5];
    tp2 = (tp2  << 8) + data[6];
    tp2 = (tp2  << 8) + data[7];
      //Serial.println(tp2);

      uint32_t tp3 = data[8];
    tp3 = (tp3  << 8) + data[9];
    tp3 = (tp3  << 8) + data[10];
    tp3 = (tp3  << 8) + data[11];
      //Serial.println(tp3);

      uint32_t tp4 = data[12];
    tp4 = (tp4  << 8) + data[13];
    tp4 = (tp4  << 8) + data[14];
    tp4 = (tp4  << 8) + data[15];
     // Serial.println(tp3);
  uint32_t tab[4] = {tp1,tp2,tp3,tp4};
     for(uint32_t i=0; i<4; i++){
      //Serial.println(tab[i]);
     }
  }

void PPGMax86_SNR1_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len){
    uint32_t tp1;
    tp1 = data[0];
    tp1 = (tp1  << 8) + data[1];
    tp1 = (tp1  << 8) + data[2];
    tp1 = (tp1  << 8) + data[3];

   // Serial.println(tp1);
   if(tp1%100==0){
    float SNR_neg= tp1;
    SNR_neg= -(SNR_neg/10000);
    //Serial.println("Négatif: "+String(SNR_neg));
   }
   else{
   float SNR_pos= tp1;
   SNR_pos= SNR_pos/100;
   //Serial.println("Positif: "+String(SNR_pos));}
  }
  }
  void PPGMax86_SNR2_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len){
    uint32_t tp1;
    tp1 = data[0];
    tp1 = (tp1  << 8) + data[1];
    tp1 = (tp1  << 8) + data[2];
    tp1 = (tp1  << 8) + data[3];

   // Serial.println(tp1);
   if(tp1%100==0){
    float SNR_neg= tp1;
    SNR_neg= -(SNR_neg/10000);
    //Serial.println("Négatif: "+String(SNR_neg));
   }
   else{
   float SNR_pos= tp1;
   SNR_pos= SNR_pos/100;
   //Serial.println("Positif: "+String(SNR_pos));}
  }
  }

  void  TempCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
 Gb1 = 10.50;
 P_O1 = 25.00;
 P_G1 = 74.93;
 P_R1 = 22792.78;
 P_T1 = 0.00;
 
    nineRAM = data[3];
    nineRAM = (nineRAM  << 8) + data[2];
    nineRAM= (nineRAM  << 8) + data[1];
    nineRAM = (nineRAM  << 8) + data[0]; 
 //Serial.println("-- "+String(nineRAM));
   sixRAM = data[7];
   sixRAM = (sixRAM  << 8) + data[6];
   sixRAM= (sixRAM  << 8) + data[5];
   sixRAM = (sixRAM  << 8) + data[4]; 
 //Serial.println("-- "+String(sixRAM));

 double VRta =  nineRAM+ Gb1 * ( sixRAM / 12.0);
  double AMB = (sixRAM / 12.0) / VRta * pow(2, 19);
float sensor_Temp = P_O1 + (AMB - P_R1) / P_G1 + P_T1 * pow((AMB - P_R1), 2);
Serial.print(" Sensor temperature: ");
  Serial.print(sensor_Temp, 2);
  Serial.print(" C");
  Serial.println();

}
void  PossCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  aTensorFlow = data[3];
   aTensorFlow = (aTensorFlow  << 8) + data[2];
   aTensorFlow = (aTensorFlow  << 8) + data[1];
   aTensorFlow = (aTensorFlow  << 8) + data[0];
    bTensorFlow = data[7];
   bTensorFlow = (bTensorFlow  << 8) + data[6];
   bTensorFlow = (bTensorFlow  << 8) + data[5];
   bTensorFlow = (bTensorFlow  << 8) + data[4];
   cTensorFlow = data[11];
   cTensorFlow = (cTensorFlow  << 8) + data[10];
   cTensorFlow = (cTensorFlow  << 8) + data[9];
   cTensorFlow = (cTensorFlow  << 8) + data[8];
   Serial.println(String(aTensorFlow));
   Serial.println(String(bTensorFlow));
   Serial.println(String(cTensorFlow));
}
