---
layout: page
title: OHB BLE Protocol
permalink: /OHB-ble-protocol/
---
# OHB BLE Protocol Version 1.1.0

In order to facilitate compatibility beween software, the BLE protocols used to write tests and firmwares should be indicated in their respective READMEs using a badge as follow:  
![](https://img.shields.io/badge/OHB--BLE--protocol-1.1.0-blueviolet) 

<strong>Note</strong>: If not otherwise specified, each datatype is presented in [Little Endian](https://en.wikipedia.org/wiki/Endianness) format.

---
# Error Service
UUID: 0x1200

| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  Error |  0x1201 |  4 |  READ &#124; NOTIFY |

## Error characteristic
Characteristic containing the current state of each sensor. The values are updated only once during initialization and should not change during runtime.
Zero signify that the sensor initialization was successfull, while a non-zero value is the sign of a problem during startup.
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
             <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=1 align="center">IMU</td>
            <td colspan=1 align="center">PPG</td>
            <td colspan=1 align="center">Temp</td>
            <td colspan=1 align="center">Tens</td>
        </tr>
    </tbody>
</table>

---

# IMU Service
UUID: 0x1200
<table>
    <thead>
        <tr>
            <th>Characteristic</th>
            <th>UUID</th>
            <th>Buffer size</th>
            <th>Permissions</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Acceleromter</td>
            <td>0x1102</td>
            <td>11</td>
            <td>READ &#124; NOTIFY</td>
        </tr>
        <tr>
            <td>Gyroscope</td>
            <td>0x1103</td>
            <td>11</td>
            <td> READ &#124; NOTIFY </td>
        </tr>
        <tr>
            <td>Magnetometer</td>
            <td>0x1104</td>
            <td>10</td>
            <td> READ &#124; NOTIFY</td>
        </tr>
    </tbody>
</table>
## Accelerometer Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">timestamp (uint32) </td>
            <td colspan=1 align="center"></td>
            <td colspan=2 align="center">X value (float)</td>
            <td colspan=2 align="center">Y value (float)</td>
            <td colspan=2 align="center">Z value (float)</td>
        </tr>
    </tbody>
</table>

## Gyroscope Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">timestamp (uint32) </td>
            <td colspan=1 align="center"></td>
            <td colspan=2 align="center">X value (float)</td>
            <td colspan=2 align="center">Y value (float)</td>
            <td colspan=2 align="center">Z value (float)</td>
        </tr>
    </tbody>
</table>

## Magnetometer Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">timestamp (uint32) </td>
            <td colspan=2 align="center">X value (float)</td>
            <td colspan=2 align="center">Y value (float)</td>
            <td colspan=2 align="center">Z value (float)</td>
        </tr>
    </tbody>
</table>

---

# PPG Service
UUID: 0x1300
<table>
    <thead>
        <tr>
            <th>Characteristic</th>
            <th>UUID</th>
            <th>Buffer size</th>
            <th>Permissions</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>PPG1</td>
            <td>0x1305</td>
            <td>12</td>
            <td>READ &#124; NOTIFY </td>
        </tr>
        <tr>
            <td>PPG2</td>
            <td>0x1307</td>
            <td>12</td>
            <td> READ &#124; NOTIFY </td>
        </tr>
        <tr>
            <td>SNR1</td>
            <td>0x1313</td>
            <td>4</td>
            <td>READ &#124; NOTIFY</td>
        </tr>
        <tr>
            <td>SNR2</td>
            <td>0x1314</td>
            <td>4</td>
            <td>READ &#124; NOTIFY</td>
        </tr>
    </tbody>
</table>

## PPG1 Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">timestamp (uint32) </td>
            <td colspan=4 align="center">val1 (uint32)</td>
            <td colspan=4 align="center">val2 (uint32)</td>
        </tr>
    </tbody>
</table>

## PPG2 Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">timestamp (uint32) </td>
            <td colspan=4 align="center">val1 (uint32)</td>
            <td colspan=4 align="center">val2 (uint32)</td>
        </tr>
    </tbody>
</table>

## SNR1 Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">val (uint32)</td>
        </tr>
    </tbody>
</table>

## SNR2 Characteristic
<table>
    <thead>
        <tr>
            <th>0</th>
            <th>1</th>
            <th>2</th>
            <th>3</th>
            <th>4</th>
            <th>5</th>
            <th>6</th>
            <th>7</th>
            <th>8</th>
            <th>9</th>
            <th>10</th>
            <th>11</th>
            <th>12</th>
            <th>13</th>
            <th>14</th>
            <th>15</th>
            <th>16</th>
            <th>17</th>
            <th>18</th>
            <th>19</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td colspan=4 align="center">val (uint32)</td>
        </tr>
    </tbody>
</table>

---

# Utils Service
UUID: 0x1400

<table>
    <thead>
        <tr>
            <th>Characteristic</th>
            <th>UUID</th>
            <th>Buffer size</th>
            <th>Permissions</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>Start/Stop</td>
            <td>0x1401</td>
            <td>1</td>
            <td>READ &#124; WRITE</td>
        </tr>
            <td>Led intensity</td>
            <td>0x1402</td>
            <td>1</td>
            <td>READ &#124; WRITE &#124; NOTIFY</td>
        </tr>
        <tr>
            <td>Sample rate</td>
            <td>0x1403</td>
            <td>1</td>
            <td>READ &#124; WRITE</td>
        </tr>
        <tr>
            <td>Calibration trigger</td>
            <td>0x1404</td>
            <td>1</td>
            <td>READ &#124; WRITE</td>
        </tr>
    </tbody>
</table>

## Start/Stop Characteristic
Writing to the first byte of this chracteristic toggles the acquisition state of the OHB. Zero will stop the acquisition while a non-zero value will start it.

## Led Intensity Characteristic
Writing to the first byte of this chracteristic sets the led instensity of the PPG leds.
<table>
    <thead>
        <tr>
            <th>min value</th>
            <th>max value</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td>1</td>
            <td>255</td>
        </tr>
    </tbody>
</table>

This charcteristic can be monitored to track the change in intensity induced by the auto callibration process.

## Sample Average Characteristic

TODO

## Sample Rate Characteristic

TODO

## Calibration CHaracteristic

Writing to the first byte of this characteristic triggers the auto callibration of the PPG leds. The intensity change can be monitored by subscribing to notifications of the led intensity characteristic.

# Change Log

## [1.1.0] - 2022-27-03

### Added

- Created Protocol

