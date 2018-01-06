hd44780 examples
================
The hd44780 library package inclues many examples that demonstrate the capabilities of the library.

#### The examples are broken up into various sub directories.

* `hd44780examples`<br>
Contains special purpose example sketches that are used like templates for the i/o class examples.
The examples allow using a "wrapper sketch" to define the lcd object
and lcd geometry which then includes the hd44780 example sketch as an
alternative to having to edit the actual sketch.
While these sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose as shipped in the hd44780 library package, is to act as a template sketch for the i/o class wrapper sketches and are not intended to be used directly.
See the ioClass specific subdirectories for the examples for each ioClass.

* `ioClass`<br>
Contains subdirectories for each included hd44780 i/o class containing sketches
that are specific to the i/o class. They also contain wrapper sketches for the sketches mentioned above.

* `otherLibraries`<br>
Contains subdirectories for other (non hd44780) libraries that contain wrapper sketches for various hd44780examples sketches.
This is intended to allow easy benchmarking of other libraries for comparison purposes.
