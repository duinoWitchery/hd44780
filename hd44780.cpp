//  vi:ts=4
// -----------------------------------------------------------------------
//  hd44780.cpp - hd44780 base class
//  Copyright (c) 2014-2016  Bill Perry
//  (Derivative work of arduino.cc IDE LiquidCrystal library)
//  Note:
//    Original Copyrights for LiquidCrystal are a mess as originally none were
//    specified, but in Nov 2015 these have appeared so they are included:
//
//  Copyright (C) 2006-2008 Hans-Christoph Steiner. All rights reserved.
//  Copyright (c) 2010 Arduino LLC. All right reserved.
//
//  See licenseInfo.txt for a history of the copyrights of LiquidCrystal
// ------------------------------------------------------------------------
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
// 2017.01.07  bperrybap - added blinkLED() and fatalError() 
// 2016.12.26  bperrybap - new constructors
// 2016.10.17  bperrybap - corrected DDRAM address mask in createChar()
// 2016.10.15  bperrybap - createChar() restores DDRAM location when possible
// 2016.09.08  bperrybap - changed param order of iowrite() to match ioread()
// 2016.08.06  bperrybap - changed iosend() to iowrite()
// 2016.08.06  bperrybap - added status() and read()
// 2016.07.27  bperrybap - added return status for command() and iosend()
// 2016.07.20  bperrybap - merged hd44780 base class and i/o classes into a
//                         single library.
// 2016.06.08  bperrybap - removed pre IDE 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2016.05.14  bperrybap - added LCD 1.0 API functions
// 2016.05.05  bperrybap - added support for 8 bit mode
// 2014.02.15  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ------------------------------------------------------------------------

#include "hd44780.h"

hd44780::hd44780()
{
	/*
 	 * Set up default execution times for clear/home and
	 * all other instructions and data writes.
	 * Setting exec times here
	 * allows application to call setExecTimes() before begin()
	 * should it be necessary to override defaults.
	 */

	setExecTimes(HD44780_CHEXECTIME, HD44780_INSEXECTIME);

	markStart(0); // initialize last start time to 'now'
}

hd44780::hd44780(uint8_t cols, uint8_t rows) : _cols(cols), _rows(rows)
{
	setExecTimes(HD44780_CHEXECTIME, HD44780_INSEXECTIME);
	markStart(0); // initialize last start time to 'now'
}

hd44780::hd44780(uint8_t cols, uint8_t rows, uint32_t chExecTimeUs, uint32_t insExecTimeus) :
		 _cols(cols), _rows(rows), _chExecTime(chExecTimeUs), _insExecTime(insExecTimeus)
{
	setExecTimes(HD44780_CHEXECTIME, HD44780_INSEXECTIME);
	markStart(0); // initialize last start time to 'now'
}

//
// initialize the LCD
// Note: The hd44780 spec uses the term "line" whereas the
//  IDE LiquidCrystal library uses the term "row" so this
//  library (hd44780) will continue using the term "row"
//  rather than "line".
//  Other than name, they are the same.
//
// Returns 0 on success, non zero on initialization failure
//
int hd44780::begin(uint8_t cols, uint8_t rows, uint8_t dotsize)
{
int rval = 0;

	_cols = cols; // FIXME, this may not be needed anymore
	_rows = rows;
	/*
	 * Limit lines/rows to max in the row offset table
	 */
	if(_rows > sizeof(_rowOffsets) / sizeof(_rowOffsets[0]))
		_rows = sizeof(_rowOffsets) / sizeof(_rowOffsets[0]);

	/*
	 * create default row/line offset table of addresses for each row/line
	 * See here for further explanation of lcd memory addressing:
	 * http://web.alfredstate.edu/weimandn/lcd/lcd_addressing/lcd_addressing_index.html
	 * This default will handle:
	 * 40x2, 20x4, 20x2, 16x2, 16x1 (type 2), 16x4
	 * displays not supported by this:
	 * - 16x1 (type 1), This uses a discontigous memory for the single line
	 *   (This requires ugly mods to the library to make work)
	 * - 40x4 is dual 40x2 displays using dual E signals which is not supported.
	 */
	setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

	/*
	 * SEE PAGE 45/46 of Hitachi HD44780 spec FOR INITIALIZATION SPECIFICATION.
	 * according to datasheet,we need at least 40ms after power rises above 2.7V
	 * before sending commands.The Arduino AVR can turn on and start running way
	 * before 4.5V so we'll wait 100ms to add some additional margin
	 */
	delay(100); 
  
	// default to 4 bit mode (can be overridded by i/o class library)
	_displayfunction = HD44780_4BITMODE;

	// call h/w i/o class init function
	if((rval = ioinit())) // intentional assignment
	{
		return(rval);
	}

	if (_rows > 1)
	{
		_displayfunction |= HD44780_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotsize != HD44780_5x8DOTS) && (rows == 1))
	{
		_displayfunction |= HD44780_5x10DOTS;
	}

	/*
	 * ========================================================================
	 * The init sequence below is shown in the hitachi HD44780 datasheet
	 * figures 23 & 24, pages 45 & 46
	 *
	 * NOTE:
	 * IMO, most explanations and comments about this sequence are wrong as the
	 * code authors don't really understand what it is actually doing.
	 * Adding to the confusion is that the hitachi HD44780 datasheet does
	 * a very poor job explaining this sequence. 
	 * Also, adding to the confusion is the note about FunctionSet on page 27:
	 *  ----------------------------------------------------------------------
	 *	Note: Perform the function at the head of the program before executing
	 *	any instructions (except for the read busy flag and address instruction)
	 *	From this point, the function set instruction cannot be executed unless
	 *	the interface data length is changed.
	 *  ----------------------------------------------------------------------
	 *
	 * It isn't clear what information this note is trying to convey.
	 *
	 * It appears to be implying that FunctionSet must be done first and that it
	 * cannot be executed later unless the data length is changed.
	 * If that is what it saying...
	 * This is in direct conflict with the comand sequence in the
	 * Initialize by Instruction in figures 23 & 24, particulary in
	 * figure 23 as the 4th command sent will be a FunctionSet command
	 * setting data length to 8 bits as well as the prefered the N and F bits
	 * when the data length is guaranteed to already be 8 bits. 
	 *
	 * But since sending a FunctionSet command with DL=1 along with setting N
	 * and F bits works when the display is already in 8 bit mode, my conclusion
	 * is that the note on page 27 is obviously incorrect and can be ignored.
	 *
	 * --
	 *
	 * When using 4 bit length mode, the LCD expects the nibbles on d4-d7 and
	 * expects the upper nibble of the 8 bits first.
	 * Therefore, when using 4 bit mode, it is critical that the host and
	 * the LCD are in sync on the nibbles.
	 *
	 * The command sequences in figures 23 and 24 are not simple retries and
	 * there is nothing magical about the timing.
	 * They are specifically chosen to ensure that after completion,
	 * the LCD is in the desired interface mode (8 bit or 4 bit).
	 * The two sequences start off with an identical 3 command sequence of
	 * FunctionSet commands which are used to reliably put the LCD into
	 * 8 bit length mode regardless of what mode or state the LCD happends
	 * to be in and regardless of whether the host is controlling all 8 data
	 * pin signals or just d4-d7.
	 * The only difference between the two sequences is that the 4 bit
	 * initializaion (figure 24) puts the LCD interface into 4 bit mode
	 * after the LCD is assured to be in 8 bit mode.
	 *
	 * 
	 * Below is a detailed explanation of the hd44780 initalization sequence
	 *
	 * --- bperrybap
	 *
	 * The overall methodology is to carefully use a command sequence that can
	 * first reliably put the LCD back into 8 bit mode and then if using 4 bit
	 * mode, a command can be issued to put the LCD into 4 bit mode.
	 * This sequence is not reseting the display but rather re-initalizing the
	 * host interface mode (8 bit or 4 bit)
	 * It also ensures that a host that can only control data lines D4-D7 is
	 * able to reliably get into nibble sync with the LCD and put the LCD into
	 * 4 bit mode regardless of the what state the LCD is currently in.
	 *
	 * This special initailization command sequence is required beause there is
	 * no way to actully reset the LCD to its power up default state and
	 * there is no way of knowing if the LCD is currently in 4bit or 8bit mode.
	 * Even if the h/w inteface is 8 bits, the LCD could potentially be in
	 * 4 bit mode.
	 * Also, if the LCD is in 4 bit mode, the LCD could be in the middle of
	 * receiving nibbles. i.e. the system was reset after only a single
	 * nibble was sent to the LCD.
	 * When the LCD is in 4 bit mode and out of nibble sync with the host:
	 * 	the LCD has received the upper/1st nibble of a command/data byte and
	 *	is waiting for the lower/2nd nibble.
	 *	After the 2nd nibble is recieved, the LCD will go off and execute an
	 *	unknown command. This is why the firt delay is so long. The host has to
	 *  accomodate the longest possible command execution time.
	 *	The next two FunctionSet commands will be interpreted
	 *	as a single FunctionSet command by the LCD to put the display back into 
	 *	8 bit mode.
	 *
	 * It appears that all delays specified in figures 23 & 24 are 2.7x the
	 * execution times in table 6. It appears that these values were chosen to
	 * accomodate LCDs that are running chips clocked slower than the 270kHz
	 * reference used in table 6
	 * A 2.7x factor would allow the Initialize by Instruction sequence to work
	 * with chips clocked down to 100Khz.
	 *
	 * In each of the 3 steps, EN is strobed once as the host will be
	 * treating the LCD h/w interface as if it is 8 bits wide regardless of
	 * whether the actual h/w interface to the LCD may only be 4 bits.
	 * d4-d7 signals on the hd44780 interface are set for 
	 * a function set "goto 8bit mode" command.
	 * Also, d0-d3 signals on the physical hd44780 interface are either set to
	 * low, or not driven in the case of 4 bit only h/w host interfaces.
	 *
	 * Here is the command sequence shown seperately for each of 3 states the
	 * LCD can be in. 
	 * This will demonstrate why there must be a command sequence
	 * of three "goto 8 bit mode" followed by one "goto to 4 bit mode" if
	 * using 4 bit mode.
	 *
	 * ========================================================================
	 * LCD in 8 BIT mode:
	 * =================
	 * 1) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		8bit host: LCD interprets command 0x30
	 *			since lower 4 bits will be low and read as zeros.
	 *		4bit host: LCD interprets command 0x3X
	 *			since lower 4 bits will be unpredictable
	 *			FUNCTION_SET & 8BIT are set, so it gets interpreted as a
	 *			"goto 8bit mode" and so this is essentially a NOP as the
 	 *			display is already 8 bit mode
	 *
	 * 2) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		8bit host: LCD interprets command 0x30
	 *			since lower 4 bits will be low and read as zeros.
	 *		4bit host: LCD interprets command 0x3X
	 *			since lower 4 bits will be unpredictable
	 *			FUNCTION_SET & 8BIT are set, so it gets interpreted as a
	 *			"goto 8bit mode" and so this is essentially a NOP as the
 	 *			display is already 8 bit mode
	 *
	 * 3) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		8bit host: LCD interprets command 0x30
	 *			since lower 4 bits will be low and read as zeros.
	 *		4bit host: LCD interprets command 0x3X
	 *			since lower 4 bits will be unpredictable
	 *			FUNCTION_SET & 8BIT are set, so it gets interpreted as a
	 *			"goto 8bit mode" and so this is essentially a NOP as the
 	 *			display is already 8 bit mode
	 *
	 * LCD in 4 bit mode and expecting upper/1st nibble:
	 * =================================================
	 * 1) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		LCD pulls in nibble as upper 4 bits of 8 bit command
	 *
	 * 2) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		LCD pulls in nibble as lower 4 bits to complete 8 bits of command
	 *		LCD interprets command 0x33.
	 *			the LCD sees a Goto 8bit mode command and changes to 8 bit mode.
	 *
	 * 3) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		LCD interprets command 0x30 on 8bit hosts since
	 *			 lower 4 bits will be low and read as zeros.
	 *		LCD interprets command 0x3X on 4 bit hosts since
	 *			 lower 4 bits will be unpredictable
	 *		since FUNCTION_SET & 8BIT are set, it gets interpreted as
	 *			 a "goto 8bit mode" and so
	 * 		this is essentially a NOP as the display is already 8 bit mode
	 *
	 * LCD in 4 bit mode and expecting lower/2nd nibble:
	 * (out of nibble sync with host)
	 * ========================================================================
	 * 1) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		LCD pulls in nibble as lower 4 bits to complete 8 bits of command
	 *		LCD interprets command with upper nibble (unknown) and lower
	 *		nibble 0x3
	 *		There is no telling what this command does as the upper nibble is
	 *		unknown. And THAT is why the first delay in the sequence must be so
	 *		long. It isn't that the the first "function set" takes that long it
	 *		is because there is no telling what command might now be executing
	 *		and BUSY cannot be used yet.
	 *		Therefore, the host must blind wait the worst case amount of time.
	 *		A 4.1ms delay appears to be 2.7x the worst case instruction
	 *		execution time (clear & home are 1.52ms) at a 270kHz LCD clock rate.
	 *		(4.1ms would accomodate a chip clocked at 100kHz instead of 270 kHz)
	 *
	 * 2) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		If LCD happened to go into 8BIT mode in step 1
	 *			LCD interprets this command as 0x30
	 *				since d0-d3 lower 4 bits will be read as zeros in 8 bit mode
	 *				This is essentially a NOP as the display is already in
	 *				8 bit mode
	 *
	 *		If LCD still in 4 bit mode,
	 *			then this is upper nibble of next command.
	 *
	 * 3) Set the upper 4 bits (d4-d7) of Functionset: FUNCTION_SET|8BIT = 0x3
	 *		If LCD happened to go into 8BIT mode in step 1
	 *			LCD interprets this command as 0x30
	 *				since d0-d3 lower 4 bits will be read as zeros in 8 bit mode
	 *				This is essentially a NOP as the display is already in
	 *				8 bit mode
	 *
	 *		If LCD still in 4 bit mode
	 *			LCD pulls in nibble as lower 4 bits to complete 8 bits of cmd
	 *			LCD interprets command 0x33.
	 *			the LCD sees a Goto 8bit mode command and changes to 8 bit mode.
	 *
	 *
	 * =======================================================================
	 * Once the display in 8 bit mode, if the interface is to be in 4 bit mode:
	 * =======================================================================
 	 * - Set the upper 4 bits of Functionset: with 8BIT clear
	 *	this puts the LCD in 4 bit and it now expects upper nibble
	 */

    
	/*
	 * Note:
	 * The initialization sequence below uses special hd44780 internal API
	 * 4 bit commands.
	 * Internal 4 bit commands are commands that are sent to the LCD using
	 * only a single strobe of EN even if the host h/w interface is 4 bit only.
	 * In other words it is indicating that only the upper 4 bits need to be
	 * sent to the display - so effectively the host must ensure that the
	 * upper four bits are placed on D4-d7 and that EN is stobed only once
	 * including if the host h/w interface is only 4 bits wide.
	 *
	 * 8bit only i/o interface libraries can ignore this as the bits for these
	 * special commands are in the proper d4-d7 bit locations so the full
	 * 8 bit byte can be presented to the LCD.
	 * i.e.
	 *  - 8 bit interfaces can treat 4bit commands the same as regular commands
	 *  - 4 bit only interfaces will send just the upper 4 bits (d4-d7)
	 *
	 * Additional Note:
	 *	The Initiatlization by Instruction figures 23 & 24 of the hitachi spec
	 *	seems to use delays that are 2.7x the specified instruction times based
	 *	on a 270khz clock from table 6
	 *  i.e. 4.1ms is 2.7x 1.52ms of clear/home commands
	 *	and 100us is 2.7x 37us for all the other instructions (cmd & data)
	 *  The delay values appear to be accomodating LCDs clocked down to 100kHz
	 *	rather than at the 270Khz reference.
	 *	This library will wait a little bit longer given the delay functions
	 *	used. However, since iowrite() honors execution times, 
	 *  the delays here are in addition to the configured execution times
	 *	and technically should not be needed.
	 *  So if the application called setExecTimes() with excution
	 *  values prior to begin() those will be in addition to these
	 *  delays so everthing should work no matter how slow the LCD is.
	 *
	 * Note that delay() is used here vs delayMicroSeconds() as delay()
	 * on later versions of Arduino core code calls some scheduling functions
	 * to potentially allow other code to execute during this time period.
	 *
	 * delay() can be used because this code is never called from a constructor
	 */
	iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET|HD44780_8BITMODE);
	delay(5); // wait 5ms vs 4.1ms, some are slower than spec

	iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET|HD44780_8BITMODE);
	delay(1); // wait 1ms vs 100us
    
	iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET|HD44780_8BITMODE);
	delay(1); // wait 1ms vs 100us

	/*
	 * At this point the LCD is guaranteed to be in 8 bit mode
	 *
	 * If the interface needs to be put into 4 bit mode, the
	 * goto 4bit mode command is sent as another special internal
	 * "4bit" command because the 4bit only host talks 4 bits and the LCD
	 * isn't in 4 bit mode yet.
	 */
	if(!(_displayfunction & HD44780_8BITMODE))
		iowrite(HD44780_IOcmd4bit, HD44780_FUNCTIONSET|HD44780_4BITMODE);

	/*
	 * At this point the LCD is in 8 bit mode for 8 bit host interfaces,
	 * and in 4 bit mode for 4bit only host interfaces so we can 
	 * now use the "normal" library command() & API interface functions
	 */

	// set # lines, font size, etc.
	command(HD44780_FUNCTIONSET | _displayfunction);  


	// turn the display on with no cursor or blinking default
	_displaycontrol = HD44780_DISPLAYON | HD44780_CURSOROFF | HD44780_BLINKOFF;  
	display();

	clear(); // clear display

	// Initialize to default text direction (for romance languages)
	_displaymode = HD44780_ENTRYLEFT2RIGHT;
	// set the entry mode
	rval = command(HD44780_ENTRYMODESET | _displaymode);

// FIXME
#ifdef LATER
	// quick check to see if BUSY is working on devices that support reads
	{
	int lcdstatus = status();
		if(lcdstatus >= 0)
		{
			// if BUSY bit is set then there is a problem
			if(lcdstatus &  0x80)
				return(RV_EBUSY);
		}
	}
#endif
	backlight(); // turn on the backlight, if supported

	return(rval);
}

int hd44780::clear()
{
	return(command(HD44780_CLEARDISPLAY));  // clear display, set cursor to 0,0
}

int hd44780::home()
{
	return(command(HD44780_RETURNHOME));  // set cursor position to 0,0
}

int hd44780::setRowOffsets(int row0, int row1, int row2, int row3)
{
	_rowOffsets[0] = row0;
	_rowOffsets[1] = row1;
	_rowOffsets[2] = row2;
	_rowOffsets[3] = row3;
	return(RV_ENOERR);
}


int hd44780::setCursor(uint8_t col, uint8_t row)
{
	// while this could return RV_EINVAL for invalid parameters
	// it is going to adjust them to maintain compability with existing LiquidCrystal library
	// and to allow a back door way to set the ddram address - see below

	if ( row >= _rows ) 
	{
		// set to max line (rows/lines start at 0, _rows is 1 based)
		row = _rows-1;
	}
	/*
	 * if col is too large, it can't corrupt the command since
	 * SETDDRRAMADDR is bit 7 of the byte
	 * It is intionally not checked to allow allow:
	 * - positioning off the end of the visable line
	 * - if line 0 is used, it is a backdoor to do SETDDRAMADDR to any address
	 */

#ifdef later
	// in right to left mode the cursor position wil be incorrect.
	// however, things like home() and clear() will not work as expected either.
	// home() positions back to DDRAMADDR 0 
	// clear() positions back to DDRAMADDR 0 and removes the right to left mode
	// It isn't clear how home() and clear() should be handled.
	// if they position to (0,0) then they have to expcility do it as well as
	// restore rightToLeft() mode when being used.
	//
	if(!(_displaymode & HD44780_ENTRYLEFT2RIGHT))
		return(command(HD44780_SETDDRAMADDR | (_cols - col -1 + _rowOffsets[row])));
	else
#endif
		return(command(HD44780_SETDDRAMADDR | (col + _rowOffsets[row])));
}

// turn off display pixels
int hd44780::noDisplay()
{
	_displaycontrol &= ~HD44780_DISPLAYON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// turn on display pixels
int hd44780::display()
{
	_displaycontrol |= HD44780_DISPLAYON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turns off underline cursor
int hd44780::noCursor()
{
	_displaycontrol &= ~HD44780_CURSORON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}
// Turn on underline cursor
int hd44780::cursor()
{
	_displaycontrol |= HD44780_CURSORON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turn on off the blinking cursor
int hd44780::noBlink()
{
	_displaycontrol &= ~HD44780_BLINKON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// Turn on the blinking cursor
int hd44780::blink()
{
	_displaycontrol |= HD44780_BLINKON;
	return(command(HD44780_DISPLAYCONTROL | _displaycontrol));
}

// These API functions scroll/shift the display contents without changing the RAM
int hd44780::scrollDisplayLeft(void)
{
	return(command(HD44780_CURDISPSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT));
}
int hd44780::scrollDisplayRight(void)
{
	return(command(HD44780_CURDISPSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT));
}

// This is for text that flows Left to Right
int hd44780::leftToRight(void)
{
	_displaymode |= HD44780_ENTRYLEFT2RIGHT;
	return(command(HD44780_ENTRYMODESET | _displaymode));
}

// This is for text that flows Right to Left
int hd44780::rightToLeft(void)
{
	_displaymode &= ~HD44780_ENTRYLEFT2RIGHT;
	return(command(HD44780_ENTRYMODESET | _displaymode));
}

// This moves the cursor one space to the right
int hd44780::moveCursorRight(void)
{
	return(command(HD44780_CURDISPSHIFT | HD44780_CURSORMOVE | HD44780_MOVERIGHT));
}

// This moves the cursor one space to the left
int hd44780::moveCursorLeft(void)
{
	return(command(HD44780_CURDISPSHIFT | HD44780_CURSORMOVE | HD44780_MOVELEFT));
}

// This will enable autoshifting display as new characters are written.
// If mode is left to right, shift is left
// if mode is right to left, shift is right
int hd44780::autoscroll(void)
{
	_displaymode |= HD44780_ENTRYAUTOSHIFT;
	return(command(HD44780_ENTRYMODESET | _displaymode));
}

// This will disable autoshifting when new characters are written
int hd44780::noAutoscroll(void)
{
	_displaymode &= ~HD44780_ENTRYAUTOSHIFT;
	return(command(HD44780_ENTRYMODESET | _displaymode));
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
int hd44780::createChar(uint8_t location, uint8_t charmap[])
{
int rval;
int ddramaddr;

	location &= 0x7; // we only have 8 locations 0-7

	ddramaddr = status(); // fetch status which includes ddram address

	if(ddramaddr < 0) // status() failed, so just set address to 0
		ddramaddr = 0;
	else
		ddramaddr &= 0x7f; // strip off BUSY bit

	rval = command(HD44780_SETCGRAMADDR | (location << 3));
	if(rval)
		return(rval);
	for (int i=0; i<8; i++)
	{
		if(write(charmap[i]) != 1)
			return(RV_EIO);
	}

	// put LCD back into DDRAM mode so write() works
	return(setCursor(ddramaddr , 0));
}

// turn on backlight at full intensity
int hd44780::backlight(void)
{
	return(iosetBacklight(-1)); // max brightness
}

// turn off backlight
int hd44780::noBacklight(void)
{
	return(iosetBacklight(0));
}

// turn on pixels and backlight
int hd44780::on ( void )
{
int rval;

	rval = display();
	backlight(); // ignore any issues for backlight control
	return(rval);
}

// turn off pixels and backlight
int hd44780::off ( void )
{
   noBacklight(); // ignore any issues for backlight control
   return(noDisplay());
}

// command() - send hd44780 command byte to lcd
//
// Returns 0 on success, non zero if command failed
//
inline int hd44780::command(uint8_t value)
{
int status;

	status = iowrite(HD44780_IOcmd, value);

	// executime time depends on command
	if((value == HD44780_CLEARDISPLAY) || (value == HD44780_RETURNHOME))
		markStart(_chExecTime);
	else
		markStart(_insExecTime);

	return(status);
}

// status() - read status byte from LCD
// returns:
// 	success: 8 bit hd44780 status byte (busy flag & address)
//	failure: neagative value (error or read/status not supported by i/o subclass
int hd44780::status()
{
int rvalue = ioread(HD44780_IOcmd);
	markStart(0); // status reads do not require execution time
	return(rvalue);
}

// read() - read a data byte from LCD
// returns:
// 	success: 8 bit value read
//	failure: neagative value (error or read not supported by i/o subclass
int hd44780::read()
{
int rvalue = ioread(HD44780_IOdata);
	markStart(_insExecTime);
	return(rvalue);
}

// write() - send data character byte to lcd
// returns number of bytes successfully written to device
// i.e. 1 if success or 0 if no character was processed (error)
size_t hd44780::write(uint8_t value)
{
int status = 1; //assume success

	if(iowrite(HD44780_IOdata, value))
		status = 0; // write was unsuccessful
	markStart(_insExecTime);
	
	return status;
}

//============================================================================
// A couple of functions that really shouldn't be here.
// blinkLED() and fatalError()
//
// NOTE: I REALLY do not like having these functions here and they definitely
// do not belong in this class; HOWEVER.... blinking a built in LED in arduino
// is not as easy as it should be to ensure that works across many different
// cores and boards.
// Some cores didn't correclty define their LED_BUILTIN define, and some boards
// use use an active LOW LED.
// To make things simpler for the hd44780 examples and ensure consistency as
// well as making maintenance much easier,
// I held my nose and moved those two functions to here.
// -- bperrybap

// blinkLED() - blink builtin LED
// blinks a built in LED if there is one.
// returns zero if successful
//

// helper macros to turn on built in LED as some boards like the ESP8266 use
// active low LEDs

#if defined(ARDUINO_ARCH_ESP8266)
#define ledBuiltinOn() digitalWrite(LED_BUILTIN, LOW)
#define ledBuiltinOff() digitalWrite(LED_BUILTIN, HIGH)
#else
#define ledBuiltinOn() digitalWrite(LED_BUILTIN, HIGH)
#define ledBuiltinOff() digitalWrite(LED_BUILTIN, LOW)
#endif

int hd44780::blinkLED(int blinks)
{
#ifdef LED_BUILTIN
	pinMode(LED_BUILTIN, OUTPUT);
	// blink out error code
	for(int i = 0; i< blinks; i++)
	{
		ledBuiltinOn();
		delay(100);
		ledBuiltinOff();
		delay(250);
	}
	return(0);
#else
	// No built in LED, so do "nothing"
	if(blinks){} // "nop" if to eliminate warning, will be optimized out

	return(RV_ENOTSUP);
#endif
}

// fatalError() - loop & blink an error code
void hd44780::fatalError(int errcode)
{
	while(1)
	{
		blinkLED(errcode);	// blink LED if possible
		delay(1500);		// using delay() ensures watchdogs don't trip
	}
}
//============================================================================
