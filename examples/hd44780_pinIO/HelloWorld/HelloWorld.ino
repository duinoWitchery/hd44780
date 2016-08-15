// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// Sketch will print "Hello, World!" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//

#include <hd44780.h>	// can be left out in arduino.cc IDE 1.6.7 and later
#include <hd44780_pinIO.h>

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7;
hd44780_pinIO lcd(rs, en, d4, d5, d6, d7);

//with backlight control:
//	backlight control requires two additional parameters
//	- an additional pin to contro the backlight
//	- backlight active level which tells the library the level
//		needed to turn on the backlight.
//		note: If the backlight control pin supports PWM, dimming can be done
//
//const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7, bl=10, blLevel=HIGH;
//hd44780_pinIO lcd(rs, en, d4, d5, d6, d7, bl, blLEvel);

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
#ifdef LED_BUILTIN
		pinMode(LED_BUILTIN, OUTPUT);
		while(1)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			delay(500);
			digitalWrite(LED_BUILTIN, LOW);
			delay(500);
		}
#else
		while(1){} // spin and do nothing
#endif
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
