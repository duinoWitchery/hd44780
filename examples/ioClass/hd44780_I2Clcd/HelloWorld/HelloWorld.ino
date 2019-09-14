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
//	incorrectly hooking things up can damage the processor.
// 
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_I2Clcd i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_I2Clcd i/o class.

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
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop() {}
