# Firmware_PPG86141_IMU_TensorFlow_Temperature-v2

This firmware allows to send by BLE data from PPG (photo-diode(s), SNR), IMU (accel, gyro, mag), TensorFlow and Temperature between peripherals (NRF52 boards or wearable with movuino) and centrals (NRF52 boards, HTML page or app).

The example provided uses **NRF52 boards**. 

The services and characteristics used for the BLE are :

#### Error Service & characteristic
The Error Characteristic allows to send boolean errors from IMU, PPG, Temperature and TensorFlow. Thus, when error is 0 data is detected else 1.

ErrorCharacteristic (4-bytes) [0] : errorIMU, [1] = errorPPG86, [2] = errorTemp, [3] = errorTens

UUID ErrorService = **0x1200**,
UUID ErrorCharacteristic = **0x1201**

#### IMU Service & characteristics

With the IMU Service and characteristics, data from accelerometer, gyrometer and magnetometer can be send by BLE.

AccCharacteristic (11-bytes) : [0-3] = timestamp [4], [5-6] = AccX, [7-8] = AccY, [9-10]= AccZ
GyroCharacteristic (11-bytes) : [0-3] = timestamp [4], [5-6] = GX, [7-8] = GY, [9-10]= GZ
MagCharacteristic (11-bytes) : [0-3] = timestamp [4], [5-6] = MX, [7-8] = MY, [9-10]= MZ

UUID IMUService = **0x1101**,
UUID AccCharacteristic = **0x1102**,
GyroCharacteristic = **0x1103**,
MagCharacteristic = **0x1104**

#### PPG Max 86140 - 86141 Service & Characteristics

With the PPG Service and characteristics, data from PPG can be send by BLE. We can have these type of sensor configuartions :

+ 1 PD - 2 LEDS

ledSeq1A_PPG1Characteristic1 = characteristic for data taken from photo-diode

ledSeq1A_PPG1Characteristic1 (20-bytes) : [0-3] = timestamp [4], [4-7] = sample 1, [8-11] = sample 2, [12-15] = sample 3, [16-19] = sample 4

SNR1_2PPG1Characteristic2 (4-bytes) : [0-3] = SNR photo-diode

PPG86Service = **0x1300**,
ledSeq1A_PPG1Characteristic2 = **0x1301**,
SNR1_2PPG1Characteristic2 = **0x1315**,

+ 2 PDs - 1 LED

ledSeq1A_PPG1Characteristic2 = characteristic for data taken from photo-diode 1
ledSeq1A_PPG2Characteristic2 = characteristic for data taken from photo-diode 2

ledSeq1A_PPG1Characteristic2 (12-bytes) : [0-3] = timestamp [4], [4-7] = sample 1 , [8-11] = sample 2

ledSeq1A_PPG2Characteristic2 (12-bytes) : [0-3] = timestamp [4], [4-7] = sample 1 , [8-11] = sample 2

SNR1_2PPG1Characteristic2 (4-bytes) : [0-3] = SNR photo-diode 1

SNR2_2PPG1Characteristic2 (4-bytes) : [0-3] = SNR photo-diode 2

PPG86Service = **0x1300**,
ledSeq1A_PPG1Characteristic2 = **0x1305**,
ledSeq1A_PPG2Characteristic2 = **0x1307**,
SNR1_2PPG1Characteristic2 = **0x1313**,
SNR2_2PPG2Characteristic2 = **0x1314**

+ 2 PDs - 3 LEDs

ledSeq1A_PPG1Characteristic3 = characteristic for data taken from photo-diode 1
ledSeq1A_PPG2Characteristic3 = characteristic for data taken from photo-diode 2

ledSeq1A_PPG1Characteristic3 (12-bytes) : [0-3] = timestamp [4], [4-7] = sample 1 , [8-11] = sample 2

ledSeq1A_PPG2Characteristic3 (12-bytes) : [0-3] = timestamp [4], [4-7] = sample 1 , [8-11] = sample 2

SNR1_3PPG1Characteristic3 (4-bytes) : [0-3] = SNR photo-diode 1

SNR2_3PPG1Characteristic3 (4-bytes) : [0-3] = SNR photo-diode 2

PPG86Service = **0x1300**,
ledSeq1A_PPG1Characteristic2 = **0x1309**,
ledSeq1A_PPG2Characteristic2 = **0x1311**,
SNR1_2PPG1Characteristic2 = **0x1317**,
SNR2_2PPG2Characteristic2 = **0x1318**

#### Temperature Service & characteristics

With the Temperature Service and characteristic, the temperature measurement is sent by BLE.

TempCharacteristic (8-bytes) : [0-3] = NineRam, [4-7] = SixRam

TempService = **UUID16_SVC_HEALTH_THERMOMETER**,
TempCharacteristic = **UUID16_CHR_TEMPERATURE_MEASUREMENT**,
TempCharacteristicType= **UUID16_CHR_TEMPERATURE_TYPE**


#### TensorFlow Service & characteristics

With the TensorFlow Service and characteristic, data allowing to know the  gesture recognition ("flap", "still", "walk") are sent.

PossCharacteristic (12-bytes) : [0-3] = flap, [4-7] = still, [8-11] = walk

TensorFlowService = **0x1500**
PossCharacteristic = **0x1501**


#### HeartRate Service & characteristics

In order to detect the peripheral in the Bluetooth Settings for iPhone, we added the HeartRate taken from Adafruit Bluefruit nrf52 librairies --> Peripheral. However, it is not sent by BLE. 

### Firmware_PPG86141_IMU_TensorFlow_Temperature

To send and receive data between peripherals (NRF52 boards or wearable with movuino) and centrals (NRF52 boards), this firmware is used.



### Firmware_PPG86141_IMU_TensorFlow_Temperature_and_webBLE

To send and receive data between peripherals (NRF52 boards or wearable with movuino) and centrals (HTML page opened with Chrome), this firmware is used.

### MPU9250_asukiaaa
This library allows to use the IMU with acc, gyro and mag.

### Max86141
This library allows to read data from LEDs and Photo-diodes in order to measure Heart Rate or Oxygen Saturation


### ProtoCentral_MLX90632_Non-contact_temperature_Library
This library is use to measure temperature.


## Installation
+ **Max86141** : **version 1.0.4**
+ **TensorFlow**: Arduino_TensorFlowLite librairie version **1.15.0-ALPHA**
+ **MPU9250_asukiaaa** : **version 1.5.11**

## Use

+ Choose your PPG sensor type (PDLEDs, PDsLED or PDsLEDs) by activating the #define
<p align="center"><img width="200" src="https://user-images.githubusercontent.com/47628329/156622531-7f8c6de8-a089-4c18-8624-6c1727f6303c.png"></p>

+ To read data from PPG, IMU, Temperature, TensorFlow activate the #define from peripheral and central
<p align="center"><img width="200" src="https://user-images.githubusercontent.com/47628329/156010773-e1a3c952-65e3-4916-afef-292665ef7b79.png"></p>

+ To read data on Serial Monitor or send by BLE activate the #define from peripheral
<p align="center"><img width="200" src="https://user-images.githubusercontent.com/47628329/156010756-466be41c-46be-4552-b925-e32fa0832059.png"></p>
