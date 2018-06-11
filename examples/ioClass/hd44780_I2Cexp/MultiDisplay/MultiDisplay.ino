// vi:ts=4
// ----------------------------------------------------------------------------
// MultiDisplay - simple demonstration of accessing multiple lcds
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCDs with PCF8574 or MCP23008 chip based backpacks
// WARNING:
//	Use caution when using 3v only processors like arm and ESP8266 processors
//	when interfacing with 5v modules as not doing proper level shifting or
//	incorrectly hooking things up can damage the processor.
//
// Sketch will print lcd instance number on top line with i2c address
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

// All displays will be assumed to be 16x2
// Even if display is larger the sketch should still work correctly
const int LCD_ROWS = 2;
const int LCD_COLS = 16;


// this lcd[] array declares an array of lcd objects.
// an array makes using multiple lcd easy as you can acces the object
// by using the lcds instance number as in index into the array.
// This really only works if all the lcds are the same.
//

hd44780_I2Cexp lcd[16]; // auto locate & configure up to 16 displays
int NumLcd;		// number of LCD displays found.

// It is also possible to create multiple/seperate lcd objects
// and the library can still automatically locate them.
// Example:
// hd4480_I2Cexp lcd1;
// hd4480_I2Cexp lcd2;
// The individual lcds would be referenced as lcd1 and lcd2
// i.e. lcd1.home() or lcd2.clear()
//
// It is also possible to specify the i2c address
// when defining the lcd object.
// Example:
// hd44780_I2Cexp lcd1(0x20);
// hd44780_I2Cexp lcd2(0x27);
// This ensures that each each lcd object is assigned to a specific
// lcd device rather than letting the library automatically asign it.

void setup()
{
	/*
	 * Locate all the displays by attempting to intialize each one
	 */
	for(NumLcd = 0; NumLcd < 16; NumLcd++)
	{
		/*
		 * If begin fails, then assume we have no more displays
		 */
		if(lcd[NumLcd].begin(LCD_ROWS, LCD_COLS) != 0)
			break;
	}
	if(NumLcd == 0)
	{
		// no LCD devices found, blink the onboard LED if possible

		fatalError(1); // this never returns
	}

	for(int n = 0; n < NumLcd; n++)
	{

		/*
		 * Label the display with its instance number
		 * and i2c address
		 */
		lcd[n].setCursor(0, 0);
		lcd[n].print("LCD:");
		lcd[n].print(n);

		lcd[n].print(" (0x");
		lcd[n].print(lcd[n].getProp(hd44780_I2Cexp::Prop_addr), HEX);
		lcd[n].print(")");
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

		//  write the 'uptime' to each display found
		for(int n = 0; n < NumLcd; n++)
		{
			// set the cursor to column 0, line 1
			// (note: line 1 is the second row, counting begins with 0):
			lcd[n].setCursor(0, 1);
			// print uptime on lcd device: (time since last reset)
			PrintUpTime(lcd[n], secs);
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

// fatalError() - loop & blink an error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}
