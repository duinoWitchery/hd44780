static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDiSpeed - LCD Interface Speed test
// Copyright 2012-2020 Bill perry
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
// 2018.03.23 bperrybap - bumped default instruction time to 38us
// 2016.04.14 bperrybap - slimmed down for inclusion as hd44780 example
// 2013.09.01 bperrbap  - refactored to make it easier to select interface
// 2013.06.01 bperrybap - added support for I2C class and ADAFRUIT i2c/spi board
// 2012.03.15 bperrybap - Original creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

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

// if you have a slow display uncomment these defines
// to override the default execution times.
//  CHEXECTIME is the execution time for clear and home commands
// INSEXECTIME is the execution time for everything else; cmd/data
// times are in Us 
// NOTE: if using, you must enable both
// Although each display can have seperate times, these values will be used
// on all displays.

//#define LCD_CHEXECTIME 2000
//#define LCD_INSEXECTIME 38


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
void showFPS(unsigned long etime, int cols, int rows, const char *fpstype);
void showByteXfer(unsigned long FPStime);
void fatalError(int ecode);

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
		fatalError(1); // this never returns
	}

// NOTE: Can't check initalization status on other libraries
// NOTE: fm's LiquidCrystal_I2C class and other LiquidCrystal_I2C
// 	classes define LiquidCrystal_I2C_h, but they don't initialize the
//	the same way. We check for I2CIO use when checking for
//	LiquidCrystal_I2C use to see which "LiquidCrystal_I2C" is
//	is being used since fm's uses begin() while all the others use init()
//
#elif defined(LiquidCrystal_I2C_h) && !defined(_I2CIO_H_)
	lcd.init();
	lcd.backlight(); // LiquidCrystal_I2C does not turn on backlight by default.
#elif defined(LCDISPEED_CALL_CUSTOM_LCDINIT)
	custom_LCDinit(); // code is in LCDiSpeed wrapper sketch (this is FUGLY!!!)
#else
	lcd.begin(LCD_COLS, LCD_ROWS);
#endif

#ifdef WIRECLOCK
#if defined(WIRE_HAS_SETCLOCK) || ((ARDUINO >= 157) && !defined(MPIDE))
	Wire.setClock(WIRECLOCK); // set i2c clock bit rate, if asked
#else
#error attempting to use Wire.setClock on IDE that does not support it
#endif
#endif

#if defined (WIRE_HAS_GETCLOCK)
	lcd.print("i2c clock:");
	lcd.setCursor(0,1);
	lcd.print(Wire.getClock());
	lcd.print(" Hz");
	delay(3000);
#endif


}

void loop(void)
{
unsigned long etime;

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

	showFPS(etime, LCD_COLS, LCD_ROWS, 0);

#ifdef iLCD
	/*
	 * If the size of the display is not size of the "standad"
	 * display, calculate Independent FPS and Frame update time
	 * (rate & time for a "standard" display - default of 16x2)
	 * This is simply a matter of scaling the time based on the
	 * ratio of the display sizes.
	 */

	if((iLCD_COLS != LCD_COLS) || (iLCD_ROWS != LCD_ROWS))
	{
		etime = etime *iLCD_ROWS * iLCD_COLS / LCD_ROWS / LCD_COLS;

		/*
		 * show independent FPS rate & Frame update time
		 */
		showFPS(etime, iLCD_COLS, iLCD_ROWS, "i");
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
void showFPS(unsigned long etime, int cols, int rows, const char* fpstype)
{
float fps;


	/*
	 * calculate Frame update time and FPS rate
	 * The 10.0 is for the 10 frames done per iteration
	 * one for each digit 0-9
	 */

	fps = (10.0 * FPS_iter) * 1000000.0/(etime);


	lcd.clear();
	lcd.print(cols);
	lcd.print('x');
	lcd.print(rows);
	if(fpstype)
		lcd.print(fpstype);
	lcd.print("FPS:");
	if(fps < 1000)
		lcd.print(" ");
	lcd.print(fps, 2);

	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0,1);
	}
	else
	{
		delay(DELAY_TIME);
		lcd.clear();
	}
	if(fpstype)
		lcd.print(fpstype);
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
// fatalError() - loop & blink and error code
void fatalError(int ecode)
{
#if defined(hd44780_h)
	// if using hd44780 library use built in fatalError()
	hd44780::fatalError(ecode);
#else
	if(ecode){} // dummy if statement to remove warning about not using ecode
	while(1)
	{
		delay(1); // delay to prevent WDT on some cores
	}
#endif
}
