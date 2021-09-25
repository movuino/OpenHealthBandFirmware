

void setupIMUService(void)
  {
  IMUService.begin();
  AccCharacteristic.setProperties(CHR_PROPS_NOTIFY);
  AccCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  AccCharacteristic.setFixedLen(11);
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
  MagCharacteristic.setFixedLen(11);
  MagCharacteristic.setCccdWriteCallback(cccd_callback);  // Optionally capture CCCD updates
  MagCharacteristic.begin();
  }


void setupErrorService(void){
  ErrorService.begin();
  ErrorCharacteristic.setProperties(CHR_PROPS_READ | CHR_PROPS_NOTIFY);
  ErrorCharacteristic.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
  ErrorCharacteristic.setFixedLen(1);
  ErrorCharacteristic.setCccdWriteCallback(cccd_callback2);  // Optionally capture CCCD updates
  ErrorCharacteristic.begin();
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
