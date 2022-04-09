#include "MPU9250.h"

MPU9250 mpu;
long startTime;
long samplesTaken = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Wire.begin();

  MPU9250Setting setting;
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
      Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
      delay(5000);
    }
  }
  uint8_t waking_up_frequencyLPM = 9;
  mpu.write_byte(0x69, LP_ACCEL_ODR, waking_up_frequencyLPM);
  Serial.println(mpu.read_byte(0x69, LP_ACCEL_ODR));

  startTime = millis();
}

void loop() {
  if (mpu.update()) {
    samplesTaken = samplesTaken + 1;
    Serial.print("Sample Rate : Hz[");
    Serial.print((float)(samplesTaken) / ((millis() - startTime) / 1000.0), 2);
    Serial.print("]");
    Serial.println();
    Serial.println();
    print_roll_pitch_yaw();
  }
}

void print_roll_pitch_yaw() {
  Serial.print("Yaw, Pitch, Roll: ");
  Serial.print(mpu.getYaw(), 2);
  Serial.print(", ");
  Serial.print(mpu.getPitch(), 2);
  Serial.print(", ");
  Serial.println(mpu.getRoll(), 2);
}
