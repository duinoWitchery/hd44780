// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// Sketch will print "Hello, World!" on top row of lcd
// and will print the amount of time since the Arduino has been reset
// on the second row.
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// This sketch demonstrates using the Nortake CU165ECBP-T2J display
//
// Datasheet can be found here:
//	http://www.blog.zapro.dk/wp-content/CU165.pdf
// Full datasheet/release notes:
//	http://www.blog.zapro.dk/wp-content/CU165ECPB_T2J_specification.pdf
//
// The device uses SPI for communication.
// Pinout:
// 1 Vcc
// 2 GND
// 3 /CS  (SS)
// 4 CLK  (SCK)
// 5 DATA (MOSI)
//

#include <SPI.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU165ECPB.h> // include io class header

// constructor parameters:
// lcd([cs], [clock, data])
// If no parameters, then library will use SS, SCK, and MOSI and h/w SPI if <SPI.h> included
// If cs parameter, used, then use it for SS
// if clock, and data pins match SPI signals SCK and MOSI h/w spi will be used if <SPI.h> included
//
// NOTE: Leonardo h/w is "stupid" and does not bring out SS (it only drives an LED)
// 	Because of this an explicity cs pin has been defined
//
const int cs = 10;
hd44780_NTCU165ECPB lcd(cs); // declare lcd object

// LCD geometry
const int LCD_ROWS = 1;
const int LCD_COLS = 16;

void setup()
{
	// initialize LCD with number of columns and rows: 
	if( lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible

		fatalError(1); // this never returns
	}
	
	// Print a message to the LCD
	lcd.print("Hello, World!");
	delay(2000);
	lcd.clear();
}

void loop()
{
static unsigned long lastsecs = -1; // pre-initialize with non zero value
unsigned long secs;

	secs = millis() / 1000;

	// see if 1 second has passed
	// so the display is only updated once per second
	if(secs != lastsecs)
	{
		lastsecs = secs; // keep track of last seconds

		lcd.setCursor(0, 0);

		// print uptime on lcd device: (time since last reset)
		PrintUpTime(lcd, secs);
	}
}

// PrintUpTime(outdev, secs) - print uptime in HH:MM:SS format
// outdev - the device to send output
//   secs - the total number of seconds uptime
void PrintUpTime(Print &outdev, unsigned long secs)
{
unsigned int hr, mins, sec;

	// convert total seconds to hours, mins, seconds
	mins =  secs / 60;	// how many total minutes
	hr = mins / 60;		// how many total hours
	mins = mins % 60;	// how many minutes within the hour
	sec = secs % 60;	// how many seconds within the minute
		

	// print uptime in HH:MM:SS format
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

// fatalError() - loop & blink and error code
void fatalError(int ecode)
{
#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	while(1)
	{

		// blink out error code
		for(int i = 0; i< ecode; i++)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			delay(100);
			digitalWrite(LED_BUILTIN, LOW);
			delay(250);
		}
		delay(1500);
	}
#else
	while(1){} // spin and do nothing
#endif
}
