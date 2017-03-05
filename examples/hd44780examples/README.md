hd44780examples
===============

The examples included in this directory should work with any LiquidCrystal API 
compatible lcd library.
All the examples have the ability to be included by another "wrapper sketch"
which can be used to define the LCD object and LCD geometry.

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

- `lcdproc`<br>
This is a sketch that will allow using an LCD display with the linux/unix
lcdproc system. See the sketch for more details.
