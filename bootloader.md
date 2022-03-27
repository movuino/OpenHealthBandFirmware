---
layout: page
title: Bootloader
permalink: /bootloader/
---

## Burn the bootloader with a Segger J-link

- Install the [JLink Software and Documentation Pack](https://www.segger.com/downloads/jlink) from Segger.

Make sure the OHB is powered on.

In the Arduino IDE
- Select `Tools > Board > Adafruit Bluefruit Feather52`
- Select `Tools > Programmer > J-Link for Feather52`
- Select `Tools > Burn Bootloader` with the board and J-Link connected

### Burn the bootloader from the command line with nrfjprog

- Install the [nrf command line tools](https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download)
- In a terminal, run
```bash
nrfjprog --program <PATH_TO_ARDUINO>/packages/movuino/hardware/movuino/1.0.0/bootloader/feather_nrf52840_express/feather_nrf52840_express_bootloader-0.3.2_s140_6.1.1.hex -f nrf52 --chiperase --reset
```