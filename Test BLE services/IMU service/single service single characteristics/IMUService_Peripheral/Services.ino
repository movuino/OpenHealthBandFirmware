void setupIMUService(void)
{
  IMUService.begin();
  IMUCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  IMUCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  IMUCharacteristic.setFixedLen(6);
  IMUCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  IMUCharacteristic.begin();
}
