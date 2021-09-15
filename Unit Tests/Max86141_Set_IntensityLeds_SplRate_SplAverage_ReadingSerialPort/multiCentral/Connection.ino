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
  /* Serial.print("Discovering PPG Service ... ");

  if ( !PPGService.discover(conn_handle) )
    {
    Serial.println("PPG service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
  // Once HRM service is found, we continue to discover its characteristic
  Serial.println("Found PPG service");
  if ( !rawPPGCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("rawPPG characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("rawPPG characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( rawPPGCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive rawPPG characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify for C1 Measurement. Increase DEBUG LEVEL for troubleshooting");
    }   */
    /**/
    /*  Serial.print("Discovering IMU Service ... ");

    if ( !IMUService.discover(conn_handle) )
    {
    Serial.println("IMU service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
  // Once HRM service is found, we continue to discover its characteristic
    Serial.println("Found IMU service");
   if ( !AccCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("AccCharacteristic characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("AccCharacteristic characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( AccCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive AccCharacteristic characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify AccCharacteristict. Increase DEBUG LEVEL for troubleshooting");
    } 
    
     if ( !GyroCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("GyroCharacteristic characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
    Serial.println("GyroCharacteristic characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if ( GyroCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive GyroCharacteristic characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify GyroCharacteristic. Increase DEBUG LEVEL for troubleshooting");
    }   
    
     if ( !MagCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("MagCharacteristic characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
    Serial.println("MagCharacteristic characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (MagCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive Mag Characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify GyroCharacteristic. Increase DEBUG LEVEL for troubleshooting");
    }      */
    /**/    
      /*Serial.print("Discovering Temp Service ... ");

     if ( !TempService.discover(conn_handle) )
    {
    Serial.println("Temp service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
    Serial.println("Temp service found");
     if ( !TempCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("TempCharacteristic characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("TempCharacteristic characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (TempCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive TempCharacteristic characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify TempCharacteristic characteristic. Increase DEBUG LEVEL for troubleshooting");
    } */     

     /**/    
/*     if ( !TensorFlowService.discover(conn_handle) )
    {
    Serial.println("TensorFlowService service not found");
    // disconnect since we couldn't find HRM service
    Bluefruit.disconnect(conn_handle);
    return;
    }
     if ( !PossCharacteristic.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("PossCharacteristic characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("PossCharacteristic characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (PossCharacteristic.enableNotify() )
    {
    Serial.println("Ready to receive PossCharacteristic characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify PossCharacteristic characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   */


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

/////////////////////// PDLEDs ///////////////////////////////
     if ( !ledSeq1A_PPG1Characteristic1.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1A_PPG1Characteristic1 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1A_PPG1Characteristic1 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1A_PPG1Characteristic1.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1A_PPG1Characteristic1 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1A_PPG1Characteristic1 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   

    if ( !tagSeq1A_PPG1Characteristic1.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1A_PPG1Characteristic1 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1A_PPG1Characteristic1 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1A_PPG1Characteristic1.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1A_PPG1Characteristic1 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1A_PPG1Characteristic1 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   
    
     if ( !ledSeq1B_PPG1Characteristic1.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1B_PPG1Characteristic1 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1B_PPG1Characteristic1 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1B_PPG1Characteristic1.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1B_PPG1Characteristic1 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1B_PPG1Characteristic1 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  
     if ( !tagSeq1B_PPG1Characteristic1.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1B_PPG1Characteristic1 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1B_PPG1Characteristic1 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1B_PPG1Characteristic1.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1B_PPG1Characteristic1 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1B_PPG1Characteristic1 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  
  /////////////////////////////////////////////////////////////

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

    if ( !tagSeq1A_PPG1Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1A_PPG1Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1A_PPG1Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1A_PPG1Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1A_PPG1Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1A_PPG1Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   
    
     if ( !ledSeq1B_PPG1Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1B_PPG1Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1B_PPG1Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1B_PPG1Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1B_PPG1Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1B_PPG1Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  
     if ( !tagSeq1B_PPG1Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1B_PPG1Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1B_PPG1Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1B_PPG1Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1B_PPG1Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1B_PPG1Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  





    if ( !ledSeq1A_PPG2Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1A_PPG2Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1A_PPG2Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1A_PPG2Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1A_PPG2Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1A_PPG2Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   

    if ( !tagSeq1A_PPG2Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1A_PPG2Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1A_PPG2Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1A_PPG2Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1A_PPG2Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1A_PPG2Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }   
    
     if ( !ledSeq1B_PPG2Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("ledSeq1B_PPG2Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("ledSeq1B_PPG2Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (ledSeq1B_PPG2Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive ledSeq1B_PPG2Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify ledSeq1B_PPG2Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  
     if ( !tagSeq1B_PPG2Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("tagSeq1B_PPG2Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("tagSeq1B_PPG2Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (tagSeq1B_PPG2Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive tagSeq1B_PPG2Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify tagSeq1B_PPG2Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  

    if ( !SNR1_2_Characteristic2.discover() )
    {
    // Measurement chr is mandatory, if it is not found (valid), then disconnect
    Serial.println("SNR1_2_Characteristic2 characteristic not found !!!");  
    Bluefruit.disconnect(conn_handle);
    return;
    }
  Serial.println("SNR1_2_Characteristic2 characteristic found");
  delay(20);
  // Reaching here means we are ready to go, let's enable notification on measurement chr
  if (SNR1_2_Characteristic2.enableNotify() )
    {
    Serial.println("Ready to receive SNR1_2_Characteristic2 characteristic data");
    }
  else
    {
    Serial.println("Couldn't enable notify SNR1_2_Characteristic2 characteristic. Increase DEBUG LEVEL for troubleshooting");
    }  
  //////////////////////////////////////////////////////////////////
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
