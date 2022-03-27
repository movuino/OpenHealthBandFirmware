#ifndef PPGLEDCONFIGURATION_HPP
# define PPGLEDCONFIGURATION_HPP

# include <stdio.h>
# include <cstring>

# define DA          0b00000001

/* Only 1 color (Green)*/
#define LED_G      0b10000001

/* RGB LED */
#define rgbLED_R       0b00000010
#define rgbLED_IR      0b00000100
#define rgbLED_G       0b00001000
#define rgbLED_G_R     0b00010000
#define rgbLED_G_IR    0b00100000
#define rgbLED_R_IR    0b01000000
#define rgbLED_G_R_IR  0b10000000

typedef	unsigned char byte ;

size_t makeLedConfig(byte ledConfig, int *ledMode);

#endif