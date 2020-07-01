static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDLibTest - LCD library Test sketch
// Copyright 2012-2020 Bill perry
// ---------------------------------------------------------------------------
//
//  LCDlibTest is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  LCDlibTest is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with LCDLibTest.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// Sketch to measure and report the speed of the interface to the LCD and
// speed of updating the LCD as well as test some of the library extensions
// for hardware control like backlight control.
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
//
// The sketch will also report "independent" FPS and Ftime values.
// These are timing values that are independent of the size of the LCD under test.
// Currently they represent the timing for a 16x2 LCD
// The value of always having numbers for a 16x2 display
// is that these numbers can be compared to each other since they are
// independent of the size of the actual LCD display that is running the test.
//
// All times & rates are measured and calculeted from what a sketch "sees"
// using the LiquidCrystal API.
// It includes any/all s/w overhead including the time to go through the
// Arduino Print class and LCD library.
// The actual low level hardware times are obviously lower.
//
// History
// 2018.03.23 bperrybap - bumped default instruction time to 38us
// 2012.04.01 bperrybap - Original creation
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
 * Define your LCD size
 * 16x2 will work ok on larger displays but if you want the frame rate
 * for the full LCD geometry, define that here
 * Note: if these can be overridden by defines in a wrapper sketch.
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
//#define LCD_INSEXECTIME 38


/*
 * LCDlibTest Options (normally shouldn not need to change these)
 */


//#define SINGLEWAVE	// different "wave" style
#define SLOWERWAVE	// slow down wave speed
#define DELAY_TIME 3500
#define DEBUGPRINT	// turn on serial debug messages

#define TIMEBYTE	// turn on code that times a byte transfer
#define TIMEFPS		// turn on code that calculates FPS
#define FPS_iter 5 	// number of iterations to repeat each frame 
					// FPS_iter larger than about 1 allows the test to be seen
#define iLCD_ROWS 2 // independent FPS row size
#define iLCD_COLS 16 // independent FPS col size

#define TRACKTIME	// turn on code that displays elapsed time
#define STATUSBARS	// turn on status bars on left & right

// ============================================================================
// End of user configurable options
// ============================================================================


// Turn on extra stuff for certain libraries
//

#if defined(hd44780_h) || defined(LiquidCrystal_I2C_h) || (defined(_LCD_H_) && defined(FOUR_BITS) && defined(BACKLIGHT_ON))
#define ONOFFCMDS	// If on() and off() commands exist
#define SETBACKLIGHTCMD	// if setbacklight() exists
#define BACKLIGHTCMDS	// if backlight()/noBacklight() exist
#endif




// Data format is for each custom character is 8 bytes
// Pixels within the bytes is as follows:
// lowest byte is at top, MSB of byte is on right
// only lower 5 bits of each byte is used.


// Data for a set of new characters for a bargraph
// start with 1 underbar and add additional bars until full 5x8 block

const uint8_t charBitmap[][8] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f}, // char 0 
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f}, // char 1
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f}, // char 2
	{0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f}, // char 3
	{0x00, 0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}, // char 4
	{0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}, // char 5
	{0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}, // char 6
	{0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f}, // char 7
};
/*
 * prototypes
 */

char animchar(int col, int iter);
void showFPS(unsigned long etime, const char *type);
unsigned long timeFPS(uint8_t iter, uint8_t cols, uint8_t rows);
void showByteXfer(Print &dev, unsigned long FPStime);
void fatalError(int ecode);

void setup()
{
int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
unsigned long etime;


#ifdef DEBUGPRINT
	Serial.begin(9600);
#if (ARDUINO > 101)
	do
	{
	// wait on serial port to be ready but timout out after 3 seconds
	// this is for sytems that use virtual serial over USB.
		if(millis() > 3000) // millis starts at 0 after reset
			break;
		delay(10); // easy way to allow some cores to call yield()
	} while(!Serial);
#endif
#endif

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
#else
	lcd.begin(LCD_COLS, LCD_ROWS);
#endif

#ifdef DEBUGPRINT
Serial.println("LCD initialized");
#endif

#ifdef WIRECLOCK
#if (ARDUINO >= 157) && !defined(MPIDE)
	Wire.setClock(WIRECLOCK); // set i2c clock bit rate, if asked
#else
#error attempting to use Wire.setClock on IDE that does not support it
#endif
#endif

	// create the custom bargraph characters
	for ( int i = 0; i < charBitmapSize; i++ )
	{
		lcd.createChar ( i, (uint8_t *)charBitmap[i] );
	}

	// must do something like set cursor postion after defining chars
	// on most libraries (not hd44780)
	// to reset address back to display ram
	lcd.setCursor(0,0);


#if defined(TIMEBYTE) || defined(TIMEFPS)
#ifdef DEBUGPRINT
	Serial.println("Byte timing test");
#endif
	delay(10);		// delay to ensure no previous commands still pending
	etime = timeFPS(FPS_iter, LCD_COLS, LCD_ROWS);
	lcd.clear();
#endif

#ifdef TIMEBYTE
	// show the average single byte xfer time during the FPS test
	showByteXfer(lcd, etime);
#ifdef DEBUGPRINT
	showByteXfer(Serial, etime);
	Serial.println();
#endif
	delay(DELAY_TIME); // show it for a while
	lcd.clear();
#endif

#ifdef TIMEFPS
	// calculate Frame update time and FPS rate for this display
	showFPS(etime, " ");

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
		showFPS(etime, "i");
	}


	lcd.clear();
#endif
	

#ifdef SETBACKLIGHTCMD

// If using hd44780 library check to see if controlling backlight is possible
#if defined(hd44780_h)

 	// try to turn on the backlight
 	// if it fails then backlight control isn't possible
	if(lcd.setBacklight(-1))
	{
#ifdef DEBUGPRINT
Serial.println("LCD API 1.0 setBacklight() not supported");
#endif
		goto skip_dimmingBL;
	}

#endif

#ifdef DEBUGPRINT
Serial.println("Dimming Backlight");
#endif

	// Print a message to the LCD.
	lcd.print("   Dimming BL");

	// soft blink the backlight 3 times by ramping down then back up
	// Hardware that does not support dimming will see the backlight as full on
	// during this test.
	for(int times = 0; times < 3; times++)
	{
		for(int x = 1; x < 16; x++)
		{
			lcd.setBacklight(256-x * 16);
			delay(45);
		}
		for(int x = 1; x < 16; x++)
		{
			lcd.setBacklight(x * 16);
			delay(45);
		}
	}
#else
#ifdef DEBUGPRINT
Serial.println("No LCD API 1.0 setBacklight() function");
#endif
#endif // SETBACKLIGHTCMD

skip_dimmingBL:

// Need cursor functions here
// ul cusor
// blk cursor
// ul cursor blink
// blk cursor blink
// cursorLeft()
// cursofRight()
	lcd.clear();
	lcd.print("cursor");
#ifdef DEBUGPRINT
	Serial.println("cursor");
#endif
	lcd.cursor();
	delay(DELAY_TIME); // show it for a while

	lcd.clear();
	lcd.print("Cursor Blink");
#ifdef DEBUGPRINT
	Serial.println("Cursor Blink");
#endif
	lcd.cursor();
	lcd.blink();
	delay(DELAY_TIME); // show it for a while
	lcd.noCursor();
	delay(DELAY_TIME); // show it for a while
	lcd.noBlink();


#ifdef BACKLIGHTCMDS
#ifdef DEBUGPRINT
Serial.println("Blinking Backlight");
#endif
// If using hd44780 library check to see if controlling backlight is possible
#if defined(hd44780_h)

 	// try to turn on the backlight
 	// if it fails then backlight control isn't possible
	if(lcd.setBacklight(-1))
	{
#ifdef DEBUGPRINT
Serial.println("LCD API 1.0 backlight()/noBacklight() not supported");
#endif
		goto skip_blinkBL;
	}

#endif

	lcd.home();
	lcd.print("  Blinking BL");
	for(int x = 1; x < 5; x++)
	{
		delay(500);
#ifdef DEBUGPRINT
Serial.println("Backlight OFF");
#endif
		lcd.noBacklight();	// turns off backlight but leaves pixels on
		delay(500);
#ifdef DEBUGPRINT
Serial.println("Backlight On");
#endif
		lcd.backlight();
	}
#else
#ifdef DEBUGPRINT
Serial.println("No LCD API 1.0 backlight()/noBacklight() functions");
#endif
#endif // BACKLIGHTCMDS

skip_blinkBL:

	lcd.home();
	lcd.print(" Pixels on/off");
	for(int x = 1; x < 5; x++)
	{
		delay(500);
		lcd.noDisplay(); // turns off display pixels, not backlight
		delay(500);
		lcd.display();	// turns on display pixels
	}

#ifdef later
#ifdef ONOFFCMDS
	lcd.home();
	lcd.print("  On/Off Test  ");
	for(int x = 1; x < 5; x++)
	{
		delay(500);
		lcd.off();	// turns off both display pixels and backlight
		delay(500);
		lcd.on();	// turns on both display pixels and backlight
	}
#endif
#endif

	lcd.clear();
	lcd.print("  Scroll Right ");
	delay(700);
	for(int x = 0; x < LCD_COLS/2; x++)
	{
		delay(300);
		lcd.scrollDisplayRight();
	}
	lcd.clear();
	lcd.print("  Scroll Left  ");
	delay(700);
	for(int x = 0; x < LCD_COLS/2; x++)
	{
		delay(300);
		lcd.scrollDisplayLeft();
	}

#ifdef DEBUGPRINT
Serial.println("setup() done");
#endif

	lcd.clear();
#ifndef TRACKTIME
	lcd.print("   Animations");
#ifdef DEBUGPRINT
	Serial.println("Animations");
#endif
#endif

}

#ifdef TRACKTIME
void PrintTime(Print &dev, uint8_t hr, uint8_t min, uint8_t sec)
{
	// print time in HH:MM:SS format
	// Print class does not support fixed width formatting
	// so insert a zero if number smaller than 10
	if(hr < 10)
		dev.write('0');
	dev.print((int)hr);
	dev.write(':');
	if(min < 10)
		dev.write('0');
	dev.print((int)min);
	dev.write(':');
	if(sec < 10)
		dev.write('0');
	dev.print((int)sec);
}
#endif


void loop()
{
static int loopcount = 0;
#ifdef TRACKTIME
static unsigned long lastsecs;
unsigned long secs;

	secs = millis() / 1000;
	if(secs != lastsecs)
	{
	uint8_t hr;
	int min;
	uint8_t sec;

		lastsecs = secs;
		min =  secs / 60;

		hr = min / 60;

		min = min % 60;

		sec = secs % 60;

		lcd.setCursor(4,0);
		PrintTime(lcd, hr, (uint8_t)min, sec);
#ifdef DEBUGPRINT
		PrintTime(Serial, hr, (uint8_t)min, sec);
		Serial.println();
#endif
	}

#endif


#ifdef STATUSBARS
	/*
	 * Update Status "bars"
	 * 2 up/down bars out of phase with each other.
	 */
	
	lcd.setCursor(0,0);
	lcd.write(animchar(0, loopcount/16));
	lcd.write(animchar(7, loopcount/16));

	lcd.setCursor(LCD_COLS -2,0);
	lcd.write(animchar(7, loopcount/16));
	lcd.write(animchar(0, loopcount/16));
#endif

	if(LCD_ROWS > 1)
	{
		/*
		 * Draw Wave
		 */
		lcd.setCursor(0, 1);
		for ( int c = 0; c < LCD_COLS; c++ )
		{
			lcd.write(animchar(c, loopcount));
   		}
	}
#ifdef SLOWERWAVE
	delay(35);
#endif
		
	loopcount++;

}

#if defined(SINGLEWAVE)
char animchar(int col, int iter)
{
int c;

	if(col > 7)
	{
		col = (char)( (15 -col) & 7);
	}

#if 1
	if((iter % 64) > 31)
	{
		if((iter % 32) > 15)
		{
			c = (col + 7 -(iter & 0xf));		// fall down from the edge
		}
		else
		{
			c = 7 - col -7 + (iter & 0xf);		// rise up from the edge

		}
	}
	else
#endif
	{
		if((iter % 32) > 15)
		{
			c = (7 - col + 7 -(iter & 0xf)); 	// fall down from the middle
		}
		else
		{
			c = col -7 + (iter & 0xf); 		// rise up from the middle

		}
	}


	if(c > 7)
		c = 7;

	if(c < 0)
		c = 0;

	return(char(c));
}
#else
char animchar(int col, int iter)
{
int c;
	col &= 0xf;

	if(col > 7)
	{
		col = (char)( (15 -col) & 7);
	}

	c = (col + (iter & 0xf)) & 0xf;

	if(c > 7)
	{
		c = 15 - c;
	}
	return((char) c);
}
#endif

unsigned long timeFPS(uint8_t iter, uint8_t cols, uint8_t rows)
{
char c;
unsigned long stime, etime;

	stime = micros();
	for(c = '9'; c >= '0'; c--) // do not change this unless you change the FPS calcuation as well
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
	 */

	fps = (10.0 * FPS_iter) * 1000000.0/(etime);


	lcd.clear();
	lcd.print("  ");
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
	lcd.print(type);
	lcd.print("Ftime: ");
	lcd.print((etime)/10.0/FPS_iter/1000);
	lcd.print("ms");

	delay(DELAY_TIME);
}
void showByteXfer(Print &dev, unsigned long FPStime)
{
	dev.print("ByteXfer:");

	/*
	 * Calculate average byte xfer time from time of FPS test
	 * This takes into consideration the set cursor position commands
	 * (1 per row) which are single byte commands and take the same amount of
	 * time as a data byte write.
	 * The final result is rounded up to an integer.
	 */
	dev.print((int) (FPStime / (FPS_iter * (10.0 * (LCD_COLS *  LCD_ROWS + LCD_ROWS)))+0.5));
	dev.print("uS");
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
