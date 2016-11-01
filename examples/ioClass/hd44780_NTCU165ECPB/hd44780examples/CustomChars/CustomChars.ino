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

#include <SPI.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU165ECPB.h>

#define LCD_ROWS 1
#define LCD_COLS 16

// declare the lcd object
hd44780_NTCU165ECPB lcd;

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library sketch source code
#include <examples/hd44780examples/CustomChars/CustomChars.ino>
