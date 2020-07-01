hd44780_I2Cexp examples
=======================

The examples included in this directory are for the hd44780_I2Cexp i/o class.<br>
The hd44780_I2Cexp i/o class controls an LCD using an i2c i/o exapander backpack (PCF8574 or MCP23008)


#### The following examples are included:

- `HelloWorld`<br>
Prints "Hello, World!" on the lcd

- `I2CexpDiag`<br>
Verifies configuation & operation of hd44780 LCDs based
on the Hitachi HD44780 and compatible chipsets using I2C extension
backpacks that use a simple I2C i/o expander chip. (PCF8574 or MCP23008)
Sketch supports testing of multiple displays at once.

- `LCDCustomChars`<br>
Demonstrates using custom characters

- `LineWrap`<br>
Demonstrates automatic linewrap functionality

- `MultiDisplay`<br>
Displays information on multiple displays at once.

- `ReadWrite`<br>
Demonstrates the ability to read data from the LCD.

- `Serial2LCD`<br>
Displays a message read from the serial port on the lcd.

- `SoftwareWire`<br>
Demonstrates how to use the SoftwareWire library to allow using different pins
for SDA and SCL signals.

- `UpTime`<br>
Prints the amount of time since the Arduino has been reset.

- `hd44780examples`<br>
The hd44780examples subdirectory contains
hd44780_I2Cexp class specific wrapper sketches for sketches under
examples/hd44780examples.
