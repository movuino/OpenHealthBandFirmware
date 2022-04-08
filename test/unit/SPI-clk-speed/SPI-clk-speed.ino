#include <MAX86141.h>
#include <SPI.h>

#define SS_PIN              10          // Slave Select pin.
#define READ_BYTE           0xFF        // Byte sent to the sensor indicating a reading operation.
#define REGISTER_ADDR       0xFF        // Address of the register to read.
#define SAMPLES             100         // Number of samples to take (read operations) for each clock step.
#define INITIAL_CLK         10000       // Clock value from which the value is incremented.
#define MAX_CLK             4000000     // Max clock value.
#define STEP                10000       // Value by which the clock is incremented.
#define SPIMODE             SPI_MODE0   // SPI MODE

#define IS_SUCCESS(val) val == 37       // Operation determining if the SPI read operation is a success or failure (must return a boolean).


void setup()
{
    int ret;
    int successes;

    Serial.begin(115200);
    while (!Serial)
        delay(10);
    delay(100);
    pinMode(SS_PIN, OUTPUT);
    SPI.begin();
    Serial.println("START");

    for (int clk = INITIAL_CLK; clk <= MAX_CLK; clk += STEP)
    {
        successes = 0;
        for (size_t i = 0; i < SAMPLES; i++)
        {
            SPI.beginTransaction(SPISettings(clk, MSBFIRST, SPIMODE));
            digitalWrite(SS_PIN, LOW); 
            
            SPI.transfer(REGISTER_ADDR);
            SPI.transfer(READ_BYTE);
            ret = SPI.transfer(0x00);
            SPI.endTransaction();
            digitalWrite(SS_PIN, HIGH);

            if (IS_SUCCESS(ret))
                successes++;
//            delay(10);
        }
        Serial.printf("%d,%d\n", clk, (successes / SAMPLES) * 100);
        
    }
    Serial.println("END");
}

void loop()
{    
}
