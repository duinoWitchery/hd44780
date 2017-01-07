// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCDs that are directly controlled with Arduino pins.
//
// Sketch will print "Hello, World!" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// See below for configuring the Arduino pins used.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
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
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

void setup()
{
	// -----------------------------------------------------------------------
	// hd44780 API function: setExecTimes
	//
	// setExecTimes(chExecTimeus, insExecTimeUs)
	// chExecTimeUs  - time in microseconds of clear & home commands
	// insExecTimeUs - time in microseconds of all other instructions
	//
	// configures execution times in Us for clear/home commands and all other
	// command or data instructions.
	// if the lcd module needs more execution time for operations.
	// NOTE: These execution times are not global so when using multiple
	// lcd objects, each lcd object can have its own seperate execution times.
	//
	// The execution times can be set before calling begin() to ensure that
	// the initialization code in begin() uses the proper execution times.
	// -----------------------------------------------------------------------

	// lcd.setExecTimes(2000, 37);  // uncomment this to use custom times

	// initialize LCD with number of columns and rows: 
	if( lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible
		fatalError(1); // this never returns
	}
	
	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop()
{
static unsigned long lastsecs = -1; // pre-initialize with non zero value
unsigned long secs;

	secs = millis() / 1000;

	// see if 1 second has passed
	// so the display is only updated once per second
	if(secs != lastsecs)
	{
		lastsecs = secs; // keep track of last seconds

		// set the cursor position to column 0, row 1
		// note: row 1 is the second row from top,
		// since row counting begins with 0
		lcd.setCursor(0, 1);

		// print uptime on lcd device: (time since last reset)
		PrintUpTime(lcd, secs);
	}
}

// PrintUpTime(outdev, secs) - print uptime in HH:MM:SS format
// outdev - the device to send output
//   secs - the total number of seconds uptime
void PrintUpTime(Print &outdev, unsigned long secs)
{
unsigned int hr, mins, sec;

	// convert total seconds to hours, mins, seconds
	mins =  secs / 60;	// how many total minutes
	hr = mins / 60;		// how many total hours
	mins = mins % 60;	// how many minutes within the hour
	sec = secs % 60;	// how many seconds within the minute
		

	// print uptime in HH:MM:SS format
	// Print class does not support fixed width formatting
	// so insert a zero if number smaller than 10
	if(hr < 10)
		outdev.write('0');
	outdev.print((int)hr);
	outdev.write(':');
	if(mins < 10)
		outdev.write('0');
	outdev.print((int)mins);
	outdev.write(':');
	if(sec < 10)
		outdev.write('0');
	outdev.print((int)sec);
}

// fatalError() - loop & blink and error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}
