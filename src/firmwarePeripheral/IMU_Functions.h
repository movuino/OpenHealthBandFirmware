#include <MPU9250.h>

/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccGyrMagCharacteristic = BLECharacteristic(0x1102);


/*Debug Modes*/
#define debugAcc
#define debugGyr
#define debugMag

MPU9250 mpu;
MPU9250Setting setting;

float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;

float acc_resolution = 0.00049;
float gyro_resolution = 0.06;
float mag_resolution = 1.50;
float mag_bias_factory_0 = 1.18, mag_bias_factory_1 = 1.19, mag_bias_factory_2 = 1.14;
float mag_scale_0 = 1.00, mag_scale_1 = 1.00, mag_scale_2 = 1.00;
float mag_bias_0 = 0.00, mag_bias_1 = 0.00, mag_bias_2 = 0.00;
float bias_to_current_bits = 1.00;

uint8_t*  raw_data;
uint8_t*  raw_data_mag;
uint8_t bufAccGyrMag[22];

/* functions */
void getDataAcc_Gyr();
void getDataMag();

// constants won't change:
const long interval = 8;                  // interval at which to take data (milliseconds)
unsigned long previousMillis = 0;        // will store last time data was updated

void configureIMU() {

  Serial.println("####  IMU CONFIG  ####");
  Wire.begin();

  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_125HZ;
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
  bufError[0] = errorIMU;

  /*Low Power Mode Acc, 9 => 125Hz Output Rate Data*/
  uint8_t waking_up_frequencyLPM = 9;
  mpu.write_byte(0x69, LP_ACCEL_ODR, waking_up_frequencyLPM);

  Serial.println();
}

void updateIMU() {

#ifdef BleTest
  if ( Bluefruit.connected()) {
    ssCommand = StartCharacteristic.read8();

    if (ssCommand == 1) { // Received 1 from Central to start sending data

      if (shutdown_or_restartIMU == 1) { // the sensor was shutdown
        //Init IMU/
        configureIMU();

        if (!errorIMU) {
          if (mpu.update()) {
            getDataAcc_Gyr();
            getDataMag();
          }
        }
        shutdown_or_restartIMU = 0;
      }
      else if (shutdown_or_restartIMU == 3) { // first Start when central notify
        // Init IMU //
        configureIMU();
        shutdown_or_restartIMU = 0;
        firstStartIMU = 1;
      }

      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        if (!errorIMU) {
          if (mpu.update()) {
            getDataAcc_Gyr();
            getDataMag();
          }

          CHECK_NOTIFICATION(AccGyrMagCharacteristic.notify(bufAccGyrMag, 22))

        }
      }
    }
  }
#endif
}

void getDataAcc_Gyr() {
  raw_data =  mpu.raw_data;

#ifdef BleTest
  uint32_t timestamp = millis();

  bufAccGyrMag[3] = (uint8_t)timestamp;
  bufAccGyrMag[2] = (uint8_t)(timestamp >>= 8);
  bufAccGyrMag[1] = (uint8_t)(timestamp >>= 8);
  bufAccGyrMag[0] = (uint8_t)(timestamp >>= 8);

  for (int i = 4; i <= 9; i++) {
    bufAccGyrMag[i] = raw_data[i - 4];
  }
#endif

#ifdef SerialTest
#ifdef debugAcc
  int16_t v_acc = ((int16_t) raw_data[0] << 8) | (int16_t)raw_data[1];
  aX = (float)v_acc * acc_resolution;

  v_acc = ((int16_t) raw_data[2] << 8) | (int16_t)raw_data[3];
  aY = (float)v_acc * acc_resolution;

  v_acc = ((int16_t) raw_data[4] << 8) | (int16_t)raw_data[5];
  aZ = (float)v_acc * acc_resolution;

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
  for (int i = 10; i <= 15; i++) {
    bufAccGyrMag[i] = raw_data[i - 2];
  }
#endif

#ifdef SerialTest
#ifdef debugGyr
  int16_t v_gyr = ((int16_t) raw_data[8] << 8) | (int16_t)raw_data[9];
  gX = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) raw_data[10] << 8) | (int16_t)raw_data[11];
  gY = (float)v_gyr * gyro_resolution;

  v_gyr = ((int16_t) raw_data[12] << 8) | (int16_t)raw_data[13];
  gZ = (float)v_gyr * gyro_resolution;

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
  for (int i = 16; i <= 21; i++) {
    bufAccGyrMag[i] = raw_data_mag[i - 16];
  }
#endif

  //---------------------------- Serial Communication -------------------------------------//
#ifdef SerialTest
#ifdef debugMag
  int16_t v_mag = ((int16_t) raw_data_mag[1] << 8) | (int16_t)raw_data_mag[0];
  mX = (float) (v_mag * mag_resolution * mag_bias_factory_0 - mag_bias_0 * bias_to_current_bits) * mag_scale_0;

  v_mag = ((int16_t) raw_data_mag[3] << 8) | (int16_t)raw_data_mag[2];
  mY = (float) (v_mag * mag_resolution * mag_bias_factory_1 - mag_bias_1 * bias_to_current_bits) * mag_scale_1;

  v_mag = ((int16_t) raw_data_mag[5] << 8) | (int16_t)raw_data_mag[4];
  mZ = (float) (v_mag * mag_resolution * mag_bias_factory_2 - mag_bias_2 * bias_to_current_bits) * mag_scale_2;

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
