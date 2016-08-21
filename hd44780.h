//  vi:ts=4
// -------------------------------------------------------------------------
//  hd44780.h - hd44780 base class
//  Copyright (c) 2014-2016  Bill Perry
//  (Derivative work of arduino.cc IDE LiquidCrystal library)
//  Note:
//    Original Copyrights for LiquidCrystal are a mess as originally none were
//    specified, but in Nov 2015 these have appeared so they are included:
//
//  Copyright (C) 2006-2008 Hans-Christoph Steiner. All rights reserved.
//  Copyright (c) 2010 Arduino LLC. All right reserved.
//
//  See the license.txt file for a history of the copyrights of LiquidCrystal
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

#define HD44780_VERSION		701
#define HD44780_VERSIONSTR	"0.7.1"

class hd44780 : public Print
{
public:
	hd44780();

	// default execute times in us for clear/home and instructions/data
	// The hd44780 spec uses 1520 and 37 in table 6 page 24
	// note: the spec does not specify the time for Clear display
	// these can be overridden using setExecTimes(chUs, insUs)
	static const int HD44780_CHEXECTIME = 2000; // time in us for clear&home
	static const int HD44780_INSEXECTIME = 37;

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


	// Arduino IDE LiquidCrystal lib functions
	// =======================================

	// begin()
	// returns 0 on success, non zero on initalization failure
	int begin(uint8_t cols, uint8_t rows, uint8_t charsize = HD44780_5x8DOTS);

#if 0
	// init ()
	// This will NEVER be implemented in this class as it is
	// not conformant to LCD API 1.0 and is hardware i/o specific
	void init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
	    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
	    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
#endif
	
	void clear();
	void home();
	void setCursor(uint8_t col, uint8_t row); 
	size_t write(uint8_t value);
	using Print::write; // for other Print Class write() functions
	void cursor();
	void noCursor();
	void blink();
	void noBlink();
	void display();				// turn on LCD pixels
	void noDisplay();			// turn off LCD pixels
	void scrollDisplayLeft();
	void scrollDisplayRight();
	void autoscroll(); 			// auto horizontal scrolling
	void noAutoscroll();		// no auto horizontal scrolling
	void leftToRight();
	void rightToLeft();
	void createChar(uint8_t charval, uint8_t charmap[]);
	void moveCursorLeft();
	void moveCursorRight();
	// sets memory address for each row, added in IDE 1.6.0
	void setRowOffsets(int row0, int row1, int row2, int row3);

	// Mandatory LCD API 1.0 functions
	// ================================

	// init(); // not implemented
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
	inline void __attribute__((deprecated("Use cursor() instead"))) cursor_on() {cursor();}
	inline void __attribute__((deprecated("Use noCursor() instead"))) cursor_off() {noCursor();}
	inline void __attribute__((deprecated("Use blink() instead"))) blink_on() {blink();}
	inline void __attribute__((deprecated("Use noBlink() instead"))) blink_off() { noBlink();}
#else 

	inline void __attribute__((deprecated)) cursor_on() {cursor();}
	inline void __attribute__((deprecated)) cursor_off() {noCursor();}
	inline void __attribute__((deprecated)) blink_on() {blink();}
	inline void __attribute__((deprecated)) blink_off() { noBlink();}
#endif

	// optional LCD API 1.0 functions
	// ==============================
	inline void setBacklight(uint8_t dimvalue) {iosetBacklight(dimvalue);}
	inline void setContrast(uint8_t contvalue) {iosetContrast(contvalue);}
	void on(void);			// turn on LCD pixels and backlight
	void off(void);			// turn off LCD pixels and backlight

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
	// status() is exists in LCD 1.0 API but is different
	// This status() function will be consistent across all i/o subclasses
	// ===================================================================
	void backlight(void);		// turn on Backlight (max brightness)
	void noBacklight(void);		// turn off Backlight
	int read(void);

	// set execution times for commmands to override defaults
	inline void setExecTimes(uint32_t chExecTimeUs, uint32_t insExecTimeUs)
		{ _chExecTime = chExecTimeUs; _insExecTime = insExecTimeUs;}

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

	// i/o subclass functions
	virtual int ioinit() {return 0;}	// optional
	virtual int ioread(hd44780::iotype type) {if(type) return -1;else return -1;}	// optional, return fail if not implemented
	virtual int iowrite(uint8_t value, hd44780::iotype type)=0;// mandatory
	virtual void iosetBacklight(uint8_t dimvalue){if(dimvalue) return;}	// optional
	virtual void iosetContrast(uint8_t contvalue){if(contvalue) return;}// optional

	uint8_t _rowOffsets[4]; // memory address of start of each row/line

	// stuff for tracking execution times
	inline void markStart(uint32_t exectime) { _startTime = (uint32_t) micros(); _execTime = exectime;}
	uint32_t _chExecTime;	// time in Us of execution time for clear/home
	uint32_t _insExecTime;	// time in Us of execution time for instructions or data
	uint32_t _startTime;	// 'start' time of last thing sent to LCD (cmd or data)
	uint32_t _execTime;		// execution time in Us of last thing sent to LCD (cmd or data)

};

// LED_BUILTIN define fixups for Teensy and ChipKit
#if !defined(LED_BUILTIN)
#if defined(CORE_TEENSY)
#define LED_BUILTIN CORE_LED0_PIN
#else
#if defined(PIN_LED1)
#define LED_BUILTIN PIN_LED1
#endif
#endif
#endif


#endif
