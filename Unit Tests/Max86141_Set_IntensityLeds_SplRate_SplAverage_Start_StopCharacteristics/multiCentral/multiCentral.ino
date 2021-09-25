#include <bluefruit.h>

BLEClientService        ErrorService(0x1200);
BLEClientCharacteristic ErrorCharacteristic(0x1201);

/*PPG 86140 - 86141 & characteristics*/
BLEClientService PPG86Service(0x1300);
////// PDLEDS ////////////
/*BLEClientCharacteristic ledSeq1A_PPG1Characteristic1(0x1301);
BLEClientCharacteristic tagSeq1A_PPG1Characteristic1(0x1302);
BLEClientCharacteristic ledSeq1B_PPG1Characteristic1(0x1303);
BLEClientCharacteristic tagSeq1B_PPG1Characteristic1(0x1304);*/

///// PDsLED ///////////
BLEClientCharacteristic ledSeq1A_PPG1Characteristic2(0x1305);
//BLEClientCharacteristic tagSeq1A_PPG1Characteristic2(0x1306);
BLEClientCharacteristic ledSeq1A_PPG2Characteristic2(0x1307);
/*BLEClientCharacteristic tagSeq1A_PPG2Characteristic2(0x1308);
BLEClientCharacteristic ledSeq1B_PPG1Characteristic2(0x1309);
BLEClientCharacteristic tagSeq1B_PPG1Characteristic2(0x1310);
BLEClientCharacteristic ledSeq1B_PPG2Characteristic2(0x1311);
BLEClientCharacteristic tagSeq1B_PPG2Characteristic2(0x1312);*/
BLEClientCharacteristic SNR1_2_Characteristic2(0x1313);
BLEClientCharacteristic SNR2_2_Characteristic2(0x1314);


BLEClientService Start_StopService(1400);
BLEClientCharacteristic StartCharacteristic(0x1401);
BLEClientCharacteristic intensityLedsCharacteristic(0x1402);
BLEClientCharacteristic smplRateCharacteristic(0x1403);
BLEClientCharacteristic smplAvgCharacteristic(0x1404);

int errorFlag=0;
byte Started = 0, intensity_leds = 0, smpl_rate= 0, smpl_avg= 0;
//int Started = 0, intensity_leds = 0, smpl_rate= 0, smpl_avg= 0;
String inData;

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
 ///// PDLEDs //////
 /*ledSeq1A_PPG1Characteristic1.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_1_notify_callback);
 ledSeq1A_PPG1Characteristic1.begin();
 tagSeq1A_PPG1Characteristic1.setNotifyCallback(PPGMax86_tagSeq1A_PPG1_1_notify_callback);
 tagSeq1A_PPG1Characteristic1.begin();
 ledSeq1B_PPG1Characteristic1.setNotifyCallback(PPGMax86_ledSeq1B_PPG1_1_notify_callback);
 ledSeq1B_PPG1Characteristic1.begin();
 tagSeq1B_PPG1Characteristic1.setNotifyCallback(PPGMax86_tagSeq1B_PPG1_1_notify_callback);
 tagSeq1B_PPG1Characteristic1.begin();*/

 ///// PDsLED ///////////
ledSeq1A_PPG1Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG1_2_notify_callback);
ledSeq1A_PPG1Characteristic2.begin();

//tagSeq1A_PPG1Characteristic2.setNotifyCallback(PPGMax86_tagSeq1A_PPG1_2_notify_callback);
//tagSeq1A_PPG1Characteristic2.begin();

/*ledSeq1A_PPG2Characteristic2.setNotifyCallback(PPGMax86_ledSeq1A_PPG2_2_notify_callback);
ledSeq1A_PPG2Characteristic2.begin();*/

/*tagSeq1A_PPG2Characteristic2.setNotifyCallback(PPGMax86_tagSeq1A_PPG2_2_notify_callback);
tagSeq1A_PPG2Characteristic2.begin();

ledSeq1B_PPG1Characteristic2.setNotifyCallback(PPGMax86_ledSeq1B_PPG1_2_notify_callback);
ledSeq1B_PPG1Characteristic2.begin();

tagSeq1B_PPG1Characteristic2.setNotifyCallback(PPGMax86_tagSeq1B_PPG1_2_notify_callback);
tagSeq1B_PPG1Characteristic2.begin();

ledSeq1B_PPG2Characteristic2.setNotifyCallback(PPGMax86_ledSeq1B_PPG2_2_notify_callback);
ledSeq1B_PPG2Characteristic2.begin();

tagSeq1B_PPG2Characteristic2.setNotifyCallback(PPGMax86_tagSeq1B_PPG2_2_notify_callback);
tagSeq1B_PPG2Characteristic2.begin();*/

/*SNR1_2_Characteristic2.setNotifyCallback(PPGMax86_SNR1_2_notify_callback);
SNR1_2_Characteristic2.begin();

SNR2_2_Characteristic2.setNotifyCallback(PPGMax86_SNR2_2_notify_callback);
SNR2_2_Characteristic2.begin();*/

  Start_StopService.begin();
  StartCharacteristic.begin();

  intensityLedsCharacteristic.begin();
  smplRateCharacteristic.begin();
  smplAvgCharacteristic.begin();
  
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
  while (Serial.available() > 0) {
    char recieved = Serial.read();
    inData += recieved;
  /* Format input data : 010,14,00,1 where 010 is the intensity, 14 the sample rate (= 128 sps), 00 the sample average (= 1) and 1 is start sending,*/
    if (recieved == '\n') {
     
      intensity_leds = inData.substring(0, 3).toInt();
      smpl_rate = inData.substring(4, 6).toInt();
      smpl_avg =  inData.substring(7,9).toInt();
      Started = inData.substring(10).toInt();
      Serial.println(intensity_leds);
      Serial.println(smpl_rate);
      Serial.println(smpl_avg);
      Serial.println(Started);
      //Started = inData.toInt();
      inData = ""; // Clear recieved buffer
    }
  }

  if ( Bluefruit.connected() ) {
    if( (Start_StopService.discovered())){
  if(Started ==1){
      StartCharacteristic.write8(Started);

      intensityLedsCharacteristic.write8(intensity_leds);
      
      smplRateCharacteristic.write8(smpl_rate);

      smplAvgCharacteristic.write8(smpl_avg);
  }
  if(Started ==2){
      StartCharacteristic.write8(Started);
      Started = 0;
  }
    }
    
   /* if( (Start_StopService.discovered())){
  if(Started ==1){
      StartCharacteristic.write8(Started);
  }
  if(Started ==2){
      StartCharacteristic.write8(Started);
      Started = 0;
  }
    }*/

    /*if(PPG86Service.discovered()){
      intensityLedsCharacteristic.write8(Started);
      intensity_leds = 0;
      
      smplRateCharacteristic.write8(Started);
      smpl_rate = 0;

      smplAvgCharacteristic.write8(Started);
      smpl_avg = 0;
    }*/
    
    }
  
}
