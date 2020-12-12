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
    Bluefruit.disconnect(conn_handle0);
  }
  else {
    Serial.println("error detected ");
  }
}
