#include "ImuHandler.hpp"

ImuHandler::ImuHandler(): _error(0), _mpu(IMU_ADDRESS)
{
    memset(this->_buffAcc, 0, sizeof(this->_buffAcc));
    memset(this->_buffGyr, 0, sizeof(this->_buffGyr));
    memset(this->_buffMag, 0, sizeof(this->_buffMag));
}

ImuHandler::~ImuHandler()
{

}

int ImuHandler::configure(void)
{
    uint8_t sensorId;

    if (this->_mpu.readId(&sensorId) == 0) {
    Serial.println("MPU9250 connection succeeded !");
    Serial.println("sensorId: " + String(sensorId));
    }
    else {
        Serial.println("Error cannot read sensor ID");
        this->_error = 1;
    }
    if (!this->_error)
    {
        this->_mpu.beginAccel();
        this->_mpu.beginGyro();
        this->_mpu.beginMag();
    }
    return this->_error;
}

void ImuHandler::_updateAcc(void)
{
    if (this->_mpu.accelUpdate() == 0)
    {
        uint32_t timestamp = millis();
        this->_buffAcc[3] = (uint8_t)timestamp;
        this->_buffAcc[2] = (uint8_t)(timestamp >>= 8);
        this->_buffAcc[1] = (uint8_t)(timestamp >>= 8);
        this->_buffAcc[0] = (uint8_t)(timestamp >>= 8);
        this->_buffAcc[4] = 16;

        for (int i = 5; i <= 10; i++) {
            this->_buffAcc[i] = this->_mpu.accelBuff[i - 5];
        }
    } else {
        Serial.println("Cannot read Acc");
    }
}

void ImuHandler::_updateGyr(void)
{
    if (this->_mpu.gyroUpdate() == 0)
    {
        uint32_t timestamp = millis();
        this->_buffGyr[3] = (uint8_t)timestamp;
        this->_buffGyr[2] = (uint8_t)(timestamp >>= 8);
        this->_buffGyr[1] = (uint8_t)(timestamp >>= 8);
        this->_buffGyr[0] = (uint8_t)(timestamp >>= 8);
        this->_buffGyr[4] = 16;

        for (int i = 5; i <= 10; i++) {
            this->_buffGyr[i] = this->_mpu.gyroBuff[i - 5];
        }
    } else {
        Serial.println("Cannot read Gyr");
    }
}

void ImuHandler::_updateMag(void)
{
    if (this->_mpu.magUpdate() == 0)
    {
        uint32_t timestamp = millis();
        this->_buffGyr[3] = (uint8_t)timestamp;
        this->_buffGyr[2] = (uint8_t)(timestamp >>= 8);
        this->_buffGyr[1] = (uint8_t)(timestamp >>= 8);
        this->_buffGyr[0] = (uint8_t)(timestamp >>= 8);

        for (int i = 4; i <= 9; i++) {
            this->_buffMag[i] = this->_mpu.magBuff[i - 4];
        }
    } else {
        Serial.println("Cannot read Mag");
    }
}

void ImuHandler::update(void)
{
    this->_updateAcc();
    this->_updateGyr();
    this->_updateMag();
}

int ImuHandler::getError(void)
{
    return this->_error;
}

uint8_t *ImuHandler::getAccTab(void)
{
    return this->_buffAcc;
}

uint8_t *ImuHandler::getGyrTab(void)
{
    return this->_buffGyr;
}

uint8_t *ImuHandler::getMagTab(void)
{
    return this->_buffMag;
}
