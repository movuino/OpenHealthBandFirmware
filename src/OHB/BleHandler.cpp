#include "BleHandler.hpp"

extern bool isAcquisitionStarted;

void connection_callback(uint16_t conn_handle)
{
    char central_name[64] = { 0 };

    BLEConnection* connection = Bluefruit.Connection(conn_handle);

    connection = Bluefruit.Connection(conn_handle);
    connection->getPeerName(central_name, sizeof(central_name));

    Serial.printf("Request to change MTU: %d\n", connection->requestMtuExchange(247));
    Serial.printf("MTU: %u\n", connection->getMtu());
    Serial.printf("Connection interval: %u\n", connection->getConnectionInterval());
    Serial.printf("PHY length: %u\n", connection->getPHY());
    Serial.printf("Data lenght: %u\n", connection->getDataLength());
    Serial.printf("Connected to: %s\n", central_name);
}

void disconnection_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;

  Serial.printf("Disconnected, reason = %x\n", reason);
  isAcquisitionStarted = false;
  Serial.println("Advertising!");
}

void startAvertising(void)
{

    // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include Name
  Bluefruit.Advertising.addName();

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}



void setupErrorService(void)
{
  ErrorService.begin();
  ErrorCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  ErrorCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ErrorCharacteristic.setFixedLen(4);
  ErrorCharacteristic.begin();

  const char defaultPayload[4] = {0, 0, 0, 0};
  ErrorCharacteristic.write(defaultPayload, 4);
}

void setupIMUService(void)
{
  IMUService.begin();
  AccCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  AccCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  AccCharacteristic.setFixedLen(32);
  AccCharacteristic.begin();
  /*Gyro*/
  GyroCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  GyroCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  GyroCharacteristic.setFixedLen(11);
  GyroCharacteristic.begin();
  /*Mag*/
  MagCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  MagCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  MagCharacteristic.setFixedLen(10);
  MagCharacteristic.begin();

  AccCharacteristic.write(IMU.getAccTab(), IMU_ACC_TAB_SIZE);
  AccCharacteristic.write(IMU.getGyrTab(), IMU_GYR_TAB_SIZE);
  AccCharacteristic.write(IMU.getMagTab(), IMU_MAG_TAB_SIZE);
}

void setupPPGService(void)
{
  PPG86Service.begin();

  ledSeq1A_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ledSeq1A_PPG1Characteristic2.setFixedLen(12);
  ledSeq1A_PPG1Characteristic2.begin();

  ledSeq1A_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ledSeq1A_PPG2Characteristic2.setFixedLen(12);
  ledSeq1A_PPG2Characteristic2.begin();

  SNR1_2PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY |  CHR_PROPS_READ);
  SNR1_2PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  SNR1_2PPG1Characteristic2.setFixedLen(4);
  SNR1_2PPG1Characteristic2.begin();

  SNR2_2PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ);
  SNR2_2PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  SNR2_2PPG2Characteristic2.setFixedLen(4);
  SNR2_2PPG2Characteristic2.begin();

  ledSeq1A_PPG1Characteristic2.write(PPG.getPD1Tab(), PPG_VAL_TAB_SIZE);
  ledSeq1A_PPG2Characteristic2.write(PPG.getPD2Tab(), PPG_VAL_TAB_SIZE);
  SNR1_2PPG1Characteristic2.write(PPG.getSNR1Tab(), PPG_SNR_TAB_SIZE);
  SNR2_2PPG2Characteristic2.write(PPG.getSNR2Tab(), PPG_SNR_TAB_SIZE);

}

void setupUtilsService(void)
{
  Start_StopService.begin();

  StartCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
  StartCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  StartCharacteristic.setFixedLen(1);
  StartCharacteristic.begin();

  intensityLedsCharacteristic.setProperties(CHR_PROPS_NOTIFY | CHR_PROPS_READ | CHR_PROPS_WRITE);
  intensityLedsCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  intensityLedsCharacteristic.setFixedLen(1);
  intensityLedsCharacteristic.begin();

  smplRateCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
  smplRateCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  smplRateCharacteristic.setFixedLen(1);
  smplRateCharacteristic.begin();

  smplAvgCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
  smplAvgCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  smplAvgCharacteristic.setFixedLen(1);
  smplAvgCharacteristic.begin();

  calibrationCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
  calibrationCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  calibrationCharacteristic.setFixedLen(1);
  calibrationCharacteristic.begin();

  globalCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  globalCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  globalCharacteristic.begin();

  StartCharacteristic.write8(0);
  intensityLedsCharacteristic.write8(0);
  smplRateCharacteristic.write8(0);
  smplAvgCharacteristic.write8(0);
  calibrationCharacteristic.write8(0);
}
