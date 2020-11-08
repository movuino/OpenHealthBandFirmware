#include <MPU9250_asukiaaa.h>


MPU9250_asukiaaa mySensor(0x69);
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float accelRange=16.0;
float gyroRange=2000.0;
uint8_t* accelBuf;
uint8_t* gyroBuf;
uint8_t* magBuf;

int dataMode=0; //0 : raw int | 1 : Float 

void setup() {
  Serial.begin(115200);
  Serial.println("started");
  mySensor.beginAccel();
  mySensor.beginGyro();
  mySensor.beginMag();
}

void loop() {
  /*
  uint8_t sensorId;
  if (mySensor.readId(&sensorId) == 0) {
    Serial.println("sensorId: " + String(sensorId));
  } else {
    Serial.println("Cannot read sensorId");
  }
*/
  if (mySensor.accelUpdate() == 0) {
    if(dataMode==0){
         accelBuf=mySensor.accelBuff;
         int16_t v = ((int16_t) accelBuf[0]) << 8 | accelBuf[1];
         aX=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
         v = ((int16_t) accelBuf[2]) << 8 | accelBuf[3];
         aY=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
         v = ((int16_t) accelBuf[4]) << 8 | accelBuf[5];
         aZ=((float) -v) * accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
         }
    else if (dataMode==1){
      aX = mySensor.accelX();
      aY = mySensor.accelY();
      aZ = mySensor.accelZ();
      aSqrt = mySensor.accelSqrt();      
      }
      } else {
    Serial.println("Cannod read accel values");
  }
if (mySensor.gyroUpdate() == 0) {
    if(dataMode==0){
        gyroBuf=mySensor.gyroBuff;
          int16_t v = ((int16_t) gyroBuf[0]) << 8 | gyroBuf[1];
         gX=((float) -v) * gyroRange / (float) 0x8000;
           v = ((int16_t) gyroBuf[2]) << 8 | gyroBuf[3];
         gY=((float) -v) * gyroRange / (float) 0x8000;
          v = ((int16_t) gyroBuf[4]) << 8 | gyroBuf[5];
         gZ=((float) -v) * gyroRange / (float) 0x8000;
    }
    else if (dataMode==1){
          gX = mySensor.gyroX();
      gY = mySensor.gyroY();
      gZ = mySensor.gyroZ(); 
    }
      } else {
    Serial.println("Cannod read gyro values");
  }
 if (mySensor.magUpdate() == 0) {
    if(dataMode==0){
        magBuf=mySensor.magBuff;
        mX=(((int16_t) magBuf[0]) << 8) | magBuf[1];
        mY=(((int16_t) magBuf[2]) << 8) | magBuf[3];
        mZ=(((int16_t) magBuf[4]) << 8) | magBuf[5];
    }
    else if (dataMode==1){
        mX = mySensor.magX();
        mY = mySensor.magY();
        mZ = mySensor.magZ();
    }
      } else {
    Serial.println("Cannot read mag values");
  }



      
  Serial.print("aX: " + String( aX));
  Serial.print(" aY: " + String( aY));
  Serial.println(" aZ: " + String( aZ));
  Serial.print("gX: " + String( gX));
  Serial.print(" gY: " + String( gY));
  Serial.println(" gZ: " + String( gZ));
  Serial.print("mX: " + String( mX));
  Serial.print(" mY: " + String( mY));
  Serial.println(" mZ: " + String( mZ));

 /* if (mySensor.gyroUpdate() == 0) {
    gX = mySensor.gyroX();
    gY = mySensor.gyroY();
    gZ = mySensor.gyroZ();
    Serial.println("gyroX: " + String(gX));
    Serial.println("gyroY: " + String(gY));
    Serial.println("gyroZ: " + String(gZ));
  } else {
    Serial.println("Cannot read gyro values");
  }

  if (mySensor.magUpdate() == 0) {
    mX = mySensor.magX();
    mY = mySensor.magY();
    mZ = mySensor.magZ();
    mDirection = mySensor.magHorizDirection();
    Serial.println("magX: " + String(mX));
    Serial.println("maxY: " + String(mY));
    Serial.println("magZ: " + String(mZ));
    Serial.println("horizontal direction: " + String(mDirection));
  } else {
    Serial.println("Cannot read mag values");
  }

  Serial.println("at " + String(millis()) + "ms");
  Serial.println(""); // Add an empty line*/
  delay(10);
}
