void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  /*Serial.print("Characteristic 1 Measurement: ");*/
  
  if (data[1] == 1) {
    Serial.println("PPG not detected");
    errorFlag++;
  }
  else Serial.println("PPG Detected");

  if (errorFlag == 0) {
    Serial.println("no error detected disabling notify");
    ErrorCharacteristic.disableNotify();
    // Bluefruit.disconnect(conn_handle0);
  }
  else {
    Serial.println("error detected ");
  }
}

void PPGMax86_ledSeq1A_PPG1_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("tp1 : "+String(tp1));
  float bat = tp1/100;
  Serial.println(bat);
  
}
