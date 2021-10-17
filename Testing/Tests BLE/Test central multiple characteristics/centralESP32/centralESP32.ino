//  https://github.com/nkolban/esp32-snippets/issues/837

#include "BLEDevice.h"
//#include "BLEScan.h"
//0x1165 / 0x1166 OK
//0x1101 / 0x1102 not OK
// The remote service we wish to connect to
static  BLEUUID serviceUUID(BLEUUID((uint16_t)0x1165)); 
// The characteristic of the remote service we are interested in
static BLEUUID    charUUID(BLEUUID((uint16_t)0x1166));
//static BLEUUID    charUUID2(BLEUUID((uint16_t)0x1167));

static BLEAddress *pServerAddress;
static boolean doConnect = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLERemoteCharacteristic* pRemoteCharacteristic2;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
//  Serial.print("Notify callback for characteristic ");
  for (int i = 0; i < length; i++) {
    Serial.print(pData[i]);
    Serial.print(" ");
  }
  Serial.println();
}

bool connectToServer(BLEAddress pAddress) {
  Serial.print("Forming a connection to ");
  Serial.println(pAddress.toString().c_str());

  BLEClient*  pClient  = BLEDevice::createClient();
  Serial.println(" - Created client");

  // Connect to the remove BLE Server.
  pClient->connect(pAddress, (esp_ble_addr_type_t)1);
//  pClient->connect(pAddress, BLE_ADDR_TYPE_RANDOM); //  Should work as well - BLE_ADDR_TYPE_RANDOM is equal 1 in enumerator definition
  Serial.println(" - Connected to server");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our service");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic");


  // Obtain a reference to the characteristic in the service of the remote BLE server.
 /* pRemoteCharacteristic2 = pRemoteService->getCharacteristic(charUUID2);
  if (pRemoteCharacteristic2 == nullptr) {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID2.toString().c_str());
    return false;
  }
  Serial.println(" - Found our characteristic2");*/

  // Read the value of the characteristic.
  std::string value = pRemoteCharacteristic->readValue();
  Serial.print("The characteristic value was: ");
  Serial.println(value.c_str());
    // Read the value of the characteristic.
 /* std::string value2 = pRemoteCharacteristic2->readValue();
  Serial.print("The characteristic2 value was: ");
  Serial.println(value2.c_str());
*/
  pRemoteCharacteristic->registerForNotify(notifyCallback);
  
  //pRemoteCharacteristic2->registerForNotify(notifyCallback);
}
/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.print("BLE Advertised Device found: ");
      Serial.println(advertisedDevice.toString().c_str());

      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(serviceUUID)) {

        //
        Serial.print("Found our device!  address: ");
        advertisedDevice.getScan()->stop();

        pServerAddress = new BLEAddress(advertisedDevice.getAddress());
        doConnect = true;

      } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks


void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 30 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
} // End of setup.


const uint8_t notificationOff[] = {0x0, 0x0};
const uint8_t notificationOn[] = {0x1, 0x0};
bool onoff = true;
void loop() {

  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("We are now connected to the BLE Server.");
      connected = true;
    } else {
      Serial.println("We have failed to connect to the server; there is nothin more we will do.");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
 /* if (connected) {
    if (onoff) {
//      Serial.println("Notifications turned on");
      pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
    }
    else     {
//      Serial.println("Notifications turned off");
      pRemoteCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOff, 2, true);
    }

    onoff = onoff ? 0 : 1;
  }*/

  delay(100); // Delay a second between loops.
} // End of loop
