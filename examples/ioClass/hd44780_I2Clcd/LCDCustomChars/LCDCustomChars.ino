static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDCustomChars - simple demonstration of lcd custom characters
// Created by Bill Perry 2016-10-06
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// This sketch is for LCD modules that have a native I2C interface such as
// PCF2119x, PCF2116, or certain RayStar LCDs rather than those LCD modules that
// use an i/o expander chip based based backpack.
// NOTE:
//	These devices usually need external pullups as they typically are not on
//	the module.
// WARNING:
//	Use caution when using 3v only processors like arm and ESP8266 processors
//	when interfacing with 5v modules as not doing proper level shifting or
//  incorrectly hooking things up can damage the processor.
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

#include <Wire.h>
#include <hd44780.h> // include hd44780 library header file
#include <hd44780ioClass/hd44780_I2Clcd.h> // i2c LCD i/o class header

// Note, i2c address can be specified or automatically located
// If you wish to use a specific address comment out this constructor
// and use the constructor below that specifies the address

// declare the lcd object for auto i2c address location
hd44780_I2Clcd lcd;

//
// enter address of LCD.
// Addresses seen so far include:
// - 0x3a, 0x3b (PCF2119x)
// - 0x3c (unknwon chip)
// - 0x3d (unknwon chip)
// - 0x3e (unknwon chip)
// - 0x3f (unknwon chip)

// declare i2c address and constructor for specified i2c address
//const int i2c_addr = 0x3e;
//hd44780_I2Clcd lcd(i2c_addr); // use device at this address

// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;


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
	// note:
	//	begin() will automatically turn on the backlight
	//
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		status = -status; // convert negative status value to positive number

		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// initalization was successful, the backlight should be on now

	// create 8 custom characters
	// int rval = createChar(charval, charmap[]);
	//
	// createChar() creates a custom character
	// for the character at the charval codepoint.
	// It returns zero if successful.
	//
	// The display must be initialized *before* you attempt
	// to create custom characters.
	//
	// Note: On hd44780 displays there are 8 custom characters.
	// They are assigned to character code point values 0x00 to 0x0f
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
  
	if(LCD_COLS > 8 )
		lcd.print("Custom Chars");
	else
		lcd.print("CustChrs");

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
