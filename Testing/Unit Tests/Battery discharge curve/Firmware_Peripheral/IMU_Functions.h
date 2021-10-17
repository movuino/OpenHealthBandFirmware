#include <MPU9250_asukiaaa.h>

/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);


MPU9250_asukiaaa mySensor(0x69);
//float aX, aY, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
uint32_t  aX, aY, aZ;
float aX2, aY2, aZ2, gX2, gY2, gZ2;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t   accScale = 16;
uint8_t   gyroScale = 2000;

//bool dataReady = false;

float accelRange = 16.0;
//todo : getter and setter for scales

bool errorIMU = false;
uint8_t bufAcc[11];
uint8_t bufGyro[11];
uint8_t bufMag[10];

uint8_t bufAcc_Gy_Ma[32];
/*Debug Modes*/
//#define debugAcc1;
//#define debugAcc2;

void configureIMU() {
  /*Read sensorId*/
  uint8_t sensorId;

  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  }
  else {
    Serial.println("Error cannot read sensor ID");
    errorIMU = true;
  }
  /*Begin IMU*/
  if (!errorIMU) {
    mySensor.beginAccel();
    mySensor.beginGyro();
    mySensor.beginMag();
  }
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

    /////////////////////////////////
    bufAcc_Gy_Ma[3] = (uint8_t)timestamp;
    bufAcc_Gy_Ma[2] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[1] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[0] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[4] = 16;
    /////////////////////////////////

    for (int i = 5; i <= 10; i++) {
      bufAcc[i] = accelBuf[i - 5];
      /////////////////////////////
      bufAcc_Gy_Ma[i] = accelBuf[i - 5];
      /////////////////////////////
#ifdef debugAcc2
      Serial.print(String(i));
      Serial.print(":");
      Serial.print(String(bufAcc[i]));
      Serial.print(" ");
#endif
    }
#ifdef debugAcc1
    Serial.println(" ");

    int16_t v = ((int16_t) accelBuf[0]) << 8 | accelBuf[1];
    aX2 = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0

    v = ((int16_t) accelBuf[2]) << 8 | accelBuf[3];
    aY2 = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0

    v = ((int16_t) accelBuf[4]) << 8 | accelBuf[5];
    aZ2 = ((float) - v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0


    Serial.print(String(aX2));
    Serial.print(" ");
    Serial.print(String(aY2));
    Serial.print(" ");
    Serial.print(String(aZ2));
    Serial.println(" ");
#endif
    dataReady = true;
  } else {
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
    bufGyro[4] = 2000;
    //////////////////////////////
    bufAcc_Gy_Ma[14] = (uint8_t)timestamp;
    bufAcc_Gy_Ma[13] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[12] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[11] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[15] = 2000;

    for (int i = 16; i <= 21; i++) {
      bufAcc_Gy_Ma[i] = gyroBuf[i - 16];
    }
    ///////////////////////////////

    for (int i = 5; i <= 10; i++) {
      bufGyro[i] = gyroBuf[i - 5];
    }


    int16_t v = ((int16_t) gyroBuf[0]) << 8 | gyroBuf[1];

    gX2 = ((float) - v) * gyroScale / (float) 0x8000;
    //Serial.println("gX2: "+String(gX2));
    v = ((int16_t) gyroBuf[2]) << 8 | gyroBuf[3];
    gY2 = ((float) - v) * gyroScale / (float) 0x8000;
    //Serial.println("gY2: "+String(gY2));
    v = ((int16_t) gyroBuf[4]) << 8 | gyroBuf[5];
    gZ2 = ((float) - v) * gyroScale / (float) 0x8000;
    //Serial.println("gZ2: "+String(gZ2));
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

    //////////////////////////////
    bufAcc_Gy_Ma[25] = (uint8_t)timestamp;
    bufAcc_Gy_Ma[24] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[23] = (uint8_t)(timestamp >>= 8);
    bufAcc_Gy_Ma[22] = (uint8_t)(timestamp >>= 8);

    for (int i = 26; i <= 31; i++) {
      bufAcc_Gy_Ma[i] = magBuf[i - 26];
    }
    /////////////////////////////
    for (int i = 4; i <= 9; i++) {
      bufMag[i] = magBuf[i - 4];
#ifdef debugMag
      Serial.println(String(bufMag[i]));
#endif
    }

  } else {
    Serial.println("Cannod read accel values");
  }
}
