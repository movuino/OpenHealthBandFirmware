#ifndef PPGHANDLER_HPP
# define PPGHANDLER_HPP

# include "MAX86141.h"
# include "SPI.h"

# include "PpgLedConfiguration.hpp"

# define MOVUINO // comment out to use Adafruit nrf52

# ifdef MOVUINO
#  define SS_PIN 10
# else
#  define SS_PIN 19
# endif

# define SPICLK 1000000
# define CORRECTION_MIN_INTERVAL 50
# define PPG_VAL_TAB_SIZE 12
# define PPG_SNR_TAB_SIZE 4

class PpgHandler {
    private:
        MAX86141    _pulseOx1;
        int         _ledMode[10];
        int         _error;
        long        _startTime;
        long        _sampesTaken;
        uint8_t     _PD1Tab[PPG_VAL_TAB_SIZE];
        uint8_t     _PD2Tab[PPG_VAL_TAB_SIZE];
        uint8_t     _SNR1Tab[PPG_SNR_TAB_SIZE];
        uint8_t     _SNR2Tab[PPG_SNR_TAB_SIZE];
        float       _snrPD1;
        float       _snrPD2;
        bool        _isCalibrating;
        void        (*_correctionCallback)(int);
        long        _lastCorrection;
        int         _sampleAvg;
        int         _sampleRate;

        int         _calibrate(void);

    public:
        PpgHandler();
        ~PpgHandler();
        int configure(void);
        int update(void);
        int getError(void);
        void calibrate(void);

        void setCorrectionCallback(void (*f)(int));
        void setIntensity(int intensity);
        void setSampleAvg(int value);
        void setSampleRate(int value);
        void setSample(int avg, int rate);

        uint8_t *getPD1Tab(void);
        uint8_t *getPD2Tab(void);
        uint8_t *getSNR1Tab(void);
        uint8_t *getSNR2Tab(void);
};

#endif