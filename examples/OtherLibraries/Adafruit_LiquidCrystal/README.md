Adafruit_LiquidCrystal hd44780 examples
========================================

The Adafruit_LiquidCrystal library must be installed in order to use these sketches.

#### The examples are broken up into various sub directories.

* `4bit`<br>
Contains example sketches that are for LCDs that are wired up directly to Arduino pins.

* `I2C`<br>
Contains example sketches that are for LCDs connected to an Adafruit #292 backpack when using the i2c MCP23008 i/o expander chip to control the LCD.

* `SPI`<br>
Contains example sketches that are for LCDs connected to an Adafruit #292 backpack when using the shift register to control the LCD.

Note: that while adafruit uses the term "SPI" the #292 backpack it is not using SPI.
It is mearly using a shift register.
Also, the Adafruit_liquidCrystal library is not using the SPI h/w but rather simply calling the s/w shiftout() function to do the transfer.
