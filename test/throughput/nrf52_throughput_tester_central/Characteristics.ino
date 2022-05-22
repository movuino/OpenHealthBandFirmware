
void ErrorCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Measurement contains of control byte0 and measurement (8 or 16 bit) + optional field
  // if byte0's bit0 is 0 --> measurement is 8 bit, otherwise 16 bit.
  //Serial.print("Characteristic 1 Measurement: ");//
#ifdef IMU9250
  if (data[0] == 1) {
    Serial.println("IMU not detected");
    errorFlag++;
  }
  else Serial.println("IMU Detected");
#endif

#ifdef PPG_Max86141
  if (data[1] == 1) {
    Serial.println("PPG not detected");
    errorFlag++;
  }
  else Serial.println("PPG Detected");
#endif

  if (errorFlag == 0) {
    Serial.println("no error detected disabling notify");
    ErrorCharacteristic.disableNotify();
    delay(50);
    // Bluefruit.disconnect(conn_handle0);
  }
  else {
    Serial.println("error detected ");
  }
}

void AccGyrMag_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  recieved_data++;
  uint32_t  timeStamp = data[0];
  timeStamp = (timeStamp << 8) + data[1];
  timeStamp = (timeStamp << 8) + data[2];
  timeStamp = (timeStamp << 8) + data[3];

  int16_t v_acc = ((int16_t)data[4] << 8) | (int16_t)data[5];
  aX = (float)v_acc * acc_resolution;

  v_acc = ((int16_t)data[6] << 8) | (int16_t)data[7];
  aY = (float)v_acc * acc_resolution;

  v_acc = ((int16_t)data[8] << 8) | (int16_t)data[9];
  aZ = (float)v_acc * acc_resolution;

  //  Serial.println("----- Accelometer data ----- :");
  //  Serial.println(" A " + String( aX) + " " + String( aY) + " " + String( aZ) + " ");

  int16_t v_gyr = ((int16_t) data[10] << 8) | (int16_t)data[11];
  gX = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) data[12] << 8) | (int16_t)data[13];
  gY = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) data[14] << 8) | (int16_t)data[15];
  gZ = (float)v_gyr * gyro_resolution;

  //  Serial.println("----- Gyrometer data ----- :");
  //  Serial.print(" G " + String( gX) + " " + String( gY) + " " + String( gZ));
  //  Serial.println();

  int16_t v_mag = ((int16_t) data[17] << 8) | (int16_t)data[16];
  mX = (float) (v_mag * mag_resolution * mag_bias_factory_0 - mag_bias_0 * bias_to_current_bits) * mag_scale_0;

  v_mag = ((int16_t) data[19] << 8) | (int16_t)data[18];
  mY = (float) (v_mag * mag_resolution * mag_bias_factory_1 - mag_bias_1 * bias_to_current_bits) * mag_scale_1;

  v_mag = ((int16_t) data[21] << 8) | (int16_t)data[20];
  mZ = (float) (v_mag * mag_resolution * mag_bias_factory_2 - mag_bias_2 * bias_to_current_bits) * mag_scale_2;

  //  Serial.println("----- Magnetometer data ----- :");
  //  Serial.print(" M " + String( mX) + " " + String( mY) + " " + String( mZ));
  //  Serial.println();
  //  Serial.println();

}

void PDLEDs_PD1_SNR1_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {

  int timestamp = ( (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0] );

  int dataPD_1 = ( (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4] );

  int dataPD_2 = ( (data[11] << 24) | (data[10] << 16) | (data[9] << 8) | data[8] );

  int dataPD_3 = ( (data[15] << 24) | (data[14] << 16) | (data[13] << 8) | data[12] );

  int dataPD_4 = ( (data[19] << 24) | (data[18] << 16) | (data[17] << 8) | data[16] );

  int snr = ( (data[23] << 24) | (data[22] << 16) | (data[21] << 8) | data[20] );
}




void PDsLED_PD1_PD2_SNR1_SNR2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {

  recieved_dataPDsLED++;

  int timestamp = ( (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0] );

  int dataPD1_1 = ( (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4] );

  int dataPD1_2 = ( (data[11] << 24) | (data[10] << 16) | (data[9] << 8) | data[8] );

  int snr1 = ( (data[15] << 24) | (data[14] << 16) | (data[13] << 8) | data[12] );

  int dataPD2_1 = ( (data[19] << 24) | (data[18] << 16) | (data[17] << 8) | data[16] );

  int dataPD2_2 = ( (data[23] << 24) | (data[22] << 16) | (data[21] << 8) | data[20] );

  int snr2 = ( (data[27] << 24) | (data[26] << 16) | (data[25] << 8) | data[24] );

  int tab[3] = {timestamp, dataPD1_1, dataPD1_2};

  int tab1[3] = {timestamp, dataPD2_1, dataPD2_2};

//  Serial.println("----- PPG data ----- :");
//  Serial.println("##### From PD1 #####:");
//  for (int i = 1; i < 3; i++) {
//    if (tab[i] != 0) {
//      Serial.println(tab[i]);
//    }
//  }
//
//  Serial.println();
//  Serial.println("##### Signal Noise Ratio from PD1 #####:");
//
//  if (snr1 % 100 == 0) {
//    float SNR_neg = snr1;
//    SNR_neg = -(SNR_neg / 10000);
//    Serial.println("Negatif: " + String(SNR_neg));
//  }
//  else {
//    float SNR_pos = snr1;
//    SNR_pos = SNR_pos / 100;
//    Serial.println("Positif: " + String(SNR_pos));
//  }
//
//
//  Serial.println();
//  Serial.println("##### From PD2 #####:");
//  for (int i = 1; i < 3; i++) {
//    if (tab1[i] != 0) {
//      Serial.println(tab1[i]);
//    }
//  }
//
//  Serial.println();
//  Serial.println("##### Signal Noise Ratio from PD2 #####:");
//
//  if (snr2 % 100 == 0) {
//    float SNR_neg = snr2;
//    SNR_neg = -(SNR_neg / 10000);
//    Serial.println("Negatif: " + String(SNR_neg));
//  }
//  else {
//    float SNR_pos = snr2;
//    SNR_pos = SNR_pos / 100;
//    Serial.println("Positif: " + String(SNR_pos));
//  }
//  Serial.println();
//  Serial.println();
}


void PDsLEDs_PD1_PD2_SNR1_SNR2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {

  recieved_dataPDsLEDs++;

  int timestamp = ( (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0] );

  int dataPD1_1 = ( (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4] );

  int dataPD1_2 = ( (data[11] << 24) | (data[10] << 16) | (data[9] << 8) | data[8] );

  int snr1 = ( (data[15] << 24) | (data[14] << 16) | (data[13] << 8) | data[12] );

  int dataPD2_1 = ( (data[19] << 24) | (data[18] << 16) | (data[17] << 8) | data[16] );

  int dataPD2_2 = ( (data[23] << 24) | (data[22] << 16) | (data[21] << 8) | data[20] );

  int snr2 = ( (data[27] << 24) | (data[26] << 16) | (data[25] << 8) | data[24] );

  int tab[3] = {timestamp, dataPD1_1, dataPD1_2};

  int tab1[3] = {timestamp, dataPD2_1, dataPD2_2};

  //  Serial.println("----- PPG data ----- :");
  //  Serial.println("##### From PD1 #####:");
  //  for (int i = 1; i < 3; i++) {
  //    if (tab[i] != 0) {
  //      Serial.println(tab[i]);
  //    }
  //  }
  //
  //  Serial.println();
  //  Serial.println("##### Signal Noise Ratio from PD1 #####:");
  //
  //  if (snr1 % 100 == 0) {
  //    float SNR_neg = snr1;
  //    SNR_neg = -(SNR_neg / 10000);
  //    Serial.println("Negatif: " + String(SNR_neg));
  //  }
  //  else {
  //    float SNR_pos = snr1;
  //    SNR_pos = SNR_pos / 100;
  //    Serial.println("Positif: " + String(SNR_pos));
  //  }
  //
  //
  //  Serial.println();
  //  Serial.println("##### From PD2 #####:");
  //  for (int i = 1; i < 3; i++) {
  //    if (tab1[i] != 0) {
  //      Serial.println(tab1[i]);
  //    }
  //  }
  //
  //  Serial.println();
  //  Serial.println("##### Signal Noise Ratio from PD2 #####:");
  //
  //  if (snr2 % 100 == 0) {
  //    float SNR_neg = snr2;
  //    SNR_neg = -(SNR_neg / 10000);
  //    Serial.println("Negatif: " + String(SNR_neg));
  //  }
  //  else {
  //    float SNR_pos = snr2;
  //    SNR_pos = SNR_pos / 100;
  //    Serial.println("Positif: " + String(SNR_pos));
  //  }
  //  Serial.println();
  //  Serial.println();
}
