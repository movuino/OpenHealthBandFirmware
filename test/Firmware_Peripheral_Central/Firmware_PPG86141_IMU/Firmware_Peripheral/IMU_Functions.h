#include <MPU9250.h>

MPU9250 mpu;
MPU9250Setting setting;

long startTimeImu;
long samplesTakenImu = 0;

float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t*  raw_data;
uint8_t*  raw_data_mag;

float acc_resolution = 0.00049;
float gyro_resolution = 0.06;
float mag_resolution = 1.50;
float mag_bias_factory_0 = 1.18, mag_bias_factory_1 = 1.19, mag_bias_factory_2 = 1.14;
float mag_scale_0 = 1.00, mag_scale_1 = 1.00, mag_scale_2 = 1.00;
float mag_bias_0 = 0.00, mag_bias_1 = 0.00, mag_bias_2 = 0.00;
float bias_to_current_bits = 1.00;

uint8_t bufAcc[11];
uint8_t bufGyro[11];
uint8_t bufMag[10];

/*Debug Modes*/
//#define debugAcc
//#define debugGyr
//#define debugMag
//#define SampleRateIMU

void configureIMU() {

  Serial.println("####  IMU CONFIG ####");
  Wire.begin();

  setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
  setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
  setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
  setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_125HZ;
  setting.gyro_fchoice = 0x00;
  setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
  setting.accel_fchoice = 0x00;
  setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;

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

  startTimeImu = millis();

  Serial.println();

}
void updateIMU() {
  if (mpu.update()) {

    raw_data =  mpu.raw_data;

    /*
      Serial.println("Acc resolution : " + String(mpu.get_acc_resolution(setting.accel_fs_sel)));
      Serial.println("Gyro resolution: " + String(mpu.get_gyro_resolution(setting.gyro_fs_sel)));
      Serial.println("Mag resolution: " + String(mpu.get_mag_resolution(setting.mag_output_bits)));
      Serial.println("Mag bias factory 0: " + String(mpu.mag_bias_factory[0]));
      Serial.println("Mag bias 0: " + String(mpu.mag_bias[0]));
      Serial.println("Mag scale 0: " + String(mpu.mag_scale[0]));

      Serial.println("Mag bias factory 1: " + String(mpu.mag_bias_factory[1]));
      Serial.println("Mag bias 1: " + String(mpu.mag_bias[1]));
      Serial.println("Mag scale 1: " + String(mpu.mag_scale[1]));

      Serial.println("Mag bias factory 2: " + String(mpu.mag_bias_factory[2]));
      Serial.println("Mag bias 2: " + String(mpu.mag_bias[2]));
      Serial.println("Mag scale 2: " + String(mpu.mag_scale[2]));
    */

    uint32_t timestamp = millis();
    bufAcc[3] = (uint8_t)timestamp;
    bufAcc[2] = (uint8_t)(timestamp >>= 8);
    bufAcc[1] = (uint8_t)(timestamp >>= 8);
    bufAcc[0] = (uint8_t)(timestamp >>= 8);
    bufAcc[4] = 16;

    for (int i = 5; i <= 10; i++) {
      bufAcc[i] = raw_data[i - 5];
    }

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

    uint32_t timestamp1 = millis();
    bufGyro[3] = (uint8_t)timestamp1;
    bufGyro[2] = (uint8_t)(timestamp1 >>= 8);
    bufGyro[1] = (uint8_t)(timestamp1 >>= 8);
    bufGyro[0] = (uint8_t)(timestamp1 >>= 8);
    bufGyro[4] = 16;

    for (int i = 5; i <= 10; i++) {
      bufGyro[i] = raw_data[i + 3];
    }

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

    uint32_t timestamp2 = millis();
    bufMag[3] = (uint8_t)timestamp2;
    bufMag[2] = (uint8_t)(timestamp2 >>= 8);
    bufMag[1] = (uint8_t)(timestamp2 >>= 8);
    bufMag[0] = (uint8_t)(timestamp2 >>= 8);

    raw_data_mag =  mpu.raw_data_mag;

    for (int i = 4; i <= 9; i++) {
      bufMag[i] = raw_data_mag[i - 4];
    }

#ifdef debugMag
    //float bias_to_current_bits = mpu.get_mag_resolution(setting.mag_output_bits) / mpu.get_mag_resolution(MAG_OUTPUT_BITS::M16BITS);
    //Serial.println("bias_to_current_bits : " + String(bias_to_current_bits));

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

#ifdef SampleRateIMU
    samplesTakenImu = samplesTakenImu + 1;
    Serial.println();
    Serial.print("Sample Rate : Hz[");
    Serial.print((float)(samplesTakenImu) / ((millis() - startTimeImu) / 1000.0), 2);
    Serial.print("]");
    Serial.println();
    Serial.println();
#endif
  }
}
