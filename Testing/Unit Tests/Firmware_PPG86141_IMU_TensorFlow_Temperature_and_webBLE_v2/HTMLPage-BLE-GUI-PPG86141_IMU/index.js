// The serviceUuid must match the serviceUuid of the device you would like to connect
// Copyright (c) 2018 p5ble
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

const BLE_ERROR_SERVICE_UUID = 0x1200;
const BLE_ERROR_CHARACTERISTIC_UUID = 0x1201;


const BLE_IMU_SERVICE_UUID = 0x1101;
const BLE_ACC_CHARACTERISTIC_UUID = 0x1102;
const BLE_GYR_CHARACTERISTIC_UUID = 0x1103;
const BLE_MAG_CHARACTERISTIC_UUID = 0x1104;

const BLE_PPG86_SERVICE_UUID = 0x1300;
////// PDLEDs ////////

////// PDsLED ////////
const BLE_LEDSEQ1A_PD1CHARACTERISTIC2_UUID = 0x1305;
const BLE_LEDSEQ1A_PD2CHARACTERISTIC2_UUID = 0x1307;
const BLE_SNR1_2PD1CHARACTERISTIC2_UUID = 0x1313;
const BLE_SNR2_2PD2CHARACTERISTIC2_UUID = 0x1314;


///////////////////////////////////////////////////////
const BLE_StartStop_SERVICE_UUID = 0x1400
let BLE_Start_CHARACTERISTIC
let BLE_IntensityLEDS_CHARACTERISTIC
let BLE_SampleRate_CHARACTERISTIC
let BLE_SampleAvg_CHARACTERISTIC
//let inputIntensityLEDS
//let inputSampleRate
//let inputSampleAvg
///////////////////////////////////////////////////////

let LEDSEQ1A_PD1Characteristic2Line;
let LEDSEQ1A_PD2Characteristic2Line;

var LEDSEQ1A_PD1CHARACTERISTIC2;
var LEDSEQ1A_PD2CHARACTERISTIC2;
var SNR1_2PD1CHARACTERISTIC2;
var SNR2_2PD2CHARACTERISTIC2;
var errorCharacteristic;
var ACCCharacteristic;
var GYRCharacteristic;
var MAGCharacteristic;

let PPG86service;
let errorStatus;
let IMUAccXLine;
let IMUAccYLine;
let IMUAccZLine;
let IMUGyrXLine;
let IMUGyrYLine;
let IMUGyrZLine;
let IMUMagXLine;
let IMUMagYLine;
let IMUMagZLine;

let gyrData = {x: null, y: null, z: null}
let pitch
let roll
let yaw

function int16ToFloat32(inputArray, startIndex, length) {
  var output = new Float32Array(inputArray.length-startIndex);
  for (var i = startIndex; i < length; i++) {
    var int = inputArray[i];
    // If the high bit is on, then it is a negative number, and actually counts backwards.
    var float = (int >= 0x8000) ? -(0x10000 - int) / 0x8000 : int / 0x7FFF;
    output[i] = float;
  }
  return output;
}

async function onConnectButtonClick() {

// ################## Received from device #############
try{

   console.log('Requesting any Bluetooth Device...');
    const device = await navigator.bluetooth.requestDevice({
     // filters: [...] <- Prefer filters to save energy & show relevant devices.
        acceptAllDevices: true,
        optionalServices:[BLE_PPG86_SERVICE_UUID,BLE_ERROR_SERVICE_UUID,BLE_IMU_SERVICE_UUID
                          ,BLE_StartStop_SERVICE_UUID]
      });

    console.log('Connecting to GATT Server...');
    const server = await device.gatt.connect();

    // Note that we could also get all services that match a specific UUID by
    // passing it to getPrimaryServices().
    console.log('Getting Services...');
    const services = await server.getPrimaryServices();

    console.log('Getting Characteristics...');
    for (const service of services) {
      console.log('> Service: ' + service.uuid);
      const characteristics = await service.getCharacteristics();

      characteristics.forEach(characteristic => {
        console.log('>> Characteristic: ' + characteristic.uuid + ' ' +
            getSupportedProperties(characteristic));

          if(characteristic.uuid== '00001201-0000-1000-8000-00805f9b34fb'){
            errorCharacteristic= characteristic;
            errorCharacteristic.addEventListener('characteristicvaluechanged',(event)=>{
   const value = event.target.value; // Read the error buffer
  const IMUError = value.getUint8(0); // Extract the value for each sensors
  const PPG86Error = value.getUint8(1);
  console.log("IMUError: ", IMUError, " PPGError: ", PPG86Error);
  document.getElementById("IMU-state").classList = IMUError ? "status-ko" : "status-ok"; // Update DOM
  document.getElementById("PPG-state").classList = PPG86Error ? "status-ko" : "status-ok";
            });
        errorCharacteristic.startNotifications();
          }

         if(characteristic.uuid== '00001102-0000-1000-8000-00805f9b34fb'){
            ACCCharacteristic= characteristic;
            ACCCharacteristic.addEventListener('characteristicvaluechanged',(event)=>{
    const value = event.target.value;
    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
   //console.log("timeStamp 111: ",timestamp);
   var v = value.getInt8(5)<<8 | value.getInt8(6);
   var x = ((v * -1) * 16) / 0x8000;

    v = value.getInt8(7)<<8 | value.getInt8(8);
   var y = ((v * -1) * 16) / 0x8000;

    v = value.getInt8(9)<<8 | value.getInt8(10);
   var z = ((v * -1) * 16) / 0x8000;
  
  IMUAccXLine.append(new Date().getTime(), x)
  IMUAccYLine.append(new Date().getTime(), y)
  IMUAccZLine.append(new Date().getTime(), z)

            }
              );
              ACCCharacteristic.startNotifications();
            }
          

          if(characteristic.uuid== '00001103-0000-1000-8000-00805f9b34fb'){
            GYRCharacteristic= characteristic;
            GYRCharacteristic.addEventListener('characteristicvaluechanged',(event) => {
  const value = event.target.value;
    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
   
  var v1 = value.getInt8(5)<<8 | value.getInt8(6);

  var x = (v1 * -1) * 2000 / 0x8000;

  v1 = value.getInt8(7)<<8 | value.getInt8(8);
  var y = (v1 * -1) * 2000 / 0x8000;

  v1 = value.getInt8(9)<<8 | value.getInt8(10);
  var z = (v1 * -1) * 2000 / 0x8000;
  
    IMUGyrXLine.append(new Date().getTime(), x)
    IMUGyrYLine.append(new Date().getTime(), y)
    IMUGyrZLine.append(new Date().getTime(), z)
  });
            GYRCharacteristic.startNotifications();
          }
 if(characteristic.uuid== '00001104-0000-1000-8000-00805f9b34fb'){
            MAGCharacteristic= characteristic;
            MAGCharacteristic.addEventListener('characteristicvaluechanged',(event) => {
  const value = event.target.value;

    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
   
    var x = (value.getInt8(4)<<8 | value.getInt8(5) * (((( 174 - 128) * 0.5) / 128) + 1));
  var y = (value.getInt8(6)<<8 | value.getInt8(7) * (((( 177 - 128) * 0.5) / 128) + 1));

  var z = (value.getInt8(8)<<8 | value.getInt8(9) * (((( 164 - 128) * 0.5) / 128) + 1));
 //console.log(x,y,z)
  IMUMagXLine.append(new Date().getTime(), x)
  IMUMagYLine.append(new Date().getTime(), y)
  IMUMagZLine.append(new Date().getTime(), z)
  });
            MAGCharacteristic.startNotifications();
          }

          if(characteristic.uuid== '00001305-0000-1000-8000-00805f9b34fb'){
            LEDSEQ1A_PD1CHARACTERISTIC2= characteristic;
            LEDSEQ1A_PD1CHARACTERISTIC2.addEventListener('characteristicvaluechanged',LEDSEQ1A_PD1CHARACTERISTIC2Changed);
            LEDSEQ1A_PD1CHARACTERISTIC2.startNotifications();
          }
         if(characteristic.uuid== '00001307-0000-1000-8000-00805f9b34fb'){
            LEDSEQ1A_PD2CHARACTERISTIC2=  characteristic;
            LEDSEQ1A_PD2CHARACTERISTIC2.addEventListener('characteristicvaluechanged',LEDSEQ1A_PD2CHARACTERISTIC2Changed);
             LEDSEQ1A_PD2CHARACTERISTIC2.startNotifications();
          }
          if(characteristic.uuid== '00001313-0000-1000-8000-00805f9b34fb'){
            SNR1_2PD1CHARACTERISTIC2=  characteristic;
            SNR1_2PD1CHARACTERISTIC2.addEventListener('characteristicvaluechanged',SNR1_2PD1CHARACTERISTIC2Changed);
            SNR1_2PD1CHARACTERISTIC2.startNotifications();
          }
          if(characteristic.uuid== '00001314-0000-1000-8000-00805f9b34fb'){
            SNR1_2PD2CHARACTERISTIC2=  characteristic;
            SNR1_2PD2CHARACTERISTIC2.addEventListener('characteristicvaluechanged',SNR1_2PD2CHARACTERISTIC2Changed);
             SNR1_2PD2CHARACTERISTIC2.startNotifications();
          }

          if(characteristic.uuid== '00001401-0000-1000-8000-00805f9b34fb'){
            BLE_Start_CHARACTERISTIC = characteristic;
          }

          if(characteristic.uuid== '00001402-0000-1000-8000-00805f9b34fb'){
            BLE_IntensityLEDS_CHARACTERISTIC = characteristic
          }

          if(characteristic.uuid== '00001403-0000-1000-8000-00805f9b34fb'){
          BLE_SampleRate_CHARACTERISTIC = characteristic
          }

          if(characteristic.uuid== '00001404-0000-1000-8000-00805f9b34fb'){
          BLE_SampleAvg_CHARACTERISTIC = characteristic
          }

      });
    }
  } catch(error) {
    console.log('Argh! ' + error);
  }
}

function startReceiving(){
   var data = new Uint8Array(1);
   data[0] = 1
   BLE_Start_CHARACTERISTIC.writeValue(data)
}

function stopReceiving(){
   var data = new Uint8Array(1);
   data[0] = 2
   BLE_Start_CHARACTERISTIC.writeValue(data)
}

function sendValues(){
  var inputIntensityLEDS = new Uint8Array(1)
  var inputSampleRate = new Uint8Array(1)
  var inputSampleAvg = new Uint8Array(1)

  inputIntensityLEDS[0] = parseInt(document.getElementById("intensity").value,10)
  
  //inputSampleRate[0] = parseInt(document.getElementById("smpl_rate").value,10)
  var selectElmt1 = document.getElementById("smpl_rate");
  var valeurselectionnee1 = selectElmt1.options[selectElmt1.selectedIndex].value
  console.log("valeurselectionnee1 "+valeurselectionnee1)
  if (valeurselectionnee1 == 25) {
      inputSampleRate[0] = 0
  }
  else if (valeurselectionnee1 == 50) {inputSampleRate[0]= 1}
  else if (valeurselectionnee1 == 84) {inputSampleRate[0]= 2}
  else if (valeurselectionnee1 == 100) {inputSampleRate[0]= 3}
  else if (valeurselectionnee1 == 200) {inputSampleRate[0]= 4}
  else if (valeurselectionnee1 == 400) {inputSampleRate[0]= 5}
  else if (valeurselectionnee1 == 128) { inputSampleRate[0]= 14
  }
  else if (valeurselectionnee1 == 256) {inputSampleRate[0]= 15}
  else if (valeurselectionnee1 == 512) {inputSampleRate[0]= 16}
  else if (valeurselectionnee1 == 1024) {inputSampleRate[0]= 17}
  else if (valeurselectionnee1 == 2048) {inputSampleRate[0]= 18}
  else {inputSampleRate[0]= 19}

  //inputSampleAvg[0] = document.getElementById("smpl_avg").value
  var selectElmt = document.getElementById("smpl_avg");
  var valeurselectionnee = selectElmt.options[selectElmt.selectedIndex].value
    console.log("valeurselectionnee "+valeurselectionnee)

  if (valeurselectionnee == 1) {
      inputSampleAvg[0] = 0
  }
  else if (valeurselectionnee == 2) {inputSampleAvg[0]= 1 
}
  else if (valeurselectionnee == 4) {inputSampleAvg[0]= 2}
  else if (valeurselectionnee == 8) {inputSampleAvg[0]= 3}
  else if (valeurselectionnee == 16) {inputSampleAvg[0]= 4}
  else if (valeurselectionnee == 32) {inputSampleAvg[0]= 5}
  else if (valeurselectionnee == 64) {inputSampleAvg[0]= 6}
  else {inputSampleAvg[0]= 7 }

  console.log("les valeurs saisies sont: "+inputIntensityLEDS[0] + " "+inputSampleRate[0]+" "+ inputSampleAvg[0])

  BLE_IntensityLEDS_CHARACTERISTIC.writeValue(inputIntensityLEDS)
  BLE_SampleRate_CHARACTERISTIC.writeValue(inputSampleRate)
  BLE_SampleAvg_CHARACTERISTIC.writeValue(inputSampleAvg)

}

window.onload = () => {
  // wait for the page to load so that the DOM elements are available
  console.log(navigator.bluetooth ? "Web-BLE Available" : "Web-BLE Unavailable");
  document.getElementById("connect-btn").addEventListener("click", onConnectButtonClick);
  document.getElementById("start-btn").addEventListener("click", startReceiving);
  document.getElementById("stop-btn").addEventListener("click", stopReceiving);

  document.getElementById("start-btn").disabled = false;
  document.getElementById("stop-btn").disabled = false;

  document.getElementById("apply-btn").addEventListener("click", sendValues);

  //document.getElementById("disconnect-btn").addEventListener("click", onDisconnected);
  
  let IMUAccSmoothie = new SmoothieChart();
  let IMUGyrSmoothie = new SmoothieChart();
  let IMUMagSmoothie = new SmoothieChart()

  IMUAccXLine = new TimeSeries();
  IMUAccYLine = new TimeSeries();
  IMUAccZLine = new TimeSeries();
  IMUGyrXLine = new TimeSeries();
  IMUGyrYLine = new TimeSeries();
  IMUGyrZLine = new TimeSeries();
  IMUMagXLine = new TimeSeries()
  IMUMagYLine = new TimeSeries()
  IMUMagZLine = new TimeSeries()

  IMUAccSmoothie.addTimeSeries(IMUAccXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUAccSmoothie.addTimeSeries(IMUAccYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUAccSmoothie.addTimeSeries(IMUAccZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });
  IMUAccSmoothie.streamTo(document.getElementById("smoothie-imu-acc"));

  IMUGyrSmoothie.addTimeSeries(IMUGyrXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUGyrSmoothie.addTimeSeries(IMUGyrYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUGyrSmoothie.addTimeSeries(IMUGyrZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });
  IMUGyrSmoothie.streamTo(document.getElementById("smoothie-imu-gyr"));

  IMUMagSmoothie.addTimeSeries(IMUMagXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUMagSmoothie.addTimeSeries(IMUMagYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUMagSmoothie.addTimeSeries(IMUMagZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });
  IMUMagSmoothie.streamTo(document.getElementById("smoothie-imu-mag"));

  let LEDSEQ1A_PD1Characteristic2Smoothie = new SmoothieChart(); // Create SmoothieChart objects
  LEDSEQ1A_PD1Characteristic2Line = new TimeSeries(); // Create a TimeSeries to store the data for each sensor
  LEDSEQ1A_PD1Characteristic2Smoothie.addTimeSeries(LEDSEQ1A_PD1Characteristic2Line, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 }); // Link TimeSeries to the SmoothieChart object and add a bit of color
  LEDSEQ1A_PD1Characteristic2Smoothie.streamTo(document.getElementById("smoothie-pd1-led")); // Link the SoothieChart object to its canvas in the HTML

  let LEDSEQ1A_PD2Characteristic2Smoothie = new SmoothieChart(); // Create SmoothieChart objects
  LEDSEQ1A_PD2Characteristic2Line = new TimeSeries(); // Create a TimeSeries to store the data for each sensor
  LEDSEQ1A_PD2Characteristic2Smoothie.addTimeSeries(LEDSEQ1A_PD2Characteristic2Line, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 }); // Link TimeSeries to the SmoothieChart object and add a bit of color
  LEDSEQ1A_PD2Characteristic2Smoothie.streamTo(document.getElementById("smoothie-pd2-led")); // Link the SoothieChart object to its canvas in the HTML
  
}

/*function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(BLE_StartStop_SERVICE_UUID, gotCharacteristics);
}

function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  // Set the first characteristic as myCharacteristic
  BLE_Start_CHARACTERISTIC = characteristics[0]
  BLE_IntensityLEDS_CHARACTERISTIC = characteristics[1]
  BLE_SampleRate_CHARACTERISTIC = characteristics[2]
  BLE_SampleAvg_CHARACTERISTIC = characteristics[3]
}*/

/* Utils */

function getSupportedProperties(characteristic) {
  let supportedProperties = [];
  for (const p in characteristic.properties) {
    if (characteristic.properties[p] === true) {
      supportedProperties.push(p.toUpperCase());
    }
  }
  return '[' + supportedProperties.join(', ') + ']';
}

async function LEDSEQ1A_PD1CHARACTERISTIC2Changed(event) {
  /*const value = event.target.value;
  const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
  const ledSeq1A_PD1Characteristic2 = (value.getUint8(4) << 24) | (value.getUint8(5) << 16) | (value.getUint8(6) << 8) | value.getUint8(7); // shifts 4 Uint8 to make a Uint32
  //console.log("timestamp: ", timestamp, "ledSeq1A_PD1Characteristic2: ", ledSeq1A_PD1Characteristic2);
    LEDSEQ1A_PD1Characteristic2Line.append(new Date().getTime(), ledSeq1A_PD1Characteristic2);*/
  const value = event.target.value;
  const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
  const data1 = (value.getUint8(4) << 24) | (value.getUint8(5) << 16) | (value.getUint8(6) << 8) | value.getUint8(7); // shifts 4 Uint8 to make a Uint32
  const data2 = (value.getUint8(8) << 24) | (value.getUint8(9) << 16) | (value.getUint8(10) << 8) | value.getUint8(11); // shifts 4 Uint8 to make a Uint32

  //console.log("timestamp: ", timestamp);

  console.log("data1: ", data1,"data2: ", data2);

  var tab;
  tab = [data1,data2]

  //console.log("data1: ", data1,"data2: ", data2,"data3: ", data3,"data4: ", data4);
  for (var i = 0; i<2; i++) {
    if(tab[i]!=0){
    LEDSEQ1A_PD1Characteristic2Line.append(new Date().getTime(), tab[i]);
  }
}
}

async function LEDSEQ1A_PD2CHARACTERISTIC2Changed(event) {
    /*const value = event.target.value;
    const ledSeq1A_PD2Characteristic2 = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
    console.log("ledSeq1A_PD2Characteristic2: ", ledSeq1A_PD2Characteristic2);
    LEDSEQ1A_PD2Characteristic2Line.append(new Date().getTime(), ledSeq1A_PD2Characteristic2);*/
  const value = event.target.value;
  const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
  const data1 = (value.getUint8(4) << 24) | (value.getUint8(5) << 16) | (value.getUint8(6) << 8) | value.getUint8(7); // shifts 4 Uint8 to make a Uint32
  const data2 = (value.getUint8(8) << 24) | (value.getUint8(9) << 16) | (value.getUint8(10) << 8) | value.getUint8(11); // shifts 4 Uint8 to make a Uint32
  
    //console.log("timestamp: ", timestamp);

  var tab;
  tab = [data1,data2]
    for (var i = 0; i<2; i++) {
      if(tab[i]!=0){
    LEDSEQ1A_PD2Characteristic2Line.append(new Date().getTime(), tab[i]);
  }
}
}

async function SNR1_2PD1CHARACTERISTIC2Changed(event) {
    const value = event.target.value;
    var snr1_2PD1CHARACTERISTIC2 = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
    
    //console.log("snr1_2PD1CHARACTERISTIC2: ", snr1_2PD1CHARACTERISTIC2);
    if(snr1_2PD1CHARACTERISTIC2 %100==0){
      var snr_neg= snr1_2PD1CHARACTERISTIC2;
          snr_neg= -(snr_neg/10000);
      //console.log("snr1_2PD1CHARACTERISTIC2: ",snr_neg);
 document.getElementById('snr1_2PD1').innerHTML = String(snr_neg);
    }
    else{
      var snr_pos= snr1_2PD1CHARACTERISTIC2;
      snr_pos = snr_pos/100;
      //console.log("snr1_2PD1CHARACTERISTIC2: ",snr_pos);
  document.getElementById('snr1_2PD1').innerHTML = String(snr_pos);
    }
}
async function SNR1_2PD2CHARACTERISTIC2Changed(event) {
    const value = event.target.value;
    const snr1_2PD2CHARACTERISTIC2 = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
    
    //console.log("snr1_2PD2CHARACTERISTIC2: ", snr1_2PD2CHARACTERISTIC2);

    if(snr1_2PD2CHARACTERISTIC2 %100==0){
      var snr_neg= snr1_2PD2CHARACTERISTIC2;
          snr_neg= -(snr_neg/10000);
     // console.log("snr1_2PD2CHARACTERISTIC2: ",snr_neg);
       document.getElementById('snr2_2PD2').innerHTML = String(snr_neg);
    }
    else{
      var snr_pos= snr1_2PD2CHARACTERISTIC2;
      snr_pos = snr_pos/100;
     // console.log("snr1_2PD2CHARACTERISTIC2: ",snr_pos);
      document.getElementById('snr2_2PD2').innerHTML = String(snr_pos);
    }
}

