hd44780ioclass
==============

This directory contains the source code for each of the i/o classes that are
included in the hd44780 library package.
Each i/o class has its own .h file.

#### The library currenly comes with the following i/o subclasses:

* `hd44780_pinIO` control LCD using direct Arduino Pin connections

* `hd44780_I2Cexp` control LCD using i2c i/o exapander backpack (PCF8574 or MCP23008)

* `hd44780_I2Clcd` control LCD with native i2c interface (PCF2116, PCF2119x, etc...)

* `hd44780_NTCU165ECPB` control Noritake CU165ECBP-T2J LCD display over SPI

* `hd44780_NTCUUserial` control Noritake CU-U Series VFD display in serial mode

See each header file for further details.

