// vi:ts=4
// ----------------------------------------------------------------------------
// ReadWrite - simple demonstration reading data from LCD
// Created by Bill Perry 2016-08-19
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for the Noritake CU-U series VFDs, in native serial mode
//
// The purpose of the sketch is to demonstrate the ability to read data from
// the LCD.
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
//

#include <SPI.h>                                // optional, include to use h/w spi
#include <hd44780.h>                            // main hd44780 header
#include <hd44780ioClass/hd44780_NTCUUserial.h> // Noritake CU-U serial i/o class header

// constructor parameters:
// lcd([cs], [clock, data])
// If no parameters, then library will use SS, SCK, and MOSI pins
// If cs parameter specified, then use it for chip select then SCK and MOSI
// If <SPI.h> is included and clock & data pins match h/w SPI pins SCK and MOSI,
//  h/w spi will be used
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
// not use h/w spi. All the other boards will use the h/w SPI pins.
// Consult board pinout diagram to see where SS, SCK, and MOSI are available.
//

#if defined(ARDUINO_AVR_LEONARDO) || ( (USB_VID == 0x2341) && (USB_PID == 0x8036) )
const int cs=10, clk=13, data=11; // uno SPI pins (s/w bit banging will be used)
#else
const int cs=SS, clk=SCK, data=MOSI; // use h/w SPI pins on all other boards
#endif
hd44780_NTCUUserial lcd(cs, clk, data); // declare lcd object

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
				fatalError(3);
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

// fatalError() - loop & blink an error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}

