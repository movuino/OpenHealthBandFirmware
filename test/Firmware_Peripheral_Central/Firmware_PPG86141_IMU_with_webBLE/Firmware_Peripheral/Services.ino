
void setupErrorService(void) {
  ErrorService.begin();
  ErrorCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  ErrorCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ErrorCharacteristic.setFixedLen(4);
  ErrorCharacteristic.setCccdWriteCallback(cccd_callback2);  // Optionally capture CCCD updates
  ErrorCharacteristic.begin();
}
void setupIMUService(void)
{
  IMUService.begin();
  AccCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  AccCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  AccCharacteristic.setFixedLen(32);
  AccCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  AccCharacteristic.begin();
  /*Gyro*/
  GyroCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  GyroCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  GyroCharacteristic.setFixedLen(11);
  GyroCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  GyroCharacteristic.begin();
  /*Mag*/
  MagCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  MagCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  MagCharacteristic.setFixedLen(10);
  MagCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  MagCharacteristic.begin();
}

void setupPPGMax86(void) {
  PPG86Service.begin();

#ifdef PDsLED
  ////////////////////////// PDsLED ////////////////////////////////////
  ledSeq1A_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ledSeq1A_PPG1Characteristic2.setFixedLen(12);
  ledSeq1A_PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  ledSeq1A_PPG1Characteristic2.begin();

  ledSeq1A_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ledSeq1A_PPG2Characteristic2.setFixedLen(12);
  ledSeq1A_PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  ledSeq1A_PPG2Characteristic2.begin();

  SNR1_2PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  SNR1_2PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  SNR1_2PPG1Characteristic2.setFixedLen(4);
  SNR1_2PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  SNR1_2PPG1Characteristic2.begin();

  SNR2_2PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  SNR2_2PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  SNR2_2PPG2Characteristic2.setFixedLen(4);
  SNR2_2PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  SNR2_2PPG2Characteristic2.begin();
#endif

}

void setupStart_StopService(void) {
  Start_StopService.begin();

  StartCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
  StartCharacteristic.setPermission(SECMODE_OPEN, SECMODE_OPEN);
  StartCharacteristic.setFixedLen(1);
  StartCharacteristic.begin();

  intensityLedsCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_WRITE);
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
}
