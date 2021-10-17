void setupTimeStampService(void)
{
  timeStampService.begin();

  timeStampCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  timeStampCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  timeStampCharacteristic.setFixedLen(4);
  timeStampCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  timeStampCharacteristic.begin();
}
