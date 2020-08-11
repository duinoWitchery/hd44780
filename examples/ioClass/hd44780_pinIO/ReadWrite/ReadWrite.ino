// vi:ts=4
// ----------------------------------------------------------------------------
// ReadWrite - simple demonstration reading data from LCD
// Created by Bill Perry 2016-08-19
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCDs that are directly controlled with Arduino pins.
//
// The purpose of the sketch is demonstrate the ability to read data from
// the LCD. As such, it requires an extra Arduino pin to control the R/W LCD
// pin. See below for configuring the Arduino pins used.
//
// Sketch will print the amount of time since the Arduino has been reset
// on the top row and then read the data from the LCD to print it on the
// second row
//
// If there are errors and the arduino supports a built in LED,
// an error status code will blink on the built in LED.
// Error codes:
// (1) lcd device initalization failed
// (2) lcd device does not support reads
// (3) error reading data from lcd device
// (4) error writing data to lcd device
// (5) read data mismatch
//

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, rw=A0, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // esp8266 Lolin/Wemos D1 R1 (uno form factor)
#elif defined(ARDUINO_ARCH_ESP32)
// note: GPIO12 needs a pulldown resistor
const int rs=12, rw=A0, en=13, db4=17, db5=16, db6=27, db7=14; // esp32 espduino32 D1 R32 (uno form factor)
#else
const int rs=8, rw=A0, en=9, db4=4, db5=5, db6=6, db7=7;       // for all other devices
#endif
hd44780_pinIO lcd(rs, rw, en, db4, db5, db6, db7);

//with backlight control:
//	backlight control requires two additional parameters
//	- an additional pin to control the backlight
//	- backlight active level which tells the library the level
//		needed to turn on the backlight.
//		note: If the backlight control pin supports PWM, dimming can be done
//			using setBacklight(dimvalue);
//
//	WARNING: some lcd keypads have a broken backlight circuit
//		If you have a lcd keypad, it is recommended that you first run the
//		LCDKeypadCheck sketch to verify that the backlight circuitry
//		is ok before enabling backlight control.
//		However, the hd44780_PinIO class will autodetect the issue and
//		work around it in s/w. If the backlight circuitry is broken,
//		dimming will not be possible even if the backlight pin supports PWM.
//
#if defined (ARDUINO_ARCH_ESP8266)
// esp8266 Lolin/Wemos D1 R1 (uno form factor)
//const int rs=D8, rw=A0, en=D9, db4=D4, db5=D5, db6=D6, db7=D7, bl=D10, blLevel=HIGH;
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
//       Dimming will not work on esp32 as it does not have analogWrite()
//const int rs=12, rw=A0, en=13, db4=17, db5=16, db6=27, db7=14, bl=5, blLevel=HIGH;
#else
//const int rs=8, rw=A0, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
#endif
//hd44780_pinIO lcd(rs, rw, en, db4, db5, db6, db7, bl, blLevel);

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
			{
				lcd.clear();
				lcd.print("read fail");
				fatalError(3);
			}

			// check for ':' characters in col 2 and 5
			// if not there, consider it a fatal read error
			if((col == 2 || col == 5) && c != ':')
			{
				lcd.clear();
				lcd.print("read fail");
				fatalError(5);
			}

			lcd.setCursor(col, 1);
			if(lcd.write((uint8_t) c) != 1)
			{
				lcd.clear();
				lcd.print("write fail");
				fatalError(4);
			}
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
	hd44780::fatalError(ecode); // does not return
}

