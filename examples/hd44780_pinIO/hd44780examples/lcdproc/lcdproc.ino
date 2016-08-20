// ----------------------------------------------------------------------------
// lcdproc - LCD library lcdproc wrapper sketch
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
// hd44780/examples/hd44780examples/lcdproc/lcdproc.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/lcdproc
//

#include <hd44780.h>		// can be omitted starting with IDE 1.6.7
#include <hd44780_pinIO.h>

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
//const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7;
//hd44780_pinIO lcd(rs, en, d4, d5, d6, d7);

// with backlight control:
const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7, bl=10, blLevel=HIGH;
hd44780_pinIO lcd(rs, en, d4, d5, d6, d7, bl, blLevel);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/lcdproc/lcdproc.ino>
