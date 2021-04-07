void setupIMUService(void)
{
  IMUService.begin();
  AccCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  AccCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  AccCharacteristic.setFixedLen(6);
  AccCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  AccCharacteristic.begin();
  /*Gyro*/
  GyroCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  GyroCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  GyroCharacteristic.setFixedLen(6);
  GyroCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  GyroCharacteristic.begin();
  /*Mag*/
  MagCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  MagCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  MagCharacteristic.setFixedLen(7);
  MagCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  MagCharacteristic.begin();
  
}

void setupTimeStampService(void)
{
  timeStampService.begin();
  timeStampCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  timeStampCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  timeStampCharacteristic.setFixedLen(16);
  timeStampCharacteristic.setCccdWriteCallback(cccd_callback2);  // Optionally capture CCCD updates
  timeStampCharacteristic.begin();
}
