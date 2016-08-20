//  vi:ts=4
// -----------------------------------------------------------------------
// lcdproc hd44780 serial device
//
// lcdproc is the linux equivilent of Windows LCD Smartie
// Information on how to set this up and use it can be found here:
// https://milesburton.com/USD_LCD_Display_(HD44780)_Running_on_Linux_via_Arduino
// http://lcdproc.omnipotent.net/
//
// This sketch implments a serial hd44780 interface device which can be
// used by lcdproc
// This code should work with any LiquidCrystal compatible library.
//
// While you can modify this sketch to use any "LiquidCrystal" library 
// and modify the default lcd parametes,
// wrapper sketches are included in the hd44780 i/o libraries that 
// declare the lcd object and the needed defines to do this for you.
// -----------------------------------------------------------------------

#ifndef HD44780_LCDOBJECT
/*
 * If you do not use one of the hd44780_xxx i/o class wrapper sketches,
 * you must pick a library/interface and add your own lcd object constructor.
 * the lcd object must be named "lcd"
 * Add your includes and constructor.
 */

// Examples

// LiquidCrystal class is for 4 pin directly connected interface
// initialize the library with the Arduino pin numbers of the LCD interface pins
#include <LiquidCrystal.h>
const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LiquidTWI 'faster' adafruit i2c library
//#include <Wire.h>
//#include <LiquidTWI.h>
//LiquidTWI lcd(0); // Connect via i2c, default address #0 (A0-A2 not jumpered)
//#define WIRECLOCK 400000

// Adafruit_LiquidCrystal I2C 
//#include <Wire.h>
//#include <Adafruit_LiquidCrystal.h>
//Adafruit_LiquidCrystal lcd(0);


#endif
/*
 * Define your LCD  parameters
 * These must match what you configured in LCDd.conf on the linux host
 */
#ifndef LCD_COLS
#define LCD_COLS 20
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 4
#endif

#ifndef BAUDRATE
#define BAUDRATE 9600
#endif

void setup()
{ 
	Serial.begin(BAUDRATE);
	// set up the LCD's number of columns and columns: 
	lcd.begin(LCD_COLS, LCD_ROWS);

#ifdef WIRECLOCK
#if (ARDUINO > 10507) && !defined(MPIDE)
	Wire.setClock(WIRECLOCK); // set i2c clock bit rate, if asked
#endif
#endif

	lcd.clear();
	lcd.print("hd44780");
	lcd.setCursor(0,1);
	lcd.print("LCD Proc");
	delay(1000);
}

uint8_t serial_getch()
{
int incoming;  

	while (!Serial.available()){ }
	// read the incoming byte:
	incoming = Serial.read();

	return (incoming & 0xff);
}

void loop()
{
uint8_t rxbyte;

	rxbyte = serial_getch(); // Fetch byte

	if(rxbyte==0xFE) // If command 
		lcd.command(serial_getch()); // Pass through raw hd44780 command
	else
		lcd.write(rxbyte); //Otherwise send it as text character
}
