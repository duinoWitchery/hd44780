hd44780 examples
================
The hd44780 library package inclues many examples that demonstrate the capabilities of the library.

#### The examples are broken up into various sub directories.

* `hd44780examples`<br>
These are not really example sketches.<br>
**They should not be looked at as examples for how to use the library.**<br>
They are special purpose sketches that are used as include files for the i/o class examples.
These sketches allow using a "wrapper sketch" to define the lcd object
and lcd geometry which then includes the hd44780 example sketch as an
alternative to having to edit the actual sketch.
While these sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose as shipped in the hd44780 library package, is to act as an include file for the i/o class wrapper sketches and are not intended to be used directly.  
**See the ioClass specific subdirectories for the examples for each ioClass.**

* `ioClass`<br>
Contains subdirectories for each included hd44780 i/o class containing sketches
that are specific to the i/o class. They also contain wrapper sketches for the special purpose sketches mentioned above.

* `otherLibraries`<br>
Contains subdirectories for other (non hd44780) libraries that contain wrapper sketches for various special purpose hd44780examples sketches.
Its primary purpose is to provide easy benchmarking of other libraries for comparison purposes by using LCDiSpeed.
