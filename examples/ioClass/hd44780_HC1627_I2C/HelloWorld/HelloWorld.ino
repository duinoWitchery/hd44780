// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2020-06-26
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for LCD devices like the Tsingtek Display HC1627 modules
// in i2c mode. Devices such as HC1627-B-LWH-I2C-30 or HC1627-SYH-I2C-30
// These devices have a native I2C interface rather than use a simple I2C
// i/o expander chip such as a PCF8574 or MCP23008.
//
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
// While not all modules use the same pinout,
// Be VERY careful and check your datasheet
//
// This pin table is for the Tsingtek Display HC1627-SYH-I2C-30 or
// HC1627-SYH-I2C-30 module when it is strapped for i2c mode operation
//
// pin 14 is the pin closest to the edge of the PCB
// 14 - connect to gnd
// 13 - connect to vcc
// 12 - ID0 --- controls bit 1 of 7 bit i2c address, strap accordingly
// 11 - ID1 --- controls bit 2 of 7 bit i2c address, strap accordingly
// 10 - not connected
//  9 - not connected
//  8 - SCL
//  7 - SDA
//  6 - connect to gnd
//  5 - connect to gnd
//  4 - connect to gnd
//  3 - Vo Contrast Voltage input
//  2 - VCC (5v)
//  1 - LCD gnd
// 15 - Backlight Anode (+5v)
// 16 - Backlight Cathode (Gnd)
//
// I2C IDx address table
// ID1 ID0 ADDR
//  L   L   0x38/0x39
//  L   H   0x3A/0x3B
//  H   L   0x3C/0x3D
//  H   H   0x3E/0x3F
//
// library only needs to know the base address (the lower address)
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_HC1627_I2C i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_HC1627_I2C i/o class.

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_HC1627_I2C.h> // i2c LCD i/o class header

// Note, i2c address can be specified or automatically located
// If you wish to use a specific address comment out this constructor
// and use the constructor below that specifies the address

// declare the lcd object for auto i2c address location
hd44780_HC1627_I2C lcd;

//
// manually enter base address of LCD.
// Base Addresses 
// - 0x38, 0x3A, 0x3C, or 0x3E

// declare i2c address and constructor for specified i2c address
//const int i2c_addr = 0x38;
//hd44780_HC1627_I2C lcd(i2c_addr); // use device at this address


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
