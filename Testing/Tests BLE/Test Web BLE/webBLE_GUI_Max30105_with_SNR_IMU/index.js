const BLE_ERROR_SERVICE_UUID = 0x1200;
const BLE_ERROR_CHARACTERISTIC_UUID = 0x1201;

const BLE_PPG_SERVICE_UUID = 0x1165;
const BLE_PPG_CHARACTERISTIC_UUID = 0x1166;
const BLE_SNR_CHARACTERISTIC_UUID = 0x1167;

const BLE_IMU_SERVICE_UUID = 0x1101;
const BLE_ACC_CHARACTERISTIC_UUID = 0x1102;
const BLE_GYR_CHARACTERISTIC_UUID = 0x1103;
const BLE_MAG_CHARACTERISTIC_UUID = 0x1104;


let errorStatus;
let PPGRedLine;
let PPGGreenLine;
let PPGIrLine;
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
  console.log("Requesting any Bluetooth device...");
  const device = await navigator.bluetooth.requestDevice({
    // open dialog for the user to pair with a device
    acceptAllDevices: true,
    optionalServices: [BLE_ERROR_SERVICE_UUID, BLE_IMU_SERVICE_UUID, BLE_PPG_SERVICE_UUID],
  });
  console.log("Requested " + device.name + " (" + device.id + ")");

  const server = await device.gatt.connect(); // Connects to the chosen device
  console.log("server", server);

  const errorService = await server.getPrimaryService(BLE_ERROR_SERVICE_UUID); // Get the selected service
  console.log("service", errorService);

  const errorCharacteristic = await errorService.getCharacteristic(BLE_ERROR_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("characteristic", errorCharacteristic);

  const value = await errorCharacteristic.readValue(); // Read the error buffer
  const IMUError = value.getUint8(0); // Extract the value for each sensors
  const PPGError = value.getUint8(1);
  console.log("IMUError: ", IMUError, " PPGError: ", PPGError);
  document.getElementById("IMU-state").classList = IMUError ? "status-ko" : "status-ok"; // Update DOM
  document.getElementById("PPG-state").classList = PPGError ? "status-ko" : "status-ok";

  const PPGservice = await server.getPrimaryService(BLE_PPG_SERVICE_UUID); // Get the selected service
  console.log("Error service", PPGservice);

  const PPGCharacteristic = await PPGservice.getCharacteristic(BLE_PPG_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("Error characteristic", PPGCharacteristic);

  await PPGCharacteristic.startNotifications(); // Enable notification from the characteristic
  console.log("PPGCharacteristic Notifications started");
  PPGCharacteristic.addEventListener("characteristicvaluechanged", (event) => {
    const value = event.target.value;
    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
    const redPPG = (value.getUint8(4) << 24) | (value.getUint8(5) << 16) | (value.getUint8(6) << 8) | value.getUint8(7);
    const irPPG = (value.getUint8(8) << 24) | (value.getUint8(9) << 16) | (value.getUint8(10) << 8) | value.getUint8(11);
    const greenPPG = (value.getUint8(12) << 24) | (value.getUint8(13) << 16) | (value.getUint8(14) << 8) | value.getUint8(15);
    //console.log("timestamp: ", timestamp, "redPPG: ", redPPG, "irPPG: ", irPPG, "greenPPG: ", greenPPG);
    PPGRedLine.append(new Date().getTime(), redPPG);
    PPGGreenLine.append(new Date().getTime(), greenPPG);
	PPGIrLine.append(new Date().getTime(), irPPG);
	//console.log(new Date().getTime())
  });
////////////////////////////////////////////////////////////////
const SNRCharacteristic = await PPGservice.getCharacteristic(BLE_SNR_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("Error characteristic", SNRCharacteristic);

  await SNRCharacteristic.startNotifications(); // Enable notification from the characteristic
  console.log("SNRCharacteristic Notifications started");
  SNRCharacteristic.addEventListener("characteristicvaluechanged", (event) => {
    const value = event.target.value;
    var snr1_2PPG1CHARACTERISTIC2 = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3); // shifts 4 Uint8 to make a Uint32
    
    //console.log("snr1_2PPG1CHARACTERISTIC2: ", snr1_2PPG1CHARACTERISTIC2);
    if(snr1_2PPG1CHARACTERISTIC2 %100==0){
      var snr_neg= snr1_2PPG1CHARACTERISTIC2;
          snr_neg= -(snr_neg/10000);
      console.log("snr1_2PPG1CHARACTERISTIC2: ",snr_neg);
 document.getElementById('snr1_2PPG1').innerHTML = String(snr_neg);
    }
    else{
      var snr_pos= snr1_2PPG1CHARACTERISTIC2;
      snr_pos = snr_pos/100;
      console.log("snr1_2PPG1CHARACTERISTIC2: ",snr_pos);
  document.getElementById('snr1_2PPG1').innerHTML = String(snr_pos);
    }
  }
  );

/////////////////////////////////////////////////////////////////

  const IMUservice = await server.getPrimaryService(BLE_IMU_SERVICE_UUID); // Get the selected service
  console.log("Error service", IMUservice);

  const ACCCharacteristic = await IMUservice.getCharacteristic(BLE_ACC_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("Error characteristic", ACCCharacteristic);

  const GYRCharacteristic = await IMUservice.getCharacteristic(BLE_GYR_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("Error characteristic", GYRCharacteristic);

  const MAGCharacteristic = await IMUservice.getCharacteristic(BLE_MAG_CHARACTERISTIC_UUID); // Get the selected characteristic
  console.log("Error characteristic", MAGCharacteristic);

  await ACCCharacteristic.startNotifications(); // Enable notification from the characteristic
  console.log("ACCCharacteristic Notifications started");
  ACCCharacteristic.addEventListener("characteristicvaluechanged", (event) => {
	const value = event.target.value;
    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
    const x = ((value.getInt16(5) * -1) * 16) / 0x8000
    const y = ((value.getInt16(7) * -1) * 16) / 0x8000
	const z = ((value.getInt16(9) * -1) * 16) / 0x8000
	// pitch = 180 * Math.atan(x/Math.sqrt(y*y + z*z))/Math.PI;
	// roll = 180 * Math.atan(y/Math.sqrt(x*x + z*z))/Math.PI;
	// yaw = 180 * Math.atan(z/Math.sqrt(x*x + z*z))/Math.PI;
	pitch = Math.atan2(y, (Math.sqrt(x**2 + y**2)))
	roll = Math.atan2(-x ,( Math.sqrt(y**2 + z**2)));
	

	console.log(roll, pitch, yaw)
	IMUAccXLine.append(new Date().getTime(), roll)
	IMUAccYLine.append(new Date().getTime(), pitch)
	IMUAccZLine.append(new Date().getTime(), yaw)

	// console.log(pitch, roll)
	// gyrData = {x, y, z}

	//console.log("acc timestamp: ", timestamp, "x: ", x, "y: ", y, "z: ", z);
  });

  await GYRCharacteristic.startNotifications(); // Enable notification from the characteristic
  console.log("GyrCharacteristic Notifications started");
  GYRCharacteristic.addEventListener("characteristicvaluechanged", (event) => {
	const value = event.target.value;
    const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
    const x = ((value.getInt16(5) * -1) * 2000) / 0x8000
    const y = ((value.getInt16(7) * -1) * 2000) / 0x8000
	const z = ((value.getInt16(9) * -1) * 2000) / 0x8000
	IMUGyrXLine.append(new Date().getTime(), x)
	IMUGyrYLine.append(new Date().getTime(), y)
	IMUGyrZLine.append(new Date().getTime(), z)
	//console.log("gyro timestamp: ", timestamp, "x: ", x, "y: ", y, "z: ", z);
  });

  await MAGCharacteristic.startNotifications(); // Enable notification from the characteristic
  console.log("magCharacteristic Notifications started");
  MAGCharacteristic.addEventListener("characteristicvaluechanged", (event) => {
	const value = event.target.value;
	//console.log("mag" ,value)
    /*const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
    const x = value.getFloat32(4)
	const y = value.getFloat32(8)
	const z = value.getFloat32(12)*/
   const timestamp = (value.getUint8(0) << 24) | (value.getUint8(1) << 16) | (value.getUint8(2) << 8) | value.getUint8(3);
  var x = (value.getInt8(4)<<8 | value.getInt8(5) * (((( 174 - 128) * 0.5) / 128) + 1));
  var y = (value.getInt8(6)<<8 | value.getInt8(7) * (((( 177 - 128) * 0.5) / 128) + 1));
  var z = (value.getInt8(8)<<8 | value.getInt8(9) * (((( 164 - 128) * 0.5) / 128) + 1));
  
	IMUMagXLine.append(new Date().getTime(), x)
	IMUMagYLine.append(new Date().getTime(), y)
	IMUMagZLine.append(new Date().getTime(), z)
	// console.log(x)
	let Yh = (y * Math.cos(roll)) - (z * Math.sin(roll));
	let Xh = (x * Math.cos(pitch))+(y * Math.sin(roll)*Math.sin(pitch)) + (z * Math.cos(roll) * Math.sin(pitch));
	yaw = Math.atan2(Yh, Xh)
	//console.log("gyro timestamp: ", timestamp, "x: ", x, "y: ", y, "z: ", z);
  });
}

window.onload = () => {
  // wait for the page to load so that the DOM elements are available
  console.log(navigator.bluetooth ? "Web-BLE Available" : "Web-BLE Unavailable");
  document.getElementById("connect-btn").addEventListener("click", onConnectButtonClick);
  let PPGRedSmoothie = new SmoothieChart(); // Create SmoothieChart objects
  let PPGiRSmoothie = new SmoothieChart();
  let PPGGreenSmoothie = new SmoothieChart();
  let IMUAccSmoothie = new SmoothieChart()
  let IMUGyrSmoothie = new SmoothieChart()
  let IMUMagSmoothie = new SmoothieChart()
  PPGRedLine = new TimeSeries(); // Create a TimeSeries to store the data for each sensor
  PPGGreenLine = new TimeSeries();
  PPGIrLine = new TimeSeries();
  IMUAccXLine = new TimeSeries()
  IMUAccYLine = new TimeSeries()
  IMUAccZLine = new TimeSeries()
  IMUGyrXLine = new TimeSeries()
  IMUGyrYLine = new TimeSeries()
  IMUGyrZLine = new TimeSeries()
  IMUMagXLine = new TimeSeries()
  IMUMagYLine = new TimeSeries()
  IMUMagZLine = new TimeSeries()
  PPGRedSmoothie.streamTo(document.getElementById("smoothie-ppg-red")); // Link the SoothieChart object to its canvas in the HTML
  PPGRedSmoothie.addTimeSeries(PPGRedLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 }); // Link TimeSeries to the SmoothieChart object and add a bit of color
  PPGiRSmoothie.streamTo(document.getElementById("smoothie-ppg-ir"));
  PPGiRSmoothie.addTimeSeries(PPGIrLine, { strokeStyle: "rgb(171, 14, 14)", lineWidth: 3 });
  PPGGreenSmoothie.streamTo(document.getElementById("smoothie-ppg-green"));
  PPGGreenSmoothie.addTimeSeries(PPGGreenLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUAccSmoothie.streamTo(document.getElementById("smoothie-imu-acc"));
  IMUAccSmoothie.addTimeSeries(IMUAccXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUAccSmoothie.addTimeSeries(IMUAccYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUAccSmoothie.addTimeSeries(IMUAccZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });
  IMUGyrSmoothie.streamTo(document.getElementById("smoothie-imu-gyr"));
  IMUGyrSmoothie.addTimeSeries(IMUGyrXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUGyrSmoothie.addTimeSeries(IMUGyrYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUGyrSmoothie.addTimeSeries(IMUGyrZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });
  IMUMagSmoothie.streamTo(document.getElementById("smoothie-imu-mag"));
  IMUMagSmoothie.addTimeSeries(IMUMagXLine, { strokeStyle: "rgb(255, 0, 0)", lineWidth: 3 });
  IMUMagSmoothie.addTimeSeries(IMUMagYLine, { strokeStyle: "rgb(0, 255, 0)", lineWidth: 3 });
  IMUMagSmoothie.addTimeSeries(IMUMagZLine, { strokeStyle: "rgb(0, 0, 255)", lineWidth: 3 });

}