void setupTimeStampService(void)
{
  timeStampService.begin();

  timeStampCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  timeStampCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  timeStampCharacteristic.setFixedLen(16);
  timeStampCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  timeStampCharacteristic.begin();
}

void setupIMUService(void)
{
  IMUService.begin();

  IMUCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  IMUCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  IMUCharacteristic.setFixedLen(16);
  IMUCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  IMUCharacteristic.begin();
}
