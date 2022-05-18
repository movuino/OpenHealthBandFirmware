#include <bluefruit.h>
#include "connection.h"
#define IMU9250
#define PPG_Max86141
#define PDsLEDs
/**
   Callback invoked when scanner pick up an advertising data
   @param report Structural advertising data
*/
void scan_callback(ble_gap_evt_adv_report_t* report)
{
  // Since we configure the scanner with filterUuid()
  // Scan callback only invoked for device with hrm service advertised
  // Connect to device with HRM service in advertising
  Bluefruit.Central.connect(report);
}

/**
   Callback invoked when an connection is established
   @param conn_handle
*/
void connect_callback(uint16_t conn_handle)
{
  Serial.println("Connected");

  ASSERT_SERVICE(ErrorService, conn_handle);
  ASSERT_CHARACTERISTIC(ErrorCharacteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(ErrorCharacteristic);
  

#ifdef IMU9250
  ASSERT_SERVICE(IMUService, conn_handle);
  ASSERT_CHARACTERISTIC(AccCharacteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(AccCharacteristic);
 
  ASSERT_CHARACTERISTIC(GyroCharacteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(GyroCharacteristic);

  ASSERT_CHARACTERISTIC(MagCharacteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(MagCharacteristic);
#endif // IMU9250

#ifdef PPG_Max86141
  ASSERT_SERVICE(PPG86Service, conn_handle);

  #ifdef PDsLED
  ASSERT_CHARACTERISTIC(ledSeq1A_PPG1Characteristic2);
  delay(20);
  ENABLE_NOTIFICATIONS(ledSeq1A_PPG1Characteristic2);

  ASSERT_CHARACTERISTIC(ledSeq1A_PPG2Characteristic2);
  delay(20);
  ENABLE_NOTIFICATIONS(ledSeq1A_PPG2Characteristic2);

  ASSERT_CHARACTERISTIC(SNR1_2_Characteristic2);
  delay(20);
  ENABLE_NOTIFICATIONS(SNR1_2_Characteristic2);

  ASSERT_CHARACTERISTIC(SNR2_2_Characteristic2);
  delay(20);
  ENABLE_NOTIFICATIONS(SNR2_2_Characteristic2);
#endif // PDsLED

#ifdef PDLEDs
    ASSERT_CHARACTERISTIC(ledSeq1A_PPG1Characteristic1);
    delay(20);
    ENABLE_NOTIFICATIONS(ledSeq1A_PPG1Characteristic1);

    ASSERT_CHARACTERISTIC(SNR1_1_Characteristic1);
    delay(20);
    ENABLE_NOTIFICATIONS(SNR1_1_Characteristic1);
#endif // PDLEDs
  
#ifdef PDsLEDs
  ASSERT_CHARACTERISTIC(ledSeq1A_PPG1Characteristic3);
  delay(20);
  ENABLE_NOTIFICATIONS(ledSeq1A_PPG1Characteristic3);

  ASSERT_CHARACTERISTIC(ledSeq1A_PPG2Characteristic3);
  delay(20);
  ENABLE_NOTIFICATIONS(ledSeq1A_PPG2Characteristic3);

  ASSERT_CHARACTERISTIC(SNR1_3_Characteristic3);
  delay(20);
  ENABLE_NOTIFICATIONS(SNR1_3_Characteristic3);

  ASSERT_CHARACTERISTIC(SNR2_3_Characteristic3);
  delay(20);
  ENABLE_NOTIFICATIONS(SNR2_3_Characteristic3);
#endif // PDsLEDs
  
  ASSERT_SERVICE(Start_StopService, conn_handle);
  ASSERT_CHARACTERISTIC(StartCharacteristic);
  Serial.println("Sending START packet on Start characteristic");
  StartCharacteristic.write8(1);

#endif // PPG_Max86141
}

/**
   Callback invoked when a connection is dropped
   @param conn_handle
   @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
*/
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
