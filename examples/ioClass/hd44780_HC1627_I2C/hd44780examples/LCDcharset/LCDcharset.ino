// ----------------------------------------------------------------------------
// LCDcharset - Display LCD character set
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example LCDcharset.
// Note:
// This is not a normal sketch and should not be used as model or example
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o class.
// The source code for this sketch lives in hd44780 examples:
// hd44780/examples/hd44780examples/LCDcharset/LCDcharset.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/LCDcharset
//

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_HC1627_I2C.h> // include i/o class header

// define the LCD geometry
#define LCD_COLS 16
#define LCD_ROWS 2

// declare the lcd object for auto i2c address location
hd44780_HC1627_I2C lcd;

// declare i2c address and constructor for specified i2c base address
//const int i2c_addr = 0x3b;
//hd44780_HC1627_I2C lcd(i2c_addr); // use device at this address

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDcharset sketch source code
#include <examples/hd44780examples/LCDcharset/LCDcharset.ino>
