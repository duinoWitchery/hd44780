hd44780_I2Cexp examples
=======================

The examples included in this directory are for the hd44780_I2Cexp i/o class.<br>
The hd44780_I2Cexp i/o class controls an LCD using an i2c i/o exapander backpack (PCF8574 or MCP23008)


#### The following examples are included:

- `HelloWorld`<br>
Sketch will print "Hello, World!" on top row of lcd
and will print the amount of time since the Arduino has been reset
on the second row.

- `I2CexpDiag`<br>
Sketch to verify configuation & operation of hd44780 LCDs based
on the Hitachi HD44780 and compatible chipsets using I2C extension
backpacks that use a simple I2C i/o expander chip.
Sketch supports testing of multiple displays at once.

- `LineWrap`<br>
Sketch to demonstrate linewrap functionality

- `MultiDisplay`<br>
Sketch will display information on multiple displays at once.

- `ReadWrite`<br>
The purpose of the sketch is to demonstrate the ability to read data from
the LCD.

- `hd44780examples`<br>
The hd44780examples subdirectory contains
hd44780_I2Cexp class specific wrapper sketches for sketches under
examples/hd44780examples.
