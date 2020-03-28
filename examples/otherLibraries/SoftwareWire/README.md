SoftwareWire hd44780 examples
==================================

These examples require that the SoftwareWire library be installed.

The SoftwareWire library can be installed using the IDE library manager.
SoftwareWire provides a Wire compatable library that allows the user to
configure pins for the SDA and SCL signals.
While it offers additional flexibility, it will not be as fast as using the
dedicated i2c h/w inside the microcontroller.

These examples use the hd44780 library hd44780_I2Cexp i/o class using
SoftwareWire instead of the Wire library.

Note: using a software Wire library is not necessary on the esp8266 platform as the wire library on that platform already provides a mechanims to change the pins used for i2c.

WARNING:
The SoftWareWire developers made a major change to the library June 2018
which is in the subsequent 1.5.1 release that causes it be unusable by
the hd44780 library.
Until this change is backed out, you will not be able to use 
SoftWareWire beyond version 1.5.0 with the hd44780 library.
