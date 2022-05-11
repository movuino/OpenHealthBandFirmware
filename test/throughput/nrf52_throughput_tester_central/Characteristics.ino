
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


void AccCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  recieved_data++;
  uint8_t  timeStamp = data[0];
  timeStamp = (timeStamp  << 8) + data[1];
  timeStamp = (timeStamp << 8) + data[2];
  timeStamp = (timeStamp << 8) + data[3];

  int16_t v_acc = ((int16_t)data[5] << 8) | (int16_t)data[6];
  aX = (float)v_acc * acc_resolution;

  v_acc = ((int16_t)data[7] << 8) | (int16_t)data[8];
  aY = (float)v_acc * acc_resolution;

  v_acc = ((int16_t)data[9] << 8) | (int16_t)data[10];
  aZ = (float)v_acc * acc_resolution;

  //  Serial.println("----- Accelometer data ----- :");
  //  Serial.println(" A " + String( aX) + " " + String( aY) + " " + String( aZ) + " ");
}

void  GyroCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  uint8_t timeStamp = data[0];
  timeStamp = (timeStamp  << 8) + data[1];
  timeStamp = (timeStamp << 8) + data[2];
  timeStamp = (timeStamp << 8) + data[3];

  int16_t v_gyr = ((int16_t) data[5] << 8) | (int16_t)data[6];
  gX = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) data[7] << 8) | (int16_t)data[8];
  gY = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) data[9] << 8) | (int16_t)data[10];
  gZ = (float)v_gyr * gyro_resolution;

  //  Serial.println("----- Gyrometer data ----- :");
  //  Serial.print(" G " + String( gX) + " " + String( gY) + " " + String( gZ));
  //  Serial.println();
}

void  MagCharacteristic_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
  uint8_t timeStamp = data[0];
  timeStamp = (timeStamp  << 8) + data[1];
  timeStamp = (timeStamp << 8) + data[2];
  timeStamp = (timeStamp << 8) + data[3];

  int16_t v_mag = ((int16_t) data[5] << 8) | (int16_t)data[4];
  mX = (float) (v_mag * mag_resolution * mag_bias_factory_0 - mag_bias_0 * bias_to_current_bits) * mag_scale_0;

  v_mag = ((int16_t) data[7] << 8) | (int16_t)data[6];
  mY = (float) (v_mag * mag_resolution * mag_bias_factory_1 - mag_bias_1 * bias_to_current_bits) * mag_scale_1;

  v_mag = ((int16_t) data[9] << 8) | (int16_t)data[8];
  mZ = (float) (v_mag * mag_resolution * mag_bias_factory_2 - mag_bias_2 * bias_to_current_bits) * mag_scale_2;

  //  Serial.println("----- Magnetometer data ----- :");
  //  Serial.print(" M " + String( mX) + " " + String( mY) + " " + String( mZ));
  //  Serial.println();
  //  Serial.println();
}

// void PPGMax86_LED1_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
//   uint32_t tp1;
//   tp1 = data[0];
//   tp1 = (tp1  << 8) + data[1];
//   tp1 = (tp1  << 8) + data[2];
//   tp1 = (tp1  << 8) + data[3];

//   uint32_t  tp2 = data[4];
//   tp2 = (tp2  << 8) + data[5];
//   tp2 = (tp2  << 8) + data[6];
//   tp2 = (tp2  << 8) + data[7];

//   uint32_t tp3 = data[8];
//   tp3 = (tp3  << 8) + data[9];
//   tp3 = (tp3  << 8) + data[10]; 
//   tp3 = (tp3  << 8) + data[11];


// #ifdef PDLEDs
//   uint32_t tp4 = data[12];
//   tp4 = (tp4  << 8) + data[13];
//   tp4 = (tp4  << 8) + data[14];
//   tp4 = (tp4  << 8) + data[15];

//   uint32_t tp5 = data[16];
//   tp5 = (tp5  << 8) + data[17];
//   tp5 = (tp5  << 8) + data[18];
//   tp5 = (tp5  << 8) + data[19];
// #endif

//   int samples;
// #ifndef PDLEDs
//   samples = 3;
//   uint32_t tab[3] = {tp1, tp2, tp3};
// #else
//   samples = 5;
//   uint32_t tab[5] = {tp1, tp2, tp3, tp4, tp5};
// #endif

// Serial.println("----- PPG data ----- :");
// Serial.printf("%d samples :", samples);
// for (uint32_t i = 1; i < samples; i++) {
//   if (tab[i] != 0) {
//     Serial.println(tab[i]);
//   }
// }
// Serial.println();
// Serial.println();

// }

void PPGMax86_ledSeq1A_PPG1_1_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("tp1 - timestamp: "+String(tp1));

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

  uint32_t tp5 = data[16];
  tp5 = (tp5  << 8) + data[17];
  tp5 = (tp5  << 8) + data[18];
  tp5 = (tp5  << 8) + data[19];
  //Serial.println("tp5 : "+String(tp5));

  uint32_t tab[5] = {tp1, tp2, tp3, tp4, tp5};

  //  Serial.println("----- PPG data ----- :");
  //  Serial.println("4 samples :");
  //  for (uint32_t i = 1; i < 5; i++) {
  //    if (tab[i] != 0) {
  //      Serial.println(tab[i]);
  //    }
  //  }
  //  Serial.println();
  //  Serial.println();
}

void PPGMax86_SNR1_1_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];

  //Serial.println("Signal Noise Ratio");

  if (tp1 % 100 == 0) {
    float SNR_neg = tp1;
    SNR_neg = -(SNR_neg / 10000);
    //Serial.println("Négatif: "+String(SNR_neg));
  }
  else {
    float SNR_pos = tp1;
    SNR_pos = SNR_pos / 100;
    //Serial.println("Positif: "+String(SNR_pos));}
  }
}

void PPGMax86_ledSeq1A_PPG1_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("tp1 - timestamp: "+String(tp1));

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

  uint32_t tab[3] = {tp1, tp2, tp3};

  //  Serial.println("----- PPG data ----- :");
  //  Serial.println("4 samples :");
  //  for (uint32_t i = 1; i < 3; i++) {
  //    if (tab[i] != 0) {
  //      Serial.println(tab[i]);
  //    }
  //  }
}

void PPGMax86_ledSeq1A_PPG2_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;

  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("--- timestamp "+String(tp1));

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

  uint32_t tab[3] = {tp1, tp2, tp3};

  //  for (uint32_t i = 1; i < 3; i++) {
  //    Serial.println(tab[i]);
  //  }
  //  Serial.println();
  //  Serial.println();
}

void PPGMax86_SNR1_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];

  //Serial.println("Signal Noise Ratio");

  if (tp1 % 100 == 0) {
    float SNR_neg = tp1;
    SNR_neg = -(SNR_neg / 10000);
    //Serial.println("Négatif: "+String(SNR_neg));
  }
  else {
    float SNR_pos = tp1;
    SNR_pos = SNR_pos / 100;
    //Serial.println("Positif: "+String(SNR_pos));}
  }
}
void PPGMax86_SNR2_2_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];

  // Serial.println(tp1);
  if (tp1 % 100 == 0) {
    float SNR_neg = tp1;
    SNR_neg = -(SNR_neg / 10000);
    //Serial.println("Négatif: "+String(SNR_neg));
  }
  else {
    float SNR_pos = tp1;
    SNR_pos = SNR_pos / 100;
    //Serial.println("Positif: "+String(SNR_pos));}
  }
}

void PPGMax86_ledSeq1A_PPG1_3_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("tp1 - timestamp: "+String(tp1));

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

  uint32_t tab[3] = {tp1, tp2, tp3};

  //  Serial.println("----- PPG data ----- :");
  //  Serial.println("4 samples :");
  //  for (uint32_t i = 1; i < 3; i++) {
  //    if (tab[i] != 0) {
  //      Serial.println(tab[i]);
  //    }
  //  }
}

void PPGMax86_ledSeq1A_PPG2_3_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;

  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];
  //Serial.println("--- timestamp "+String(tp1));

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

  uint32_t tab[3] = {tp1, tp2, tp3};

  //  for (uint32_t i = 1; i < 3; i++) {
  //    Serial.println(tab[i]);
  //  }
  //  Serial.println();
  //  Serial.println();
}

void PPGMax86_SNR1_3_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];

  //Serial.println("Signal Noise Ratio");

  if (tp1 % 100 == 0) {
    float SNR_neg = tp1;
    SNR_neg = -(SNR_neg / 10000);
    //Serial.println("Négatif: "+String(SNR_neg));
  }
  else {
    float SNR_pos = tp1;
    SNR_pos = SNR_pos / 100;
    //Serial.println("Positif: "+String(SNR_pos));}
  }
}
void PPGMax86_SNR2_3_notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len) {
  uint32_t tp1;
  tp1 = data[0];
  tp1 = (tp1  << 8) + data[1];
  tp1 = (tp1  << 8) + data[2];
  tp1 = (tp1  << 8) + data[3];

  // Serial.println(tp1);
  if (tp1 % 100 == 0) {
    float SNR_neg = tp1;
    SNR_neg = -(SNR_neg / 10000);
    //Serial.println("Négatif: "+String(SNR_neg));
  }
  else {
    float SNR_pos = tp1;
    SNR_pos = SNR_pos / 100;
    //Serial.println("Positif: "+String(SNR_pos));}
  }
}
