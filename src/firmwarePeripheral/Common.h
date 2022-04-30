
/* Bool Errors  */
bool errorIMU = true;
bool errorPPG86 = true;
uint8_t bufError[2];


/* Tests */
/* Print data on Serial Monitor when BLE is unenabled */
//#define SerialTest
/* Sending data when BLE is enabled */
#define BleTest

#define DEBUG

#ifdef DEBUG
# define CHECK_NOTIFICATION(condition) \
  if (!condition)                     \
    Serial.printf("Notification failed on line %d\n", __LINE__);
#else
# define CHECK_NOTIFICATION(condition) condition;
#endif

/* Shutdown or restart the sensor */
bool shutdown_or_restartPPG = 0, shutdown_or_restartIMU = 0;

/*Start or Stop command received from Central*/
bool ssCommand = 0;

/*Error Service & characteristic*/
BLEService ErrorService = BLEService(0x1200);
BLECharacteristic ErrorCharacteristic = BLECharacteristic(0x1201);

BLEDis bledis;    // DIS (Device Information Service) helper class instance
BLEBas blebas;    // BAS (Battery Service) helper class instance

BLEService        hrms = BLEService(UUID16_SVC_HEART_RATE);
BLECharacteristic hrmc = BLECharacteristic(UUID16_CHR_HEART_RATE_MEASUREMENT);
BLECharacteristic bslc = BLECharacteristic(UUID16_CHR_BODY_SENSOR_LOCATION);

BLEService Start_StopService = BLEService(0x1400);
BLECharacteristic StartCharacteristic = BLECharacteristic(0x1401);
BLECharacteristic intensityLedsCharacteristic = BLECharacteristic(0x1402);
BLECharacteristic smplRateCharacteristic = BLECharacteristic(0x1403);
BLECharacteristic smplAvgCharacteristic = BLECharacteristic(0x1404);
