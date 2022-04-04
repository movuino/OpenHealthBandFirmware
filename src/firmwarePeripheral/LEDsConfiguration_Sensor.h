/*
 * This file allows to choose the configuration of the sensor we are using and which LEDs we want to active
 * DA : Direct Ambient
 */


/* Possible LED Configurations */

#define DA          0b00000001

/* RGB LED */
#define rgbLED_R       0b00000010
#define rgbLED_IR      0b00000100
#define rgbLED_G       0b00001000
#define rgbLED_G_R     0b00010000
#define rgbLED_G_IR    0b00100000
#define rgbLED_R_IR    0b01000000
#define rgbLED_G_R_IR  0b10000000

/* Only 1 color (Green)*/
#define LED_G      0b10000001

int config(byte led_Configuration) {
      int size = 0;

      if (led_Configuration == rgbLED_G) { //Only Green activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_G | DA)) { // Green and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == rgbLED_IR) { //Only IR activted in RGB LED
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_IR | DA)) { //IR and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == rgbLED_R) { //Only RED activted in RGB LED
            ledMode[0] = {3/*LED3 (Sequence 1A, 0-3)*/};
            size = 1;
      }

      if (led_Configuration == (rgbLED_R | DA)) { // RED and DA
            ledMode[0] = 3/*LED3 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_IR)) { // Green and IR activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_IR | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_R)) { // Green and RED activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_R | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/; 
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (rgbLED_IR | rgbLED_R)) { // IR and RED activted in RGB LED
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (rgbLED_IR | rgbLED_R | DA)) { // IR, RED and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_IR | rgbLED_R)) { // Green, IR and RED activted in RGB LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (rgbLED_G | rgbLED_IR | rgbLED_R | DA)) { // Green, IR, RED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 0-3)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            ledMode[3] = 9/*Direct Ambient (Sequence 2B, 4-9)*/;
            size = 4;
      }

      if (led_Configuration == rgbLED_G_R) { // Green and RED pulsed simultaneously
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_G_R | DA)) { // Green and RED pulsed simultaneously and DA
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == rgbLED_G_IR) { // Green and IR pulsed simultaneously
            ledMode[0] = 4/*LED1 and LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_G_IR | DA)) { // Green and IR pulsed simultaneously and DA
            ledMode[0] = 4/*LED1 and LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == rgbLED_R_IR) { // RED and IR pulsed simultaneously
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_R_IR | DA)) { // RED and IR pulsed simultaneously and DA
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == rgbLED_G_R_IR) { // Green, RED and IR pulsed simultaneously
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (rgbLED_G_R_IR | DA)) { // Green, RED and IR pulsed simultaneously and DA
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_G) { //Only 1 Green LED 
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_G | DA)) { // 1 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }
      
      if (led_Configuration == (LED_G | LED_G)) { // 2 Green LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }
      
      if (led_Configuration == (LED_G | LED_G | DA)) { // 2 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_G | LED_G | LED_G)) { // 3 Green LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_G | LED_G | LED_G | DA)) { // 3 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            ledMode[3] = 9/*Direct Ambient (Sequence 2B, 4-9)*/;
            size = 4;
      }

      return size;

}
