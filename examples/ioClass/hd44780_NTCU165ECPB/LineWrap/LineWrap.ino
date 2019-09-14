// vi:ts=4
// ----------------------------------------------------------------------------
// LineWrap - simple demonstration of automatic linewrap functionality
// Created by Bill Perry 2017-05-10
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for the Nortake CU165ECBP-T2J display
//
// Sketch demonstrates hd44780 library automatic line wrapping functionality.
//
// (Configure LCD_COLS & LCD_ROWS if desired/needed)
// Expected behavior of the sketch:
// - display a banner announcing the test.
// - print the configured LCD geometry 
// - print a long text string to demostrate automatic line wrapping 
// - print lots of characters (slowly) to show how the full wrapping works.
// (loop)
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED with the initalization error
// code.
//
// ----------------------------------------------------------------------------
// Datasheet can be found here:
//	http://www.blog.zapro.dk/wp-content/CU165.pdf
// Full datasheet/release notes:
//	http://www.blog.zapro.dk/wp-content/CU165ECPB_T2J_specification.pdf
//
// The device uses SPI for communication.
// Pinout:
// 1 Vcc
// 2 GND
// 3 /CS  (SS)   - Digital Pin 10 on Uno
// 4 CLK  (SCK)  - Digital Pin 13 on Uno
// 5 DATA (MOSI) - Digital Pin 11 on Uno
//
//
// ----------------------------------------------------------------------------

// include the needed headers
#include <SPI.h> // optional, include to use h/w spi
#include <hd44780.h>							// main hd44780 header
#include <hd44780ioClass/hd44780_NTCU165ECPB.h> // include io class header

// declare Arduino pins used for LCD functions
// and the lcd object

// constructor parameters:
// lcd([cs], [clock, data])
// If no parameters, then library will use SS, SCK, and MOSI pins
// If cs parameter specified, then use it for chip select then SCK and MOSI
// If <SPI.h> is included and clock & data pins match h/w SPI pins SCK and MOSI,
//	 h/w spi will be used
// If h/w spi is not possible, then the code will fall back to bit banging.
//
// NOTE:
//  - Leonardo h/w is "stupid" and does not bring out SS
//     (it only drives an LED)
//  - Leonardo does not bring SPI signals to female headers,
//     they are only on 6 pin ISP header.
//  - ESP8266 is does not use naked constants for digital pin numbers 
//
//
// To work around these pin issues in this sketch,
// Leonardo will use uno digital pins for SPI h/w which means it will
// will not use h/w spi. All the other boards will use the h/w SPI pins.
// Consult board pinout diagram to see where SS, SCK, and MOSI are available.
//

#if defined(ARDUINO_AVR_LEONARDO) || ( (USB_VID == 0x2341) && (USB_PID == 0x8036) )
const int cs=10, clk=13, data=11; // uno SPI pins (s/w bit banging will be used)
#else
const int cs=SS, clk=SCK, data=MOSI; // use h/w SPI pins on all other boards
#endif
hd44780_NTCU165ECPB lcd(cs, clk, data); // declare lcd object

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 1;

void setup()
{
int status;

	// initialize LCD with number of columns and rows: 
	// hd44780 returns a status from begin() that can be used
	// to determine if initalization failed.
	// the actual status codes are defined in <hd44780.h>
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed so blink error code using the onboard LED if possible
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
}

void loop()
{
	lcd.clear();
	lcd.print("WrapTest"); 
	delay(2000);
	lcd.clear();

	//print the configured LCD geometry
	lcd.print(LCD_COLS);
	lcd.print("x");
	lcd.print(LCD_ROWS);
	delay(3000);
	lcd.clear();

	// print a long text string
	// without line wrapping enabled, the text would not wrap properly
	// to the next line.

	if(LCD_COLS == 8)
		lcd.print("A long text line");
	else
		lcd.print("This is a very long line of text");
	delay(3000);

	lcd.clear();

	// now print 2 full displays worth of characters to show
	// the full wrapping.

	lcd.cursor(); // turn on cursor so you can see where it is.

	char c = '0'; // start at the character for the number zero
	for(int i = 2*LCD_COLS*LCD_ROWS; i; i--)
	{
		lcd.print(c++);
		delay(200); // slow things down to watch the printing & wrapping

		if(c > 0x7e) // wrap back to beginning of printable ASCII chars
			c = '!'; 
	}
	delay(3000);
	lcd.noCursor(); // turn off cursor
}
