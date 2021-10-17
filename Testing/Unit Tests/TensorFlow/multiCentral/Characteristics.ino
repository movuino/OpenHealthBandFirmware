void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  /*Serial.print("Characteristic 1 Measurement: ");*/

  
  if(data[0]==1){ 
    Serial.println("TensorFlow not detected");
    errorFlag++;
  }
  else Serial.println("TensorFlow Detected");
  
  if(errorFlag==0) {
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
    timeStamp = data[0];
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
    timeStamp = data[0];
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

  mX=(((int16_t) data[5]) << 8) | data[4];
  mY=(((int16_t) data[7]) << 8) | data[6];
  mZ=(((int16_t) data[9]) << 8) | data[8];
  Serial.print("mX: " + String( mX));
  Serial.print("mY: " + String( mY));
  Serial.println("mZ: " + String( mZ));
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
