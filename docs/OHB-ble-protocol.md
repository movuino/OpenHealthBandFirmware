![](https://img.shields.io/badge/OHB--BLE--protocol-1.1.0-blueviolet)  
OHB BLE Protocol Version 1.1.0

Note: If not otherwise specified, each datatype is presented in [Little Endian](https://en.wikipedia.org/wiki/Endianness) format.
# Error Service
UUID: 0x1200

| Characteristic  | UUID  | Buffer size | Permissions |
|---|---|---|---|
|  Error |  0x1201 |  4 |  READ \| NOTIFY |

## Error characteristic
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


------


# IMU
## Accelerometer
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
            <td colspan=4 align="center">X value (uint32)</td>
            <td colspan=4 align="center">Y value (uint32)</td>
            <td colspan=4 align="center">Z value (uint32)</td>
        </tr>
    </tbody>
</table>

## Gyroscope
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
            <td colspan=4 align="center">X value (uint32)</td>
            <td colspan=4 align="center">Y value (uint32)</td>
            <td colspan=4 align="center">Z value (uint32)</td>
        </tr>
    </tbody>
</table>

## Magnetometer
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
            <td colspan=4 align="center">X value (uint32)</td>
            <td colspan=4 align="center">Y value (uint32)</td>
            <td colspan=4 align="center">Z value (uint32)</td>
        </tr>
    </tbody>
</table>

## Change Log
...
