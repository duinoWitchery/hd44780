// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for the Noritake CU-U series VFDs, in native serial mode
//
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_NTCUUserial i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_NTCUUserial i/o class.

#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_NTCUUserial.h> // Noritake CU-U serial i/o class header


// declare Arduino pins used for LCD functions
// and the lcd object
const int stb=5, sck=6, sio=4;
hd44780_NTCUUserial lcd(stb, sck, sio);


// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

void setup()
{
	int status;

	// initialize LCD with number of columns and rows:
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	// See the values RV_XXXX
	//
	// looking at the return status from begin() is optional
	// it is being done here to provide feedback should there be an issue
	//
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		status = -status; // convert negative status value to positive number

		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop() {}
