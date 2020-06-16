// vi:ts=4
// ----------------------------------------------------------------------------
// I2Cexpdiag - i2c LCD i/o expander backpack diagnostic tool
// Created by Bill Perry 2016-06-17
// Copyright 2016-2020 - Under GPL v3
// 
// ----------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  I2Cexpdiag is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  I2Cexpdiag is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with I2Cexpdiag.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------
// Description:
// -----------------------------------------------------------------------
// Sketch to verify configuation & operation of hd44780 LCDs based
// on the Hitachi HD44780 and compatible chipsets using I2C extension
// backpacks that use a simple I2C i/o expander chip.
//
// WARNING:
//	Use caution when using 3v only processors like arm and ESP8266 processors
//	when interfacing with 5v modules as not doing proper level shifting or
//	incorrectly hooking things up can damage the processor.
//
// It requires the hd44780 library which can be installed using the
// Arduino IDE library manager starting with IDE 1.6.2 or can found here:
// https://github.com/duinoWitchery/hd44780
//
// It is recommended to run the sketch with a serial monitor
// to be able to see the diagnostic messages, in case there are any issues.
//
// While the sketch will work without a serial monitor connection,
// on some USB based boards like the Teensy or Leonardo,
// if there is no USB serial port connection,
// the code will wait up to 3 seconds for a USB serial connection to be
// established before starting.
//
// The serial port is configured to 9600 baud.
//
// The sketch will perform checks on the i2c bus as well as mutiple LCD
// expander based devices if multiple devices are connected to the i2c bus.
// Information is provided on the LCDs as well as sent to the serial monitor.
//
// -----------------------------------------------------------------------
// To use this sketch:
// -----------------------------------------------------------------------
//
// 1. install hd44780 library
//	the hd44780 library can be installed using the Arduino IDE library manager
//	starting with IDE 1.6.2 or can found here:
//		https://github.com/duinoWitchery/hd44780
//
// 2. Hookup up i2c i/o expander backpack based LCD devices
//    and only the i2c LCD backpack(s) if possible.
//	Currently supports PCF8574 or MCP32008 devices
//	(can test more than one LCD device at a time)
//	WARNING: 3v only systems like ARM/Teensy3/ESP8266 devices will need to take
//		precautions on SDA and SCL connections if using 5V I2C devices.
//		Level shifters are recommended and should be used.
//		It is possible to cheat and hook the pullups to 3v instead of 5v.
//		If doing this, there can be no other pullups that are conected to 5V
//		on any device that is connected to the bus.
//		Many of the backpacks ind other devices nclude 5v pullups on them
//		so if pullups to 3v are being used to cheat, the backpack or other
//		device will likely require hardware modification.
//		Failure to do this 3v pullup "cheat" correctly can damage the processor
//		which is why using actual level shifters is recommended.
//
// 3. Connect to the board using the IDE serial monitor; set the baud rate to 9600
//	While using the serial monitor is not required, additional information
//	will be sent to the serial monitor.
//	NOTE: If you have IDE older than 1.6.6 you will have to connect to the
//		serial monitor *after* the upload completes as the upload will
//		not reconnect after the upload completes.
//
//	It is recommended to connect to the serial monitor *before* you upload the
//	sketch so that the serial monitor is reconnected as soon as the sketch is
//	uploaded to avoid character loss.
//	This is particularly helpful on boards that use a USB virtual serial port
//	like leonardo.
//
// 4. compile and upload sketch
//	Note:
//		if you have a slow LCD display you may need to modify the
//		execution times to be longer than the hd44780 defaults.
//		Scroll down to modify the defines: LCD_CHEXECTIME, LCD_INSEXECTIME
//
// -----------------------------------------------------------------------
// Expected behavior
// -----------------------------------------------------------------------
// Highlights of sketch behavior
//	- display information about the enviroment on the serial monitor.
//	- probe the i2c bus to check for external pullup resistors 
//	- scan the i2c bus and show all devices found
//		NOTE: Arduino 2560 boards have 10k external pullups on the arduino board
//		      Arduino DUE V3 boards have 1.5k external pullups on the arduino board
//	- attempt to initalize each LCD device detected
//	- attempt to blink backlight of each initalized LCD 3 times
//	- display information about each each initialized LCD device
//		this includes i2c address and configuration information
//		and information about missing pullups.
//		Note that pins work differently for some of the ESP8266 boards.
//		The ESP8266 core use GPIO bit numbers and a few boards use Dn or Pn defines
//		to do do pin # mapping to bit numbers.
//		The code attempts to accurately report that information.
//	- test internal LCD display memory
//		 LCD expander must be able to control r/w line
//	- perform a backlight blink test
//	- drop into a loop and display the system uptime on each display
//    *P on the display indicates missing external pullups
//
//	If the sketch cannot determine any usable LCD devices 
//	the code will fall into a fatal error and blink out an error code:
//	Error codes:
//		[1] no i2c devices found on i2c bus
//		[2] no working LCD devices
//		[3] i2c bus is not usable
//
// -----------------------------------------------------------------------
// Also note:
// With respect to the the jumper on certain backpacks:
// On the bacpacks Ive seen so far, it controls the backlight control.
// Depending on the backpack, it can
// - force the backlight on
// - force the backlight off
// - allow backlight control by PCF8574
//
// So you may have to experiment with the jumper in/out to be able
// to see anything on the display if you have a display that uses
// light pixels on a dark background.
//
// -----------------------------------------------------------------------
// NOTE:
//  This sketch is a diagnostic tool, as such, it is not an example of
//  library usage.
//  It uses internal library information and APIs that are not needed
//  and should not normally used by sketches.
//  Sketches that use these undocumented capabilities may not work correctly
//  in future releases
// -----------------------------------------------------------------------
// 
// History
// 2020.06.16 bperrybap  - report begin() status when it fails
// 2020.06.03 bperrybap  - added SDA/SCL pin output decodes for STM32 platform
// 2020.05.18 bperrybap  - reduced defualt max displays to 4 to save memory
// 2020.05.18 bperrybap  - hack workaround for RogerClarks STM32 cores
// 2020.05.14 bperrybap  - check for F_CPU define
// 2020.05.13 bperrybap  - removed ifdef check for INPUT_PULLUP
// 2020.03.28 bperrybap  - tweak for ESP32 core
// 2019.07.28 bperrybap  - clarified define to disable ESP8266 specific pin
//                         decoding
// 2018.10.16 bperrybap  - better shorted pin testing
// 2018.06.17 bperrybap  - check for SDA and SCL shorted together
// 2018.03.23 bperrybap  - bumped default instruction time to 38us
// 2016.12.25 bperrybap  - updates for ESP8266
// 2016.08.07 bperrybap  - added lcd memory tests
// 2016.07.27 bperrybap  - added defines for setting execution times
// 2016.06.17 bperrybap  - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// -----------------------------------------------------------------------

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

// ============================================================================
// user configurable options below this point
// ============================================================================

// Uncomment and use this line instead of the one below if you have a SYDZ backpack
//hd44780_I2Cexp lcd[1]={{I2Cexp_ADDR_UNKNOWN, I2Cexp_BOARD_SYDZ}}; // to run on a single SYDZ based backpack

// set maximum number of displays to auto locate & configure
// this can be any number between 1 and 16
// this was reduced from 16 to allow running on AVRs with smaller RAM
hd44780_I2Cexp lcd[4];

// All displays will be assumed to be 16x2
// Even if display is larger the sketch should still work correctly
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

// If code has issues compiling for ESP8266 / ESP32 cores,
// and/or breaks in printDigitalPin() function,
// turn on this define to disable
// ESP specific pin decoding
//#define I2CEXPDIAG_CFG_NO_DECODE_ESPXXXXPINS

// if you have slow displays uncomment these defines
// to override the default execution times.
//  CHEXECTIME is the execution time for clear and home commands
// INSEXECTIME is the execution time for everything else; cmd/data
// times are in Us 
// NOTE: if using, you must enable both
// Although each display can have seperate times, these values will be used
// on all displays.

//#define LCD_CHEXECTIME 2000
//#define LCD_INSEXECTIME 38

// ============================================================================
// End of user configurable options
// ============================================================================


// for now create SDA and SCL defines for chipkit boards, as it is missing
// note: this should continue to work if they eventually add these defines/const values
#if !defined(SDA) || !defined(SCL)
#if defined(_DTWI0_SDA_PIN) && defined(_DTWI0_SCL_PIN)
#define SDA _DTWI0_SDA_PIN
#define SCL _DTWI0_SCL_PIN
#endif
#endif

// Create SDA and SCL defines for RogerClark's STM32 platform.
// https://github.com/rogerclarkmelbourne/Arduino_STM32
// Note, this is total BULLSHIT as I offered a fix for this and he refused to
// even accept this as an issue.
// https://github.com/rogerclarkmelbourne/Arduino_STM32/issues/777
// Roger's platform does have SDA and SCL defines but they are currently
// in SoftWire.h so they only exist when SoftWire.h is included.
// While they should always exist, I made an alternative
// suggestion to move them to utility/WireBase.h so at least they
// would exist when either Wire.h or SoftWire.h was included.
//
// This hack includes SoftWire.h on that platform just to get the symbols
// NOTE:
// there is no Wire library support in Roger's STM32F2 core
#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
#include <SoftWire.h>
#endif

/*
 * Nasty kludges for @#@#@ AVR progmem CRAP
 * PSTR() macro uses static char instead of const char
 * This matters on C++ so we can't use const on our progmem strings
 * because you can't mix const and non const in the same section.
 * It could be declared as "static" instead of "const" or
 * alternatively you can use a different section.
 * But we still need to redefine PROGMEM to keep from getting warnings
 * for for each and every use of F() and PSTR() macros on older versions of the IDE.
 */

#if defined(__AVR__)
#include <avr/pgmspace.h>
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif
#define PROGMEMDIAG __attribute__((section(".progmem.diag")))
#define P(name)   const char name[] PROGMEMDIAG   // declare a const string in AVR Progmem

#else
// The rest of the world is so much simpler and normal
#define P(name)   const char name[]   // declare a const string
#endif

/*
 * declare some horizonal 'line' strings in program memory
 * so they aren't duplicated on each use.
 */
P(_hline) =  "--------------------------------------------------------------------";
P(_hstar) =  "********************************************************************";
P(_PASSED) = "PASSED";
P(_FAILED) = "FAILED";

#ifndef BIT
#define BIT(_bitnum) (1 << _bitnum)
#endif

#ifdef __AVR__
#define hline (const __FlashStringHelper *)_hline
#define hstar (const __FlashStringHelper *)_hstar
#define PASSED (const __FlashStringHelper *)_PASSED
#define FAILED (const __FlashStringHelper *)_FAILED
#else
#define hline _hline
#define hstar _hstar
#define PASSED _PASSED
#define FAILED _FAILED
#endif


#define MAX_ERRORS 16

#define DEFPROMPT ((const char *) 0)

int NumLcd;		// number of LCD displays found.
uint16_t WorkingLCD = 0; // bitmap of LCDs that are "working"
// macros to process working lcd info
#define isWorkingLCD(_n) (WorkingLCD & BIT(_n))
#define setWorkingLCD(_n) WorkingLCD |= BIT(_n)
#define clrWorkingLCD(_n) WorkingLCD &= ~BIT(_n)
#define anyWorkingLCD (WorkingLCD) // non zero if there are any working LCDs




// convert a define to a string
#define define2str(s) _str(s)
#define _str(...) #__VA_ARGS__

void setup()
{
int nopullups;

	// give lots of time to allow the main power to ramp up and
	// allow all the hardware time settle
	// this is because the procesors often start running before full VCC
	// has been achieved.
	delay(100);

	Serial.begin(9600);

#if (ARDUINO > 101)
	do
	{
	// wait on serial port to be ready but timout out after 5 seconds
	// this is for sytems that use virtual serial over USB.
		if(millis() > 5000) // millis starts at 0 after reset
			break;
		delay(10); // easy way to allow some cores to call yield()
	} while(!Serial);
#endif

	Serial.println();
	Serial.println(hstar);
	Serial.println(F("Serial Initialized"));

	Serial.println(hline);
	Serial.println(F("I2CexpDiag - i2c LCD i/o expander backpack diagnostic tool"));
#ifdef HD44780_VERSIONSTR
	Serial.println(hline);
	Serial.print(F("hd44780 lib version: "));
	Serial.println(HD44780_VERSIONSTR);
#endif

#if ARDUINO < 10605 
	// wait 3 seconds on older IDEs
	// to allow users some time to manually start monitor
	delay(3000);
#endif

	Serial.println(hline);
	showSystemConfig();

	Serial.println(hline);
	nopullups = i2cpulluptest();

	if(nopullups < 0)
	{
		Serial.println(F("I2C bus not usable"));
		fatalError(3); // this never returns
	}

	Serial.println(hline);
	Wire.begin();
	if(!showI2Cdevices()) // show all i2c devices on bus
	{
		Serial.println(F("No I2C devices found"));

		fatalError(1); // this never returns
	}
	Serial.println(hline);

	Serial.print(F("Scanning i2c bus for all lcd displays ("));
	Serial.print(sizeof(lcd)/sizeof(hd44780_I2Cexp));
	Serial.println(F(" max)"));

	/*
	 * Locate all the displays by attempting to intialize each one
	 */
	for(NumLcd = 0; NumLcd < (int) (sizeof(lcd)/sizeof(hd44780_I2Cexp)); NumLcd++)
	{
	char buf[16];
	int status;
		// set custom exectution times if configured
#if defined(LCD_CHEXECTIME) && defined(LCD_INSEXECTIME)
		lcd[NumLcd].setExecTimes(LCD_CHEXECTIME, LCD_INSEXECTIME);
#endif

		// If begin fails, then assume we have no more displays
		if((status = lcd[NumLcd].begin(LCD_ROWS, LCD_COLS)) != 0)
		{
			if(NumLcd == 0)
			{
				Serial.print("LCD 0 begin() failed: ");
				Serial.println(status);
			}
			break;
		}

		setWorkingLCD(NumLcd); // mark LCD as "working"

		Serial.print(F(" LCD at address: "));
		Serial.print(F("0x"));
		Serial.print(lcd[NumLcd].getProp(hd44780_I2Cexp::Prop_addr), HEX);

		Serial.print(F(" | config: "));
		Serial.print(lcdConfigStr(buf, lcd[NumLcd]));

		Serial.print(F(" | R/W control: "));
		// it takes r/w control to read LCD status
		// assume if reading status fails, no r/w control
		if(lcd[NumLcd].status() < 0)
			Serial.print(F("No"));
		else
			Serial.print(F("Yes"));

		Serial.println();

		// attempt to blink backlight 3 times
		for(int i = 0; i < 3; i++)
		{
			lcd[NumLcd].noBacklight(); // turn off backlight
			delay(150);
			lcd[NumLcd].backlight(); // turn on backlight
			delay(200);
		}
		
	}

	if(!NumLcd)
	{
		Serial.println(F("No working LCD devices"));
		fatalError(2); // this never returns
	}
	Serial.print(F("Total LCD devices found: "));
	Serial.println(NumLcd);
	Serial.println(hline);

	Serial.println(F("LCD Display Memory Test"));
	for(int n = 0; n < NumLcd; n++)
	{
	int errors, lcdstatus;

		Serial.print(F("Display: "));
		Serial.println(n);

		// check for r/w control
		// by attempting to read lcd status
		if((lcdstatus = lcd[n].status()) >= 0)
		{
			if(lcdstatus & 0x80) // check for stuck BUSY status
			{
				Serial.println(F(" LCD stuck BUSY status"));
				clrWorkingLCD(n); // mark LCD as no longer "working"
				continue;
			}

			Serial.print(F(" Walking 1s data test:\t"));
			// try a few different locations which also tests addressing
			errors = lcdw1test(lcd[n], 0);
			errors += lcdw1test(lcd[n], 0x40);
			errors += lcdw1test(lcd[n], 0x10);
			errors += lcdw1test(lcd[n], 0x50);
			if(errors)
			{
				Serial.print(FAILED);
			}
			else
			{
				Serial.print(PASSED);
			}

			Serial.println();

			Serial.print(F(" Address line test:\t"));
			errors = lcdAddrLineTest(lcd[n], 0x00, 0x27); // 1st block of memory
			errors += lcdAddrLineTest(lcd[n], 0x40, 0x67); // 2nd block of memory
			if(errors)
			{
				Serial.print(FAILED);
				clrWorkingLCD(n); // mark LCD as no longer "working"
			}
			else
			{
				Serial.print(PASSED);
			}

		
#if 0
			Serial.println();

			// quick/short test of DDRAM
			// note: avoid  <CR> and <LF> in value range since those
			// are currently dropped by write()
			// Also, the hd44780 has 80 bytes of ram but it is not contiguous.
			// The 1st 40 bytes is 0x00 - 0x27
			// the 2nd 40 bytes is 0x40 - 0x67
			// attempting to use 0x28 - 0x3f or 0x68-0x7f will fail as there
			// technically is no memory there so it maps internally to other
			// locations and a memory test would fail.
			//
			// this quick test will test a few values on the 2nd chunk of memory.
			// 
			Serial.print(F("  Quick DDRAM memory test: "));
			errors = lcdDDRAMtest(lcd[n], 0x40, 0x67, '0', '9');

			if(errors)
				Serial.print(FAILED);
			else
				Serial.print(PASSED);
#endif
			if(errors)
			{
				Serial.println();
				Serial.println(F("Memory test failures are usually due to poor solder connections"));
				Serial.println(F("Most common cause is poor solder joints on pins soldered to the LCD"));
			}
		}
		else
		{
			Serial.print(F(" (R/W control not supported)"));
		}
		Serial.println();
	}
	Serial.println(hline);

	if(!anyWorkingLCD)
	{
		Serial.println(F("No working LCD devices"));
		fatalError(2); // this never returns
	}
		

	for(int n = 0; n < NumLcd; n++)
	{
	char buf[16];

		if(!(isWorkingLCD(n)))
			continue; //skip over non working LCDs

		//showLCDconfig(Serial, lcd[n]);

		/*
		 * Label the display with its instance number
		 * i2c address and config info on 2nd line
		 */
		lcd[n].clear();
		lcd[n].setCursor(0, 0);
		lcd[n].print(F("LCD:"));
		lcd[n].print(n);

		if(nopullups)
		{
			lcd[n].setCursor(5,0);
			lcd[n].print(F(" NoPullups"));
		}


		lcd[n].setCursor(0, 1);
		lcd[n].print(F("0x"));
		lcd[n].print(lcd[n].getProp(hd44780_I2Cexp::Prop_addr), HEX);
		lcd[n].print(',');
		lcd[n].print(lcdConfigStr(buf, lcd[n]));

	}
	Serial.println(F("Each working display should have its backlight on"));
	Serial.println(F("and be displaying its #, address, and config information"));
	Serial.println(F("If all pixels are on, or no pixels are showing, but backlight is on, try adjusting contrast pot"));
	Serial.println(F("If backlight is off, wait for next test"));
	delay(10000);
	Serial.println(hline);

	Serial.println(F("Blinking backlight test: to verify BL level autodetection"));
	Serial.println(F("If backlight is mostly off but"));
	Serial.println(F("you briefly see \"BL Off\" on display with backlight on,"));
	Serial.println(F("then the library autodetected incorrect BL level"));
	Serial.println(F("and the library cannot autoconfigure the device"));
	delay(2000);

	// blink display backlight 3 times
	// print "BL Off" on TOP line when display is off
	// A SYDZ board will light up when the display backlight is turned off
	//

	for(int i = 0; i < 3; i++)
	{
		// turn off backlight and print "BL Off" on all displays
		// i2c boards like SYDZ will turn on backlight as library
		// cann't properly determine backlight level on that backpack
		for(int n = 0; n < NumLcd; n++)
		{
			lcd[n].noBacklight();
			lcd[n].setCursor(5, 0);
			lcd[n].print(F("  BL Off  "));
		}
		delay(2000);

		// erase "BL Off" text from on all displays
		for(int n = 0; n < NumLcd; n++)
		{
			lcd[n].setCursor(5, 0);
			if(nopullups)
			{
				lcd[n].print(F(" NoPullups"));
			}
			else
			{
				lcd[n].print(F("          "));
			}
		}
 		// wait some time for pixels to turn off
		// before turning backlights back on.
		// (liqudcrystal is actually really slow at transitions)
		delay(250);
		for(int n = 0; n < NumLcd; n++)
		{
			lcd[n].backlight();
		}
		delay(500);
		
	}
	Serial.println(hline);

	// relabel all displays with their i2c address and pullup status
	// and erase 2nd line on all displays
	for(int n = 0; n < NumLcd; n++)
	{
		if(!isWorkingLCD(n))
			continue;
		lcd[n].setCursor(0,0);
		lcd[n].print(F("LCD:"));
		lcd[n].print(n);
		lcd[n].print(F(" (0x"));
		lcd[n].print(lcd[n].getProp(hd44780_I2Cexp::Prop_addr), HEX);
		lcd[n].print(')');

		if(nopullups)
		{
			lcd[n].print(F(" *P"));
		}

		lcd[n].setCursor(0, 1);
		for(int c= 0; c< LCD_COLS; c++)
			lcd[n].write(' ');
	}

	Serial.println(F("Displaying 'uptime' on all displays"));
	Serial.println(hline);
}
 
void loop()
{
static unsigned long lastsecs = -1; // pre-initialize with non zero value
unsigned long secs;

	secs = millis() / 1000;

	// see if 1 second has passed
	// so the display is only updated once per second
	if(secs != lastsecs)
	{
		lastsecs = secs; // keep track of last seconds

		//  write the 'uptime' to each working display
		for(int n = 0; n < NumLcd; n++)
		{
			if(!isWorkingLCD(n))
				continue; // skip over non working displays
			// set the cursor to column 0, line 1
			// (note: line 1 is the second row, counting begins with 0):
			if(lcd[n].setCursor(0, 1))
			{
				clrWorkingLCD(n); // mark display as no longer working
				// output uptime and error message to serial port
				PrintUpTime(Serial, secs);
				Serial.print(F(" - Error on Display: "));
				Serial.println(n);
			}
			else
			{
				// print uptime on lcd device: (time since last reset)
				PrintUpTime(lcd[n], secs);
			}
			if(!anyWorkingLCD)
			{
				Serial.println(hstar);
				Serial.println(F("No working LCD devices"));
				PrintUpTime(Serial, secs);
				Serial.print(" - Fatal error: ");
				Serial.println(2);
				fatalError(2); // this never returns
			}
		}
	}
}

// PrintUpTime(outdev, secs) - print uptime in HH:MM:SS format
// outdev - the device to send output
//   secs - the total number of seconds uptime
void PrintUpTime(Print &outdev, unsigned long secs)
{
unsigned int hr, mins, sec;

	// convert total seconds to hours, mins, seconds
	mins =  secs / 60;	// how many total minutes
	hr = mins / 60;		// how many total hours
	mins = mins % 60;	// how many minutes within the hour
	sec = secs % 60;	// how many seconds within the minute
		

	// print uptime in HH:MM:SS format
	// Print class does not support fixed width formatting
	// so insert a zero if number smaller than 10
	if(hr < 10)
		outdev.write('0');
	outdev.print((int)hr);
	outdev.write(':');
	if(mins < 10)
		outdev.write('0');
	outdev.print((int)mins);
	outdev.write(':');
	if(sec < 10)
		outdev.write('0');
	outdev.print((int)sec);
}



// printDigitalPin(outdev, pin) - print digital pin #
// outdev - the device to send output
//    pin - pin number
//
// This function will also print the digital pin number "symbol name" as
// it can be different from the naked constant.
// So far the only core where this happens is the ESP8266.
// It is messy and ugly and won't be 100% accurate at
// detecting the use of Dn or Pn mappings used by the ESP8266 variants.
// For ESP8266 modules it will print GPIO# and if it detects a variant
// that uses Dn or Pn mapping will print the associated symbol.
// If more than a single symbol is used for the pin value, it will print
// all symbols associated with the pin value.
// 
// Details:
//
// ESP8266 core does not use naked pin#s as Arduino pin numbers like other
// cores.
// in the ESP8266 core, naked constants are bit numbers in the GPIO output
// port register.  While this makes things MUCH faster, This can make things
// confusing since some variants (notably WeMos D1 and NodeMCU) decided to include
// Dn defines to do pin to bit mapping.
// This means that using pin N is the not same as using Dn with those
// variants.
// i.e. D5 may not be the same pin as 5
// 
// This makes things very difficult as there is no direct way to map the
// naked constant values back to the Dn symbol names for those particular
// variants. This is very unfortunate since boards that use Dn pin mapping
// print Dn numbers on them rather than the GPIO bit# and for most pins,
// Dn is the not the same as N.
// This mismatched mapping is usually the case for Pn mapping variants for the i2c
// pins.
// Most variants use GPIO 4 for SDA and GPIO 5 for SCL and do not use or/
// include Dn pin mapping defines.
// The Wemos D1 and NodeMCU board variants have really made a mess of things.
// Those variants include Dn pin mappings and D4 is sometimes 4 and D5 is
// not 5.
// And some boards have a D14 and D15 labels on the I2C SDA and SCL header
// pins that map to GPIO 4 and GPIO 5. On those boards D14 is the same as
// D4 and D15 is the same as D3
//
void printDigitalPin(Print &outdev, int pin)
{
	// On all cores, print the pin value
	outdev.print(pin);

#if defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
	// print GPIO# for all ESP8266 & ESP32 boards since those cores uses GPIO bit numbers
	// as the pin number.
	outdev.print(F(" (GPIO"));
	outdev.print(pin);
	outdev.print(')');
#endif

#if !defined(I2CEXPDIAG_CFG_NO_DECODE_ESPXXXXPINS)

	// this next part is ugly.
	// It is trying to convert the GPIO bit number back to a pin define name
	// It is dependent on the boards.txt file which is subject to change
	// it also makes certain assumptions about the symbols in the variant
	// pins_arduino.h file
	// based on the board define in the boards.txt file all of which is
	// also subject to change.

	// In older ESP8266 cores, two different WeMos boards 
	// "WeMos D1 R1" and "WeMos D1 R1 & mini" use the same board define
	// ESP8266_WEMOS_D1MINI so there is no way to tell them apart.
	// Bug filed:  https://github.com/esp8266/Arduino/issues/4303
	// and fixed. New define is ESP8266_WEMOS_D1R1
	// When compiled on older ESP8266 cores, this code will end up only look for
	// Dn pins 0-8 on D1R1 boards.
	// luckily i2c pins fall inside of this range on these variants.
	

// These boards have D0 to D8 symbols
#if defined(ARDUINO_ESP8266_WEMOS_D1MINI) || defined(ARDUINO_ESP8266_WEMOS_D1MINIPRO) || \
	defined(ARDUINO_ESP8266_WEMOS_D1MINILITE) || defined(ARDUINO_ESP8266_NODEMCU) || \
	defined(ARDUINO_ESP8266_WEMOS_D1R1)

	if(pin == D0)
		outdev.print(F(" D0"));
	if(pin == D1)
		outdev.print(F(" D1"));
	if(pin == D2)
		outdev.print(F(" D2"));
	if(pin == D3)
		outdev.print(F(" D3"));
	if(pin == D4)
		outdev.print(F(" D4"));
	if(pin == D5)
		outdev.print(F(" D5"));
	if(pin == D6)
		outdev.print(F(" D6"));
	if(pin == D7)
		outdev.print(F(" D7"));
	if(pin == D8)
		outdev.print(F(" D8"));
#endif

// boards that support D9 and D10
// Note that older ESP8266 cores don't define ARDUINO_ESP8266_WEMOS_D1R1
// so boards using older cores won't see these pins.
#if defined(ARDUINO_ESP8266_NODEMCU) || defined(ARDUINO_ESP8266_WEMOS_D1R1)
	if(pin == D9)
		outdev.print(F(" D9"));
	if(pin == D10)
		outdev.print(F(" D10"));
#endif

// Boards that support D10 to D15
// Note that older ESP8266 cores don't define ARDUINO_ESP8266_WEMOS_D1R1
// so those boards won't see these pins.
#if defined(ARDUINO_ESP8266_WEMOS_D1R1)
	if(pin == D11)
		outdev.print(F(" D11"));
	if(pin == D12)
		outdev.print(F(" D12"));
	if(pin == D13)
		outdev.print(F(" D13"));
	if(pin == D14)
		outdev.print(F(" D14"));
	if(pin == D15)
		outdev.print(F(" D15"));
#endif

// This define is for the DigiStump Oak board that stupidly uses Pn names
#if defined(ARDUINO_ESP8266_OAK)
	if(pin == P0)
		outdev.print(F(" P0"));
	if(pin == P1)
		outdev.print(F(" P1"));
	if(pin == P2)
		outdev.print(F(" P2"));
	if(pin == P3)
		outdev.print(F(" P3"));
	if(pin == P4)
		outdev.print(F(" P4"));
	if(pin == P5)
		outdev.print(F(" P5"));
	if(pin == P6)
		outdev.print(F(" P6"));
	if(pin == P7)
		outdev.print(F(" P7"));
	if(pin == P8)
		outdev.print(F(" P8"));
	if(pin == P9)
		outdev.print(F(" P9"));
	if(pin == P10)
		outdev.print(F(" P10"));
	if(pin == P10)
		outdev.print(F(" P10"));
#endif

#endif // I2CEXPDIAG_CFG_NO_DECODE_ESPXXXXPINS

// Special ugly code for Roger's maple/stm32duino cores
// They don't define SDA or SCL at all but code
// above gets them from SoftWire
// Since they are #define symbols that reverence other PXX symbols
// we will print define string instead of its value
#if defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
	if(pin == SDA)
		outdev.print(F(" " define2str(SDA))); // print define string not its final value
	if(pin == SCL)
		outdev.print(F(" " define2str(SCL))); // print define string not its final value
#endif

//////////////////////////////////////////////////////////////////////////
// code for STM32 platform defines
#if defined(ARDUINO_ARCH_STM32)
#if defined(PA0)
	if(pin == PA0)
		outdev.print(F(" PA0"));
#endif
#if defined(PA1)
	if(pin == PA1)
		outdev.print(F(" PA1"));
#endif
#if defined(PA2)
	if(pin == PA2)
		outdev.print(F(" PA2"));
#endif
#if defined(PA3)
	if(pin == PA3)
		outdev.print(F(" PA3"));
#endif
#if defined(PA4)
	if(pin == PA4)
		outdev.print(F(" PA4"));
#endif
#if defined(PA5)
	if(pin == PA5)
		outdev.print(F(" PA5"));
#endif
#if defined(PA6)
	if(pin == PA6)
		outdev.print(F(" PA6"));
#endif
#if defined(PA7)
	if(pin == PA7)
		outdev.print(F(" PA7"));
#endif
#if defined(PA8)
	if(pin == PA8)
		outdev.print(F(" PA8"));
#endif
#if defined(PA9)
	if(pin == PA9)
		outdev.print(F(" PA9"));
#endif
#if defined(PA10)
	if(pin == PA10)
		outdev.print(F(" PA10"));
#endif
#if defined(PA11)
	if(pin == PA11)
		outdev.print(F(" PA11"));
#endif
#if defined(PA12)
	if(pin == PA12)
		outdev.print(F(" PA12"));
#endif
#if defined(PA13)
	if(pin == PA13)
		outdev.print(F(" PA13"));
#endif
#if defined(PA14)
	if(pin == PA14)
		outdev.print(F(" PA14"));
#endif
#if defined(PA15)
	if(pin == PA15)
		outdev.print(F(" PA15"));
#endif
#if defined(PB0)
	if(pin == PB0)
		outdev.print(F(" PB0"));
#endif
#if defined(PB1)
	if(pin == PB1)
		outdev.print(F(" PB1"));
#endif
#if defined(PB2)
	if(pin == PB2)
		outdev.print(F(" PB2"));
#endif
#if defined(PB3)
	if(pin == PB3)
		outdev.print(F(" PB3"));
#endif
#if defined(PB4)
	if(pin == PB4)
		outdev.print(F(" PB4"));
#endif
#if defined(PB5)
	if(pin == PB5)
		outdev.print(F(" PB5"));
#endif
#if defined(PB6)
	if(pin == PB6)
		outdev.print(F(" PB6"));
#endif
#if defined(PB7)
	if(pin == PB7)
		outdev.print(F(" PB7"));
#endif
#if defined(PB8)
	if(pin == PB8)
		outdev.print(F(" PB8"));
#endif
#if defined(PB9)
	if(pin == PB9)
		outdev.print(F(" PB9"));
#endif
#if defined(PB10)
	if(pin == PB10)
		outdev.print(F(" PB10"));
#endif
#if defined(PB11)
	if(pin == PB11)
		outdev.print(F(" PB11"));
#endif
#if defined(PB12)
	if(pin == PB12)
		outdev.print(F(" PB12"));
#endif
#if defined(PB13)
	if(pin == PB13)
		outdev.print(F(" PB13"));
#endif
#if defined(PB14)
	if(pin == PB14)
		outdev.print(F(" PB14"));
#endif
#if defined(PB15)
	if(pin == PB15)
		outdev.print(F(" PB15"));
#endif
#if defined(PC0)
	if(pin == PC0)
		outdev.print(F(" PC0"));
#endif
#if defined(PC1)
	if(pin == PC1)
		outdev.print(F(" PC1"));
#endif
#if defined(PC2)
	if(pin == PC2)
		outdev.print(F(" PC2"));
#endif
#if defined(PC3)
	if(pin == PC3)
		outdev.print(F(" PC3"));
#endif
#if defined(PC4)
	if(pin == PC4)
		outdev.print(F(" PC4"));
#endif
#if defined(PC5)
	if(pin == PC5)
		outdev.print(F(" PC5"));
#endif
#if defined(PC6)
	if(pin == PC6)
		outdev.print(F(" PC6"));
#endif
#if defined(PC7)
	if(pin == PC7)
		outdev.print(F(" PC7"));
#endif
#if defined(PC8)
	if(pin == PC8)
		outdev.print(F(" PC8"));
#endif
#if defined(PC9)
	if(pin == PC9)
		outdev.print(F(" PC9"));
#endif
#if defined(PC10)
	if(pin == PC10)
		outdev.print(F(" PC10"));
#endif
#if defined(PC11)
	if(pin == PC11)
		outdev.print(F(" PC11"));
#endif
#if defined(PC12)
	if(pin == PC12)
		outdev.print(F(" PC12"));
#endif
#if defined(PC13)
	if(pin == PC13)
		outdev.print(F(" PC13"));
#endif
#if defined(PC14)
	if(pin == PC14)
		outdev.print(F(" PC14"));
#endif
#if defined(PC15)
	if(pin == PC15)
		outdev.print(F(" PC15"));
#endif

#endif // ARDUINO_ARCH_STM32
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// print the analog pin if it matches the pin #
#if defined(A0) || defined(PIN_A0)
	if(pin == A0)
		outdev.print(F(" A0"));
#endif
#if defined(A1) || defined(PIN_A1)
	if(pin == A1)
		outdev.print(F(" A1"));
#endif
#if defined(A2) || defined(PIN_A2)
	if(pin == A2)
		outdev.print(F(" A2"));
#endif
#if defined(A3) || defined(PIN_A3)
	if(pin == A3)
		outdev.print(F(" A3"));
#endif
#if defined(A4) || defined(PIN_A4)
	if(pin == A4)
		outdev.print(F(" A4"));
#endif
#if defined(A5) || defined(PIN_A5)
	if(pin == A5)
		outdev.print(F(" A5"));
#endif
#if defined(A6) || defined(PIN_A6)
	if(pin == A6)
		outdev.print(F(" A6"));
#endif
#if defined(A7) || defined(PIN_A7)
	if(pin == A7)
		outdev.print(F(" A7"));
#endif
#if defined(A8) || defined(PIN_A8)
	if(pin == A8)
		outdev.print(F(" A8"));
#endif
#if defined(A9) || defined(PIN_A9)
	if(pin == A9)
		outdev.print(F(" A9"));
#endif
#if defined(A10) || defined(PIN_A10)
	if(pin == A10)
		outdev.print(F(" A10"));
#endif
#if defined(A11) || defined(PIN_A11)
	if(pin == A11)
		outdev.print(F(" A11"));
#endif
#if defined(A12) || defined(PIN_A12)
	if(pin == A12)
		outdev.print(F(" A12"));
#endif
#if defined(A13) || defined(PIN_A13)
	if(pin == A13)
		outdev.print(F(" A13"));
#endif
#if defined(A14) || defined(PIN_A14)
	if(pin == A14)
		outdev.print(F(" A14"));
#endif
#if defined(A15) || defined(PIN_A15)
	if(pin == A15)
		outdev.print(F(" A15"));
#endif
//////////////////////////////////////////////////////////////////////////

	return;
}

/*
 * dump everthying we know about the system environment
 */
void showSystemConfig(void)
{

#ifdef ARDUINO
	Serial.print(F("Reported Arduino Revision: "));
#if ARDUINO > 158 // ARDUINO rev format changed after 1.5.8 to #.##.## (breaks after 3.x.x for 16 int bit calc)
	Serial.print(ARDUINO/10000);
	Serial.print('.');
	Serial.print((ARDUINO%10000)/100);
	Serial.print('.');
	Serial.println((ARDUINO%10000)%100);
#elif ARDUINO >= 100 // 1.0.0 to 1.5.8 uses rev format #.#.#
	Serial.print(ARDUINO/100);
	Serial.print('.');
	Serial.print((ARDUINO%100)/10);
	Serial.print('.');
	Serial.println((ARDUINO%100)%10);
#else
	Serial.print(F("0."));
	Serial.println(define2str(ARDUINO)); // print the raw string vs as a number
#endif

#endif

// only print board name if platform hands it to us
#if defined(ARDUINO_BOARD) || defined(BOARD_NAME)
	Serial.print(F("Arduino Board: "));
#if defined(ARDUINO_BOARD)
	Serial.print(ARDUINO_BOARD);
#if defined(ARDUINO_VARIANT)
	Serial.print(F(" Arduino Variant: "));
	Serial.print(ARDUINO_VARIANT);
#endif
#elif defined(BOARD_NAME)
	Serial.print(BOARD_NAME);
#else
	Serial.print(F("unknown"));
#endif
	Serial.println();
#endif

#if defined(__AVR__)
	Serial.print(F("CPU ARCH: AVR - "));
#elif defined(__arm__)
	Serial.print(F("CPU ARCH: arm - "));
#elif defined(__PIC32MX__)
	Serial.print(F("CPU ARCH: pic32 - "));
#elif defined(ARDUINO_ARCH_ESP8266)
	Serial.print(F("CPU ARCH: ESP8266 - "));
#elif defined(ARDUINO_ARCH_ESP32)
	Serial.print(F("CPU ARCH: ESP32 - "));
#elif defined(ARDUINO_ARCH_STM32)
	Serial.print(F("CPU ARCH: STM32 - "));
#endif

	Serial.print(F("F_CPU: "));
// just in case the core does not define this
// (like cores in RogerClark's STM32 platform)
#ifdef F_CPU
	Serial.println(F_CPU);
#else
	Serial.println("undefined");
#endif

	Serial.println(hline);

	Serial.print(F("SDA digital pin: "));
	printDigitalPin(Serial, SDA);
	Serial.println();
	Serial.print(F("SCL digital pin: "));
	printDigitalPin(Serial, SCL);
	Serial.println();

}



/*
 * Test for external pullup on pin
 * returns less than zero if pin appears to be driven low
 * returns zero if pin appears to have external pullup on it
 * returns greather than zero if pin appears to NOT have a pullup on it
 *
 * i.e. zero means pullup exists and non zero means there is no pullup.
 * and positive/negative indicates more information
 */
int pullupOnPin(uint8_t pin)
{
int status;

	// test to see if pin is pulled/stuck low
	// Arduino 1.0 didn't support INPUT_PULLUP
	// it was added in the next release 1.0.1
	// the code used to check for INPUT_PULLUP macro
	// and work around it
	// but many cores didn't use a macro so this check caused issues
	// now the code will simply fail to compile ungracefully if the symbol doesn't exist

	pinMode(pin, INPUT_PULLUP);
	delay(20);
	if(digitalRead(pin) == LOW)
	{
		status = -1; // pin appears to be driven low
		goto leave;
	}

	// test to see if high is an external pullup
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(20);
	pinMode(pin,INPUT);
	delayMicroseconds(10);
	if(digitalRead(pin) == HIGH)
		status = 0; // pin appears to have external pullup
	else
		status = 1; // pin appears to NOT have an external pullup

leave:
	pinMode(pin,INPUT); // ensure pin is left in input mode
	return(status);

}
/*
 * test of two pins are shorted together
 * returns > zero if pins are shorted
 * returns zero if pins are not shorted
 * returns < zero if indeterminate
 */
int pinsShorted(uint8_t p1, uint8_t p2)
{
int rval = 0; // assume not shorted

	pinMode(p1, INPUT_PULLUP);
	pinMode(p2, INPUT_PULLUP);
	delay(150); // this needs quite a while for chipkit/pic32 to let signals rise up

	// check to see if both pins are high
	if((digitalRead(p1) != HIGH) || (digitalRead(p2) != HIGH))
		return(-1);  // can't determine if pins are shorted

	pinMode(p1, OUTPUT);
	digitalWrite(p1, LOW);
	delay(5);

	if(digitalRead(p2) == LOW)
		rval = 1;

	// put back into input state
	pinMode(p1, INPUT);
	pinMode(p2, INPUT);

	return(rval);
}

/*
 * Test for external pullups on i2c signals
 *
 * returns 0 if both pullups appear ok
 * returns positive if no pullup exist on either pin (soft error on AVR)
 * returns negative if I2C cannot function
 * 	-1 if either pin is driven low
 * 	-2 if pins are shorted together
 */
int i2cpulluptest()
{
int rval = 0;
int s;

    Serial.print(F("Checking for required external I2C pull-up on SDA - "));
    if ( (s = pullupOnPin(SDA)) )
	{
		if(s > 0)
		{
			rval = 1;
        	Serial.println(F("NO"));
		}
		else
		{
        	Serial.println(F("STUCK LOW"));
			rval = -1;
		}
	}
	else
	{
    	Serial.println(F("YES"));
	}
    Serial.print(F("Checking for required external I2C pull-up on SCL - "));
    if ( (s = pullupOnPin(SCL)) )
	{
		if(s > 0)
		{
			if(!rval)
				rval = 1;
        	Serial.println(F("NO"));
		}
		else
		{
        	Serial.println(F("STUCK LOW"));
			rval = -1;
		}
	}
	else
	{
    	Serial.println(F("YES"));
	}

	// check for short between SDA and SCL if no pins stuck low
	if(rval >= 0)
	{
   		Serial.print(F("Checking for I2C pins shorted together - "));
		s = pinsShorted(SDA, SCL);
		if(!s)
		{
			Serial.println(F("Not Shorted"));
		}
		else if(s > 0)
		{
			Serial.println(F("Shorted"));
			rval = -2;
		}
		else
		{
			Serial.println(F("Undetermined"));
		}
	}
	
	if(rval)
	{
        Serial.println(hstar);
		if(rval  > 0)
		{
        	Serial.println(F("WARNING: I2C requires external pullups for proper operation"));
			Serial.println(F("It may appear to work without them, but may be unreliable and slower"));
			Serial.println(F("Do not be surprised if it fails to work correctly"));
	        Serial.println(F("Install external pullup resistors to ensure proper I2C operation"));
		}
		else
		{
			if(rval == -1)
        		Serial.println(F("ERROR: SDA or SCL stuck pin"));
			else
        		Serial.println(F("ERROR: SDA and SCL shorted together"));
		}
        Serial.println(hstar);
    }

	return(rval);
}


#ifdef LATER
// i2cexpPinsTest - test for shorts on expander port pins
// FIXME currenly only works with PCF8574
// Also, the pin with the backlight can fail as the backlight
// circuitry can pull the pin low on active HIGH backlight circuits.
// so not sure that test can ever be made to work as desired.
int i2cexpPinsTest(uint8_t addr)
{
int bitdiffs = 0;
int rval;
uint8_t wdata, rdata;

	// to test for shorted/broken/stuck pin:
	// - set all pins but one to outputs and low
	// - set single pin an input with pullup enabled
	// - read back the port register. (all 8 pins)
	//
	// If the input pin is not a high there is an issue
	// note:
	// r/w will be low even if En goes high, so the port should read back
	// what was written.
	// An active high backlight curcuit will create false positives since
	// the backlight transistor will pull the input pin low.
	//

	for(int pin = 0; pin < 8; pin++)
	{
		wdata = (1 << pin); // convert pin# to bit position mask

        Wire.beginTransmission(addr);
        Wire.write(wdata);
        Wire.endTransmission();
        Wire.requestFrom((int)addr, 1);
        rval = Wire.read();
		rdata = uint8_t (rval);
        Wire.endTransmission();
        if (rdata != wdata)
		{
			bitdiffs |= (rdata ^ wdata);
		}
	}
	if(bitdiffs)
	{
		Serial.print(F("i2cExpander port error: "));
		Serial.print(F("Pins/Bits:"));
		for(uint8_t bit=0; bit < 8; bit++)
		{
			if(bitdiffs & (1<< bit))
			{
				Serial.print(' ');
				Serial.print(bit);
			}
		}
		Serial.println();
	}
    return bitdiffs;
}
#endif



/*
 * Returns number of i2c devices found
 */

int showI2Cdevices(void)
{
uint8_t error, address;
int devcount = 0;
 
	Serial.println(F("Scanning i2c bus for devices.."));
 
	/*
 	 * Note: 
	 * 	Addresses below 8 are reserved for special use
	 * 	Addresses above 0x77 are reserved for special use
	 */
	for(address = 8; address <= 0x77; address++ )
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
		if (error == 0)
		{
			devcount++;
			Serial.print(F(" i2c device found at address 0x"));
			if (address<16)
				Serial.print('0');
			Serial.println(address,HEX);
		}
		else if (error==4)
		{
			Serial.print(F(" Unknown error at address 0x"));
			if (address<16)
				Serial.print('0');
			Serial.println(address,HEX);
		}   
		// chipkit stuff screws up if you do beginTransmission() too fast
		// after an endTransmission()
		// below 20us will cause it to fail
		// ESP8286 needs to make sure WDT doesn't fire.
		// normally yield() would be used to prevent WDT, but yield() doesn't
		// exist on older IDEs so we use delay(1) which calls yield()
		// The delay(1) is overkill for chipkit and not needed for other chips,
		// but it won't hurt and keeps things compatible with older IDEs.
		
		delay(1);

	}

	Serial.print(F("Total I2C devices found: "));
	Serial.println(devcount);

	return(devcount);
}

/*
 * lcdw1est()
 * Walk a bit through a single memory location to see if
 * basic reads/writes work.
 */

uint8_t lcdw1test(hd44780 &lcd, uint8_t addr)
{
uint8_t errors = 0;
int rdata;

	for(uint8_t pat = 1;  pat != 0; pat <<= 1)
	{
		lcd.setCursor(addr,0);
		lcd.write(pat);
		lcd.setCursor(addr,0);
		rdata = lcd.read();

		if(rdata < 0)
		{
			Serial.print(F(" Read Error after writing "));
			Serial.println((unsigned int)pat, HEX);
			errors++;
			delay(1); // easy way to allow some cores to call yield()
		}
		else if((rdata != pat))
		{
			if(!errors)
				Serial.println();
			Serial.print(F("\tCompare error: addr: "));
			Serial.print(addr, HEX);
			Serial.print(F(" read "));
			Serial.print((unsigned int)rdata, HEX);
			Serial.print(F(" != wrote "));
			Serial.println((unsigned int)pat, HEX);

			errors++;
			delay(1); // easy way to allow some cores to call yield()
		}
	}
	return(errors);
}

/*
 * lcdAddrLineTest() - address line test
 * writes the memory addres to each locaton and verifies contents.
 *
 * This will verify that all memory is being addressed correctly.
 */
int lcdAddrLineTest(hd44780 &lcd, uint8_t saddr, uint8_t eaddr)
{
int errors = 0;
int rdata;

	// first write to all the memory locations
	for(uint8_t addr = saddr;  addr <= eaddr; addr++)
	{
		// ugly hack, skip over addresses of codes for  <CR> and <NL> 
		// since write() currently drops <cr> and <nl> characters
		if(addr == '\n' || addr == '\r')
			continue;
		lcd.setCursor(addr,0);
		if(lcd.write(addr) != 1)
		{
			if(!errors)
				Serial.println();
			Serial.print(F("\tRead Error addr: "));
			Serial.println((unsigned int)addr, HEX);
			errors++;
			delay(1); // easy way to allow some cores to call yield()
		}
	}

	// now go back and verify that each memory location has the
	// proper contents.
	for(uint8_t addr = saddr;  addr <= eaddr; addr++)
	{
		// ugly hack, skip over addresses of codes for <CR> and <NL> 
		// since write() currently drops <cr> and <nl> characters
		if(addr == '\n' || addr == '\r')
			continue;

		lcd.setCursor(addr,0);
		rdata = lcd.read();

		if(rdata < 0)
		{
			if(!errors)
				Serial.println();
			Serial.print(F("\tRead Error addr: "));
			Serial.println((unsigned int)addr, HEX);
			errors++;
			delay(1); // easy way to allow some cores to call yield()
		}
		else if((rdata != addr))
		{
			if(!errors)
				Serial.println();
			Serial.print(F("\tCompare error: addr: "));
			Serial.print(addr, HEX);
			Serial.print(F(" read "));
			Serial.print((unsigned int)rdata, HEX);
			Serial.print(F(" != wrote "));
			Serial.println((unsigned int)addr, HEX);

			errors++;
			delay(1); // easy way to allow some cores to call yield()
		}
	}
	return(errors);
}


/*
 * Walk incrementing values through incrementing memory locations.
 *
 * A value starting at sval ending at eval will be walked through memory.
 * The starting address will be filled in with sval and the value will
 * incremented through all locations to be tested. Values are written through
 * incrementing addresses.
 *
 * All the values are read and compared to expected values.
 *
 * Then process starts over again by incrementing the starting value.
 * This repeats until the starting value reaches the ending value.
 *
 * Each memory location will tested with an incrementing value
 * eval-sval+1 times.
 *
 * If sval is 0 and eval is 255,
 * every memory location will be tested for every value.
 *
 */

int lcdDDRAMtest(hd44780 &lcd, uint8_t saddr, uint8_t eaddr,
	 	uint8_t sval, uint8_t eval)
{
uint8_t addr;
int data;
int rdata;
uint8_t errors = 0;
uint8_t lval = sval;

	/*
	 * perform each interation of test across memory with
	 * an incrementing pattern
	 * starting at sval and bumping sval each iteration.
	 */
	do
	{
		/*
		 * write sequentially through all addresses
		 */

		data = lval;

		// use serCursor to set initial DDRAM address
		// writes will bump it
		lcd.setCursor(saddr, 0);
		for(addr = saddr; addr <= eaddr; addr++)
		{

			lcd.write((uint8_t)data);

			if(++data > eval)
				data = sval;
		}

		/*
		 * Now go back and verify the pages
		 */

		data = lval;

		// use serCursor to set initial DDRAM address
		// reads will bump it
		lcd.setCursor(saddr, 0);
		for(addr = saddr; addr <= eaddr; addr++)
		{
			rdata = lcd.read();

			if(rdata < 0)
			{
				Serial.print(F(" Read Error, addr: "));
				Serial.print((unsigned int)addr, HEX);
				Serial.print(F(" sval: "));
				Serial.print(sval);
				Serial.print(F(" expected data: "));
				Serial.print(data);
				Serial.println();

				if(++errors > MAX_ERRORS)
					return(errors);
		
			} else if(data != rdata)
			{
				Serial.print(F(" Verify error: ("));
				Serial.print((unsigned int) addr);
				Serial.print(F(") read "));
				Serial.print((unsigned int)rdata, HEX);
				Serial.print(F(" != wrote "));
				Serial.print((unsigned int)data, HEX);
				Serial.println();

				if(++errors > MAX_ERRORS)
					return(errors);
			}
			if(++data > eval)
				data = sval;
		}
	} while(lval++ != eval);

	return(errors);
}

// create a LCD configuration string
// requires being handed a 16 byte buffer to hold the string
// returns the original buffer pointer for convenience.
char * lcdConfigStr(char *str, hd44780_I2Cexp &lcd)
{
int rv;
char *p = str;
#if 1
	switch(lcd.getProp(hd44780_I2Cexp::Prop_expType))
	{
		case I2Cexp_PCF8574:
			*p++ = 'P';
			break;
		case I2Cexp_MCP23008:
			*p++ = 'M';
			break;
		default:
			*p++ = 'U';
	}
#else
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_expType + '0';
#endif
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_rs) + '0';

	// r/w support may or may not be enabled.
	rv = lcd.getProp(hd44780_I2Cexp::Prop_rw);
	if((unsigned int) rv <= 7) // check if r/w is supported
		*p++ = rv + '0';

	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_en) + '0';
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_d4) + '0';
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_d5) + '0';
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_d6) + '0';
	*p++ = lcd.getProp(hd44780_I2Cexp::Prop_d7) + '0';
	rv = lcd.getProp(hd44780_I2Cexp::Prop_bl);
	if((unsigned int) rv <= 7) // check if bl control is supported
	{
		*p++ = rv + '0';
#if 1
		if(lcd.getProp(hd44780_I2Cexp::Prop_blLevel) == HIGH)
			*p++ = 'H';
		else
			*p++ = 'L';
#else
		if(lcd.getProp(hd44780_I2Cexp::Prop_blLevel) == HIGH)
			*p++ = '1';
		else
			*p++ = '0';
#endif
	}

	*p = 0; // terminate string

	return(str);
}

// fatalError() - loop & blink an error code
void fatalError(int ecode)
{
	hd44780::fatalError(ecode); // does not return
}

void waitinput(const char *prompt)
{
	if(prompt)
		Serial.print(prompt);
	else
		Serial.print(F("<Press <ENTER> or click [Send] to Continue>"));

	while(Serial.available())
		Serial.read(); // swallow all input

	while(!Serial.available()){} // wait on serial input

	Serial.println();
}
