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
 * If not using a hd44780 library i/o class wrapper example sketch,
 * you must modify the sketch to include any needed header files for the
 * intended library and define the lcd object.
 *
 * Add your includes and constructor.
 * The lcd object must be named "lcd"
 * and comment out the #error message.
 */

#error "Use i/o class wrapper sketch instead; otherwise, edit appropriately."

// Examples

// hd44780 with hd44780_I2Cexp i/o class
//#include <Wire.h>
//#include <hd44780.h> // include hd44780 library header file
//#include <hd44780ioClass/hd44780_I2Cexp.h> // i/o expander/backpack class
//hd44780_I2Cexp lcd; // auto detect backpack and pin mappings

// hd44780 with hd44780_pinIO class is for 4 pin directly connected interface
// initialize the library with the Arduino pin numbers of the LCD interface pins
//#include <Wire.h>
//#include <hd44780.h> // include hd44780 library header file
//#include <hd44780ioClass/hd44780_pinIO.h> // hd44780 i/o class for pin control
// without backlight control:
//const int rs=12, en=11, db4=5, db5=4, db6=3, db7=2; // IDE LiquidCrystal pins
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7; // lcd keypad shield pins
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7);
//with backlight control:
//const int rs=8, en=9, db4=4, db5=5, db6=6, db7=7, bl=10, blLevel=HIGH;
//hd44780_pinIO lcd(rs, en, db4, db5, db6, db7, bl, blLEvel);

//const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7; // pins for lcd keypad shield
//hd44780_pinIO lcd(rs, en, d4, d5, d6, d7);

// LiquidCrystal class is for 4 pin directly connected interface
// initialize the library with the Arduino pin numbers of the LCD interface pins
//#include <LiquidCrystal.h>
//const int rs=8, en=9, d4=4, d5=5, d6=6, d7=7;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LiquidTWI 'faster' adafruit i2c library
//#include <Wire.h>
//#include <LiquidTWI.h>
//LiquidTWI lcd(0); // Connect via i2c, default address #0 (A0-A2 not jumpered)
//#define WIRECLOCK 400000

// Adafruit I2C for their #292 i2c/spi backpack
//#include <Wire.h>
//#include <Adafruit_LiquidCrystal.h>
//Adafruit_LiquidCrystal lcd(0); // use default i2c address

//#include <PCF8574_I2C_LCD.h>
////PCF8574_I2C_LCD lcd((PCF8574_address)0x27);

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
	// print out a banner that indicates lcd proc device
	// and pramameters like baudrate and geometry
	lcd.print("LCD Proc:");
	lcd.print("hd44780");
	lcd.setCursor(0,1);
	lcd.print(BAUDRATE);
	lcd.print(' ');
	lcd.print(LCD_COLS);
	lcd.print('x');
	lcd.print(LCD_ROWS);
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
