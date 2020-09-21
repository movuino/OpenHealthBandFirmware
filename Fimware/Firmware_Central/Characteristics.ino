void timeStampCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.

  /*Serial.print("Characteristic 1 Measurement: ");
  Serial.println(data[0]);
  Serial.println(data[1]);
  Serial.println(data[2]);
  Serial.println(data[3]);*/
  uint32_t timestamp = data[3]; //0
  timestamp = (timestamp  << 8) + data[2];
  timestamp = (timestamp << 8) + data[1];
  timestamp = (timestamp << 8) + data[0];
  receivedSamplesNb++;
  Serial.println(receivedSamplesNb);
  //Serial.println(timestamp);  
  
//  uint16_t value;
//  memcpy(&value, , sizeof(len));
//  memcpy(&value, data+1, 2);
  //Serial.println(value);
//  if ( data[0] & bit(0) )
//  {
//    uint16_t value;
//    memcpy(&value, data+1, 2);
//
//    Serial.println(value);
//  }
//  else
//  {
//    Serial.println(data[1]);
//  }
}
