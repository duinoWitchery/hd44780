// ----------------------------------------------------------------------------
// LCDiSpeed - LCD Interface Speed test: NewLiquidCrystal with LiquidCrystal_I2C
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

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

// ugliness to allow this sketch to locate the hd44780 example sketch
// note: hd44780.h is not needed, it is only included to get its directory on the include path
#include <hd44780.h>
#undef hd44780_h // undefine this so the example sketch does not think hd44780 is being used.

// cols and rows don't have to be exact to still get accurate transfer numbers
#define LCD_COLS 16
#define LCD_ROWS 2

// declare the lcd object
// Note: The i2c address and pin mappings must match the backpack
// The pin mapping selected matches the "LiquidCrystal_I2C" library
// in the IDE library manager.
const uint8_t i2cAddr = 0x27;
const int rs=0, rw=1, en=2, db4=4, db5=5, db6=6, db7=7, bl=3;
typeof(POSITIVE) blpol=POSITIVE; // backlight polarity level
LiquidCrystal_I2C lcd(i2cAddr, en, rw, rs, db4, db5, db6, db7, bl, blpol);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDiSpeed sketch source code
#include <examples/hd44780examples/LCDiSpeed/LCDiSpeed.ino>
