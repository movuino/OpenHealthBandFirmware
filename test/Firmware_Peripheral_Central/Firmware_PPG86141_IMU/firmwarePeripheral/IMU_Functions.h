#include <MPU9250.h>

/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);


/*Debug Modes*/
#define debugAcc
#define debugGyr
#define debugMag
#define SampleRateIMU

MPU9250 mpu;
MPU9250Setting setting;

float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t*  raw_data;
uint8_t*  raw_data_mag;

uint8_t bufAcc[11];
uint8_t bufGyro[11];
uint8_t bufMag[10];

/* functions */
void testingSampleRateIMU();
void getDataAcc_Gyr();
void getDataMag();

void configureIMU() {

  Serial.println("####  IMU CONFIG  ####");
  Wire.begin();

  MPU9250Setting setting;
  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_333HZ;
  setting.gyro_fchoice = 0x00;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_184HZ;
  setting.accel_fchoice = 0x01;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_218HZ_0;

  if (!mpu.setup(0x69, setting)) {  // change to your own address
    while (1) {
      Serial.println("MPU connection failed !");
      delay(5000);
    }
  }
  else {
    Serial.println("MPU9250 connection succeeded !");
    errorIMU = false;
  }

  /*Low Power Mode Acc, 10 => 250Hz Output Rate Data*/
  uint8_t waking_up_frequencyLPM = 10;
  mpu.write_byte(0x69, LP_ACCEL_ODR, waking_up_frequencyLPM);
  Serial.println(mpu.read_byte(0x69, LP_ACCEL_ODR));

  Serial.println();
}

void updateIMU() {

  if (mpu.update()) {

    getDataAcc_Gyr();
    getDataMag();

#ifdef BleTest
    if ( Bluefruit.connected()) {
      if (start_stop_Sending == "send") {

        if ( ErrorCharacteristic.notify(bufError, 2) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( AccCharacteristic.notify(bufAcc, 11) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if ( GyroCharacteristic.notify(bufGyro, 11) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if ( MagCharacteristic.notify(bufMag, 10) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
      }
    }
#endif
  }
}

void getDataAcc_Gyr() {
  raw_data =  mpu.raw_data;

#ifdef BleTest
  uint32_t timestamp = millis();
  bufAcc[3] = (uint8_t)timestamp;
  bufAcc[2] = (uint8_t)(timestamp >>= 8);
  bufAcc[1] = (uint8_t)(timestamp >>= 8);
  bufAcc[0] = (uint8_t)(timestamp >>= 8);
  bufAcc[4] = 16;

  for (int i = 5; i <= 10; i++) {
    bufAcc[i] = raw_data[i - 5];
  }
#endif

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef debugAcc
  int16_t v_acc = ((int16_t) raw_data[0] << 8) | (int16_t)raw_data[1];
  aX = (float)v_acc * mpu.get_acc_resolution(setting.accel_fs_sel);

  v_acc = ((int16_t) raw_data[2] << 8) | (int16_t)raw_data[3];
  aY = (float)v_acc * mpu.get_acc_resolution(setting.accel_fs_sel);

  v_acc = ((int16_t) raw_data[4] << 8) | (int16_t)raw_data[5];
  aZ = (float)v_acc * mpu.get_acc_resolution(setting.accel_fs_sel);

  Serial.println("----- Accelerometer data ----- :");
  Serial.print(String(aX));
  Serial.print(" ");
  Serial.print(String(aY));
  Serial.print(" ");
  Serial.print(String(aZ));
  Serial.println(" ");
#endif
#endif

#ifdef BleTest
  uint32_t timestamp1 = millis();
  bufGyro[3] = (uint8_t)timestamp1;
  bufGyro[2] = (uint8_t)(timestamp1 >>= 8);
  bufGyro[1] = (uint8_t)(timestamp1 >>= 8);
  bufGyro[0] = (uint8_t)(timestamp1 >>= 8);
  bufGyro[4] = 16;

  for (int i = 5; i <= 10; i++) {
    bufGyro[i] = raw_data[i + 3];
  }
#endif

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef debugGyr
  int16_t v_gyr = ((int16_t) raw_data[8] << 8) | (int16_t)raw_data[9];
  gX = (float)v_gyr * mpu.get_gyro_resolution(setting.gyro_fs_sel);

  v_gyr = ((int16_t) raw_data[10] << 8) | (int16_t)raw_data[11];
  gY = (float)v_gyr * mpu.get_gyro_resolution(setting.gyro_fs_sel);

  v_gyr = ((int16_t) raw_data[12] << 8) | (int16_t)raw_data[13];
  gZ = (float)v_gyr * mpu.get_gyro_resolution(setting.gyro_fs_sel);

  Serial.println("----- Gyrometer data ----- :");
  Serial.print(String(gX));
  Serial.print(" ");
  Serial.print(String(gY));
  Serial.print(" ");
  Serial.print(String(gZ));
  Serial.println(" ");
#endif
#endif
}

void getDataMag() {

  raw_data_mag =  mpu.raw_data_mag;

#ifdef BleTest
  uint32_t timestamp2 = millis();
  bufMag[3] = (uint8_t)timestamp2;
  bufMag[2] = (uint8_t)(timestamp2 >>= 8);
  bufMag[1] = (uint8_t)(timestamp2 >>= 8);
  bufMag[0] = (uint8_t)(timestamp2 >>= 8);

  for (int i = 4; i <= 9; i++) {
    bufMag[i] = raw_data_mag[i - 4];
  }
#endif

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef debugMag
  float bias_to_current_bits = mpu.get_mag_resolution(setting.mag_output_bits) / mpu.get_mag_resolution(MAG_OUTPUT_BITS::M16BITS);
  int16_t v_mag = ((int16_t) raw_data_mag[1] << 8) | (int16_t)raw_data_mag[0];
  mX = (float) (v_mag * mpu.get_mag_resolution(setting.mag_output_bits) * mpu.mag_bias_factory[0] - mpu.mag_bias[0] * bias_to_current_bits) * mpu.mag_scale[0];

  v_mag = ((int16_t) raw_data_mag[3] << 8) | (int16_t)raw_data_mag[2];
  mY = (float) (v_mag * mpu.get_mag_resolution(setting.mag_output_bits) * mpu.mag_bias_factory[1] - mpu.mag_bias[1] * bias_to_current_bits) * mpu.mag_scale[1];

  v_mag = ((int16_t) raw_data_mag[5] << 8) | (int16_t)raw_data_mag[4];
  mZ = (float) (v_mag * mpu.get_mag_resolution(setting.mag_output_bits) * mpu.mag_bias_factory[2] - mpu.mag_bias[2] * bias_to_current_bits) * mpu.mag_scale[2];

  Serial.println("----- Magnetometer data ----- :");
  Serial.print(String(mX));
  Serial.print(" ");
  Serial.print(String(mY));
  Serial.print(" ");
  Serial.print(String(mZ));
  Serial.println(" ");
#endif
#endif
}


void testingSampleRateIMU() {

  long startTimeIMU = micros();
  long samplesTakenIMU = 0;

  while (samplesTakenIMU < 125) {
    if (mpu.update()) {

      samplesTakenIMU = samplesTakenIMU + 1;
      getDataAcc_Gyr();
      getDataMag();
    }

#ifdef BleTest
    if ( Bluefruit.connected()) {
      if (start_stop_Sending == "send") {

        if ( ErrorCharacteristic.notify(bufError, 2) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

        if ( AccCharacteristic.notify(bufAcc, 11) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if ( GyroCharacteristic.notify(bufGyro, 11) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }
        if ( MagCharacteristic.notify(bufMag, 10) ) {
          //Serial.print("IMUCharacteristic updated to: ");
          //Serial.println(timeStampValue);
        } else {
          //Serial.println("ERROR: Notify not set in the CCCD or not connected!");
        }

      }
    }
#endif
  }

  long endTimeIMU = micros();

  Serial.print("IMU samples avec BLE[");
  Serial.print(samplesTakenIMU);
  Serial.print("]");
  Serial.println();
  Serial.print("IMU Sample Rate : Hz[");
  Serial.print((float)(samplesTakenIMU) / ((endTimeIMU - startTimeIMU) / 1000000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.print("Elapsed Time : Us[");
  Serial.print(endTimeIMU - startTimeIMU);
  Serial.print("]");
  Serial.println();
  Serial.println();

}
