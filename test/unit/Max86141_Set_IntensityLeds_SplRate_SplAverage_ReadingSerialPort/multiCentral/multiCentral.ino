#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);

/*PPG 86140 - 86141 & characteristics*/
BLEClientService PPG86Service(0x1300);
////// PDLEDS ////////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic1(0x1301);
BLEClientCharacteristic tagSeq1A_PPG1Characteristic1(0x1302);
BLEClientCharacteristic ledSeq1B_PPG1Characteristic1(0x1303);
BLEClientCharacteristic tagSeq1B_PPG1Characteristic1(0x1304);

///// PDsLED ///////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic2(0x1305);
BLEClientCharacteristic tagSeq1A_PPG1Characteristic2(0x1306);
BLEClientCharacteristic ledSeq1A_PPG2Characteristic2(0x1307);
BLEClientCharacteristic tagSeq1A_PPG2Characteristic2(0x1308);
BLEClientCharacteristic ledSeq1B_PPG1Characteristic2(0x1309);
BLEClientCharacteristic tagSeq1B_PPG1Characteristic2(0x1310);
BLEClientCharacteristic ledSeq1B_PPG2Characteristic2(0x1311);
BLEClientCharacteristic tagSeq1B_PPG2Characteristic2(0x1312);
BLEClientCharacteristic SNR1_2_Characteristic2(0x1313);



int errorFlag=0;


void setup() {
  Serial.begin(115200);
 // while ( !Serial )
  delay(10);   // for nrf52840 with native usb
  
  Serial.println("Initialise the Bluefruit nRF52 module");
  Bluefruit.begin(0, 1);
  Bluefruit.setName("OHB Receiver");
  
  // Initialize Custom Service Client
  ErrorService.begin();
  // set up callback for receiving measurement
  ErrorCharacteristic.setNotifyCallback(ErrorCharacteristic_notify_callback);
  ErrorCharacteristic.begin();

 PPG86Service.begin();
 
 ///// PDsLED ///////////
ledSeq1A_PPG1Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_2_notify_callback);
ledSeq1A_PPG1Characteristic2.begin();

ledSeq1A_PPG2Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG2_2_notify_callback);
ledSeq1A_PPG2Characteristic2.begin();

SNR1_2_Characteristic2.setNotifyCallback(PPGMax86_SNR1_2_notify_callback);
SNR1_2_Characteristic2.begin();
  
  // Increase Blink rate to different from PrPh advertising mode
  Bluefruit.setConnLedInterval(250);

  // Callbacks for Central
  Bluefruit.Central.setDisconnectCallback(disconnect_callback);
  Bluefruit.Central.setConnectCallback(connect_callback);

  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.restartOnDisconnect(true);
  Bluefruit.Scanner.setInterval(160, 80); // in unit of 0.625 ms
  Bluefruit.Scanner.filterUuid(ErrorService.uuid);
  Bluefruit.Scanner.useActiveScan(false);
  Bluefruit.Scanner.start(0); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
