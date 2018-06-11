hd44780 examples
================
The hd44780 library package inclues many examples that demonstrate the capabilities of the library.

#### The examples are broken up into various sub directories.

* `Documenatation`<br>
The Documenation sketch is not really an example sketch.
It is a dummy sketch used to allow documentation to be accessed through the IDE when the sketched is loaded into the IDE.

* `ioClass`<br>
Contains subdirectories for each included hd44780 i/o class containing example sketches
that are specific to the i/o class. They also contain wrapper sketches for the special purpose sketches mentioned above.

* `otherLibraries`<br>
Contains subdirectories for other (non hd44780) libraries that contain wrapper sketches for various special purpose hd44780examples sketches.
Its primary purpose is to provide easy benchmarking of other libraries for comparison purposes by using LCDiSpeed.

**The sketches in the hd44780examples area are special purpose sketches that are used as include files for the i/o class examples.
They should not be looked at as examples for how to use the library.**
While the hd44780examples sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose, as shipped in the hd44780 library package, is to act as an include file for the i/o class wrapper sketches and are not intended to be used directly.  
**See the ioClass specific subdirectories for the examples for each ioClass.**
