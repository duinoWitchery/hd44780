// ----------------------------------------------------------------------------
// LCDcharset - Display LCD character set for hd44780 hd44780_pinIO i/o class
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

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h>

// define the LCD geometry
#define LCD_COLS 16
#define LCD_ROWS 2

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // for esp8266 devices
#else
//const int rs=12, en=11, db4=5, db5=4, db6=3, db7=2; // IDE LiquidCrystal pins
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7; // lcd keypad shield pins
#endif
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

//with backlight control:
#if defined (ARDUINO_ARCH_ESP8266)
//const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7, bl=D10, blLevel=HIGH;
#else
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
#endif
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLEvel);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDcharset sketch source code
#include <examples/hd44780examples/LCDcharset/LCDcharset.ino>
