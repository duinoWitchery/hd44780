// ----------------------------------------------------------------------------
// CustomChars - Demonstrate use of hd44780 custom characters
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example.
// Note:
// This is not a normal sketch and should not be used as model or exmaple
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o sub library.
// The source code for this sketch lives in the hd44780 examples.
// hd44780/examples/hd44780examples/CustomCharacters/CustomCharacters.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/CustomCharacters
//

#include <Wire.h>
#include <hd44780.h>
#include <hd44780_I2Clcd.h>

// lcd i2c address
const uint8_t I2Clcd_Addr = 0x3e;

// declare the lcd object
hd44780_I2Clcd lcd(I2Clcd_Addr);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/CustomChars/CustomChars.ino>
