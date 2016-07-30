static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDiSpeed - LCD Interface Speed test
// Copyright 2012-2016 Bill perry
// ---------------------------------------------------------------------------
//
//  LCDiSpeed is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  LCDiSpeed is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with LCDiSpeed.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// Sketch to measure and report the speed of the interface to the LCD and
// speed of updating the LCD.
// It should work on any LCD library that is LiquidCrystal API compatible.
//
// It runs a Frames/Sec (FPS) test which writes a "frame" of each digit 0-9 to
// the display.
// A "frame" is a full display of characters.
// It is created by positioning the cursor to the begining of each row
// and then writing a character to every position on the row, until the
// entire display is filled.
// The FPS test does a frame of 9's then 8's, .... down to 0's
// On fast interfaces it will not normally be seen.
//
// The sketch will then calculate & report transfer speeds and
// LCD update rates to the LCD display.
//
// Reported Information:
// - Single byte transfer speed (ByteXfer)
//		This is the time it takes for a single character to be sent from
//		the sketch to the LCD display.
//
// - Frame/Sec (FPS)
//		This is the number of times the full display can be updated
//		in one second. 
//     
// - Frame Time (Ftime)
//		This is the amount of time it takes to update the full LCD display.
//
// The sketch will also report "independent" FPS and Ftime values.
// These are timing values that are independent of the LCD size under test.
// Currently they represent the timing for a 16x2 LCD
// The value of always having numbers for a 16x2 display
// is that these numbers can be compared to each other since they are
// independent of the size of the actual LCD display that is running the test.
// i.e. you also get 16x2 timing information even if the display is not 16x2
//
// All times & rates are measured and calculated from what a sketch "sees"
// using the LCD library API.
// It includes any/all s/w overhead including the time to go through the
// Arduino Print class and LCD library.
// The actual low level hardware times are obviously lower.
//
// History
// 2016.04.14 bperrybap - slimmed down for inclusion as hd44780 example
// 2013.09.01 bperrbap  - refactored to make it easier to select interface
// 2013.06.01 bperrybap - added support for I2C class and ADAFRUIT i2c/spi board
// 2012.03.15 bperrybap - Original creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

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


#endif

// ============================================================================
// user configurable options below this point
// ============================================================================


/*
 * Define your LCD size
 * 16x2 will work ok on larger displays but if you want the frame rate
 * for the full LCD geometry, define that here
 */
#ifndef LCD_COLS
#define LCD_COLS 16
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 2
#endif

// if you have a slow display uncomment these defines
// to override the default execution times.
//  CHEXECTIME is the execution time for clear and home commands
// INSEXECTIME is the execution time for everything else; cmd/data
// times are in Us 
// NOTE: if using, you must enable both
// Although each display can have seperate times, these values will be used
// on all displays.

//#define LCD_CHEXECTIME 2000
//#define LCD_INSEXECTIME 37


/*----------------------------------------------------------------------------*
 * LCDiSpeed Options (normally should not need to change these)
 *----------------------------------------------------------------------------*/

#define FPS_iter 1 	// number of iterations to repeat each "frame" within the
					// test (current frame test is 10 full display frames,
					// 1 for each digits 0-9)
					// FPS_iter like 100 will allow the frames to be seen
					// Note: the only reason other than visual to make this
					// larger than 1 might be to compensate for Async serial
					// buffering should a serial interface be tested
					// even with 1 iteration, 340 bytes are written for a
					// 16x2 display
					// bytes written =
					//   FPS_iter * ((LCD_ROWS * LCD_COLS) + LCD_ROWS) * 10

#define iLCD		 // calculate speed of "independent" sized display
#define iLCD_ROWS 2  // independent FPS row size
#define iLCD_COLS 16 // independent FPS col size


#define DELAY_TIME 3500 // delay time to see information on lcd

// ============================================================================
// End of user configurable options
// ============================================================================

unsigned long timeFPS(uint8_t iter, uint8_t cols, uint8_t rows);
void showFPS(unsigned long etime, const char *type);
void showByteXfer(unsigned long FPStime);

void setup(void)
{
	// set up the LCD's number of columns and rows: 
	// with hd44780 library,
	// set execution times & check for initializatin failure
#if defined(hd44780_h)

		// set custom exectution times if configured
#if defined(LCD_CHEXECTIME) && defined(LCD_INSEXECTIME)
		lcd.setExecTimes(LCD_CHEXECTIME, LCD_INSEXECTIME);
#endif


	if(lcd.begin(LCD_COLS, LCD_ROWS))
	{
		// begin() failed so blink the onboard LED if possible
#ifdef LED_BUILTIN
		pinMode(LED_BUILTIN, OUTPUT);
		while(1)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			delay(500);
			digitalWrite(LED_BUILTIN, LOW);
			delay(500);
		}
#else
		while(1){} // spin and do nothing
#endif

	}
#else
	lcd.begin(LCD_COLS, LCD_ROWS); // can't check status on other libraries
#endif

#ifdef WIRECLOCK
#if (ARDUINO >= 157) && !defined(MPIDE)
	Wire.setClock(WIRECLOCK); // set i2c clock bit rate, if asked
#else
#error attempting to use Wire.setClock on IDE that does not support it
#endif
#endif
}

void loop(void)
{
unsigned long etime;
char buf[8];

	delay(10);		// delay to ensure no previous commands still pending

	/*
	 * Time an FPS test
	 */

	etime = timeFPS(FPS_iter, LCD_COLS, LCD_ROWS);

	/*
 	 * show the average single byte xfer time during the FPS test
	 */
	showByteXfer(etime);

	/*
	 * show FPS rate and Frame update time for this display
	 */

	sprintf(buf, "%dx%d", LCD_COLS, LCD_ROWS);
	showFPS(etime, buf);

#ifdef iLCD
	/*
	 * If the size of the display is not size of the "standad"
	 * display, calculate Independent FPS and Frame update time
	 * (rate & time for a "standard" display - default of 16x2)
	 * This is simply a matter of scaling the time based on the
	 * ratio of the display sizes.
	 */

	if((iLCD_COLS != LCD_COLS) && (iLCD_ROWS != LCD_ROWS))
	{
		etime = etime *iLCD_ROWS * iLCD_COLS / LCD_ROWS / LCD_COLS;

		/*
		 * show independent FPS rate & Frame update time
		 */
		sprintf(buf, "%dx%di", iLCD_COLS, iLCD_ROWS);
		showFPS(etime, buf);
	}
#endif

}

unsigned long timeFPS(uint8_t iter, uint8_t cols, uint8_t rows)
{
char c;
unsigned long stime, etime;

	stime = micros();
	for(c = '9'; c >= '0'; c--) // do not change this unless you change the FPS/ByteXfer calcuations as well
	{
		for(uint8_t i = 0; i < iter; i++)
		{
			for(uint8_t row = 0; row < rows; row++)
			{
				lcd.setCursor(0, row);
				for(uint8_t col = 0; col< cols;col++)
				{
					lcd.write(c);
				}
			}
		}
	}
	etime = micros();
	return((etime-stime));
}
void showFPS(unsigned long etime, const char *type)
{
float fps;


	/*
	 * calculate Frame update time and FPS rate
	 * The 10.0 is for the 10 frames done per iteration
	 * one for each digit 0-9
	 */

	fps = (10.0 * FPS_iter) * 1000000.0/(etime);


	lcd.clear();
	lcd.print(type);
	lcd.print("FPS: ");
	lcd.print(fps);

	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0,1);
	}
	else
	{
		delay(DELAY_TIME);
		lcd.clear();
	}
	lcd.print("Ftime: ");
	lcd.print((etime)/10.0/FPS_iter/1000);
	lcd.print("ms");

	delay(DELAY_TIME);
}
void showByteXfer(unsigned long FPStime)
{
	lcd.clear();
	lcd.print("ByteXfer:");

	/*
	 * Calculate average byte xfer time from time of FPS test
	 * This takes into consideration the set cursor position commands (1 per row) which
	 * are single byte commands and take the same amount of time as a data byte write.
	 * The final result is rounded up to an integer.
	 */
	lcd.print((int) (FPStime / (FPS_iter * (10.0 * (LCD_COLS *  LCD_ROWS + LCD_ROWS)))+0.5));
	lcd.print("uS");

	delay(DELAY_TIME); // show it for a while
}
