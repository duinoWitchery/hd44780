// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// This sketch is for the Noritake CU-U series VFDs, in native serial mode
//
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// The application note for CU-U series boards is here:
//     https://www.noritake-elec.com/includes/documents/brochure/CU-U_Application_Note.pdf
// Datasheets for specific boards, code samples, and more can be found here:
//     https://www.noritake-elec.com/products/vfd-display-module/character-display/cu-u-series
//
//
// The device uses a kind of 3-wire SPI for communication.
// Pinout:
// 1 VCC
// 2 SI/SO (DATA) MOSI - Digital Pin 11 on Uno & Leonardo
// 3 GND
// 4 STB (CS) SS       - Digital Pin 10 on Uno & Leonardo
// 5 SCK (CLK) SCK     - Digital Pin 13 on Uno & Leonardo
// 6 NC
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_NTCUUserial i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_NTCUUserial i/o class.
//

#include <SPI.h>                                // optional, include to use h/w spi
#include <hd44780.h>                            // main hd44780 header
#include <hd44780ioClass/hd44780_NTCUUserial.h> // Noritake CU-U serial i/o class header

// constructor parameters:
// lcd([cs], [clock, data])
// If no parameters, then library will use SS, SCK, and MOSI pins
// If cs parameter specified, then use it for chip select then SCK and MOSI
// If <SPI.h> is included and clock & data pins match h/w SPI pins SCK and MOSI,
//  h/w spi will be used
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
// not use h/w spi. All the other boards will use the h/w SPI pins.
// Consult board pinout diagram to see where SS, SCK, and MOSI are available.
//

#if defined(ARDUINO_AVR_LEONARDO) || ( (USB_VID == 0x2341) && (USB_PID == 0x8036) )
const int cs=10, clk=13, data=11; // uno SPI pins (s/w bit banging will be used)
#else
const int cs=SS, clk=SCK, data=MOSI; // use h/w SPI pins on all other boards
#endif
hd44780_NTCUUserial lcd(cs, clk, data); // declare lcd object

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
