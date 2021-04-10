void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  /*Serial.print("Characteristic 1 Measurement: ");*/
  if(data[0]==1) {
    Serial.println("IMU not detected");
    errorFlag++;
  }
  else Serial.println("IMU Detected");
  if(data[1]==1){ 
    Serial.println("PPG not detected");
    errorFlag++;
  }
  else Serial.println("PPG Detected");
  if(errorFlag==0) {
    Serial.println("no error detected disabling notify");
    ErrorCharacteristic.disableNotify();
   // Bluefruit.disconnect(conn_handle0);
  }
  else {
    Serial.println("error detected ");
  }
}
void rawPPGCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
    timeStamp = data[0];
    timeStamp = (timeStamp  << 8) + data[1];
    timeStamp = (timeStamp << 8) + data[2];
    timeStamp = (timeStamp << 8) + data[3];  
  //  Serial.print(" timestamp: "+String(timeStamp));
    redPPG = data[4];
    redPPG = (redPPG  << 8) + data[5];
    redPPG = (redPPG  << 8) + data[6];
    redPPG = (redPPG  << 8) + data[7];  
  //  Serial.print(" R: "+String(redPPG));
    irPPG = data[8];
    irPPG = (irPPG  << 8) + data[9];
    irPPG = (irPPG  << 8) + data[10];
    irPPG = (irPPG  << 8) + data[11];
  //  Serial.print(" IR: "+String(irPPG));
    greenPPG = data[12];
    greenPPG = (greenPPG  << 8) + data[13];
    greenPPG = (greenPPG  << 8) + data[14];
    greenPPG = (greenPPG << 8) + data[15];  
  //  Serial.println(" G: "+String(greenPPG));
    #ifdef debugPPGData
    Serial.print("[0:4] "+String(data[0]));
    Serial.print(" "+String(data[1]));
    Serial.print(" "+String(data[2]));
    Serial.println(" "+String(data[3]));
    Serial.print("[4:7] "+String(data[4]));
    Serial.print("  "+String(data[5]));
    Serial.print(" "+String(data[6]));
    Serial.println(" "+String(data[7]));
    Serial.print("[8:11]"+String(data[8]));
    Serial.print(" "+String(data[9]));
    Serial.print(" "+String(data[10]));
    Serial.println(" "+String(data[11]));
     Serial.print("[12:15]"+String(data[12]));
    Serial.print(" "+String(data[13]));
    Serial.print(" "+String(data[14]));
    Serial.println(" "+String(data[15]));
    #endif
}
void AccCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  //Serial.println("AccCharacteristic updated");
  int16_t v = ((int16_t) data[5]) << 8 | data[6];
  aX=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  v = ((int16_t) data[7]) << 8 | data[8];
  aY=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  v = ((int16_t) data[9]) << 8 | data[10];
  aZ=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  //Serial.print(receivedSamplesNb);
  Serial.print("accelX: " + String( aX));
  Serial.print("accelY: " + String( aY));
  Serial.println("accelZ: " + String( aZ));
}
void  GyroCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  //Serial.println("GyroCharacteristic updated");
  int16_t v = ((int16_t) data[5]) << 8 | data[6];
  gX= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[7]) << 8 | data[8];
  gY= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[9]) << 8 | data[10];
  gZ= ((float) -v) * gyroRange / (float) 0x8000;
  Serial.print("gyrolX: " + String( gX));
  Serial.print("GyroY: " + String( gY));
  Serial.println("gyroZ: " + String( gZ));
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
