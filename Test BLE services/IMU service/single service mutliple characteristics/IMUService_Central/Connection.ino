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

  // If HRM is not found, disconnect and return
  if ( !IMUService.discover(conn_handle) )
  {
    Serial.println("Found NONE");

    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);

    return;
  }

  // Once HRM service is found, we continue to discover its characteristic
  Serial.println("Found it");
  
  Serial.print("Discovering ACC characteristic ... ");
  if ( !IMUCharacteristic.discover() )
  {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("not found !!!");  
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.disconnect(conn_handle);
    return;
  }
  Serial.println("Acc characteristic Found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( IMUCharacteristic.enableNotify() )
  {
    Serial.println("Acc characteristic notify enabled");
    myTimer=millis();
  }else
  {
    Serial.println("Couldn't enable notify for C1 Measurement. Increase DEBUG LEVEL for troubleshooting");
  }
  /*Gyro*/
   Serial.print("Discovering Gyro characteristic ... ");
  if ( !GyroCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("not found !!!");  
    Serial.println("Measurement characteristic is mandatory but not found");
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("Gyro characteristic Found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( GyroCharacteristic.enableNotify() )
  {
    Serial.println("Gyro characteristic notify enabled");
    myTimer=millis();
  }else
  {
    Serial.println("Couldn't enable notify for C1 Measurement. Increase DEBUG LEVEL for troubleshooting");
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
