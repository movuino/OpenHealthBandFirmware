#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);


/*PPG 86140 - 86141 & characteristics*/
BLEClientService PPG86Service(0x1300);

////// PDLEDS (1 PD - 2 LEDs) ////////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic1(0x1301);
//BLEClientCharacteristic tagSeq1A_PPG1Characteristic1(0x1302);
BLEClientCharacteristic SNR1_1_Characteristic1(0x1315);

///// PDsLED (2 PDs - 1 LED) ///////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic2(0x1305);
//BLEClientCharacteristic tagSeq1A_PPG1Characteristic2(0x1306);
BLEClientCharacteristic ledSeq1A_PPG2Characteristic2(0x1307);
//BLEClientCharacteristic tagSeq1A_PPG2Characteristic2(0x1308);
BLEClientCharacteristic SNR1_2_Characteristic2(0x1313);
BLEClientCharacteristic SNR2_2_Characteristic2(0x1314);

///// PDsLEDs (2 PDs - 3 LEDs) ///////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic3(0x1309);
//BLEClientCharacteristic tagSeq1A_PPG1Characteristic2(0x1310);
BLEClientCharacteristic ledSeq1A_PPG2Characteristic3(0x1311);
//BLEClientCharacteristic tagSeq1A_PPG2Characteristic2(0x1312);
BLEClientCharacteristic SNR1_3_Characteristic3(0x1317);
BLEClientCharacteristic SNR2_3_Characteristic3(0x1318);

/*IMU Service & characteristics*/
BLEClientService IMUService(0x1101);
BLEClientCharacteristic AccCharacteristic(0x1102);
BLEClientCharacteristic GyroCharacteristic(0x1103);
BLEClientCharacteristic MagCharacteristic(0x1104);

/*Temp Service & characteristics*/
BLEClientService TempService(UUID16_SVC_HEALTH_THERMOMETER);
BLEClientCharacteristic  TempCharacteristic(UUID16_CHR_TEMPERATURE_MEASUREMENT);
BLEClientCharacteristic  TempCharacteristicType(UUID16_CHR_TEMPERATURE_TYPE);

/*TensorFlow Service & characteristics*/
BLEClientService        TensorFlowService(0x1500);
BLEClientCharacteristic PossCharacteristic(0x1501);

/* Enable the data measure */
#define PPG_Max86141
#define IMU9250

/* PPG Sensor Configurations */

// Sensor composed with 1 PD and 2 LEDs //
//#define PDLEDs

// Sensor composed with 2 PDs and 1 LED //
//#define PDsLED

// Sensor composed with 2 PDs and 3 LEDs //
#define PDsLEDs

float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
float acc_resolution = 0.00049;
float gyro_resolution = 0.06;
float mag_resolution = 1.50;
float mag_bias_factory_0 = 1.18, mag_bias_factory_1 = 1.19, mag_bias_factory_2 = 1.14;
float mag_scale_0 = 1.00, mag_scale_1 = 1.00, mag_scale_2 = 1.00;
float mag_bias_0 = 0.00, mag_bias_1 = 0.00, mag_bias_2 = 0.00;
float bias_to_current_bits = 1.00;

double Gb1, P_O1, P_G1, P_R1, P_T1;
float sensor_Temp;
uint32_t sixRAM, nineRAM;
int aTensorFlow, bTensorFlow, cTensorFlow;
int errorFlag = 0;

long last_samplerate;
int recieved_dataAcc = 0;
int recieved_dataPD1 = 0;
long startTime;


void setup() {
  Serial.begin(115200);
  //while ( !Serial )
  delay(10);   // for nrf52840 with native usb

  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);
  Bluefruit.setName("OHB Receiver");


  // Initialize Custom Service Client
  ErrorService.begin();
  // set up callback for receiving measurement
  ErrorCharacteristic.setNotifyCallback(ErrorCharacteristic_notify_callback);
  ErrorCharacteristic.begin();


#ifdef PPG_Max86141
  PPG86Service.begin();

#ifdef PDLEDs
  ledSeq1A_PPG1Characteristic1.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_1_notify_callback);
  ledSeq1A_PPG1Characteristic1.begin();

  SNR1_1_Characteristic1.setNotifyCallback(PPGMax86_SNR1_1_notify_callback);
  SNR1_1_Characteristic1.begin();
#endif

#ifdef PDsLED
  ledSeq1A_PPG1Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_2_notify_callback);
  ledSeq1A_PPG1Characteristic2.begin();

  ledSeq1A_PPG2Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG2_2_notify_callback);
  ledSeq1A_PPG2Characteristic2.begin();

  SNR1_2_Characteristic2.setNotifyCallback(PPGMax86_SNR1_2_notify_callback);
  SNR1_2_Characteristic2.begin();

  SNR2_2_Characteristic2.setNotifyCallback(PPGMax86_SNR2_2_notify_callback);
  SNR2_2_Characteristic2.begin();
#endif

#ifdef PDsLEDs
  ledSeq1A_PPG1Characteristic3.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_3_notify_callback);
  ledSeq1A_PPG1Characteristic3.begin();

  ledSeq1A_PPG2Characteristic3.setNotifyCallback(PPGMax86_ledSeq1A_PPG2_3_notify_callback);
  ledSeq1A_PPG2Characteristic3.begin();

  SNR1_3_Characteristic3.setNotifyCallback(PPGMax86_SNR1_3_notify_callback);
  SNR1_3_Characteristic3.begin();

  SNR2_3_Characteristic3.setNotifyCallback(PPGMax86_SNR2_3_notify_callback);
  SNR2_3_Characteristic3.begin();
#endif

#endif

#ifdef IMU9250
  IMUService.begin();
  // set up callback for receiving measurement
  AccCharacteristic.setNotifyCallback(AccCharacteristic_notify_callback);
  AccCharacteristic.begin();
  
  GyroCharacteristic.setNotifyCallback(GyroCharacteristic_notify_callback);
  GyroCharacteristic.begin();

  MagCharacteristic.setNotifyCallback(MagCharacteristic_notify_callback);
  MagCharacteristic.begin();
#endif

  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(ErrorService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0);

  startTime = micros();
}

void loop() {
  /*
  if (millis() - last_samplerate > 1000) {
    last_samplerate = millis();
    Serial.printf("Sampling Rate: %d\n", recieved_data);
    recieved_data = 0;
  }
  */
}
