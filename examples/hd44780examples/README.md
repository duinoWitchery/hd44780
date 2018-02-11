hd44780examples
===============

The examples in this directory are not really example sketches.<br>
**They should not be looked at as examples for how to use the library.**<br>
They are special purpose sketches that are used as include files for the i/o class examples.
These sketches allow using a "wrapper sketch" to define the lcd object
and lcd geometry which then includes the hd44780 example sketch as an
alternative to having to edit the actual sketch.
While these sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose, as shipped in the hd44780 library package, is to act as an include file for the i/o class wrapper sketches and are not intended to be used directly.  
**See the ioClass specific subdirectories for the examples for each ioClass.**

#### The following special purpose sketches are provided:
- `LCDcharset`<br>
Shows the entire character set of the LCD.
Miniture digits are used to show the custom characters.

- `LCDisSpeed`<br>
Shows the speed of writing characters to the display.
It shows the transfer time of an individual character as well as updating
the full display. If the geometry is different than 16x2 it will also show what the full display timing would be if the display were 16x2.
Since the sketch can work on many different libraries and h/w, it is useful for ccomparing the speed of various libraries and LCD h/w.

- `LCDlibTest`<br>
Tests various API functions and LCD capabilities
Diagnostic information is also sent to the serial port.
Its primary purpose is for internal library testing & verficiation.

- `lcdproc`<br>
Allows using a hd44780 LCD display with the linux/unix
lcdproc system. See the sketch for more details.
