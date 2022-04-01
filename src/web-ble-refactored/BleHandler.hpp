#ifndef BLE_HANDLER_H
# define BLE_HANDLER_H

# include <stdio.h>
# include <bluefruit.h>
# include "PpgHandler.hpp"
# include "ImuHandler.hpp"

/*Error Service & characteristic*/
extern BLEService ErrorService;
extern BLECharacteristic ErrorCharacteristic;

extern BLEService IMUService;
extern BLECharacteristic AccCharacteristic;
extern BLECharacteristic GyroCharacteristic;
extern BLECharacteristic MagCharacteristic;

/* PPG Max 86140 - 86141 Service & Characteristics*/
extern BLEService PPG86Service;


/* PDsLED (2 PDs - 1 LED) */
extern BLECharacteristic ledSeq1A_PPG1Characteristic2;
extern BLECharacteristic ledSeq1A_PPG2Characteristic2;

/* SNR (Signal Noise Ratio) */
extern BLECharacteristic SNR1_2PPG1Characteristic2;
extern BLECharacteristic SNR2_2PPG2Characteristic2;

/* Utils */
extern BLEService Start_StopService;
extern BLECharacteristic StartCharacteristic;
extern BLECharacteristic intensityLedsCharacteristic;
extern BLECharacteristic smplRateCharacteristic;
extern BLECharacteristic smplAvgCharacteristic;
extern BLECharacteristic calibrationCharacteristic;

extern PpgHandler PPG;
extern ImuHandler IMU;

void connection_callback(uint16_t conn_handle);
void disconnection_callback(uint16_t conn_handle, uint8_t reason);
void startAvertising(void);
void setupErrorService(void);
void setupIMUService(void);
void setupPPGService(void);
void setupUtilsService(void);

#endif