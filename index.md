[![docs](https://github.com/movuino/OpenHealthBandFirmware/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/movuino/OpenHealthBandFirmware/actions/workflows/pages/pages-build-deployment)

# Open Health Band Firmware

Firmware for Open Health Band

## Description

This repository regroups all the firmware and tests which run on Open Health Band.  
For hardware plans and schematics see [movuino/OpenHealthBand](https://github.com/movuino/OpenHealthBand). For the web BLE GUI see [movuino/OpenHealthBandVisualization](https://github.com/movuino/OpenHealthBandVisualization)

## Getting Started

### Dependencies

All OHB firmware make use of [Arduino Integrated Development Environment](https://www.arduino.cc/en/software).

#### Board definition

- Open the Arduino IDE
- Navigate to `Preferences`
- Add `https://movuino.github.io/movuino-board-index/package_movuino_index.json` as an 'Additional Board Manager URL'.

#### Libraries
The following librairies are required to build the firmware and tests:
- [MPU9250_asukiaaa](https://github.com/asukiaaa/MPU9250_asukiaaa)
- [Adafruit Bluefruit nRF52](https://github.com/adafruit/Adafruit_nRF52_Arduino/tree/master/libraries/Bluefruit52Lib)
- [Max86141](https://github.com/MakerLabCRI/Max86141)

Those libraries are all available in the Arduino Library manager.

## Documentation
Documentation for the project is available at [movuino.github.io/OpenHealthBandFirmware](https://movuino.github.io/OpenHealthBandFirmware/) or in the `/docs` folder.
- [How to burn the bootloader](https://movuino.github.io/OpenHealthBandFirmware/bootloader)
- [OHB BLE protocol](https://movuino.github.io/OpenHealthBandFirmware/OHB-ble-protocol)

## Integration
OHB firmware can integrate with the [Open Health Band Visualization](https://github.com/movuino/OpenHealthBandVisualization) project which provide real time monitoring of the OHB sensors in a web browser.

