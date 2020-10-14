void setupTimeStampService(void)
{
  timeStampService.begin();

  timeStampCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  timeStampCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  timeStampCharacteristic.setFixedLen(16);
  timeStampCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  timeStampCharacteristic.begin();
}
