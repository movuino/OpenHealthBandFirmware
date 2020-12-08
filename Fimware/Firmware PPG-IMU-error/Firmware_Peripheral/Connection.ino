void connect_callback(uint16_t conn_handle)
{
  // Get the reference to current connection
  //BLEConnection*
  connection = Bluefruit.Connection(conn_handle);
  char central_name[32] = { 0 };
  connection->getPeerName(central_name, sizeof(central_name));
  Serial.print("MTU: ");
  Serial.print(connection->getMtu());
  Serial.println(" ");
  Serial.print("Connection interval: ");
  Serial.print(connection->getConnectionInterval());
  Serial.println(" ");
  Serial.print("PHY length: ");
  Serial.print(connection->getPHY());
  Serial.println(" ");
  Serial.print("Connected to ");
  Serial.println(central_name);
  printConnParams();
}

void printConnParams(){
  Serial.println("____________________________");
    Serial.print("MTU: ");
  Serial.print(connection->getMtu());
  Serial.println(" ");
  Serial.print("Connection interval: ");
  Serial.print(connection->getConnectionInterval());
  Serial.println(" ");
  Serial.print("PHY length: ");
  Serial.print(connection->getPHY());
  Serial.println(" ");
  Serial.println("____________________________");
}


void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
  (void) conn_handle;
  (void) reason;

  Serial.print("Disconnected, reason = 0x"); Serial.println(reason, HEX);
  Serial.println("Advertising!");
}

void cccd_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)
{
    // Display the raw request packet
    Serial.print("CCCD Updated: ");
    //Serial.printBuffer(request->data, request->len);
    Serial.print(cccd_value);
    Serial.println("");

    // Check the characteristic this CCCD update is associated with in case
    // this handler is used for multiple CCCD records.
    if (chr->uuid == rawPPGCharacteristic.uuid) {
        if (chr->notifyEnabled(conn_hdl)) {
            Serial.println("Service timeStamp 'Notify' enabled");
            startTimer=millis();
        } else {
            Serial.println("Service 'Notify' disabled");
        }
    }
    else if (chr->uuid == AccCharacteristic.uuid) {
        if (chr->notifyEnabled(conn_hdl)) {
             Serial.println("Acc Characteristcis 'Notify' enabled");
            startTimer=millis();
        } else {
            Serial.println("Service 'Notify' disabled");
        }
    }
    else if (chr->uuid == GyroCharacteristic.uuid) {
        if (chr->notifyEnabled(conn_hdl)) {
            Serial.println("Gyro Characteristcis 'Notify' enabled");
            startTimer=millis();
        } else {
            Serial.println("Service 'Notify' disabled");
        }
    }
   else if (chr->uuid == MagCharacteristic.uuid) {
        if (chr->notifyEnabled(conn_hdl)) {
            Serial.println("Mag Characteristcis 'Notify' enabled");
            startTimer=millis();
        } else {
            Serial.println("Service 'Notify' disabled");
        }
    }
 
}
void cccd_callback2(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)
{
    // Display the raw request packet
    Serial.print("CCCD Updated: ");
    //Serial.printBuffer(request->data, request->len);
    Serial.print(cccd_value);
    Serial.println("");
    if (chr->uuid == ErrorCharacteristic.uuid) {
        if (chr->notifyEnabled(conn_hdl)) {
            Serial.println("Error Characteristcis 'Notify' enabled");
        } else {
            Serial.println("Error Characteristcis 'Notify' disabled");
        }
    }
}
