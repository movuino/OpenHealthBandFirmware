# OHB BLE Protocol Version 1.1.0

In order to facilitate compatibility beween software, the BLE protocols used to write tests and firmwares should be indicated in their respective READMEs using a badge as follow:  
![](https://img.shields.io/badge/OHB--BLE--protocol-1.1.0-blueviolet) 

<strong>Note</strong>: If not otherwise specified, each datatype is presented in [Little Endian](https://en.wikipedia.org/wiki/Endianness) format.

---
# Error Service
UUID: 0x1200

| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  Error |  0x1201 |  4 |  READ \| NOTIFY |

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
| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  Acceleromter |  0x1102 |  11 |  READ \| NOTIFY |
|  Gyroscope |  0x1103 |  11 |  READ \| NOTIFY |
|  Magnetometer |  0x1104 |  10 |  READ \| NOTIFY |
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
| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  PPG1 |  0x1305 |  12 |  READ \| NOTIFY |
|  PPG2 |  0x1307 |  12 |  READ \| NOTIFY |
|  SNR1 |  0x1313 |  4 |  READ \| NOTIFY |
|  SNR2 |  0x1314 |  4 |  READ \| NOTIFY |

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
| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  Start/Stop |  0x1401 |  1 |  READ \| WRITE |
|  Led intensity |  0x1402 |  1 |  READ \| WRITE \| NOTIFY |
|  Sample rate |  0x1403 |  1 |  READ \| WRITE |
|  Sample average |  0x1404 |  1 |  READ \| WRITE |
|  Calibration trigger |  0x1405 |  1 |  READ \| WRITE |

## Start/Stop Characteristic
Writing to the first byte of this chracteristic toggles the acquisition state of the OHB. Zero will stop the acquisition while a non-zero value will start it.

## Led Intensity Characteristic
Writing to the first byte of this chracteristic sets the led instensity of the PPG leds.
| min value | max value |
| --- | --- |
| 1 | 255 | 

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

