// vi:ts=4
// ----------------------------------------------------------------------------
// HelloWorld - simple demonstration of lcd
// Created by Bill Perry 2016-07-02
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for the Nortake CU165ECBP-T2J display
//
// Sketch prints "Hello, World!" on the LCD
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
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
// 3 /CS  (SS)   - Digital Pin 10 on Uno
// 4 CLK  (SCK)  - Digital Pin 13 on Uno
// 5 DATA (MOSI) - Digital Pin 11 on Uno
//
// ----------------------------------------------------------------------------
// LiquidCrystal compability:
// Since hd44780 is LiquidCrystal API compatible, most existing LiquidCrystal
// sketches should work with hd44780 hd44780_NTCU165ECPB i/o class once the
// includes are changed to use hd44780 and the lcd object constructor is
// changed to use the hd44780_I2CNTCU165ECPB i/o class/

#include <SPI.h> // optional, include to use h/w spi
#include <hd44780.h>
#include <hd44780ioClass/hd44780_NTCU165ECPB.h> // include io class header

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
const int LCD_ROWS = 1;
const int LCD_COLS = 16;

void setup()
{
	// Special Note:
	// =============
	// Since this device uses a write only SPI interface, there is no way for
	// the library/sketch to know if the device is present or if there is any
	// type of issue communicating with the device.
	// So while begin() returns a status as to whether it was successful,
	// the library will never report a failure since it has no way of detecting
	// a communication or initialization issue.

	// initialize LCD with number of columns and rows: 
	lcd.begin(LCD_COLS, LCD_ROWS);
	
	// Print a message to the LCD
	lcd.print("Hello, World!");
}

void loop(){}
