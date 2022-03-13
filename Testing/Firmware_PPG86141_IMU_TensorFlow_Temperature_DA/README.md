# Firmware_PPG86141_IMU_TensorFlow_Temperature

This firmware allows to send by BLE data from PPG (photo-diode(s), SNR), IMU (accel, gyro, mag), TensorFlow and Temperature between peripherals (NRF52 boards or wearable with movuino) and centrals (NRF52 boards, HTML page or app).

The example provided uses **NRF52 boards**. 

The services and characteristics used for the BLE are :

#### Error Service & characteristic
The Error Characteristic allows to send boolean errors from IMU, PPG, Temperature and TensorFlow. Thus, when error is 0 data is detected else 1.

|        | ErrorService | ErrorCharacteristic |
|:------:|:------:|:------:|
|UUID | **0x1200** | **0x1201** |

| 4 bytes| byte 1 | byte 2 | byte 3 | byte 4 |    
|:------:|:------:|:------:|:------:|:------:|
|ErrorCharacteristic | errorIMU | errorPPG86 | errorTemp | errorTens |

#### IMU Service & characteristics

With the IMU Service and characteristics, data from accelerometer, gyrometer and magnetometer can be send by BLE.

|        | IMUService | AccCharacteristic | GyroCharacteristic | MagCharacteristic |
|:------:|:------:|:------:|:------:|:------:|
|UUID | **0x1101** | **0x1102** | **0x1103** | **0x1104** |

|11 bytes        | 4 bytes (0-3) | 2 bytes (5-6) | 2 bytes (7-8) | 2 bytes (9-10) |    
|:------:|:------:|:------:|:------:|:------:|
|AccCharacteristic | timestamp | AccX | AccY | AccZ |
|GyroCharacteristic | timestamp | GX | GY | GZ |
|MagCharacteristic | timestamp | MX | MY | MZ |

#### PPG Max 86140 - 86141 Service & Characteristics

With the PPG Service and characteristics, data from PPG can be send by BLE. We can have these type of sensor configuartions :

+ 1 PD - 2 LEDS
**ledSeq1A_PPG1Characteristic1** = characteristic for data taken from photo-diode


|        | PPG86Service | ledSeq1A_PPG1Characteristic1 | SNR1_2PPG1Characteristic1 |
|:------:|:------:|:------:|:------:|
|UUID | **0x1300** | **0x1301** | **0x1315** |

| 20 bytes | 4 bytes (0-3) | 4 bytes (4-7) | 4 bytes (8-11) | 4 bytes (12-15) | 4 bytes (16-19) |    
|:------:|:------:|:------:|:------:|:------:|:------:|
|ledSeq1A_PPG1Characteristic1 | timestamp | sample 1 | sample 2 | sample 3 | sample 4 |

|4 bytes | 4 bytes (0-3) |
|:------:|:------:|
|SNR1_2PPG1Characteristic1 | SNR photo-diode 1 |



+ 2 PDs - 1 LED

**ledSeq1A_PPG1Characteristic2** = characteristic for data taken from photo-diode 1
**ledSeq1A_PPG2Characteristic2** = characteristic for data taken from photo-diode 2


|        | PPG86Service | ledSeq1A_PPG1Characteristic2 | ledSeq1A_PPG2Characteristic2 | SNR1_2PPG1Characteristic2 | SNR2_2PPG2Characteristic2 |
|:------:|:------:|:------:|:------:|:------:|:------:|
|UUID | **0x1300** | **0x1305** | **0x1307** | **0x1313** | **0x1314** |

| 20 bytes | 4 bytes (0-3) | 4 bytes (4-7) | 4 bytes (8-11) | 4 bytes (12-15) | 4 bytes (16-19) |    
|:------:|:------:|:------:|:------:|:------:|:------:|
|ledSeq1A_PPG1Characteristic2 | timestamp | sample 1 | sample 2 | sample 3 | sample 4 |
|ledSeq1A_PPG2Characteristic2 | timestamp | sample 1 | sample 2 | sample 3 | sample 4 |

|4 bytes | 4 bytes (0-3) |
|:------:|:------:|
|SNR1_2PPG1Characteristic2 | SNR photo-diode 1 |
|SNR2_2PPG1Characteristic2 | SNR photo-diode 2 |


+ 2 PDs - 3 LEDs

**ledSeq1A_PPG1Characteristic3** = characteristic for data taken from photo-diode 1
**ledSeq1A_PPG2Characteristic3** = characteristic for data taken from photo-diode 2


|        | PPG86Service | ledSeq1A_PPG1Characteristic3 | ledSeq1A_PPG2Characteristic3 | SNR1_2PPG1Characteristic3 | SNR2_2PPG2Characteristic3 |
|:------:|:------:|:------:|:------:|:------:|:------:|
|UUID | **0x1300** | **0x1309** | **0x1311** | **0x1317** | **0x1318** |

| 12 bytes | 4 bytes (0-3) | 4 bytes (4-7) | 4 bytes (8-11) |    
|:------:|:------:|:------:|:------:|
|ledSeq1A_PPG1Characteristic3 | timestamp | sample 1 | sample 2 | 
|ledSeq1A_PPG2Characteristic3 | timestamp | sample 1 | sample 2 |

|4 bytes | 4 bytes (0-3) |
|:------:|:------:|
|SNR1_2PPG1Characteristic3 | SNR photo-diode 1 |
|SNR2_2PPG1Characteristic3 | SNR photo-diode 2 |


#### Temperature Service & characteristics

With the Temperature Service and characteristic, the temperature measurement is sent by BLE.

|        | TempService | TempCharacteristic | TempCharacteristicType |
|:------:|:------:|:------:|:------:|
|UUID | **UUID16_SVC_HEALTH_THERMOMETER** | **UUID16_CHR_TEMPERATURE_MEASUREMENT** | **UUID16_CHR_TEMPERATURE_TYPE** |

|8 bytes | 4 bytes (0-3) | 4 bytes (4-7) |
|:------:|:------:|:------:|
|TempCharacteristic | NineRam | SixRam |


#### TensorFlow Service & characteristics

With the TensorFlow Service and characteristic, data allowing to know the  gesture recognition ("flap", "still", "walk") are sent.

|        | TensorFlowService | PossCharacteristic |
|:------:|:------:|:------:|
|UUID | **0x1500** | **0x1501** |

|12 bytes | 4 bytes (0-3) | 4 bytes (4-7) | 4 bytes (8-11) |
|:------:|:------:|:------:|:------:|
|PossCharacteristic | flap | still | walk |


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
