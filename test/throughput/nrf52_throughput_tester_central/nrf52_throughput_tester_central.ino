#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);

BLEClientService Start_StopService(0x1400);
BLEClientCharacteristic StartCharacteristic(0x1401);

/*PPG 86140 - 86141 & characteristics*/
BLEClientService PPG86Service(0x1300);
BLEClientCharacteristic PDLEDs_PD1_SNR1Characteristic(0x1301);
BLEClientCharacteristic PDsLED_PD1_PD2_SNR1_SNR2Characteristic(0x1302);
BLEClientCharacteristic PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic(0x1303);

/*IMU Service & characteristics*/
BLEClientService IMUService(0x1101);
BLEClientCharacteristic AccGyrMagCharacteristic(0x1102);

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
int recieved_data = 0, recieved_dataPDsLEDs = 0, recieved_dataPDsLED = 0;
uint8_t start_payload[] = {1};

void setup() {
  Serial.begin(115200);
  while ( !Serial )
    delay(10);   // for nrf52840 with native usb

  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);
  Bluefruit.setName("OHB Receiver");

  // Initialize Custom Service Client
  ErrorService.begin();
  // set up callback for receiving measurement
  ErrorCharacteristic.setNotifyCallback(ErrorCharacteristic_notify_callback);
  ErrorCharacteristic.begin();

  Start_StopService.begin();
  StartCharacteristic.begin();


#ifdef PPG_Max86141
  PPG86Service.begin();

#ifdef PDLEDs
  PDLEDs_PD1_SNR1Characteristic.setNotifyCallback(PDLEDs_PD1_SNR1_notify_callback);
  PDLEDs_PD1_SNR1Characteristic.begin();
#endif

#ifdef PDsLED
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.setNotifyCallback(PDsLED_PD1_PD2_SNR1_SNR2_notify_callback);
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.begin();
#endif

#ifdef PDsLEDs
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.setNotifyCallback(PDsLEDs_PD1_PD2_SNR1_SNR2_notify_callback);
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.begin();
#endif

#endif


#ifdef IMU9250
  IMUService.begin();
  AccGyrMagCharacteristic.setNotifyCallback(AccGyrMag_notify_callback);
  AccGyrMagCharacteristic.begin();
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
  last_samplerate = millis();
}

void loop() {

  if (millis() - last_samplerate > 1000) {
    last_samplerate = millis();
    Serial.printf("Sampling Rate Acc_Gyro_Mag: %d\n", recieved_data);

#ifdef PDsLEDs
    Serial.printf("Sampling Rate PDsLEDs: %d\n", recieved_dataPDsLEDs);
    recieved_dataPDsLEDs = 0;
#endif

#ifdef PDsLED
    Serial.printf("Sampling Rate PDsLED: %d\n", recieved_dataPDsLED);
    recieved_dataPDsLED = 0;
#endif
    recieved_data = 0;
  }

}
