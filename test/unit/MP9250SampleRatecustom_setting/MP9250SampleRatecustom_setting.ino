#include "MPU9250.h"
/*Debug Modes*/
//#define debugAcc
//#define debugGyr
//#define debugMag

MPU9250 mpu;

uint8_t*  raw_data;
uint8_t*  raw_data_mag;

void setup() {
  Serial.begin(115200);
  while (!Serial);
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
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }

  /*Low Power Mode Acc, 10 => 250Hz Output Rate Data*/
  uint8_t waking_up_frequencyLPM = 10;
  mpu.write_byte(0x69, LP_ACCEL_ODR, waking_up_frequencyLPM);
  Serial.println(mpu.read_byte(0x69, LP_ACCEL_ODR));

}

void loop() {

  testingSampleRate();
}

void testingSampleRate() {
  long startTime = micros();
  long samplesTaken = 0;

  while (samplesTaken < 30) {
    if (mpu.update()) {
      samplesTaken = samplesTaken + 1;

      raw_data =  mpu.raw_data;

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

      raw_data_mag =  mpu.raw_data_mag;

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
    }
  }
  long endTime = micros();

  Serial.print("samples[");
  Serial.print(samplesTaken);
  Serial.print("]");
  Serial.println();
  Serial.print("Sample Rate : Hz[");
  Serial.print((float)(samplesTaken) / ((endTime - startTime) / 1000000.0), 2);
  Serial.print("]");
  Serial.println();
  Serial.println();

}
