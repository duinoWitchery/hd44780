// vi:ts=4
// ----------------------------------------------------------------------------
// LineWrap - simple demonstration of automatic linewrap functionality
// Created by Bill Perry 2019-11-23
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for Noritake CU-U series displays like the
// Noritake CU20025ECPB-U1J or CU20025ECPB-W1J
// The Noritake CU20025ECPB display is a 20x2 line VFD display.
// It is not a hd44780 display but is nearly fully compatible with a hd44780
// display. The only real difference is the backlight control.
// So while this device is supported by the hd44780 library,
// the subclass will do its best to emulatate hd44780 functions and map
// them to equivalent functionality when possible.
//
// Sketch demonstrates hd44780 library automatic line wrapping functionality.
//
// Background:
// hd44780 LCDs do not use linear continuous memory for the characters
// on the lines on the display.
// This means that simply sending continuous characters to the
// display will not fill lines and wrap appropriately as might be expected.
// The hd44780 library solves this issue by adding a line wrapping capability
// in s/w that can be enabled & disabled.
// This allows the host to send characters to the display continuously and they
// will wrap to the next lower line when the end of the visible line has been
// reached. When on the bottom line it will wrap back to the top line.
// 
// (Configure LCD_COLS & LCD_ROWS if desired/needed)
// Expected behavior of the sketch:
// - display a banner announcing the test.
// - print the configured LCD geometry 
// - print a long text string to demostrate automatic line wrapping 
// - print lots of characters (slowly) to show how the full wrapping works.
// (loop)
//
// See below for configuring the Arduino pins used.
//
// Noritake CU-U series LCD module pins
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - not connected
//  4 - RS Register Select (rs) --- connect to Arduino pin
//  5 - Read/Write ------ connect to gnd
//  6 - Enable (en) ----- connect to Arduino pin
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4) ---- connect to Arduino pin
// 12 - Data 5 (db5) ---- connect to Arduino pin
// 13 - Data 6 (db6) ---- connect to Arduino pin
// 14 - Data 7 (db7) ---- connect to Arduino pin
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED with the initalization error
// code.
//
// ----------------------------------------------------------------------------
// pinout:
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - not connected
//  4 - RS Register Select (rs)
//  5 - Read/Write
//  6 - Enable (en)
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4)
// 12 - Data 5 (db5)
// 13 - Data 6 (db6)
// 14 - Data 7 (db7)
// ----------------------------------------------------------------------------

#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU20025ECPB_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // for esp8266 devices
#else
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;       // for all other devices
#endif
hd44780_NTCU20025ECPB_pinIO lcd(rs, en, db4, db5, db6, db7);

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 2;

void setup()
{
int status;

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// turn on automatic line wrapping
	// which automatically wraps lines to the next lower line and wraps back
	// to the top when at the bottom line
	// NOTE: 
	// noLineWrap() can be used to disable automatic line wrapping.
	// _write() can be called instead of write() to send data bytes
	// to the display bypassing any special character or line wrap processing.
	lcd.lineWrap();

}

void loop()
{
	lcd.clear();
	lcd.print("WrapTest"); 
	delay(2000);
	lcd.clear();

	//print the configured LCD geometry
	lcd.print(LCD_COLS);
	lcd.print("x");
	lcd.print(LCD_ROWS);
	delay(3000);
	lcd.clear();

	// print a long text string
	// without line wrapping enabled, the text would not wrap properly
	// to the next line.

	if(LCD_COLS == 8)
		lcd.print("A long text line");
	else
		lcd.print("This is a very long line of text");
	delay(3000);

	lcd.clear();

	// now print 2 full displays worth of characters to show
	// the full wrapping.

	lcd.cursor(); // turn on cursor so you can see where it is

	char c = '0'; // start at the character for the number zero
	for(int i = 2*LCD_COLS*LCD_ROWS; i; i--)
	{
		lcd.print(c++);
		delay(200); // slow things down to watch the printing & wrapping

		if(c > 0x7e) // wrap back to beginning of printable ASCII chars
			c = '!'; 
	}
	delay(3000);
	lcd.noCursor(); // turn off cursor
}
