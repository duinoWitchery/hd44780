static const int dummyvar = 0; 
// vi:ts=4
// ----------------------------------------------------------------------------
// I2Cexpdiag - i2c LCD i/o expander backpack diagnostic tool
// Created by Bill Perry 2016-06-17
// Copyright 2016 - Under GPL v3
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
// 2. Hookup up i2c i/o expander backpack based LCD devices.
//	Currently supports PCF8574 or MCP32008 devices
//	(can test more than one LCD at a time)
//
// 3. compile and upload sketch
//	note:
//		if you have a slow display you may need to modify the
//		execution times to be longer than the hd44780 defaults.
//		Scroll down to modify the defines: LCD_CHEXECTIME, LCD_INSEXECTIME
//
// 4. Connect to the board using the serial monitor; set the baud rate to 9600
//	While using the serial monitor is not required additional information
//	will be sent to the serial monitor.
//	NOTE:
//		When using recent IDEs, it is useful to connect to the serial
//		monitor *before* you upload the sketch so that the serial monitor
//		is reconnected as soon as the sketch is uploaded.
//		This is particularly helpful on boards that use a USB virtual
//		serial port like leonardo.
// -----------------------------------------------------------------------
// Expected behavior
// -----------------------------------------------------------------------
// Highlights of sketch behavior
//	- display information about the enviroment on the serial monitor.
//	- probe the i2c bus to check for external pullup resistors 
//	- scan the i2c bus and show all devices found
//	- attempt to initalize all LCD devices detected
//	- display information about each each initialized LCD device
//		this includes i2c address and configuration information
//		and information about missing pullups.
//	- test display memory
//		 LCD expander must be able to control r/w line
//	- perform a backlight blink test
//	- drop into a loop and display the system uptime on each display
//    *P on the display indicates missing external pullups
//
//	If the sketch cannot determine any usable LCD devices 
//	the code will fall into a fatal error and blink out an error code:
//	Error codes:
//		[1] no i2c devices found on i2c bus
//		[2] no LCD devices were successfully initalized
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
// 2016.08.07 bperrybap  - added lcd memory tests
// 2016.07.27 bperrybap  - added defines for setting execution times
// 2016.06.17 bperrybap  - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// -----------------------------------------------------------------------

#include <Wire.h>
#include <hd44780.h>
// For STUPID versions of gcc that don't hard error on missing header files
#ifndef hd44780_h
#error Missing hd44780.h header file
#endif

#include <hd44780_I2Cexp.h>
#ifndef hd44780_I2Cexp_h
#error Missing hd44780_I2Cexp.h header file
#endif

// ============================================================================
// user configurable options below this point
// ============================================================================

// Uncomment and use this line instead of the one below if you have a SYDZ backpack
//hd44780_I2Cexp lcd[1]={{I2Cexp_ADDR_UNKNOWN, I2Cexp_BOARD_SYDZ}}; // to run on a single SYDZ based backpack
hd44780_I2Cexp lcd[16]; // auto locate & configure up to 16 displays

// All displays will be assumed to be 16x2
// Even if display is larger the sketch should still work correctly
const int LCD_ROWS = 2;
const int LCD_COLS = 16;

// if you have slow displays uncomment these defines
// to override the default execution times.
//  CHEXECTIME is the execution time for clear and home commands
// INSEXECTIME is the execution time for everything else; cmd/data
// times are in Us 
// NOTE: if using, you must enable both
// Although each display can have seperate times, these values will be used
// on all displays.

//#define LCD_CHEXECTIME 2000
//#define LCD_INSEXECTIME 37

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


// convert a define to a string
#define define2str(s) _str(s)
#define _str(...) #__VA_ARGS__

void setup()
{
int nopullups;

	delay(5);	// allow the hardware time settle

	Serial.begin(9600);

#if (ARDUINO > 101)
	do
	{
	// wait on serial port to be ready but timout out after 5 seconds
	// this is for sytems that use virtual serial over USB.
		if(millis() > 5000) // millis starts at 0 after reset
			break;
	} while(!Serial);
#endif

	Serial.println(hstar);
	Serial.println(F("Serial Initialized"));

	Serial.println(hline);
	Serial.println(F("I2CexpDiag - i2c LCD i/o expander backpack diagnostic tool"));
#ifdef HD44780_VERSIONSTR
	Serial.println(hline);
	Serial.print(F("hd44780 lib version: "));
	Serial.println(HD44780_VERSIONSTR);
#endif

	Serial.println(hline);
	showSystemConfig();

	Serial.println(hline);
	nopullups = i2cpulluptest();

	Serial.println(hline);
	Wire.begin();
	if(!showI2Cdevices()) // show all i2c devices on bus
	{
		Serial.println(F("No I2C devices found"));

		fatalError(1); // this never returns
	}
	Serial.println(hline);

	Serial.println(F("Scanning i2c bus for all lcd displays"));

	/*
	 * Locate all the displays by attempting to intialize each one
	 */
	for(NumLcd = 0; NumLcd < (int) (sizeof(lcd)/sizeof(hd44780_I2Cexp)); NumLcd++)
	{
	char buf[16];
		// set custom exectution times if configured
#if defined(LCD_CHEXECTIME) && defined(LCD_INSEXECTIME)
		lcd[NumLcd].setExecTimes(LCD_CHEXECTIME, LCD_INSEXECTIME);
#endif

		// If begin fails, then assume we have no more displays
		if(lcd[NumLcd].begin(LCD_ROWS, LCD_COLS) != 0)
			break;
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
		
	}

	if(!NumLcd)
	{
		Serial.println(F("No LCD devices found"));
		fatalError(2); // this never returns
	}
	Serial.print(F("Total LCD devices found: "));
	Serial.println(NumLcd);
	Serial.println(hline);

	Serial.println(F("LCD Display Memory Test"));
	for(int n = 0; n < NumLcd; n++)
	{
	int errors;

		Serial.print(F("Display: "));
		Serial.println(n);

		// check for r/w control
		// by attempting to read lcd status
		if(lcd[n].status() >= 0)
		{
			Serial.print(F(" Walking 1s data test: "));
			// try a few different locations which also tests addressing
			errors = lcdw1test(lcd[n], 0);
			errors += lcdw1test(lcd[n], 0x40);
			errors += lcdw1test(lcd[n], 0x10);
			errors += lcdw1test(lcd[n], 0x50);
			if(errors)
				Serial.print(FAILED);
			else
				Serial.print(PASSED);

			Serial.println();

			Serial.print(F("    Address line test: "));
			errors = lcdAddrLineTest(lcd[n], 0x00, 0x27); // 1st block of memory
			errors += lcdAddrLineTest(lcd[n], 0x40, 0x67); // 2nd block of memory
			if(errors)
				Serial.print(FAILED);
			else
				Serial.print(PASSED);

		
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
			Serial.print(F(" Quick DDRAM memory test: "));
			errors = lcdDDRAMtest(lcd[n], 0x40, 0x67, '0', '9');

			if(errors)
				Serial.print(FAILED);
			else
				Serial.print(PASSED);
#endif
		
		}
		else
		{
			Serial.print(F(" (R/W control not supported)"));
		}
		Serial.println();
	}
	Serial.println(hline);

	for(int n = 0; n < NumLcd; n++)
	{
	char buf[16];

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
	Serial.println(F("Each display should be displaying its #, address, and config information"));
	Serial.println(F("If display is blank, but backlight is on, try adjusting contrast pot"));
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

		//  write the 'uptime' to each display found
		for(int n = 0; n < NumLcd; n++)
		{
			// set the cursor to column 0, line 1
			// (note: line 1 is the second row, counting begins with 0):
			lcd[n].setCursor(0, 1);
			// print uptime on lcd device: (time since last reset)
			PrintUpTime(lcd[n], secs);
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

	Serial.print(F("F_CPU: "));
	Serial.println(F_CPU);



	Serial.println(hline);
	Serial.print(F("SDA: digital pin: "));
	Serial.println(SDA);

	Serial.print(F("SCL: digital pin: "));
	Serial.println(SCL);
}



/*
 * Test for external pullup on pin
 * returns non zero if pin appears to have external pullup on it
 */
int pullupOnPin(uint8_t pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delay(20);
	pinMode(pin,INPUT);
	delayMicroseconds(10);
	return(digitalRead(pin));
}

/*
 * Test for external pullups on i2c signals
 */
int i2cpulluptest()
{
int nopullups = 0;

    Serial.print(F("Checking for required external I2C pull-up on SDA - "));
    if (!pullupOnPin(SDA))
	{
		nopullups++;
        Serial.println(F("NO"));
	}
	else
	{
    	Serial.println(F("YES"));
	}
    Serial.print(F("Checking for required external I2C pull-up on SCL - "));
    if (!pullupOnPin(SCL))
	{
		nopullups++;
        Serial.println(F("NO"));
	}
	else
	{
    	Serial.println(F("YES"));
	}
	if(nopullups)
	{
        Serial.println(hstar);
        Serial.println(F("WARNING: I2C requires external pullups for proper operation"));
		Serial.println(F("It may appear to work without them, but may be unreliable and slower"));
		Serial.println(F("Do not be surprised if it fails to work correctly"));
        Serial.println(F("Install external pullup resistors to ensure proper I2C operation"));
        Serial.println(hstar);
    }

	return(nopullups); // just say passed for now.
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
#ifdef ARDUINO_ARCH_PIC32
		// chipkit stuff screws up if you do beginTransmission() too fast
		// after an endTransmission()
		// below 20us will cause it to fail, pad it out more to provide margin
		delayMicroseconds(100);
#endif
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
		}
		else if((rdata != pat))
		{
			Serial.print(F(" Compare error: addr: "));
			Serial.print(addr, HEX);
			Serial.print(F(" read "));
			Serial.print((unsigned int)rdata, HEX);
			Serial.print(F(" != wrote "));
			Serial.println((unsigned int)pat, HEX);

			errors++;
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
			Serial.print(F(" Read Error addr: "));
			Serial.println((unsigned int)addr, HEX);
			errors++;
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
			Serial.print(F(" Read Error addr: "));
			Serial.println((unsigned int)addr, HEX);
			errors++;
		}
		else if((rdata != addr))
		{
			Serial.print(F(" Compare error: addr: "));
			Serial.print(addr, HEX);
			Serial.print(F(" read "));
			Serial.print((unsigned int)rdata, HEX);
			Serial.print(F(" != wrote "));
			Serial.println((unsigned int)addr, HEX);

			errors++;
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

// fatalError() - loop & blink and error code
void fatalError(int ecode)
{
	Serial.print(F("FATAL ERROR: "));
	Serial.println(ecode);
#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	while(1)
	{

		// blink out error code
		for(int i = 0; i< ecode; i++)
		{
			digitalWrite(LED_BUILTIN, HIGH);
			delay(100);
			digitalWrite(LED_BUILTIN, LOW);
			delay(250);
		}
		delay(1500);
	}
#else
	while(1){} // spin and do nothing
#endif
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
