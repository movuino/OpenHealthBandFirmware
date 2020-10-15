void IMUCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.

    int16_t v = ((int16_t) data[0]) << 8 | data[1];
    aX=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
    v = ((int16_t) data[2]) << 8 | data[3];
    aY=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
    v = ((int16_t) data[4]) << 8 | data[5];
    aZ=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
  Serial.print(receivedSamplesNb);
  Serial.print(" accelX: " + String( aX));
  Serial.print("accelY: " + String( aY));
  Serial.println("accelZ: " + String( aZ));
}
void GyroCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  int16_t v = ((int16_t) data[0]) << 8 | data[1];
  gX= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[2]) << 8 | data[3];
  gY= ((float) -v) * gyroRange / (float) 0x8000;
  v = ((int16_t) data[4]) << 8 | data[5];
  gZ= ((float) -v) * gyroRange / (float) 0x8000;
  Serial.print("gyrolX: " + String( gX));
  Serial.print("GyroY: " + String( gY));
  Serial.println("gyroZ: " + String( gZ));
}
void timeStampCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
 
  Serial.println("received timestamp");
 
}
