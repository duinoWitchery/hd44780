// ----------------------------------------------------------------------------
// LCDLibTest - LCD library test sketch
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example.
// Note:
// This is not a normal sketch and should not be used as model or exmaple
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o class.
// The source code for this sketch lives in the hd44780 examples.
// hd44780/examples/hd44780examples/LCDlibTest/LCDlibTest.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/LCDlibTest
//

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // include i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

// with backlight control:
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLevel);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/LCDlibTest/LCDlibTest.ino>
