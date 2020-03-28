// ----------------------------------------------------------------------------
// LCDiSpeed - LCD Interface Speed test for hd44780 hd44780_I2Cexp i/o class
//             using SoftwareWire instead of Wire
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example LCDiSpeed.
// Note:
// This is not a normal sketch and should not be used as model or example
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o class.
// The source code for this sketch lives in hd44780 examples:
// hd44780/examples/hd44780examples/LCDiSpeed/LCDiSpeed.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/LCDiSpeed
//

#include <SoftwareWire.h>
// Check for "new" SoftwareWire that breaks things
#if defined(TwoWire_h)
#error incompatible version of SoftwareWire library (use version 1.5.0)
#endif

const int sda=SDA, scl=SCL; // use same pins as h/w i2c
SoftwareWire Wire(sda,scl);

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // include i/o class header

// declare the lcd object
hd44780_I2Cexp lcd; // auto locate and autoconfig interface pins

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDiSpeed sketch source code
#include <examples/hd44780examples/LCDiSpeed/LCDiSpeed.ino>
