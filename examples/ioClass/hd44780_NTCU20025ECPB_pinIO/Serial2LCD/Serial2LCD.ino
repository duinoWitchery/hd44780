// vi:ts=4
// ----------------------------------------------------------------------------
// Serial2LCD - simple demonstration printing characters from serial port
// Created by Bill Perry 2020-06-28
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
// Sketch demonstrates how to read a message of characters from 
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
// ----------------------------------------------------------------------------
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
// ----------------------------------------------------------------------------

#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU20025ECPB_pinIO.h> // Arduino pin i/o class header

// declare Arduino pins used for LCD functions
// and the lcd object

#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // for esp8266 devices
#else
const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7;       // for all other devices
#endif
hd44780_NTCU20025ECPB_pinIO lcd(rs, en, db4, db5, db6, db7);


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
