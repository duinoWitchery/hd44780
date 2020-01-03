LiquidCrystal_PCF8574 hd44780 examples
=======================================

These examples require that the LiquidCrystal_PCF8574 library be installed.

LiquidCrystal_PCF8574 library can be installed using the IDE library manager.
It controls the LCD using an PCF8574 i/o expander backpack.
The library assumes a paticular wiring between the PCF8574 and the LCD pins and a particular backlight circuit.
```
P0 - RS
P1 - R/W
P2 - E
P3 - Backlight control (Active High)
P4 - DB4
P5 - DB5
P6 - DB6
P7 - DB7
```
Not all backpacks are wired up this way.
If the actual wiring or backlight circuit does not match what the library expects, the examples will not work.
Also, the examples must be modified to set the i2c address to match the i2c address of the PCF8574 chip.<br>
**If the address in the sketch does not match the actual address of the chip, the example will not work**.

The advantage of using hd44780 library and the hd44780_I2Cexp i/o class is that unlinke the LiquidCrystal_PCF8574 library the hd44780 library will automatically determine the i2c address, pin wiring, and backlight control configuration.
