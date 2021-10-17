#include <bluefruit.h>
#include <MPU9250_asukiaaa.h>
#include <Wire.h>

#include <TensorFlowLite.h>
#include "tensorflow/lite/experimental/micro/kernels/micro_ops.h"
#include "tensorflow/lite/experimental/micro/micro_error_reporter.h"
#include "tensorflow/lite/experimental/micro/micro_interpreter.h"
#include "tensorflow/lite/experimental/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"
#include "model.h"


MPU9250_asukiaaa mySensor(0x69);
uint32_t  aX, aY, aZ;
float aX2, aY2, aZ2, gX2, gY2, gZ2;
uint8_t*  accelBuf;
uint8_t*  gyroBuf;
uint8_t*  magBuf;
uint8_t   accScale = 16;
uint8_t   gyroScale = 2000;
//error codes
bool errorIMU = false;
bool errorTens = false;
uint8_t bufError[0];

float accelRange = 16.0;
//todo : getter and setter for scales
/*Debug Modes*/
#define debugAcc1;
//#define debugMag;


/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);


/*IMU Service & characteristics*/
BLEService IMUService = BLEService(0x1101);
BLECharacteristic AccCharacteristic = BLECharacteristic(0x1102);
BLECharacteristic GyroCharacteristic = BLECharacteristic(0x1103);
BLECharacteristic MagCharacteristic = BLECharacteristic(0x1104);

BLEService        TensorFlowService = BLEService(0x1300);
BLECharacteristic PossCharacteristic = BLECharacteristic(0x1301);

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* tflInputTensor = nullptr;
TfLiteTensor* tflOutputTensor = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 8 * 1024;
alignas(8) uint8_t tensor_arena[kTensorArenaSize];
}  // namespace
const float accelerationThreshold = 1; // threshold of significant in G's
const int numSamples = 50;

int imucounter;

//tinyml variables
const char* GESTURES[] = {
  "flap",
  "still",
  "walk"
};

#define NUM_GESTURES (sizeof(GESTURES) / sizeof(GESTURES[0]))
float possibility[NUM_GESTURES];
int poss[NUM_GESTURES];
uint8_t* p;




BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas  blebas;  // battery
BLEDfu  bledfu;  // OTA DFU service
BLEConnection* connection ;
uint32_t timeStampValue = 0;
long startTimer = 0;



int counter = 0;
int counterG;
uint8_t buf[16];
uint8_t bufAcc[10];
uint8_t bufGyro[10];
uint8_t bufMag[10];
uint32_t bufTemp[9];
uint64_t bufSix_Nine_RAM;
uint8_t* buffData;

int buflen = 16;
bool connected = false;
unsigned int timer0;
uint16_t bufData[8];
bool dataReady = false;

int samplingRateHz = 100 ;
int samplingRateMs = 1000 / 10;

int mtu = 0;
int phy = 0;
int interval = 0;

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
    for (int i = 5; i <= 10; i++) {
      bufAcc[i] = accelBuf[i - 5];
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

    for (int i = 5; i <= 10; i++) {
      bufGyro[i] = gyroBuf[i - 5];
    }

    int16_t v = ((int16_t) gyroBuf[0]) << 8 | gyroBuf[1];
    gX2 = ((float) - v) * gyroScale / (float) 0x8000;
    v = ((int16_t) gyroBuf[2]) << 8 | gyroBuf[3];
    gY2 = ((float) - v) * gyroScale / (float) 0x8000;
    v = ((int16_t) gyroBuf[4]) << 8 | gyroBuf[5];
    gZ2 = ((float) - v) * gyroScale / (float) 0x8000;


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
#ifdef debugMag
      Serial.println(String(bufMag[i]));
#endif
    }

  } else {
    Serial.println("Cannod read accel values");
  }
}


void setup() {

  Serial.begin(115200);
  //while ( !Serial )
  delay(10);   // for nrf52840 with native usb

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Serial.println("-----------------------\n");

  // Initialise the Bluefruit module
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.autoConnLed(true);
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
  Bluefruit.setTxPower(13);
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.setName("Movuino OHB - 000");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  // Start the BLE Battery Service and set it to 100%
  Serial.println("Configuring the Battery Service");
  blebas.begin();
  blebas.write(100);

  // Setup services
  setupIMUService();
  setupTensorFlowService();
  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();

  Serial.println("Ready Player One!!!");
  Serial.println("\nAdvertising");

  /*init IMU*/
  configureIMU();


  bufError[0] = errorTens;
  // Setup the Custom Service BLEService and BLECharacteristic
  //Serial.println("Configuring the Custom Service");
  setupErrorService();

  // Setup the advertising packet(s)
  ErrorCharacteristic.write(bufError, 1);

  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_model);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    error_reporter->Report(
      "Model provided is schema version %d not equal "
      "to supported version %d.",
      model->version(), TFLITE_SCHEMA_VERSION);
    return;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver resolver;
  resolver.AddBuiltin(
    tflite::BuiltinOperator_DEPTHWISE_CONV_2D,
    tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
  resolver.AddBuiltin(
    tflite::BuiltinOperator_MAX_POOL_2D,
    tflite::ops::micro::Register_MAX_POOL_2D());
  resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D,
                      tflite::ops::micro::Register_CONV_2D());
  resolver.AddBuiltin(
    tflite::BuiltinOperator_FULLY_CONNECTED,
    tflite::ops::micro::Register_FULLY_CONNECTED());
  resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX,
                      tflite::ops::micro::Register_SOFTMAX());
  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    //TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    error_reporter->Report("Set up failed\n");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  tflInputTensor = interpreter->input(0);
  tflOutputTensor = interpreter->output(0);
}

void loop() {


  if (!errorIMU) {
    updateAcc();
    updateGyro();
    //updateMag();
  }

  int samplesRead = 50;
  Serial.println("sampleRead: " + String(samplesRead));
  Serial.println("numSamples: " + String(numSamples));
  while (samplesRead == numSamples) {
    //if(!errorIMU){
    //updateAcc();
    //}
    float aSum = fabs(aX2) + fabs(aY2) + fabs(aZ2);
    //Serial.println("aSum: "+String(aSum));
    if (aSum >= accelerationThreshold) {
      samplesRead = 0;
      Serial.println("sampleRead: " + String(samplesRead));
    }
  }
  while (samplesRead < numSamples) {
    //Serial.println("SNSKK ");
    tflInputTensor->data.f[samplesRead * 6 + 0] = (aX2 + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 1] = (aY2 + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 2] = (aZ2 + 4.0) / 8.0;
    tflInputTensor->data.f[samplesRead * 6 + 3] = (gX2 + 2000.0) / 4000.0;
    tflInputTensor->data.f[samplesRead * 6 + 4] = (gY2 + 2000.0) / 4000.0;
    tflInputTensor->data.f[samplesRead * 6 + 5] = (gZ2 + 2000.0) / 4000.0;

    samplesRead++;

    if (samplesRead == numSamples) {
      Serial.println();
    }
    // }
    TfLiteStatus invokeStatus = interpreter->Invoke();
    if (invokeStatus != kTfLiteOk) {
      Serial.println("Invoke failed!");
      while (1);
      return;
    }
  }
  for (int i = 0; i < NUM_GESTURES; i++) {

    possibility[i]  = tflOutputTensor->data.f[i];
    //poss = possibility;
    poss[i]  = tflOutputTensor->data.f[i] * 10000;
    p = (uint8_t*)&poss;
    if (i == 0)
    { int tp1;
      tp1 = p[3];
      tp1 = (tp1  << 8) + p[2];
      tp1 = (tp1  << 8) + p[1];
      tp1 = (tp1  << 8) + p[0];
      Serial.println("--- " + String(tp1));
    }
    if (i == 1)
    { int tp2;
      tp2 = p[7];
      tp2 = (tp2  << 8) + p[6];
      tp2 = (tp2  << 8) + p[5];
      tp2 = (tp2  << 8) + p[4];
      Serial.println("--- " + String(tp2));
    }
    if (i == 2)
    { int tp3;
      tp3 = p[11];
      tp3 = (tp3  << 8) + p[10];
      tp3 = (tp3  << 8) + p[9];
      tp3 = (tp3  << 8) + p[8];
      Serial.println("--- " + String(tp3));
    }

    Serial.print(GESTURES[i]);
    Serial.print(": ");
    Serial.print("Float = " + String(possibility[i]));
    Serial.print("  ");
    Serial.print("Int = " + String(poss[i]));
    Serial.print(" UInt = " + String(p[i]));
    Serial.print("  ");
    Serial.print(tflOutputTensor->data.f[i], 6);
    Serial.println();

  }


  if ( Bluefruit.connected() ) {

    if ( AccCharacteristic.notify(bufAcc, 11) ) {
      //Serial.print("IMUCharacteristic updated to: ");
      //Serial.println(timeStampValue);
    } else {
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
    }
    if ( GyroCharacteristic.notify(bufGyro, 11) ) {
      //Serial.print("IMUCharacteristic updated to: ");
      //Serial.println(timeStampValue);
    } else {
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
    }

    /*if ( MagCharacteristic.notify(bufMag, 11) ) {
      //Serial.print("IMUCharacteristic updated to: ");
      //Serial.println(timeStampValue);
      } else {
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
      }*/


    if ( PossCharacteristic.notify(p, 12) ) {
      //Serial.print("IMUCharacteristic updated to: ");
      //Serial.println(timeStampValue);
    } else {
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
    }
    if ( ErrorCharacteristic.notify(bufError, 1) ) {
      //Serial.print("IMUCharacteristic updated to: ");
      //Serial.println(timeStampValue);
    } else {
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
    }



    /* if (counterG >= 1000) {
        printConnParams();
        counterG = 0;
      }*/
  }

  // Only send update once per second
  delay(1000);
}
