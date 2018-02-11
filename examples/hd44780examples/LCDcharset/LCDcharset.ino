static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDcharset - simple demonstration to show character set font in LCD
// Created by Bill Perry 2018-01-06
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
// 
// sketch displays the character for all the character codes 0x00 to 0xff
// miniture digits are assigned to the custom character codepoints corresponding
// to the codepoint value. i.e. 
// codepoint 0 is assigned miniture '0' character.
// codepoint 1 is assigned miniture '1' character. etc...
//
// ----------------------------------------------------------------------------
// History
// 2018.01.06 bperrybap - Original creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#ifndef HD44780_LCDOBJECT

#error "Special purpose sketch: Use i/o class example wrapper sketch instead."

/*
 * If not using a hd44780 library i/o class example wrapper sketch,
 * you must modify the sketch to include any needed header files for the
 * intended library and define the lcd object.
 *
 * Add your includes and constructor.
 * The lcd object must be named "lcd"
 * and comment out the #error message.
 */

#endif

// ============================================================================
// user configurable options below this point
// ============================================================================


/*
 * Define LCD size
 * 16x2 will work ok on larger displays but if you want the frame rate
 * for the full LCD geometry, define that here
 * Note: these can be (and are) overridden by defines in a wrapper sketch.
 */
#ifndef LCD_COLS
#define LCD_COLS 16
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 2
#endif


// minature digits
// To be used for the Custom characters
const PROGMEM uint8_t minidigit[10][8] = {
	{0x07,0x05,0x05,0x05,0x07,0x00,0x00,0x00}, // minature 0
	{0x02,0x06,0x02,0x02,0x07,0x00,0x00,0x00}, // minature 1
	{0x03,0x05,0x02,0x04,0x07,0x00,0x00,0x00}, // minature 2
	{0x07,0x01,0x07,0x01,0x07,0x00,0x00,0x00}, // minature 3
	{0x05,0x05,0x07,0x01,0x01,0x00,0x00,0x00}, // minature 4
	{0x07,0x04,0x06,0x01,0x07,0x00,0x00,0x00}, // minature 5
	{0x07,0x04,0x07,0x05,0x07,0x00,0x00,0x00}, // minature 6
	{0x07,0x01,0x02,0x04,0x04,0x00,0x00,0x00}, // minature 7
	{0x07,0x05,0x07,0x05,0x07,0x00,0x00,0x00}, // minature 8
	{0x07,0x05,0x07,0x01,0x01,0x00,0x00,0x00}, // minature 9
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
	if(LCD_COLS > 8)
		lcd.print("LCD ");

	lcd.print("Charset");

	// Fill in the 8 custom characters with miniture digits
	// so they can be seen in the character set
	for(uint8_t i=0; i < 8; i++)
		lcd.createChar(i, minidigit[i]); // mini digit matching each codepoint

    delay(2000);
}
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
			c = 0;
			break;
		}
    }
    delay(4000);

}
