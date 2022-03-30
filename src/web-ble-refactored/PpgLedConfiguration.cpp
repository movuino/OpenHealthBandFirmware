#include "PpgLedConfiguration.hpp"

size_t makeLedConfig(byte ledConfig, int *ledMode)
{
      size_t size = 0;

      memset(ledMode, 0, 10);
      if (ledConfig == rgbLED_G) { //Only Green activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_G | DA)) { // Green and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == rgbLED_IR) { //Only IR activted in RGB LED
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_IR | DA)) { //IR and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == rgbLED_R) { //Only RED activted in RGB LED
            ledMode[0] = {3/*LED3 (Sequence 1A, 0-3)*/};
            size = 1;
      }

      if (ledConfig == (rgbLED_R | DA)) { // RED and DA
            ledMode[0] = 3/*LED3 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == (rgbLED_G | rgbLED_IR)) { // Green and IR activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == (rgbLED_G | rgbLED_IR | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (rgbLED_G | rgbLED_R)) { // Green and RED activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == (rgbLED_G | rgbLED_R | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/; 
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (rgbLED_IR | rgbLED_R)) { // IR and RED activted in RGB LED
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == (rgbLED_IR | rgbLED_R | DA)) { // IR, RED and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (rgbLED_G | rgbLED_IR | rgbLED_R)) { // Green, IR and RED activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (rgbLED_G | rgbLED_IR | rgbLED_R | DA)) { // Green, IR, RED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 0-3)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            ledMode[3] = 9/*Direct Ambient (Sequence 2B, 4-9)*/;
            size = 4;
      }

      if (ledConfig == rgbLED_G_R) { // Green and RED pulsed simultaneously
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_G_R | DA)) { // Green and RED pulsed simultaneously and DA
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == rgbLED_G_IR) { // Green and IR pulsed simultaneously
            ledMode[0] = 4/*LED1 and LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_G_IR | DA)) { // Green and IR pulsed simultaneously and DA
            ledMode[0] = 4/*LED1 and LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == rgbLED_R_IR) { // RED and IR pulsed simultaneously
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_R_IR | DA)) { // RED and IR pulsed simultaneously and DA
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == rgbLED_G_R_IR) { // Green, RED and IR pulsed simultaneously
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (rgbLED_G_R_IR | DA)) { // Green, RED and IR pulsed simultaneously and DA
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (ledConfig == LED_G) { //Only 1 Green LED 
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (ledConfig == (LED_G | DA)) { // 1 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }
      
      if (ledConfig == (LED_G | LED_G)) { // 2 Green LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }
      
      if (ledConfig == (LED_G | LED_G | DA)) { // 2 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (LED_G | LED_G | LED_G)) { // 3 Green LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (ledConfig == (LED_G | LED_G | LED_G | DA)) { // 3 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            ledMode[3] = 9/*Direct Ambient (Sequence 2B, 4-9)*/;
            size = 4;
      }

      return size;

}