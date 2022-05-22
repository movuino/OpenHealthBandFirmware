
void setupErrorService(void) {
  ErrorService.begin();

  ErrorCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  ErrorCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ErrorCharacteristic.setFixedLen(2);
  ErrorCharacteristic.setCccdWriteCallback(cccd_callback2);  // Optionally capture CCCD updates
  ErrorCharacteristic.begin();
}

void setupIMUService(void)
{
  IMUService.begin();

  AccGyrMagCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  AccGyrMagCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  AccGyrMagCharacteristic.setFixedLen(22);
  AccGyrMagCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  AccGyrMagCharacteristic.begin();
}

void setupPPGMax86(void) {
  PPG86Service.begin();

#ifdef PDLEDs
  ////////////////////////// PDLEDs ////////////////////////////////////
  PDLEDs_PD1_SNR1Characteristic.setProperties(CHR_PROPS_NOTIFY);
  PDLEDs_PD1_SNR1Characteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  PDLEDs_PD1_SNR1Characteristic.setFixedLen(24);
  PDLEDs_PD1_SNR1Characteristic.setCccdWriteCallback(cccd_callback_PPG86);
  PDLEDs_PD1_SNR1Characteristic.begin();
#endif


#ifdef PDsLED
  ////////////////////////// PDsLED ////////////////////////////////////
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.setProperties(CHR_PROPS_NOTIFY);
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.setFixedLen(28);
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.setCccdWriteCallback(cccd_callback_PPG86);
  PDsLED_PD1_PD2_SNR1_SNR2Characteristic.begin();
#endif

#ifdef PDsLEDs
  ////////////////////////// PDsLEDs ////////////////////////////////////
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.setProperties(CHR_PROPS_NOTIFY);
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.setFixedLen(28);
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.setCccdWriteCallback(cccd_callback_PPG86);
  PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic.begin();
#endif

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
  uint8_t hrmdata[2] = { 0b00000110, 0x40 }; // Set the characteristic to use 8-bit values, with the sensor connected and detected
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
  bslc.write8(2);    // Set the characteristic to 'Wrist' (2)
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
