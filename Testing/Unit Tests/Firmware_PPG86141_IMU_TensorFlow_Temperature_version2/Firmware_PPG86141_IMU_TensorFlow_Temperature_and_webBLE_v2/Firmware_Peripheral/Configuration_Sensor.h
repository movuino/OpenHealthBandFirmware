/*
 * This file allows to choose the configuration of the sensor we are using and which LEDs we want to active
 * DA : Direct Ambient
 */


/* Possible LED Configurations*/
#define DA          0b00000001
#define LED_R       0b00000010
#define LED_IR      0b00000100
#define LED_G       0b00001000
#define LED_G_R     0b00010000
#define LED_G_IR    0b00100000
#define LED_R_IR    0b01000000
#define LED_G_R_IR  0b10000000
#define LED_2G      0b10000001

int config(byte led_Configuration) {
      int size = 0;

      if (led_Configuration == LED_G) { //Only Green
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_G | DA)) { // Green and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_IR) { // IR
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_IR | DA)) { // IR and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_R) { // RED
            ledMode[0] = {3/*LED3 (Sequence 1A, 0-3)*/};
            size = 1;
      }

      if (led_Configuration == (LED_R | DA)) { // RED and DA
            ledMode[0] = 3/*LED3 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (LED_G | LED_IR)) { // Green and IR
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (LED_G | LED_IR | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_G | LED_R)) { // Green and RED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (LED_G | LED_R | DA)) { // Green, IR and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/; 
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_IR | LED_R)) { // IR and RED
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (LED_IR | LED_R | DA)) { // IR, RED and DA
            ledMode[0] = 2/*LED2 (Sequence 1A, 0-3)*/;
            ledMode[1] = 3/*LED3 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_G | LED_IR | LED_R)) { // Green, IR and RED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            size = 3;
      }

      if (led_Configuration == (LED_G | LED_IR | LED_R | DA)) { // Green, IR, RED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 0-3)*/;
            ledMode[2] = 3/*LED3 (Sequence 2A, 0-3)*/;
            ledMode[3] = 9/*Direct Ambient (Sequence 2B, 4-9)*/;
            size = 4;
      }

      if (led_Configuration == LED_G_R) { // Green and RED pulsed simultaneously
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_G_R | DA)) { // Green and RED pulsed simultaneously and DA
            ledMode[0] = 5/*LED1 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_G_IR) { // Green and IR pulsed simultaneously
            ledMode[0] = 4/*LED1 and LED2 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_G_IR | DA)) { // Green and IR pulsed simultaneously and DA
            ledMode[0] = 4/*LED1 and LED3 (Sequence 1A, 0-3)*/; 
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_R_IR) { // RED and IR pulsed simultaneously
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_R_IR | DA)) { // RED and IR pulsed simultaneously and DA
            ledMode[0] = 6/*LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == LED_G_R_IR) { // Green, RED and IR pulsed simultaneously
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            size = 1;
      }

      if (led_Configuration == (LED_G_R_IR | DA)) { // Green, RED and IR pulsed simultaneously and DA
            ledMode[0] = 7/*LED1, LED2 and LED3 (Sequence 1A, 0-3)*/;
            ledMode[1] = 9/*Direct Ambient (Sequence 1B, 4-9)*/;
            size = 2;
      }

      if (led_Configuration == (LED_2G)) { // 2 Green LED
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            size = 2;
      }
      
      if (led_Configuration == (LED_2G | DA)) { // 2 Green LED and DA
            ledMode[0] = 1/*LED1 (Sequence 1A, 0-3)*/;
            ledMode[1] = 2/*LED2 (Sequence 1B, 4-9)*/;
            ledMode[2] = 9/*Direct Ambient (Sequence 2A, 0-3)*/;
            size = 3;
      }

      return size;

}
