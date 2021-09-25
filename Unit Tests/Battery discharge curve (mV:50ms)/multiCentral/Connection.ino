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
  Serial.print("Discovering Error Service ... ");

  // If ErrorService is not found, disconnect and return
  if ( !ErrorService.discover(conn_handle) )
    {
    Serial.println("Error service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
  // Once Error service is found, we continue to discover its characteristic
  Serial.println("Found error service");
  Serial.print("Discovering error characteristic ... ");
  if ( !ErrorCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("error characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("Error characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( ErrorCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive Error characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify for C1 Measurement. Increase DEBUG LEVEL for troubleshooting");
    }
 
    

    /**/    
    Serial.println("Discovering PPG 86 service");
     if ( !PPG86Service.discover(conn_handle) )
    {
    Serial.println("PPG86Service service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
        Serial.println("PPG86Service service found");

  ////////////////////// PDsLED /////////////////////////////////////
   if ( !ledSeq1A_PPG1Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1A_PPG1Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1A_PPG1Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1A_PPG1Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1A_PPG1Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1A_PPG1Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
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
  //Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
