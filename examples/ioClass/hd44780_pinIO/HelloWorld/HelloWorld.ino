// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCDs that are directly controlled with Arduino pins.
//
// Sketch prints "Hello, World!" on the lcd
//
// See below for configuring the Arduino pins used.
//
// While not all hd44780 use the same pinout, here is the one that most use:
// pin 1 is the pin closest to the edge of the PCB
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - Vo Contrast Voltage
//  4 - RS Register Select (rs)
//  5 - Read/Write
//  6 - Enable (en)
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4)
// 12 - Data 5 (db5)
// 13 - Data 6 (db6)
// 14 - Data 7 (db7)
// 15 - Backlight Anode (+5v)
// 16 - Backlight Cathode (Gnd)
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_pinIO i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_pinIO class.

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
// The parameters used by hd44780_pinIO are the same as those used by
// the IDE bundled LiquidCrystal library
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
// esp8266 Lolin/Wemos D1 R1 (uno form factor)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7;
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14;
#else
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;       // for all other devices
#endif
hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);

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
//const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7, bl=D10, blLevel=HIGH;
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
//       Dimming will not work on esp32 as it does not have analogWrite()
//const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14, bl=5, blLevel=HIGH;
#else
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
#endif
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLevel);

// LCD geometry
const int LCD_COLS = 16;
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
