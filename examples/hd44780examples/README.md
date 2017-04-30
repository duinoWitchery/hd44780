hd44780examples
===============

The examples included in this directory contain example sketches that are used like templates for the i/o class examples.
The examples allow using a "wrapper sketch" to define the lcd object
and lcd geometry which then includes the hd44780 example sketch as an
alternative to having to edit the actual sketch.
While these sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose, as shipped in the hd44780 library package, is to act as a template sketch for the i/o class wrapper sketches.

#### The following examples are included:

- `CustomChars`<br>
This will demonstrate using custom hd44780 characters on the display as well as show the full available character set.

- `LCDisSpeed`<br>
This will show the speed of writing characters to the display.
It will show the transfer time of an individual character as well as updating
the full display. If the geometry is different than 16x2 it will also show what the full display timing would be if the display were 16x2.
Since the sketch can work on many different libraries and h/w, it is useful for ccomparing the speed of various libraries and LCD h/w.

- `LCDlibTest`<br>
This will test various API functions and LCD capabilities
Diagnostic information is also sent to the serial port.

- `lcdproc`<br>
This is a sketch that will allow using an LCD display with the linux/unix
lcdproc system. See the sketch for more details.
