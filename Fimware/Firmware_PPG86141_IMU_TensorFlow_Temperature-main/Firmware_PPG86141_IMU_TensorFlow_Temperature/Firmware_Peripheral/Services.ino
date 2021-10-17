
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

void setupTempService(void)
{
  TempService.begin();

  TempCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  TempCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  TempCharacteristic.setFixedLen(8);
  TempCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  TempCharacteristic.begin();
}
void setupTensorFlowService(void)
{
  TensorFlowService.begin();
  PossCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  PossCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  PossCharacteristic.setFixedLen(12);
  PossCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  PossCharacteristic.begin();
}
void setupPPGMax86(void) {
  PPG86Service.begin();

  ////////////////////////// PDLEDs ////////////////////////////////////
  /*ledSeq1A_PPG1Characteristic1.setProperties(CHR_PROPS_NOTIFY);
    ledSeq1A_PPG1Characteristic1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    ledSeq1A_PPG1Characteristic1.setFixedLen(4);
    ledSeq1A_PPG1Characteristic1.setCccdWriteCallback(cccd_callback_PPG86);
    ledSeq1A_PPG1Characteristic1.begin();

    tagSeq1A_PPG1Characteristic1.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1A_PPG1Characteristic1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1A_PPG1Characteristic1.setFixedLen(1);
    tagSeq1A_PPG1Characteristic1.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1A_PPG1Characteristic1.begin();

    ledSeq1B_PPG1Characteristic1.setProperties(CHR_PROPS_NOTIFY);
    ledSeq1B_PPG1Characteristic1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    ledSeq1B_PPG1Characteristic1.setFixedLen(4);
    ledSeq1B_PPG1Characteristic1.setCccdWriteCallback(cccd_callback_PPG86);
    ledSeq1B_PPG1Characteristic1.begin();

    tagSeq1B_PPG1Characteristic1.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1B_PPG1Characteristic1.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1B_PPG1Characteristic1.setFixedLen(1);
    tagSeq1B_PPG1Characteristic1.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1B_PPG1Characteristic1.begin(); */
  ////////////////////////////////////////////////////////////////////

  ////////////////////////// PDsLED ////////////////////////////////////
  ledSeq1A_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  //ledSeq1A_PPG1Characteristic2.setFixedLen(8);
  ledSeq1A_PPG1Characteristic2.setFixedLen(16);
  ledSeq1A_PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  ledSeq1A_PPG1Characteristic2.begin();

  /* tagSeq1A_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1A_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1A_PPG1Characteristic2.setFixedLen(1);
    tagSeq1A_PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1A_PPG1Characteristic2.begin();   */

  ledSeq1A_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
  ledSeq1A_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  //ledSeq1A_PPG2Characteristic2.setFixedLen(4);
  ledSeq1A_PPG2Characteristic2.setFixedLen(16);
  ledSeq1A_PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
  ledSeq1A_PPG2Characteristic2.begin();

  /*tagSeq1A_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1A_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1A_PPG2Characteristic2.setFixedLen(1);
    tagSeq1A_PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1A_PPG2Characteristic2.begin();

    ledSeq1B_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    ledSeq1B_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    ledSeq1B_PPG1Characteristic2.setFixedLen(4);
    ledSeq1B_PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    ledSeq1B_PPG1Characteristic2.begin();


    tagSeq1B_PPG1Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1B_PPG1Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1B_PPG1Characteristic2.setFixedLen(1);
    tagSeq1B_PPG1Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1B_PPG1Characteristic2.begin();

    ledSeq1B_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    ledSeq1B_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    ledSeq1B_PPG2Characteristic2.setFixedLen(4);
    ledSeq1B_PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    ledSeq1B_PPG2Characteristic2.begin();

    tagSeq1B_PPG2Characteristic2.setProperties(CHR_PROPS_NOTIFY);
    tagSeq1B_PPG2Characteristic2.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    tagSeq1B_PPG2Characteristic2.setFixedLen(1);
    tagSeq1B_PPG2Characteristic2.setCccdWriteCallback(cccd_callback_PPG86);
    tagSeq1B_PPG2Characteristic2.begin();*/

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
  ///////////////////////////////////////////////////////////////////
}
