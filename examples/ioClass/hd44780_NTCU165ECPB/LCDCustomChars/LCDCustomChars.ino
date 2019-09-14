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
// This sketch is for the Nortake CU165ECBP-T2J display
//
// Sketch demonstrates how to assign custom characters to the eight hd44780
// custom character codepoints and how to display on the LCD using
// write() and print()
//
// You can create your own custom characters.
// Here are a couple of web pages that have a tool that will generate the data
// values needed for custom character.
// https://kakedev.github.io/GlyphGenerator/
// http://www.quinapalus.com/hd44780udg.html
// https://omerk.github.io/lcdchargen
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
const int LCD_COLS = 16;
const int LCD_ROWS = 1;

const int LongDelay = 5000;
const int ShortDelay = 800;

int customcharRow = 1; // default to printing custom chars on row 1

// Below are some custom characters for demonstration

// You can ensure that the data for these custom characters is only
// in flash and not in RAM by using the const qualifier.
// However....
// If using the AVR part, the AVR proprietary PROGMEM directive must be used.
// PROGMEM is only required on AVR parts since the AVR parts cannot directly
// access const data stored in flash like all the other processors.
// PROGMEM is an AVR specific proprietary kludge that tells the linker and
// startup code to handle the data differently.
// PROGMEM is only used by the AVR and not required by any other processor.
//
// Most non AVR cores provide AVR compatibilty by providing support (emulation)
// for the AVR proprietary PROGMEM directive and corresponding access functions,
// but some do not.
//
// Because of this AVR const data and PROGMEM issue, there is no way to
// guarantee code portability across all cores when using const data.
//
//
// the hd44780 library assumes that if a const qualifier is used on the AVR
// processor that the data has been stored in flash using the PROGMEM directive.
// There is no way for the hd44780 library to know or detect if PROGMEM has
// been used.
// So if the const qualifer is used but the PROGMEM directive is not used on
// an AVR processor, the custom char will be garbage.
//
// Examples:
// For all processors other than AVR:
// 	const char bell[8] = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
// 	const uint8_ bell[8] = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
//
// For AVR: (and non AVR cores that have AVR PROGMEM emulation)
// 	const char bell[8]  PROGMEM = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
// 	const uint8_t bell[8]  PROGMEM = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
//		OR
// 	const PROGMEM char bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
// 	const PROGMEM uint8_t bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
//		NOTE:
//			if PROGMEM is used on older AVR compilers it generates a warning.
//

uint8_t bell[8]  = {0x04,0x0e,0x0e,0x0e,0x1f,0x00,0x04,0x00};
uint8_t note[8]  = {0x02,0x03,0x02,0x0e,0x1e,0x0c,0x00,0x00};
uint8_t clockface[8] = {0x00,0x0e,0x15,0x17,0x11,0x0e,0x00,0x00};
uint8_t heart[8] = {0x00,0x0a,0x1f,0x1f,0x0e,0x04,0x00,0x00};
uint8_t duck[8]  = {0x00,0x0c,0x1d,0x0f,0x0f,0x06,0x00,0x00};
uint8_t check[8] = {0x00,0x01,0x03,0x16,0x1c,0x08,0x00,0x00};
uint8_t cross[8] = {0x00,0x1b,0x0e,0x04,0x0e,0x1b,0x00,0x00};
uint8_t smile[8] = {0x00,0x0a,0x0a,0x00,0x00,0x11,0x0e,0x00};

uint8_t degreeSymbol[8]= {0x06,0x09,0x09,0x06,0x00,0x00,0x00,0x00};
uint8_t degreeC[8]     = {0x18,0x18,0x03,0x04,0x04,0x04,0x03,0x00};
uint8_t degreeF[8]     = {0x18,0x18,0x07,0x04,0x07,0x04,0x04,0x00};

const PROGMEM uint8_t vsigbar[][8] = {
	{0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 0 bars, same as <space>
	{0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x1F}, // 1 bars
	{0x00, 0x00,0x00,0x00,0x00,0x00,0x1F,0x1F}, // 2 bars
	{0x00, 0x00,0x00,0x00,0x00,0x1F,0x1F,0x1F}, // 3 bars
	{0x00, 0x00,0x00,0x00,0x1F,0x1F,0x1F,0x1F}, // 4 bars
	{0x00, 0x00,0x00,0x1F,0x1F,0x1F,0x1F,0x1F}, // 5 bars
	{0x00, 0x00,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, // 6 bars
	{0x00, 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, // 7 bars
	{0x1F, 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F}, // 8 bars
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
		// begin() failed so blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// initalization was successful, the backlight should be on now
}
void loop(void)
{
	lcd.clear();
  
	lcd.print("Custom Chars");

	// create custom characters
	// int rval = createChar(charval, charmap[]);
	//
	// createChar() creates a custom character
	// for the character at the charval codepoint.
	// It returns zero if successful.
	//
	// to display the custom character, simply call write()
	// with the charval use in createChar()
	//
	// The display must be initialized *before* you attempt
	// to create custom characters.
	//
	// Note: On hd44780 displays there are 8 custom characters.
	// They are assigned to character codepoint values 0x00 to 0x07
	// The codepoints 0x08 to 0x0f are duplicates for 0x00 to 0x07
	// i.e. 0x08 is the same as 0x00, 0x09 same as 0x01, etc...

	// create 8 custom characters
	lcd.createChar(0, bell);
	lcd.createChar(1, note);
	lcd.createChar(2, clockface);
	lcd.createChar(3, heart);
	lcd.createChar(4, duck);
	lcd.createChar(5, check);
	lcd.createChar(6, cross);
	lcd.createChar(7, smile);

	// prepare to display the custom characters
	// on multi line displays the custom characters will be
	// displayed on the 2nd line
	// on single line displays, delay a bit to see the initial display,
	// then clear the display to display the custom characters on the top line
	if(LCD_ROWS < 2)
	{
		customcharRow = 0;
		delay(LongDelay);
		lcd.clear();
	}

	lcd.setCursor(0, customcharRow);

	// write() or print() can be used to display custom characters.
	//
	// To use write() pass the charval of the desired custom character.
	//    lcd.write(charval);
	// NOTE:
	// The Print class has an issue that does not allow 0 (zero) to be used
	// on write() without casting it. The Arduino team refuses to fix this.	
	// hd44780 has a work around in it to remove this issue so you can call
	// write() with a constant value of 0 without having to cast it.
	//
	// write() can also be used with literal characters that contain
	// an octal (base 8) escape seuence.
	//    lcd.write('\###');
	//
	// To use print() pass in the charval of the desired custom character
	// as a character *not* an integer.
	// This requires using a literal character with an octal escape sequence.
	//    lcd.print('\###');
	//

	// Since both write() & print() both accept octal escaped literal characters
	// it is the most compatible & portable way of sending custom characters
	
	// display all 8 custom characters.

	// write() with character codepoint values
	lcd.write(0); // casting to an uint8_t or byte not needed with hd44780
	lcd.write(1);

	//  write() & print() with octal escaped literal characters
	lcd.write('\002'); // this is an octal escaped literal character
	lcd.write('\003'); // this is an octal escaped literal character
	lcd.print('\004'); // this is an octal escaped literal character
	lcd.print('\005'); // this is an octal escaped literal character

	// can also drop the leading zeros on small litereal values like these
	lcd.print('\06');  // this is an octal escaped literal character
	lcd.print('\7');   // this is an octal escaped literal character

	delay(LongDelay);

	// You can also insert custom character codepoints into C strings.
	// To do so, insert the character codepoint value as an octal constant.
	// However,
	// because zero indicates the end of string in C you cannot use zero.
	// Example:
	// lcd.print("charval #1: \001"); // prints custom character at codepoint 1


	lcd.setCursor(0,customcharRow);
	lcd.print("code1: \001");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code2: \002");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code3: \003");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code4: \004");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code5: \005");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code6: \006");
	delay(ShortDelay);

	lcd.setCursor(0,customcharRow);
	lcd.print("code7: \007");
	delay(ShortDelay);

	// Another nifty trick is that you can modify custom characters on the
	// display without having to re-write the characters on the display.
	// To do this, simply create a new character for a codepoint.
	// All characters on the display which have that codepoint will "magically"
	// change to the new custom character.
	// So for example, if the entire display was written with custom character
	// zero and character zero was a bell, if you called createChar() to
	// redefine character codepoint zero to be a duck,
	// the entire display would turn to ducks without having to send any
	// characters to the display.

	// create the initial custom character
	lcd.createChar(0, bell);

	// fill the custom character line with the custom character.
	lcd.setCursor(0,customcharRow);
	for(uint8_t col=0; col<LCD_COLS; col++)
		lcd.write(0);

	delay(ShortDelay); 

	// change the single custom character previously written on the row
	// and all the characters on the row will change
	// without having to re-write the characters.
	lcd.createChar(0, note);
	delay(ShortDelay);
	lcd.createChar(0, clockface);
	delay(ShortDelay);
	lcd.createChar(0, heart);
	delay(ShortDelay);
	lcd.createChar(0, duck);
	delay(ShortDelay);
	lcd.createChar(0, check);
	delay(ShortDelay);
	lcd.createChar(0, cross);
	delay(ShortDelay);
	lcd.createChar(0, smile);
	delay(ShortDelay);
	lcd.createChar(0, degreeSymbol);
	delay(ShortDelay);
	lcd.createChar(0, degreeC);
	delay(ShortDelay);
	lcd.createChar(0, degreeF);
	delay(ShortDelay);

	// show multiple vertical bars rising
	// by changing the single custom character that is already written
	// to the entire row on the display
	for(int i = 0; i < 9; i++)
	{
		lcd.createChar(0, vsigbar[i]);
		delay(ShortDelay);
	}

	delay(LongDelay);
}
