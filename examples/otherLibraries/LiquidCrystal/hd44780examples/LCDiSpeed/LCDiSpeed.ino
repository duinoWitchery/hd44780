// ----------------------------------------------------------------------------
// LCDiSpeed - LCD Interface Speed test for NewLiquidCrystal library
// ----------------------------------------------------------------------------
// This sketch is a wrapper sketch for the hd44780 library example.
// Note:
// This is not a normal sketch and should not be used as model or example
// of hd44780 library sketches.
// This sketch is simple wrapper that declares the needed lcd object for the
// hd44780 library sketch.
// It is provided as a convenient way to run a pre-configured sketch for
// the i/o class.
// The source code for this sketch lives in the hd44780 examples.
// hd44780/examples/hd44780examples/LCDiSpeed/LCDiSpeed.ino
// From IDE:
// [File]->Examples-> hd44780/hd44780examples/LCDiSpeed
//

#include <LiquidCrystal.h>

// ugliness to allow this sketch to locate the hd44780 example sketch
// note: hd44780.h is not needed, it is only included to get its directory on the include path
#include <hd44780.h>
#undef hd44780_h // undefine this so the example sketch does not think hd44780 is being used.

// cols and rows don't have to be exact to still get accurate transfer numbers
#define LCD_COLS 16
#define LCD_ROWS 2

// declare Arduino pins used for LCD functions and the lcd object

//const int rs=12, en=11, db4=5, db5=4, db6=3, db7=2; // IDE LiquidCrystal example pins
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7; // lcd keypad shield pins
LiquidCrystal lcd(rs, en, db4, db5, db6, db7);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/LCDiSpeed/LCDiSpeed.ino>
