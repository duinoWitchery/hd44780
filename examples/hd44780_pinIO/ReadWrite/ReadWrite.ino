// vi:ts=4
// ----------------------------------------------------------------------------
// ReadWrite - simple demonstration reading data from LCD
// Created by Bill Perry 2016-08-19
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// Sketch will print the amount of time since the Arduino has been reset
// on the top row and then read the data from the LCD to print it on the
// second row
//
// If there are errors the arduino supports a built in LED,
// an error status code will blink on the built in LED.
// Error codes:
// (1) lcd device initalization failed
// (2) lcd device does not support reads
// (3) error reading data from lcd device
//

#include <hd44780.h>	// can be left out in arduino.cc IDE 1.6.7 and later
#include <hd44780_pinIO.h>

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
const int rs=8, rw=A0, en=9, d4=4, d5=5, d6=6, d7=7;
hd44780_pinIO lcd(rs, rw, en, d4, d5, d6, d7);

//with backlight control:
//	backlight control requires two additional parameters
//	- an additional pin to contro the backlight
//	- backlight active level which tells the library the level
//		needed to turn on the backlight.
//		note: If the backlight control pin supports PWM, dimming can be done
//
//const int rs=8, rw=A0, en=9, d4=4, d5=5, d6=6, d7=7, bl=10, blLevel=HIGH;
//hd44780_pinIO lcd(rs, rw, en, d4, d5, d6, d7, bl, blLEvel);

// LCD geometry
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

void setup()
{
	// initialize LCD with number of columns and rows: 
	if( lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible
		fatalError(1);
	}

	// check to see if device can read by attempting to read
	// the lcd status register. If it fails then assume it was
	// because the lcd device does not support reads.
	if(lcd.status() < 0)
	{
		lcd.print("No Read Support");
		fatalError(2);
	}
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

		// set the cursor position to top line: column 0, row 0
		lcd.setCursor(0, 0);

		// print uptime on lcd device: (time since last reset)
		PrintUpTime(lcd, secs);

		// Now copy the characters from the top line to the 2nd line
		// This is done character by character by:
		// - setting the character position to read
		// - reading a character
		// - setting the character position to write
		// - writing the charcter read
		for(int col = 0; col < LCD_COLS; col++)
		{
		int c;

			lcd.setCursor(col, 0);
			if((c = lcd.read()) < 0) // if a read error, bomb out
				fatalError(3);
			lcd.setCursor(col, 1);
			lcd.write((uint8_t) c);
		}
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
	Serial.print(F("FATAL ERROR: "));
	Serial.println(ecode);
#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	while(1)
	{

		// blink out error code
		for(int i = 0; i< ecode; i++)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			delay(100);
			digitalWrite(LED_BUILTIN, LOW);
			delay(250);
		}
		delay(1500);
	}
#else
	while(1){} // spin and do nothing
#endif
}

