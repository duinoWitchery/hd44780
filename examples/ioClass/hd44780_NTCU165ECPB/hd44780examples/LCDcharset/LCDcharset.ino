// ----------------------------------------------------------------------------
// LCDcharset - Display LCD character set for hd44780 hd44780_NTCU165ECPB i/o class
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

#include <SPI.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU165ECPB.h>

// define the LCD geometry
#define LCD_COLS 16
#define LCD_ROWS 1

// constructor parameters:
// lcd([cs], [clock, data])
// If no parameters, then library will use SS, SCK, and MOSI pins
// If cs parameter specified, then use it for chip select then SCK and MOSI
// If <SPI.h> is included and clock & data pins match h/w SPI pins SCK and MOSI,
//	 h/w spi will be used
// If h/w spi is not possible, then the code will fall back to bit banging.
//
// NOTE:
//  - Leonardo h/w is "stupid" and does not bring out SS
//     (it only drives an LED)
//  - Leonardo does not bring SPI signals to female headers,
//     they are only on 6 pin ISP header.
//  - ESP8266 is does not use naked constants for digital pin numbers 
//
//
// To work around these pin issues in this sketch,
// Leonardo will use uno digital pins for SPI h/w which means it will
// will not use h/w spi. All the other boards will use the h/w SPI pins.
// Consult board pinout diagram to see where SS, SCK, and MOSI are available.
//

#if defined(ARDUINO_AVR_LEONARDO) || ( (USB_VID == 0x2341) && (USB_PID == 0x8036) )
const int cs=10, clk=13, data=11; // uno SPI pins (s/w bit banging will be used)
#else
const int cs=SS, clk=SCK, data=MOSI; // use h/w SPI pins on all other boards
#endif
hd44780_NTCU165ECPB lcd(cs, clk, data); // declare lcd object

// tell the hd44780 sketch the lcd object has been declared
#define HD44780_LCDOBJECT

// include the hd44780 library LCDcharset sketch source code
#include <examples/hd44780examples/LCDcharset/LCDcharset.ino>
