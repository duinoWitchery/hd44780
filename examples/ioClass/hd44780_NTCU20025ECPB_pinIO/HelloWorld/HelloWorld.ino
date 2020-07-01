// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2019-11-23
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for Noritake CU-U series displays like the
// Noritake CU20025ECPB-U1J or CU20025ECPB-W1J
// The Noritake CU20025ECPB display is a 20x2 line VFD display.
// It is not a hd44780 display but is nearly fully compatible with a hd44780
// display. The only real difference is the backlight control.
// So while this device is supported by the hd44780 library,
// the subclass will do its best to emulatate hd44780 functions and map
// them to equivalent functionality when possible.
//
// Sketch prints "Hello, World!" on the lcd
//
// See below for configuring the Arduino pins used.
//
// Noritake CU-U series LCD module pins
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - not connected
//  4 - RS Register Select (rs) --- connect to Arduino pin
//  5 - Read/Write ------ connect to gnd
//  6 - Enable (en) ----- connect to Arduino pin
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4) ---- connect to Arduino pin
// 12 - Data 5 (db5) ---- connect to Arduino pin
// 13 - Data 6 (db6) ---- connect to Arduino pin
// 14 - Data 7 (db7) ---- connect to Arduino pin
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_NTCU20025ECPB_pinIO i/o class
// once the includes are changed to use hd44780 and the lcd object constructor
// is changed to use the hd44780_NTCU20025ECPB_pinIO class.

#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU20025ECPB_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object
// The parameters used by hd44780_NTCU20025ECPB_pinIO are the same as those
// used by the IDE bundled LiquidCrystal library
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // for esp8266 devices
#else
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;       // for all other devices
#endif
hd44780_NTCU20025ECPB_pinIO lcd(rs, en, db4, db5, db6, db7);

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 2;

void setup()
{
	// initialize LCD with number of columns and rows: 
	//
	// note:
	//	begin() will automatically turn on the backlight if backlight
	// 	control is specified in the lcd object constructor
	//
	lcd.begin(LCD_COLS, LCD_ROWS);

	// if backlight control was specified, the backlight should be on now

	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop() {}
