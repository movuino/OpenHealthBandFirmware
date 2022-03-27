#ifndef IMUHANDLER_HPP
# define IMUHANDLER_HPP

# include "MPU9250_asukiaaa.h"

# define IMU_ADDRESS 0x69
# define ACC_RANGE 16.0
# define IMU_ACC_TAB_SIZE 11
# define IMU_GYR_TAB_SIZE 11
# define IMU_MAG_TAB_SIZE 10

class ImuHandler {
    private:
        MPU9250_asukiaaa _mpu;
        float _aX, _aY, _aZ, _gX, _gY, _gZ, _mX, _mY, _mZ;
        uint8_t _buffAcc[IMU_ACC_TAB_SIZE];
        uint8_t _buffGyr[IMU_GYR_TAB_SIZE];
        uint8_t _buffMag[IMU_MAG_TAB_SIZE];
        int     _error;
        void    _updateAcc(void);
        void    _updateGyr(void);
        void    _updateMag(void);

    public:
        ImuHandler();
        ~ImuHandler();
        int configure(void);
        void update(void);
        int getError(void);

        uint8_t *getAccTab(void);
        uint8_t *getGyrTab(void);
        uint8_t *getMagTab(void);
};

#endif