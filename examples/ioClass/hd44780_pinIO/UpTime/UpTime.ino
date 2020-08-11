// vi:ts=4
// ----------------------------------------------------------------------------
// UpTime - simple demonstration of lcd
// Created by Bill Perry 2017-05-11
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCDs that are directly controlled with Arduino pins.
//
// Sketch will print "UpTime" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
//
// See below for configuring the Arduino pins used.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
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

void setup()
{
int status;

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	//
	// note:
	//	begin() will automatically turn on the backlight if backlight
	// 	control is specified in the lcd object constructor
	//

	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// hd44780 has a fatalError() routine that blinks an led if possible
		// begin() failed so call fatalError() with the error code.
		hd44780::fatalError(status); // does not return
	}

	// if backlight control was specified, the backlight should be on now

	// Print a message to the LCD
	lcd.print(" UpTime");

	if(LCD_ROWS < 2)
		delay(3000);
}

void loop()
{
static unsigned long lastsecs = -1; // pre-initialize with non zero value
unsigned long secs;
int status;

	secs = millis() / 1000;

	// see if 1 second has passed
	// so the display is only updated once per second
	if(secs != lastsecs)
	{
		lastsecs = secs; // keep track of last seconds

		// set the cursor position to column 0, row 1
		// note: row 1 is the second row from top,
		// since row counting begins with 0
		// if display has only 1 line, it will appear on that line
		status = lcd.setCursor(0, 1);
		if(status) // non zero status means it was unsuccesful
		{
			// setCursor() failed so call fatalError() with the error code.
			hd44780::fatalError(status); // does not return
		}

		// print uptime on lcd device: (time since last reset)
		PrintUpTime(lcd, secs);
	}
}

// PrintUpTime(outdev, secs) - print uptime in HH:MM:SS format
// outdev - the device to send output
//   secs - the total number of seconds uptime
//
// This is a fancy output routine.
// outdev is a Print class object which indicates
// where the output should be sent.
// PrintUpTime can be used with any object that uses the Print class.
// This code works with Serial objects, as well as the the hd44780 lcd objects.
// i.e. you can call with Serial: PrintUpTime(Serial, seconds);

void PrintUpTime(Print &outdev, unsigned long secs)
{
unsigned int hr, mins, sec;

	// convert total seconds to hours, mins, seconds
	mins =  secs / 60;	// how many total minutes
	hr = mins / 60;		// how many total hours
	mins = mins % 60;	// how many minutes within the hour
	sec = secs % 60;	// how many seconds within the minute
		
	// print uptime in HH:MM:SS format

	if(hr > 99)
		hr %= 100; // limit hr to 0-99

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
