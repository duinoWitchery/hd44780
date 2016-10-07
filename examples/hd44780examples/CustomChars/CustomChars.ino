static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// CustomChars - simple demonstration of lcd
// Created by Bill Perry 2016-10-06
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// Sketch demonstrates how to assign custom characters to the eight hd44780
// custom character code points.
// It will display some custom characters and then fall into a loop
// displaying the character for all the character codes 0x00 to 0xff
//
// You can create your own custom characters.
// Here are a couple of web pages that have a tool that will generate the data
// values needed for custom character.
// http://www.quinapalus.com/hd44780udg.html
// https://omerk.github.io/lcdchargen
//

/*
 * Define your LCD size
 * 16x2 will work ok on larger displays but if you want the frame rate
 * for the full LCD geometry, define that here
 */
#ifndef LCD_COLS
#define LCD_COLS 16
//#define LCD_COLS 13
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 2
//#define LCD_ROWS 1
#endif


#ifndef HD44780_LCDOBJECT
/*
 * If not using a hd44780 library i/o class wrapper example sketch,
 * you must pick a library/interface and add your own lcd object constructor.
 * The lcd object must be named "lcd"
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

// LiquidCrystal_I2C library in Arduino library manager
//#include <LiquidCrystal_I2C.h>
//#define WIRECLOCK 400000
//LiquidCrystal_I2C lcd(0x27, LCD_COLS, LCD_ROWS);

//#include <PCF8574_I2C_LCD.h>
////PCF8574_I2C_LCD lcd((PCF8574_address)0x27);


#endif


// some custom characters for demonstration

uint8_t bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
uint8_t note[8]  = {0x02,0x03,0x02,0x0e,0x1e,0x0c,0x00,0x00};
uint8_t clock[8] = {0x00,0x0e,0x15,0x17,0x11,0x0e,0x00,0x00};
uint8_t heart[8] = {0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00,0x00};
uint8_t duck[8]  = {0x00,0x0c,0x1d,0x0f,0x0f,0x06,0x00,0x00};
uint8_t check[8] = {0x00,0x01,0x03,0x16,0x1c,0x08,0x00,0x00};
uint8_t cross[8] = {0x00,0x1b,0x0e,0x04,0x0e,0x1b,0x00,0x00};
uint8_t smile[8] = {0x00,0x0a,0x0a,0x00,0x00,0x11,0x0e,0x00};

uint8_t opnheart[8] = {0x00,0x0a,0x15,0x11,0x0a,0x04,0x00,0x00};

uint8_t uparrow[8]  = {0x04,0x0e,0x1f,0x0e,0x0e,0x0e,0x00,0x00};
uint8_t ruparrow[8] = {0x00,0x0f,0x03,0x05,0x09,0x10,0x00,0x00};
uint8_t luparrow[8] = {0x00,0x1e,0x18,0x14,0x12,0x01,0x00,0x00};
uint8_t dnarrow[8]  = {0x00,0x0e,0x0e,0x0e,0x1f,0x0e,0x04,0x00};
uint8_t rdnarrow[8] = {0x00,0x10,0x09,0x05,0x03,0x0f,0x00,0x00};
uint8_t ldnarrow[8] = {0x00,0x01,0x12,0x14,0x18,0x1e,0x00,0x00};
uint8_t retarrow[8] = {0x01,0x01,0x05,0x09,0x1f,0x08,0x04,0x00};

uint8_t hourglass[8] = {0x1f,0x11,0x0a,0x04,0x0a,0x11,0x1f,0x00};
uint8_t degreeSymbol[8]= {0x06,0x09,0x09,0x06,0x00,0x00,0x00,0x00};
uint8_t degreeC[8]     = {0x18,0x18,0x03,0x04,0x04,0x04,0x03,0x00};

// "Signal Bar" graphics
uint8_t sigbar[][8] = {
#if 1
	{0x00, 0x0f,0x13,0x15,0x19,0x1e,0x00,0x00}, // 0 bars,  (circle w/slash)
#else
	{0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 0 bars, same as <space>
#endif
	{0x00, 0x00,0x00,0x00,0x00,0x00,0x10,0x00}, // 1 bars
	{0x00, 0x00,0x00,0x00,0x00,0x08,0x18,0x00}, // 2 bars
	{0x00, 0x00,0x00,0x00,0x04,0x0c,0x1c,0x00}, // 3 bars
	{0x00, 0x00,0x00,0x02,0x06,0x0e,0x1e,0x00}, // 4 bars
	{0x00, 0x00,0x01,0x03,0x07,0x0f,0x1f,0x00}, // 5 bars
};

void setup()
{
 	lcd.begin(LCD_COLS,LCD_ROWS);                      // initialize the lcd 
  
	// create 8 custom characters
	// Note: On hd44780 displays there are 8 custom characters.
	// They assigned to character code point values 0x00 to 0x0f
	// The code points 0x08 to 0x0f are duplicates for 0x00 to 0x07
	// i.e. 0x08 is the same as 0x00, 0x09 same as 0x01, etc...

	lcd.createChar(0, bell);
	lcd.createChar(1, note);
	lcd.createChar(2, clock);
	lcd.createChar(3, heart);
	lcd.createChar(4, duck);
	lcd.createChar(5, check);
	lcd.createChar(6, cross);
	lcd.createChar(7, smile);

	lcd.home();
  
#if LCD_COLS > 8
	lcd.print("Custom Chars");
#else
	lcd.print("CustChrs");
#endif


	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0, 1);
	}
	else
	{
    	delay(4000);
		lcd.clear();
	}
	for(uint8_t c = 0; c < 8; c++) // display all the custom characters
		lcd.write(c);

	delay(5000);

	// reload new custom characters
	// they will "magically" change on the display to the new custom
	// characters without having to re-write them to the display.

	lcd.createChar(0, sigbar[0]);
	lcd.createChar(1, sigbar[1]);
	lcd.createChar(2, sigbar[2]);
	lcd.createChar(3, sigbar[3]);
	lcd.createChar(4, sigbar[4]);
	lcd.createChar(5, sigbar[5]);

	lcd.createChar(6, opnheart);
	lcd.createChar(7, hourglass);
	delay(5000);

	lcd.createChar(0, uparrow);
	lcd.createChar(1, dnarrow);
	lcd.createChar(2, rdnarrow);
	lcd.createChar(3, ldnarrow);
	lcd.createChar(4, ruparrow);
	lcd.createChar(5, luparrow);
	lcd.createChar(6, retarrow);
	lcd.createChar(7, degreeC);
	delay(5000);
}

// display all character codes
// this includes the last 8 custom characters created
void loop(void)
{
static int c = 0;

    lcd.clear();
	if(LCD_COLS>15)
    	lcd.print("Codes 0");
    lcd.print("x");
	lcd.print(c, HEX);
    lcd.print("-0x"); lcd.print(c+LCD_COLS-1, HEX);

    if(LCD_ROWS > 1)
	{
    	lcd.setCursor(0, 1);
		for(int col = 0; col < LCD_COLS; col++)
			lcd.write(' ');
    	lcd.setCursor(0, 1);
	}
    else
	{
    	delay(2000);
		lcd.clear();
	}
    for(int j=0; j<LCD_COLS; j++)
	{
      	lcd.write((uint8_t)(c));
		if(++c > 255)
		{
			c=0;
			break;
		}
    }
    delay(4000);
}
