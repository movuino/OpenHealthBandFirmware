#include <bluefruit.h>

BLEService timeStampService = BLEService(0x1165);

BLECharacteristic timeStampCharacteristic = BLECharacteristic(0x1166);

BLEDis bledis;    // DIS (Device Information Service) helper class instance

uint32_t timeStampValue = 0;
long startTimer=0;
void setup() {
  Serial.begin(115200);
  while ( !Serial ) delay(10);   // for nrf52840 with native usb

  Serial.println(F("Custom TimeStamp Service and Characteristic"));
  Serial.println(F("-----------------------\n"));

  // Initialise the Bluefruit module
  Serial.println("Initialise the OHB module");
  Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);  
  Bluefruit.begin();

  // Set the advertised device name (keep it short!)
  Serial.println("Setting Device Name to 'Open Health Band'");
  Bluefruit.setName("Open Helath Band");

  // Set the connect/disconnect callback handlers
  Bluefruit.Periph.setConnectCallback(connect_callback);
  Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
  Bluefruit.Periph.setConnInterval(6, 12); // 7.5 - 15 ms
  // Configure and Start the Device Information Service
  Serial.println("Configuring the Device Information Service");
  bledis.setManufacturer("CRI");
  bledis.setModel("OHB v0.1");
  bledis.begin();

  // Setup the Custom Service BLEService and BLECharacteristic
  Serial.println("Configuring the Custom Service");
  setupTimeStampService();

  // Setup the advertising packet(s)
  Serial.println("Setting up the advertising payload(s)");
  startAdv();
}

void loop() {
  timeStampValue = millis()-startTimer;
  startTimer=millis();
  digitalToggle(LED_RED);
  
  if ( Bluefruit.connected() ) {
    // Note: We use .notify instead of .write!
    // If it is connected but CCCD is not enabled
    // The characteristic's value is still updated although notification is not sent
    if ( timeStampCharacteristic.notify32(timeStampValue) ){
      //Serial.print("timeStampCharacteristic updated to: ");
      Serial.println(timeStampValue); 
    }else{
      Serial.println("ERROR: Notify not set in the CCCD or not connected!");
    }
  }
 delay(1);
}
