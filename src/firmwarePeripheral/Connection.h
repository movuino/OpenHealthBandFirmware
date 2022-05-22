#define CCCD_CALLBACK(conn_hdl, chr, Characteristic) \
  if ((chr->uuid) == (Characteristic.uuid)) \
  { if(chr->notifyEnabled(conn_hdl)) {\
      Serial.println(" "#Characteristic" 'Notify' enabled");}\
    else { \
      Serial.println(" "#Characteristic" 'Notify' disabled");}\
  }

void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  BLEConnection* connection = Bluefruit.Connection(conn_handle);

  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
  connection->requestConnectionParameter(6);

  Serial.print("Connected to ");
  Serial.println(central_name);
}

void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
  Serial.println("Advertising!");
}


void cccd_callback2(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)
{
  // Display the raw request packet
  Serial.print("CCCD Updated: ");
  //Serial.printBuffer(request->data, request->len);
  Serial.print(cccd_value);
  Serial.println("");

  CCCD_CALLBACK(conn_hdl, chr, ErrorCharacteristic)
}

void cccd_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)
{
  // Display the raw request packet
  Serial.print("CCCD Updated: ");
  Serial.print(cccd_value);

  CCCD_CALLBACK(conn_hdl, chr, hrmc)
  CCCD_CALLBACK(conn_hdl, chr, AccGyrMagCharacteristic)
}

void cccd_callback_PPG86(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value) {
  // Display the raw request packet
  Serial.print("CCCD Updated: ");
  //Serial.printBuffer(request->data, request->len);
  Serial.print(cccd_value);
  Serial.println("");

#ifdef PDLEDs
  CCCD_CALLBACK(conn_hdl, chr, PDLEDs_PD1_SNR1Characteristic)
#endif

#ifdef PDsLED
  CCCD_CALLBACK(conn_hdl, chr, PDsLED_PD1_PD2_SNR1_SNR2Characteristic)
#endif

#ifdef PDsLEDs
  CCCD_CALLBACK(conn_hdl, chr, PDsLEDs_PD1_PD2_SNR1_SNR2Characteristic)
#endif
}
