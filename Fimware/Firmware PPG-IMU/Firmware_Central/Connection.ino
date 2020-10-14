/**
 * Callback invoked when scanner pick up an advertising data
 * @param report Structural advertising data
 */
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Since we configure the scanner with filterUuid()
  // Scan callback only invoked for device with hrm service advertised
  // Connect to device with HRM service in advertising
  Bluefruit.Central.connect(report);
}

/**
 * Callback invoked when an connection is established
 * @param conn_handle
 */
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");
  Serial.print("Discovering Custom Service ... ");
  if ( !timeStampService.discover(conn_handle) )
  {
    Serial.println("Found NONE");

    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);

    return;
  }
 Serial.println("timestamp service  found");

  Serial.print("Discovering Measurement characteristic ... ");
 /* if ( !IMUCharacteristic.discover() )
  {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("not found !!!");  
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.disconnect(conn_handle);
    return;
  }*/
 if ( !timeStampCharacteristic.discover() )
  {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("not found !!!");  
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.disconnect(conn_handle);
    return;
  }
  delay(20);
/* bool result = IMUCharacteristic.discover();
 Serial.print("discover imu characteristic : ");
 Serial.println(result);
  delay(20);*/
  
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( timeStampCharacteristic.enableNotify() )
  {
    Serial.println("Ready to receive C1 Measurement value");
   // bool imuresult=IMUCharacteristic.enableNotify();
    //Serial.print("IMU notify");
    //Serial.println(imuresult);
    myTimer=millis();
  }else
  {
    Serial.println("Couldn't enable notify for C1 Measurement. Increase DEBUG LEVEL for troubleshooting");
  }
/*IMU service*/
  if ( !IMUService.discover(conn_handle) )
  {
    Serial.println("IMU not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
  }
  Serial.println("IMU service  found");
  if ( !IMUCharacteristic.discover() )
  {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("IMU characteristic not found !!!");  
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.disconnect(conn_handle);
    return;
  }
  delay(20);
  Serial.println("IMU characteristic setup");
/* bool result = IMUCharacteristic.discover();
 Serial.print("discover imu characteristic : ");
 Serial.println(result);
  delay(20);*/
  
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( IMUCharacteristic.enableNotify() )
  {
    Serial.println("Ready to receive C1 Measurement value");
   // bool imuresult=IMUCharacteristic.enableNotify();
    //Serial.print("IMU notify");
    //Serial.println(imuresult);
    myTimer=millis();
  }else
  {
    Serial.println("Couldn't enable notify for IMU");
  }
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
