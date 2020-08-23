## OpenHealthBandFirmware
Firmware for Open Health Band
- ##### Throughput example :
  * work as a demonstrator of maximal throughtput
  * not customized yet for data sending
  * test results :
      - iPhone 11 as central  : 60 kB/s
      - other nrf52840 as central : ~100 kB/s
- ##### BLueart example :
  * testing in progress

- ##### Unit Test folder
  => Tests of each of the funcionnalities
  * ###### I2C scanner (modified from arduino I2c scanner)
  fully working band should return :
   -  0x48 : MAX30205 (Human temperature sensor)
   -  0x57 : MAX30105 (PPG)
   -  0x69 : MPU9250 (9-axis IMU)

  * ###### MAX30205_temperature
  => returns temperature

  * ###### MAX30105_Basic_Readings
  => Returns absorbance in R, G,IR channels

  * ###### adc_vbat
  => Returns % of charge of battery (on A6 P0.29 on openhealthband).

  * ###### MPU9250_rawData
  => Returns raw data of 9-axis IMU

- ##### Arduino Boards package used : Adafruit NRF52 v0.20.5 (temporary fix)
https://www.adafruit.com/package_adafruit_index.json Should be added into arduino IDE in file -> prefrences -> additionnal board manager URLs)

  The open health band will be recognized by arduino as adafuit feather nrf52840
but this profile has to be customized to make it fully work with open OpenHealthBand

  => replace the contents of your folder :<i> xxx\Arduino15\packages\adafruit\hardware\nrf52\0.20.5\variants\feather_nrf52840_express</i>  by the content of the folder " Arduino variant board definition "



- ##### Libraries used
<b> MAX30205 : </b> Protocentral MAX30205 v1.0.0 (from arduino library manager) : https://github.com/protocentral/ProtoCentral_MAX30205

  <b> MAX30105 : </b> Sparkfun MAX3010x Sensor library v1.1.1 (from arduino library manager) : https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

    <b> MPU9250 : </b> MPU9250 asukiaaa v1.5.8 (from arduino library manager) : https://github.com/asukiaaa/MPU9250_asukiaaa
