#include "PpgHandler.hpp"

PpgHandler::PpgHandler():
    _error(0),
    _startTime(0),
    _sampesTaken(0),
    _snrPD1(0),
    _snrPD2(0),
    _isCalibrating(0),
    _correctionCallback(NULL),
    _sampleAvg(0),
    _sampleRate(0)
{
    memset(this->_ledMode, 0, 10 * sizeof(int));
}

PpgHandler::~PpgHandler()
{
}

 void PpgHandler::setCorrectionCallback(void (*f)(int))
 {
    this->_correctionCallback = f;
 }

uint8_t *PpgHandler::getPD1Tab(void)
{
    return this->_PD1Tab;
}

uint8_t *PpgHandler::getPD2Tab(void)
{
    return this->_PD2Tab;
}

uint8_t *PpgHandler::getSNR1Tab(void)
{
    return this->_SNR1Tab;
}

uint8_t *PpgHandler::getSNR2Tab(void)
{
    return this->_SNR2Tab;
}

int PpgHandler::configure(void)
{
    pinMode(SS_PIN, OUTPUT);
    digitalWrite(SS_PIN, HIGH);
    
    this->_pulseOx1.spi = new SPIClass(SPI);
    this->_pulseOx1.SS = SS_PIN;
    this->_pulseOx1.spi->begin();
    delay(100);
    this->_pulseOx1.setDebug(true);
    
    size_t sequence_size = makeLedConfig(rgbLED_G |     // Green LED selected (Sequence 1A, 0-3)
                                            DA,         // Direct Ambient (Sequence 2B, 4-9)
                                            this->_ledMode);
    Serial.println(sequence_size);
    this->_pulseOx1.initialisation(
        2,              // nb_pds
        this->_ledMode,  // LedMode
        sequence_size,  // Number of sequences
        10,             // LED Intensity
        0x00,           // Sample average
        0xE,            // Sample rate
        0x3,            // Pulse width
        0x2,            // ADC Range= 16uA
        SPICLK
    );

    Serial.println(F("--Read Register-- System Control"));
    Serial.println(this->_pulseOx1.read_reg(0x0D));

    Serial.println(F("--Read Register-- PART_ID"));
    uint8_t part_id = this->_pulseOx1.read_reg(0xFF);
    Serial.println(part_id);

    Serial.println(F("--Read Temp-- 0x40"));
    this->_pulseOx1.write_reg(0x40, 0xFF);
    Serial.println(this->_pulseOx1.read_reg(0x41));

    switch (part_id)
    {
    case 36:
        Serial.println(F("MAX86140 connection succeeded !"));
        break;
    case 37:
        Serial.println(F("MAX86140 connection succeeded !"));
        break;
    default:
        Serial.println(F("Connection failed !"));
        this->_error = 1;
        break;
    }

    delay(1000);

    this->_pulseOx1.setDebug(false);
    this->_startTime = millis();
    
    return (this->_error);
}

int PpgHandler::_calibrate(void)
{
    long t;

    t = millis();
    Serial.println(this->_snrPD1);
    if ((this->_snrPD1 > 80.0 || this->_snrPD2 > 80.0) && t - this->_lastCorrection > CORRECTION_MIN_INTERVAL)
    {
        Serial.println("SATURATION");
        const int newIntensity = this->_pulseOx1.getIntensityLed() * 0.9;
        this->_pulseOx1.setIntensityLed(newIntensity, this->_ledMode);
        this->_correctionCallback(newIntensity);
        this->_lastCorrection = millis();
        return 1;
    } else if (this->_pulseOx1.getIntensityLed() == 255) {
        this->_lastCorrection = millis();
    }
    else if (t - _lastCorrection > CORRECTION_MIN_INTERVAL)
    {
        this->_isCalibrating = 0;
    }
    return 0;
}

int PpgHandler::update(void)
{
    static int cpt1 = 0;
    static int cpt2 = 0;

    if (this->_isCalibrating)
        this->_calibrate();

    uint8_t intStatus = this->_pulseOx1.read_reg(REG_INT_STAT_1);
    bool isFlagAFull = (intStatus & 0x80) >> 7;
    
    /* Make sure we only continue if we have enough data */
    if (!isFlagAFull)
    {
        return 1;
    }

    this->_pulseOx1.device_data_read1();
    this->_sampesTaken += 2;

    bool interruptStatusWithALC = this->_pulseOx1.read_reg(REG_INT_STAT_1) >> 8;

    if (interruptStatusWithALC == 0)
    {
        uint32_t timestamp;
        /* Pointer to send only 2 samples by Bluetooth (PD1) */
        if (this->_pulseOx1.tab_ledSeq1A_PD1[0] != 0 && this->_pulseOx1.tab_ledSeq1A_PD1[1] != 0)
        {
            /* Pointer to send only 2 samples by Bluetooth (PD1) */
            this->_pulseOx1.signalData_ledSeq1A_PD1[cpt1] = this->_pulseOx1.tab_ledSeq1A_PD1[0];
            this->_pulseOx1.signalData_ledSeq1A_PD1[cpt1 + 1] = this->_pulseOx1.tab_ledSeq1A_PD1[1];

            /* Pointer to send only 2 samples by Bluetooth (PD1) */
            timestamp = millis();
            _PD1Tab[3] = (uint8_t)timestamp;
            _PD1Tab[2] = (uint8_t)(timestamp >>= 8);
            _PD1Tab[1] = (uint8_t)(timestamp >>= 8);
            _PD1Tab[0] = (uint8_t)(timestamp >>= 8);


            _PD1Tab[7] = (uint8_t)this->_pulseOx1.tab_ledSeq1A_PD1[0];
            _PD1Tab[6] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
            _PD1Tab[5] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);
            _PD1Tab[4] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[0] >>= 8);

            _PD1Tab[11] = (uint8_t)this->_pulseOx1.tab_ledSeq1A_PD1[1];
            _PD1Tab[10] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
            _PD1Tab[9] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);
            _PD1Tab[8] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD1[1] >>= 8);

            cpt1 += 2;
            if (cpt1 == SIZE)
            {
                this->_snrPD1 = this->_pulseOx1.signaltonoise(this->_pulseOx1.signalData_ledSeq1A_PD1, SIZE);
                this->_snrPD1 = this->_snrPD1 > 1000.0 ? 999.0 : this->_snrPD1; // prevent 'inf' from getting down the line

                int val = 100 * this->_snrPD1;
                if (val < 0) {
                    int a = -100 * val;
                    this->_SNR1Tab[3] = (uint8_t)a;
                    this->_SNR1Tab[2] = (uint8_t)(a >>= 8);
                    this->_SNR1Tab[1] = (uint8_t)(a >>= 8);
                    this->_SNR1Tab[0] = (uint8_t)(a >>= 8);
                }
                else {
                    this->_SNR1Tab[3] = (uint8_t)val;
                    this->_SNR1Tab[2] = (uint8_t)(val >>= 8);
                    this->_SNR1Tab[1] = (uint8_t)(val >>= 8);
                    this->_SNR1Tab[0] = (uint8_t)(val >>= 8);
                }
                cpt1 = 0;
            }
        }

        if (this->_pulseOx1.tab_ledSeq1A_PD2[0] && this->_pulseOx1.tab_ledSeq1A_PD2[1])
        {
            /* Pointer to send only 2 samples by Bluetooth (PD1) */
            this->_pulseOx1.signalData_ledSeq1A_PD2[cpt1] = this->_pulseOx1.tab_ledSeq1A_PD2[0];
            this->_pulseOx1.signalData_ledSeq1A_PD2[cpt1 + 1] = this->_pulseOx1.tab_ledSeq1A_PD2[1];

            /* Pointer to send only 2 samples by Bluetooth (PD1) */
            timestamp = millis();
            _PD2Tab[3] = (uint8_t)timestamp;
            _PD2Tab[2] = (uint8_t)(timestamp >>= 8);
            _PD2Tab[1] = (uint8_t)(timestamp >>= 8);
            _PD2Tab[0] = (uint8_t)(timestamp >>= 8);


            _PD2Tab[7] = (uint8_t)this->_pulseOx1.tab_ledSeq1A_PD2[0];
            _PD2Tab[6] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
            _PD2Tab[5] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);
            _PD2Tab[4] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[0] >>= 8);

            _PD2Tab[11] = (uint8_t)this->_pulseOx1.tab_ledSeq1A_PD2[1];
            _PD2Tab[10] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
            _PD2Tab[9] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);
            _PD2Tab[8] = (uint8_t)(this->_pulseOx1.tab_ledSeq1A_PD2[1] >>= 8);

            cpt2 += 2;
            if (cpt2 == SIZE)
            {
                this->_snrPD2 = this->_pulseOx1.signaltonoise(this->_pulseOx1.signalData_ledSeq1A_PD2, SIZE);
                this->_snrPD2 = this->_snrPD2 > 1000.0 ? 999.0 : this->_snrPD2; // prevent 'inf' from getting down the line

                int val = 100 * this->_snrPD2;
                if (val < 0) {
                    int a = -100 * val;
                    this->_SNR2Tab[3] = (uint8_t)a;
                    this->_SNR2Tab[2] = (uint8_t)(a >>= 8);
                    this->_SNR2Tab[1] = (uint8_t)(a >>= 8);
                    this->_SNR2Tab[0] = (uint8_t)(a >>= 8);
                }
                else {
                    this->_SNR2Tab[3] = (uint8_t)val;
                    this->_SNR2Tab[2] = (uint8_t)(val >>= 8);
                    this->_SNR2Tab[1] = (uint8_t)(val >>= 8);
                    this->_SNR2Tab[0] = (uint8_t)(val >>= 8);
                }
                cpt2 = 0;
            }
        }
    }
    free(this->_pulseOx1.tab_ledSeq1A_PD1);
    free(this->_pulseOx1.tab_ledSeq1A_PD2);
    return (0);
}

int PpgHandler::getError(void)
{
    return this->_error;
}

void PpgHandler::setIntensity(int intensity)
{
    this->_pulseOx1.setIntensityLed(intensity, this->_ledMode);
}

void PpgHandler::setSampleAvg(int value)
{
    this->_sampleAvg = value;
    this->_pulseOx1.setSample(value, this->_sampleRate);
}

void PpgHandler::setSampleRate(int value)
{
    this->_sampleRate = value;
    this->_pulseOx1.setSample(this->_sampleAvg, value);
}

void PpgHandler::setSample(int avg, int rate)
{
    this->_sampleAvg = avg;
    this->_sampleRate = rate;
    this->_pulseOx1.setSample(avg, rate);
}

void PpgHandler::calibrate(void)
{
    this->setIntensity(255);
    this->_isCalibrating = 1;
}