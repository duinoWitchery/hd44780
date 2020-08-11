// ----------------------------------------------------------------------------
// LCDLibTest - LCD library test sketch for hd44780 hd44780_pinIO i/o class
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
#if defined (ARDUINO_ARCH_ESP8266)
//const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // esp8266 Lolin/Wemos D1 R1 (uno form factor)
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: espduino32 GPIO12 needs a pulldown resistor
//const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14;
#else
//const int rs=12, en=11, db4=5, db5=4, db6=3, db7=2; // IDE LiquidCrystal pins
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7; // lcd keypad shield pins

#endif
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

#if defined (ARDUINO_ARCH_ESP8266)
// esp8266 Lolin/Wemos D1 R1 (uno form factor)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7, bl=D10, blLevel=HIGH;
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14, bl=5, blLevel=HIGH;
#else
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
#endif
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLevel);

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/LCDlibTest/LCDlibTest.ino>
