LiquidCrystal_I2C hd44780 examples
==================================

These examples require that the LiquidCrystal_I2C library be installed.

The LiquidCrystal_I2C library can be installed using the IDE library manager.
It controls the LCD using an PCF8574 i/o expander backpack.
The library assumes a paticular wiring between the PCF8574 and the LCD pins and a particular backlight circuit.
If the wiring or backlight circuit does not match what the library expects the examples will not work.
Also, the examples must be modified to set the i2c address to match the i2c address of the PCF8574 chip. If the address in the sketch does not match the actual address of the chip, the example will not work.

The advantage of using hd44780 and the hd44780_I2Cexp i/o class is that the library will automatically determine the i2c address, pin wiring, and backlight control configuration.
