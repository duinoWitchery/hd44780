//  vi:ts=4
// -------------------------------------------------------------------------
//  hd44780.h - hd44780 base class
//  Copyright (c) 2014-2020  Bill Perry
//  (Derivative work of arduino.cc IDE LiquidCrystal library)
//  Note:
//    Original Copyrights for LiquidCrystal are a mess as originally none were
//    specified, but in Nov 2015 these have appeared so they are included:
//
//  Copyright (C) 2006-2008 Hans-Christoph Steiner. All rights reserved.
//  Copyright (c) 2010 Arduino LLC. All right reserved.
//
//  See licenseInfo.txt for a history of the copyrights of LiquidCrystal
// --------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780.  If not, see <http://www.gnu.org/licenses/>.
//
// See the license.txt file for further licensing & copyright details.
// -----------------------------------------------------------------------
//
// hd44780 is an extensible LCD library for hd44780 based LCD displays.
// The library consists of a hd44780 base class combined with one or more
// i/o subclasses to perform the i/o communication between the host and the
// hd44780 display interface.
//
// The API functionality provided by the hd44780 base class, when combined
// with an hd44780 library i/o subclass, is compatible with the API
// functionality of the Arduino LiquidCrystal library as well as compatibilty
// with most of the LCD API 1.0 Specification (nearly obsolete).
//
// The hd44780 API also provides some addtional extensions and all the API
// functions provided by hd44780 are common across all i/o subclasses.
//
// -----------------------------------------------------------------------
// History
//
// 2020-11-14  bperrybap - created internal command4bit() for begin() function
// 2019.08.11  bperrybap - support for 1 and 2 lines in setRowOffsets()
// 2018.03.23  bperrybap - bumped default instruction time from 37us to 38us
// 2017.12.23  bperrybap - added LCD API 1.0 init() function
// 2017.12.23  bperrybap - allow write() to use 0 as a value without cast
// 2017.05.11  bperrybap - added auto linewrap functionality
// 2016.12.26  bperrybap - new BUSY error status, new constructors
// 2016.09.08  bperrybap - changed param order of iowrite() to match ioread()
// 2016.08.06  bperrybap - changed iosend() to iowrite()
// 2016.08.06  bperrybap - added status() and read()
// 2016.07.27  bperrybap - added return status to command() and iosend()
// 2016.07.20  bperrybap - merged hd44780 base class and i/o classes into a
//                         single library.
//
// 2016.06.08  bperrybap - removed pre 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2016.05.14  bperrybap - added LCD 1.0 API functions
// 2016.05.05  bperrybap - added support for 8 bit mode
// 2014.02.15  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ------------------------------------------------------------------------


#ifndef hd44780_h
#define hd44780_h

#if (ARDUINO <  100) && !defined(MPIDE)
#error hd44780 library requires Arduino 1.0 or later
#endif

#include <Arduino.h>
#include <inttypes.h>
#include <Print.h>

#define HD44780_VERSION		10302
#define HD44780_VERSIONSTR	"1.3.2"

class hd44780 : public Print
{
public:
	hd44780();
	hd44780(uint8_t cols, uint8_t rows);
	hd44780(uint8_t cols, uint8_t rows, uint32_t chExecTime, uint32_t insExecTime);

	// default execute times in us for clear/home and instructions/data
	// The hd44780 spec uses 1520 and 37 in table 6 page 24
	// note: the spec does not specify the time for Clear display
	// 38us is used vs 37 as 37 had issues on VERY fast processors that could
	// send data to the LCD that fast.
	// these can be overridden using setExecTimes(chUs, insUs)
	static const int HD44780_CHEXECTIME = 2000; // time in us for clear&home
	static const int HD44780_INSEXECTIME = 38;

	// API return values
	// 0 means successful, less than zero means unsuccessful
	static const int RV_ENOERR=0;				// no error. Do not change!
	static const int RV_EIO=-1;					// i/o operation failed (generic/default error)
	static const int RV_EINVAL=-2;				// invalid argument/parameter
	static const int RV_ENOTSUP=-3;				// not supported
	static const int RV_ENXIO=-4;				// no such device or address
	static const int RV_EMSGSIZE=-5;			// Message/data too long
	static const int RV_EBUSY=-6;				// device is unexpectedly BUSY

	// commands
	static const uint8_t HD44780_CLEARDISPLAY = 0x01;
	static const uint8_t HD44780_RETURNHOME = 0x02;
	static const uint8_t HD44780_ENTRYMODESET = 0x04;
	static const uint8_t HD44780_DISPLAYCONTROL = 0x08;
	static const uint8_t HD44780_CURDISPSHIFT = 0x10;
	static const uint8_t HD44780_FUNCTIONSET = 0x20;
	static const uint8_t HD44780_SETCGRAMADDR = 0x40;
	static const uint8_t HD44780_SETDDRAMADDR = 0x80;

	// flags for entry mode set;
	static const uint8_t HD44780_ENTRYLEFT2RIGHT = 0x02;
	static const uint8_t HD44780_ENTRYAUTOSHIFT = 0x01;

	// flags for display on/off control;
	static const uint8_t HD44780_DISPLAYON = 0x04;
//	static const uint8_t HD44780_DISPLAYOFF = 0x00;
	static const uint8_t HD44780_CURSORON = 0x02;
	static const uint8_t HD44780_CURSOROFF = 0x00;
	static const uint8_t HD44780_BLINKON = 0x01;
	static const uint8_t HD44780_BLINKOFF = 0x00;

	// flags for cursor/display shift;
	static const uint8_t HD44780_DISPLAYMOVE = 0x08;
	static const uint8_t HD44780_CURSORMOVE = 0x00;
	static const uint8_t HD44780_MOVERIGHT = 0x04;
	static const uint8_t HD44780_MOVELEFT = 0x00;

	// flags for function set;
	static const uint8_t HD44780_8BITMODE = 0x10;
	static const uint8_t HD44780_4BITMODE = 0x00;
	static const uint8_t HD44780_2LINE = 0x08;
	static const uint8_t HD44780_1LINE = 0x00;
	static const uint8_t HD44780_5x10DOTS = 0x04;
	static const uint8_t HD44780_5x8DOTS  = 0x00;

	// Print class println() function is not supported
	// catch it and print an error
	size_t __attribute__ ((error("println() is not supported"))) println(const __FlashStringHelper *);
	size_t __attribute__ ((error("println() is not supported"))) println(const String &s);
	size_t __attribute__ ((error("println() is not supported"))) println(const char[]);
	size_t __attribute__ ((error("println() is not supported"))) println(char);
	size_t __attribute__ ((error("println() is not supported"))) println(unsigned char, int = DEC);
	size_t __attribute__ ((error("println() is not supported"))) println(int, int = DEC);
	size_t __attribute__ ((error("println() is not supported"))) println(unsigned int, int = DEC);
	size_t __attribute__ ((error("println() is not supported"))) println(long, int = DEC);
	size_t __attribute__ ((error("println() is not supported"))) println(unsigned long, int = DEC);
	size_t __attribute__ ((error("println() is not supported"))) println(double, int = 2);
#ifdef Printable_h
	size_t __attribute__ ((error("println() is not supported"))) println(const Printable&);
#endif
	size_t __attribute__ ((error("println() is not supported"))) println(void);

	// ALL API calls return a status.
	// This is a change to existing APIs like
	// LiquidCrystal and LCD API 1.0
	// but should be compatible with existing code.
	// 
	// Unless otherwise documented,
	// A less than zero status means the API call was not successful

	// Arduino IDE LiquidCrystal lib functions
	// =======================================

	// begin()
	// returns 0 on success, non zero on initalization failure
	int begin(uint8_t cols, uint8_t rows, uint8_t charsize = HD44780_5x8DOTS);

#if 0
	// init ()
	// This version will NEVER be implemented in this class as it is
	// not conformant to LCD API 1.0 and is hardware i/o specific
	void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
#endif
	
	int clear();
	int home();
	int setCursor(uint8_t col, uint8_t row); 
	size_t write(uint8_t value);	// does char & line processing
	size_t _write(uint8_t value);	// does not do char & line processing
// write() overloads for 0 or null which is an int
// This is only because Print class doesn't do it.
	inline size_t write(unsigned int value) { return(write((uint8_t)value)); }
	inline size_t write(int value) { return(write((uint8_t)value)); }
	inline size_t _write(unsigned int value) { return(_write((uint8_t)value)); }
	inline size_t _write(int value) { return(_write((uint8_t)value)); }

	using Print::write; // for other Print Class write() functions
	int cursor();
	int noCursor();
	int blink();
	int noBlink();
	int display();				// turn on LCD pixels
	int noDisplay();			// turn off LCD pixels
	int scrollDisplayLeft();
	int scrollDisplayRight();
	int autoscroll(); 			// auto horizontal scrolling
	int noAutoscroll();			// no auto horizontal scrolling
	int leftToRight();
	int rightToLeft();
	int createChar(uint8_t charval, uint8_t charmap[]); // no PROGMEM

	int moveCursorLeft();
	int moveCursorRight();
	// sets memory address for each row, added in IDE 1.6.0
	// hd44780 allows calls with 1, 2, 3, or 4 offsets
	int setRowOffsets(int row0, int row1=0, int row2=0, int row3=0);

	// Mandatory LCD API 1.0 functions
	// ================================

	int init(); // uses defaults; using begin(cols, rows) is recommended

#if ( __GNUC__ >= 4) && (__GNUC_MINOR__ >= 5)
	inline void __attribute__((deprecated("Use setExecTimes() instead"))) setDelay(uint32_t CmdDelay, uint32_t CharDelay)
			 {setExecTimes(CmdDelay, CharDelay);}
#else
	inline void __attribute__((deprecated)) setDelay(uint32_t CmdDelay, uint32_t CharDelay)
			 {setExecTimes(CmdDelay, CharDelay);}
#endif

	// command()
	// returns 0 on success, non zero on command failure
	int command(uint8_t);

#if 0
	// setCursor()
	// MAJOR PROBLEM:
	// The LiquidCrystal API defines this as setCursor(col, row);
	// This is fundamentally incompatible with this defintion.
	// there is no way to fix this.
	// The LiquidCrytal API is in wide spread use so it wins.
	// any LCD API 1.0 code will have to be fixed to deal wth this.
	// A quick and dirty way to work around this to define a macro
	// to flip the parameters:
	// #define setCursor(_row, _col) setCursor(_col, _row)
	// and place it just after the #include for the library
	void setCursor(uint8_t row, uint8_t col); 
#endif

	// cursor_on()
	// cursor_off()
	// blink_on()
	// blink_off()
	// These functions should be considered obsolete as LiquidCrytal API has
	// api functions for these that are in wide spread use.
	// note: only very new versions of gcc support setting warning message
	// it breaks on on older versions that shipped with older 1.x IDEs
	// so test for gcc 4.5 or greater for better deprecated messages
#if ( __GNUC__ >= 4) && (__GNUC_MINOR__ >= 5)
	inline int __attribute__((deprecated("Use cursor() instead"))) cursor_on() {return(cursor());}
	inline int __attribute__((deprecated("Use noCursor() instead"))) cursor_off() {return(noCursor());}
	inline int __attribute__((deprecated("Use blink() instead"))) blink_on() {return(blink());}
	inline int __attribute__((deprecated("Use noBlink() instead"))) blink_off() { return(noBlink());}
#else 
	inline int __attribute__((deprecated)) cursor_on() {return(cursor());}
	inline int __attribute__((deprecated)) cursor_off() {return(noCursor());}
	inline int __attribute__((deprecated)) blink_on() {return(blink());}
	inline int __attribute__((deprecated)) blink_off() { return(noBlink());}
#endif

	// optional LCD API 1.0 functions
	// ==============================
	inline int setBacklight(uint8_t dimvalue) {return(iosetBacklight(dimvalue));}
	inline int setContrast(uint8_t contvalue) {return(iosetContrast(contvalue));}
	int on(void);			// turn on LCD pixels and backlight
	int off(void);			// turn off LCD pixels and backlight

	// status();
	// The LCD API 1.0 documenation says it does not work the same
	// on all devices.
	// On one device it returns some sort of fifo status.
	// the documenation isn't clear what it should do on other h/w.
	// hd44780 mandates that all API functions work the same for all
	// i/o subclasses.
	//
	// returns:
	//	success: 8 bit hd44780 status byte read from LCD (busy flag & address)
	//	failure: negataive value (error or read/status not supported)
	int status();

	// extended LCD API 1.0 functions
	// ==============================

	// load_custom_character()
	// this function should be considered obsolete as LiquidCrystal API has
	// a createChar() function that is in wide spread use
	// LCD API documentation for this call says 7 byte array
	// but it is really 8 bytes per custom char
	// note: only very new versions of gcc support setting warning message
	// it breaks on on older versions that shipped with older 1.x IDEs
	// so test for gcc 4.5 or greater for better deprecated messages
#if ( __GNUC__ >= 4) && (__GNUC_MINOR__ >= 5)
	inline void __attribute__((deprecated("Use createChar() instead")))
	 load_custom_character(uint8_t Char_Num, uint8_t Rows[]) {createChar(Char_Num, Rows); }
#else
	inline void __attribute__((deprecated))
	 load_custom_character(uint8_t Char_Num, uint8_t Rows[]) {createChar(Char_Num, Rows); }
#endif

#if 0
	// This will NEVER be implemented in this class as it is very h/w specific
	uint8_t keypad(); // not implemented
#endif

	// Additional API functions
	// These are hd44780 lib extensions that are
	// not part of LCD 1.0 or LiquidCrystal
	// note:
	// status() exists in LCD 1.0 API but is different
	// hd44780 status() function will be consistent across all i/o subclasses
	// ===================================================================
	int backlight(void);		// turn on Backlight (max brightness)
	int noBacklight(void);		// turn off Backlight

	// PROGMEM is wreck on AVR, as there is no way to detect its use.
	// for now:
	// hd44780 assumes that const stores data in flash and 
	// on processors that use/need PROGMEM it must be used on the declaration
	inline int createChar(uint8_t charval, char charmap[])	// does not assume PROGMEM
		{ return(createChar(charval, (uint8_t *) charmap)); }

	// on processors that don't use/need PROGMEM it can be left off the declaration
	int createChar(uint8_t charval, const uint8_t charmap[]); // assumes PROGMEM

	// this function is for compatibilty with other libraries like fm's newliquidCrystal
	inline int createChar(uint8_t charval, const char charmap[]) // assumes PROGMEM
		{ return(createChar(charval, (const uint8_t *) charmap)); }

	int read(void);
	// enable automatic line wrapping (only works in left 2 right mode)
	int lineWrap(void)  { if(_displaymode & HD44780_ENTRYLEFT2RIGHT) {_wraplines=1; return(RV_ENOERR);}else{return(RV_ENOTSUP);}}
	// disable automatic line wrapping
	int noLineWrap(void){ _wraplines=0; return(RV_ENOERR);};		// turn off automatic line wrapping

	// set execution times for commmands to override defaults
	inline void setExecTimes(uint32_t chExecTimeUs, uint32_t insExecTimeUs)
		{ _chExecTime = chExecTimeUs; _insExecTime = insExecTimeUs;}

	// A few undocumented helper functions for the included examples
	static int blinkLED(int blinks);		// blink a built in LED if possible
	static void fatalError(int errcode);	// hang in loop blinking error code

protected:

	// type of data being sent through ioread()/iowrite()
	//    IOcmd  - read/write an 8 bit status/command to/from display
	//    IOdata - read/writes 8 bits of data to/from display
	// IOcmd4bit - write D4-d7 cmd bits to display using a single EN strobe
	//             8 bit host interfaces can treate this same as IOcmd
	enum iotype {HD44780_IOcmd, HD44780_IOdata, HD44780_IOcmd4bit};

	uint8_t _displayfunction;
	uint8_t _displaycontrol;
	uint8_t _displaymode;
	uint8_t _cols;
	uint8_t _rows;

	// wait for lcd to be ready
	inline void waitReady() {_waitReady((_startTime), _execTime);}
	inline void waitReady(int32_t offsetUs) {_waitReady((_startTime+offsetUs), _execTime);}

	inline void _waitReady(uint32_t _stime, uint32_t _etime)
		{while(( ((uint32_t)micros()) - _stime) < _etime){}}

private:

	uint8_t _curcol;	// current LCD col if doing char & line processing
	uint8_t _currow;	// current LCD row if doing char & line processing
	uint8_t _wraplines;	// set to nonzero if wrapping long lines

	// i/o subclass functions
	virtual int ioinit() {return 0;}	// optional - successful if not implemented
	virtual int ioread(hd44780::iotype type) {if(type) return(RV_ENOTSUP);else return(RV_ENOTSUP);}	// optional, return fail if not implemented
	virtual int iowrite(hd44780::iotype type, uint8_t value)=0;// mandatory
	virtual int iosetBacklight(uint8_t dimvalue){if(dimvalue) return(RV_ENOTSUP); else return(RV_ENOTSUP);}	// optional
	virtual int iosetContrast(uint8_t contvalue){if(contvalue) return(RV_ENOTSUP); else return(RV_ENOTSUP);}// optional

	uint8_t _rowOffsets[4]; // memory address of start of each row/line

	// stuff for tracking execution times
	inline void markStart(uint32_t exectime) { _startTime = (uint32_t) micros(); _execTime = exectime;}
	uint32_t _chExecTime;	// time in Us of execution time for clear/home
	uint32_t _insExecTime;	// time in Us of execution time for instructions or data
	uint32_t _startTime;	// 'start' time of last thing sent to LCD (cmd or data)
	uint32_t _execTime;		// execution time in Us of last thing sent to LCD (cmd or data)

	// internal API function to send only upper 4 bits of byte on LCD DB4 to DB7 pins
	int command4bit(uint8_t value)
	{
	int status = iowrite(HD44780_IOcmd4bit, value);
		markStart(_insExecTime);
		return(status);
	}

};

// LED_BUILTIN define fixups for Teensy, ChipKit, ESP8266, ESP32 cores
#if !defined(LED_BUILTIN)

#if defined(CORE_TEENSY)
#define LED_BUILTIN CORE_LED0_PIN

#elif defined(ARDUINO_ARCH_ESP8266)
#define LED_BUILTIN BUILTIN_LED

// this is for cores that incorrectly used BUILTIN_LED instead of LED_BUILTIN
// esp32 core does this, they have LED_BUILTIN but it is a const not a define
// this works around that.
#elif defined(BUILTIN_LED)
#define LED_BUILTIN BUILTIN_LED

// special check for pre 1.0.6 IDEs that didn't define LED_BUILTIN
#elif (ARDUINO <  106 ) && ( defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__))
#define LED_BUILTIN 13 // IDE bundled AVR boards use pin 13 for builtin led

#elif defined(PIN_LED1) // chipkit
#define LED_BUILTIN PIN_LED1
#endif
#endif

// endif for header guard
#endif
