void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  /*Serial.print("Characteristic 1 Measurement: ");*/
  
  if (data[0] == 1) {
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
