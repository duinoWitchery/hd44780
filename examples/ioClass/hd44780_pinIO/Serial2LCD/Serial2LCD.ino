// vi:ts=4
// ----------------------------------------------------------------------------
// Serial2LCD - simple demonstration printing characters from serial port
// Created by Bill Perry 2020-06-28
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for LCDs that are directly controlled with Arduino pins.
//
// Sketch demonstrates hd44780 how to read a message of characters from 
// serial port and display it on the LCD.
// It takes advantage of the hd44780 library automatic line
// wrapping capability.
// See the LineWrap sketch for details about line wrapping.
//
// Configure LCD_COLS, LCD_ROWS and BAUDRATE if desired/needed
// Expected behavior of the sketch:
// - characters received from serial port are displayed on LCD
// - CR and LF are ignored/dropped
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED with the initalization error
// code.
//
// Some 16x1 displays are actually a 8x2 display that have both lines on
// a single line on the display.
// If you have one of these displays, simply set the geometry to 8x2 instead
// of 16x1. 

// ----------------------------------------------------------------------------
// pinout:
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - not connected
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
// ----------------------------------------------------------------------------

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

// Note: this can be with or without backlight control:

// without backlight control:
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // esp8266 Lolin/Wemos D1 R1 (uno form factor)
#elif defined(ARDUINO_ARCH_ESP32)
// note: GPIO12 needs a pulldown resistor
const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14; // esp32 espduino32 D1 R32 (uno form factor)
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

const int BAUDRATE = 9600;

void setup()
{
int status;

	// initalize Serial port
	Serial.begin(BAUDRATE);

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed

		Serial.print("LCD initalization failed: ");
		Serial.println(status);

		// blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// turn on automatic line wrapping
	// which automatically wraps lines to the next lower line and wraps back
	// to the top when at the bottom line
	// NOTE: 
	// noLineWrap() can be used to disable automatic line wrapping.
	// _write() can be called instead of write() to send data bytes
	// to the display bypassing any special character or line wrap processing.
	lcd.lineWrap();

	lcd.print("Serial2LCD"); 
	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0,1);
		lcd.print("Baud:");
		lcd.print(BAUDRATE);
	}
}

void loop()
{

	// check to see if characters available
	// indicating a message is coming in
	if (Serial.available())
	{
		// wait some time for rest of message to arrive
		delay(100);

		// Clear the display before showing the new message
    	lcd.clear();

		// print the message on the LCD
		while (Serial.available() > 0)
		{
		char c;
			c = Serial.read();
			if(c != '\r' && c != '\n') // drop CR and LF characters
				lcd.write(c);
		}
    }
}
