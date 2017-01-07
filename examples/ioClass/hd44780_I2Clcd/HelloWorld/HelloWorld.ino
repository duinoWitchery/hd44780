// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for LCD modules that have a native I2C interface such as
// PCF2119x, PCF2116, or certain RayStar LCDs rather than those LCD modules that
// use an i/o expander chip based based backpack.
// NOTE:
//	These devices usually need external pullups as they typically are not on
//	the module.
// WARNING:
//	Use caution when using 3v only processors like arm and ESP8266 processors
//	when interfacing with 5v modules as not doing proper level shifting or
//  incorrectly hooking things up can damage the processor.
// 
// Sketch will print "Hello, World!" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Clcd.h> // i2c LCD i/o class header

// Note, i2c address can be specified or automatically located
// If you wish to use a specific address comment out this constructor
// and use the constructor below that specifies the address

// declare the lcd object for auto i2c address location
hd44780_I2Clcd lcd;

//
// enter address of LCD.
// Addresses seen so far include:
// - 0x3a, 0x3b (PCF2119x)
// - 0x3c (unknwon chip)
// - 0x3d (unknwon chip)
// - 0x3e (unknwon chip)
// - 0x3f (unknwon chip)

// declare i2c address and constructor for specified i2c address
//const int i2c_addr = 0x3e;
//hd44780_I2Clcd lcd(i2c_addr); // use device at this address


// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup()
{
Serial.begin(9600);
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
