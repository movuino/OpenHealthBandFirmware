
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

void setupHRM(void)
{
  // Configure the Heart Rate Monitor service
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.service.heart_rate.xml
  // Supported Characteristics:
  // Name                         UUID    Requirement Properties
  // ---------------------------- ------  ----------- ----------
  // Heart Rate Measurement       0x2A37  Mandatory   Notify
  // Body Sensor Location         0x2A38  Optional    Read
  // Heart Rate Control Point     0x2A39  Conditional Write       <-- Not used here
  hrms.begin();

  // Note: You must call .begin() on the BLEService before calling .begin() on
  // any characteristic(s) within that service definition.. Calling .begin() on
  // a BLECharacteristic will cause it to be added to the last BLEService that
  // was 'begin()'ed!

  // Configure the Heart Rate Measurement characteristic
  // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.heart_rate_measurement.xml
  // Properties = Notify
  // Min Len    = 1
  // Max Len    = 8
  //    B0      = UINT8  - Flag (MANDATORY)
  //      b5:7  = Reserved
  //      b4    = RR-Internal (0 = Not present, 1 = Present)
  //      b3    = Energy expended status (0 = Not present, 1 = Present)
  //      b1:2  = Sensor contact status (0+1 = Not supported, 2 = Supported but contact not detected, 3 = Supported and detected)
  //      b0    = Value format (0 = UINT8, 1 = UINT16)
  //    B1      = UINT8  - 8-bit heart rate measurement value in BPM
  //    B2:3    = UINT16 - 16-bit heart rate measurement value in BPM
  //    B4:5    = UINT16 - Energy expended in joules
  //    B6:7    = UINT16 - RR Internal (1/1024 second resolution)
  hrmc.setProperties(CHR_PROPS_NOTIFY);
  hrmc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  hrmc.setFixedLen(2);
  hrmc.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  hrmc.begin();
  /*uint8_t hrmdata[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
    hrmc.write(hrmdata, 2);

    // Configure the Body Sensor Location characteristic
    // See: https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.body_sensor_location.xml
    // Properties = Read
    // Min Len    = 1
    // Max Len    = 1
    //    B0      = UINT8 - Body Sensor Location
    //      0     = Other
    //      1     = Chest
    //      2     = Wrist
    //      3     = Finger
    //      4     = Hand
    //      5     = Ear Lobe
    //      6     = Foot
    //      7:255 = Reserved
    bslc.setProperties(CHR_PROPS_READ);
    bslc.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    bslc.setFixedLen(1);
    bslc.begin();
    bslc.write8(2);    // Set the characteristic to 'Wrist' (2)*/
}
