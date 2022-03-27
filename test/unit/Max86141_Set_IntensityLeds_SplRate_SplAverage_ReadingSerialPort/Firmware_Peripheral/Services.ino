
void setupErrorService(void) {
  ErrorService.begin();
  ErrorCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  ErrorCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ErrorCharacteristic.setFixedLen(1);
  ErrorCharacteristic.setCccdWriteCallback(cccd_callback2);  // Optionally capture CCCD updates
  ErrorCharacteristic.begin();
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
