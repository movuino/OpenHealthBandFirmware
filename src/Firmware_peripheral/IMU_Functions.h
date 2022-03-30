#include <MPU9250_asukiaaa.h>
MPU9250_asukiaaa mySensor(0x69);

float aX, aY, aZ, gX, gY, gZ, mX, mY, mZ;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t   accScale = 16;
int   gyroScale = 2000;

float accelRange = 16.0;

uint8_t bufAcc[11];
uint8_t bufGyro[11];
uint8_t bufMag[10];

/*Debug Modes*/
#define debugAcc1
#define debugGyr
#define debugMag
//#define debugAcc2

void configureIMU() {

  Serial.println("####  IMU CONFIG ####");
  /*Read sensorId*/
  uint8_t sensorId;

  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("MPU9250 connection succeeded !");
    Serial.println("sensorId: " + String(sensorId));
    errorIMU = false;
  }
  else {
    Serial.println("Error cannot read sensor ID");
  }

  /*Begin IMU*/
  if (!errorIMU) {
    mySensor.beginAccel();
    mySensor.beginGyro();
    mySensor.beginMag();
  }
  Serial.println();

}
void updateAcc() {
  if (mySensor.accelUpdate() == 0) {
    //read sensor
    accelBuf = mySensor.accelBuff;
    //read timestamp
    uint32_t timestamp = millis();
    bufAcc[3] = (uint8_t)timestamp;
    bufAcc[2] = (uint8_t)(timestamp >>= 8);
    bufAcc[1] = (uint8_t)(timestamp >>= 8);
    bufAcc[0] = (uint8_t)(timestamp >>= 8);
    bufAcc[4] = 16;

    for (int i = 5; i <= 10; i++) {
      bufAcc[i] = accelBuf[i - 5];

#ifdef debugAcc2
      Serial.print(String(i));
      Serial.print(":");
      Serial.print(String(bufAcc[i]));
      Serial.print(" ");
#endif
    }

    int16_t v = ((int16_t) accelBuf[0]) << 8 | accelBuf[1];
    aX = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0

    v = ((int16_t) accelBuf[2]) << 8 | accelBuf[3];
    aY = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0

    v = ((int16_t) accelBuf[4]) << 8 | accelBuf[5];
    aZ = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0

#ifdef debugAcc1
    Serial.println("----- Accelerometer data ----- :");
    Serial.print(String(aX));
    Serial.print(" ");
    Serial.print(String(aY));
    Serial.print(" ");
    Serial.print(String(aZ));
    Serial.println(" ");
#endif

  }
  else {
    Serial.println("Cannot read accel values");
  }
}
void updateGyro() {
  if (mySensor.gyroUpdate() == 0) {
    gyroBuf = mySensor.gyroBuff;
    uint32_t timestamp = millis();
    bufGyro[3] = (uint8_t)timestamp;
    bufGyro[2] = (uint8_t)(timestamp >>= 8);
    bufGyro[1] = (uint8_t)(timestamp >>= 8);
    bufGyro[0] = (uint8_t)(timestamp >>= 8);
    //bufGyro[4] = 2000;
    bufGyro[4] = 16;

    for (int i = 5; i <= 10; i++) {
      bufGyro[i] = gyroBuf[i - 5];
    }

    int16_t v = ((int16_t) gyroBuf[0]) << 8 | gyroBuf[1];
    gX = ((float) - v) * gyroScale / (float) 0x8000;

    v = ((int16_t) gyroBuf[2]) << 8 | gyroBuf[3];
    gY = ((float) - v) * gyroScale / (float) 0x8000;

    v = ((int16_t) gyroBuf[4]) << 8 | gyroBuf[5];
    gZ = ((float) - v) * gyroScale / (float) 0x8000;

#ifdef debugGyr
    Serial.println("----- Gyrometer data ----- :");
    Serial.print(String(gX));
    Serial.print(" ");
    Serial.print(String(gY));
    Serial.print(" ");
    Serial.print(String(gZ));
    Serial.println(" ");
#endif

  } else {
    Serial.println("Cannod read accel values");
  }
}

void updateMag() {
  if (mySensor.magUpdate() == 0) {
    magBuf = mySensor.magBuff;
    uint32_t timestamp = millis();
    bufMag[3] = (uint8_t)timestamp;
    bufMag[2] = (uint8_t)(timestamp >>= 8);
    bufMag[1] = (uint8_t)(timestamp >>= 8);
    bufMag[0] = (uint8_t)(timestamp >>= 8);

    for (int i = 4; i <= 9; i++) {
      bufMag[i] = magBuf[i - 4];
    }

    int16_t v = ((int16_t) magBuf[1]) << 8 | magBuf[0];
    mX = ((float) v);

    v = ((int16_t) magBuf[3]) << 8 | magBuf[2];
    mY = ((float) v);

    v = ((int16_t) magBuf[5]) << 8 | magBuf[4];
    mZ = ((float) v);

#ifdef debugMag
    Serial.println("----- Magnetometer data ----- :");
    Serial.print(String(mX));
    Serial.print(" ");
    Serial.print(String(mY));
    Serial.print(" ");
    Serial.print(String(mZ));
    Serial.println(" ");
#endif


  } else {
    Serial.println("Cannod read accel values");
  }
  Serial.println();

}
