#include <bluefruit.h>
#include "connection.h"
#define IMU9250
#define PPG_Max86141
#define PDsLEDs
//#define PDsLED
//#define PDLEDs

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

  ASSERT_CHARACTERISTIC(AccGyrMagCharacteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(AccGyrMagCharacteristic);
#endif // IMU9250

#ifdef PPG_Max86141
  ASSERT_SERVICE(PPG86Service, conn_handle);

#ifdef PDsLED
  ASSERT_CHARACTERISTIC(PDsLED_PD1_PD2_SNR1_SNR2Characteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(PDsLED_PD1_PD2_SNR1_SNR2Characteristic);
#endif // PDsLED

#ifdef PDLEDs
  ASSERT_CHARACTERISTIC(PDLEDs_PD1_SNR1Characteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(PDLEDs_PD1_SNR1Characteristic);
#endif // PDLEDs

#ifdef PDsLEDs
  ASSERT_CHARACTERISTIC(PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic);
  delay(20);
  ENABLE_NOTIFICATIONS(PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic);
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
**/
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;
  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
}
