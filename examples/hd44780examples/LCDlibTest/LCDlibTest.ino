static const int dummyvar = 0; // dummy declaration for older broken IDEs!!!!
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDLibTest - LCD library Test sketch
// Copyright 2012-2016 Bill perry
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
// 2012.04.01 bperrybap - Original creation
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

// Adafruit I2C 
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
// currently only done for hd44780 library

#if defined(hd44780_h)
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
void showByteXfer(unsigned long FPStime);

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

Serial.println("LCD initialized");

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
	delay(10);		// delay to ensure no previous commands still pending
	etime = timeFPS(FPS_iter, LCD_COLS, LCD_ROWS);
#endif

#ifdef TIMEBYTE
	// show the average single byte xfer time during the FPS test
	showByteXfer(etime);
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

	if((iLCD_COLS != LCD_COLS) && (iLCD_ROWS != LCD_ROWS))
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

#endif

// Need cursor functions here
// ul cusor
// blk cursor
// ul cursor blink
// blk cursor blink
// cursorLeft()
// cursofRight()
#ifdef DEBUGPRINT
Serial.println("cursor");
#endif
	lcd.clear();
	lcd.print("cursor");
	lcd.cursor();
	delay(DELAY_TIME); // show it for a while

#ifdef DEBUGPRINT
Serial.println("cursor Blink");
#endif
	lcd.clear();
	lcd.print("Cursor Blink");
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
#endif

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
#endif

}

#ifdef TRACKTIME
void lcdPrintTime(uint8_t hr, uint8_t min, uint8_t sec)
{
	if(hr < 10)
		lcd.write('0');
	lcd.print((int)hr);
	lcd.write(':');
	if(min < 10)
		lcd.write('0');
	lcd.print((int)min);
	lcd.write(':');
	if(sec < 10)
		lcd.write('0');
	lcd.print((int)sec);
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
		lcdPrintTime(hr, (uint8_t)min, sec);
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

	/*
	 * Draw Wave
	 */
	lcd.setCursor(0, 1);
	for ( int c = 0; c < LCD_COLS; c++ )
	{
        	lcd.write(animchar(c, loopcount));
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
