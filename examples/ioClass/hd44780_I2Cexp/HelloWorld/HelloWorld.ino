// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
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
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// NOTE:
//	If the sketch fails to produce the expected results, or blinks the LED,
//	run the included I2CexpDiag sketch to test the i2c signals and the LCD.
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_I2Cexp i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_I2Cexp i/o class.

#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

// If you wish to use an i/o expander at a specific address, you can specify the
// i2c address and let the library auto configure it. If you don't specify
// the address, or use an address of zero, the library will search for the
// i2c address of the device.
// hd44780_I2Cexp lcd(i2c_address); // specify a specific i2c address
//
// It is also possible to create multiple/seperate lcd objects
// and the library can still automatically locate them.
// Example:
// hd4480_I2Cexp lcd1;
// hd4480_I2Cexp lcd2;
// The individual lcds would be referenced as lcd1 and lcd2
// i.e. lcd1.home() or lcd2.clear()
//
// It is also possible to specify the i2c address
// when declaring the lcd object.
// Example:
// hd44780_I2Cexp lcd1(0x20);
// hd44780_I2Cexp lcd2(0x27);
// This ensures that each each lcd object is assigned to a specific
// lcd device rather than letting the library automatically asign it.

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
	// note:
	//	begin() will automatically turn on the backlight
	//
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// hd44780 has a fatalError() routine that blinks an led if possible
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// initalization was successful, the backlight should be on now

	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop() {}
