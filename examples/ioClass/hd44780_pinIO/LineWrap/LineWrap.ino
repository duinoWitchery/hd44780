// vi:ts=4
// ----------------------------------------------------------------------------
// LineWrap - simple demonstration of automatic linewrap functionality
// Created by Bill Perry 2017-05-10
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for LCDs that are directly controlled with Arduino pins.
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
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED with the initalization error
// code.
//
// NOTE:
//	If the sketch fails to produce the expected results, or blinks the LED,
//	run the included I2CexpDiag sketch to test the i2c signals and the LCD.
//
// Special note for certain 16x1 displays:
// Some 16x1 displays are actually a 8x2 display that have both lines on
// a single line on the display.
// If you have one of these displays, simply set the geometry to 8x2 instead
// of 16x1. 
// In normal sketches, lineWrap() mode will allow this type of display to
// properly function as a 16x1 display in that it will allow printing up to
// 16 characters on the display without having to manually set the cursor
// position to print the right characters on the half of the display.
//
// ----------------------------------------------------------------------------
// While not all hd44780 use the same pinout, here is the one that most use:
// pin 1 is the pin closest to the edge of the PCB
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - Vo Contrast Voltage
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
// 15 - Backlight Anode (+5v)
// 16 - Backlight Cathode (Gnd)
// ----------------------------------------------------------------------------

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

//with backlight control:
//	backlight control requires two additional parameters
//	- an additional pin to control the backlight
//	- backlight active level which tells the library the level
//		needed to turn on the backlight.
//		note: If the backlight control pin supports PWM, dimming can be done
//	WARNING: some lcd keypads have a broken backlight circuit
//		If you have a lcd keypad, it is recommended that you first run the
//		LCDKeypadCheck sketch to verify that the backlight circuitry
//		is ok before enabling backlight control.
//
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLEvel);

// LCD geometry
// while 16x2 will work on most displays even if the geometry is different,
// for actual wrap testing of a particular LCD it is best to use the correct
// geometry.
const int LCD_COLS = 16;
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
		status = -status; // convert negative status value to positive number

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
