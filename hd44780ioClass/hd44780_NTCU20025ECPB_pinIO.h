//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_NTCU20025ECPB_pinIO.h -
//  hd44780_NTCU20025ECPB_pinIO i/o subclass for hd44780 library
//  Copyright (c) 2019-2020  Bill Perry
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_NTCU20025ECPB_pinIO is free software: you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_NTCU20025ECPB_pinIO is distributed in the hope that it will be
//  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_NTCU20025ECPB_pinIO.
//  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements hd44780 library subclass i/o methods to control Noritake
// CU-U series displays like the Noritake CU20025ECPB-U1J or CU20025ECPB-W1J
// The Noritake CU20025ECPB display is a 20x2 line VFD display.
// It is not a hd44780 display but is nearly fully compatible with a hd44780
// display. The only real difference is the backlight control.
// So while this device is supported by the hd44780 library,
// the subclass will do its best to emulatate hd44780 functions and map
// them to equivalent functionality when possible.
//
// ---------------------------------------------------------------------------
// Note:
//	The CU20025 device is very compatible with a standard hd44780 display.
//	The only difference is that it provides dimming control of the pixels
//	whereas a standard hd44780 display does not have dimming control, but
//	often has a backlight that is controlled with h/w outside of the hd44780
//	chipset.
//
// Datasheets:
//
// Full Data sheet: (requires registration):
// https://www.noritake-elec.com/user/datasheet?SCID=147&VFDID=116&PSID=5908e690167775b2f6241569268c741e&FMTP=2#
// https://www.noritake-elec.com/products/vfd-display-module/character-display/cu-u-series
// https://www.noritake-elec.com/products/vfd-display-module/lineup?cu-u
//
// Summary Datasheet:
// https://media.digikey.com/pdf/Data%20Sheets/Noritake%20PDFs/CU20025ECPB-U1J.pdf
// http://www.farnell.com/datasheets/49448.pdf
// https://media.digikey.com/pdf/Data%20Sheets/Noritake%20PDFs/CU20025ECPB-W1J.pdf
//
// AppNote:
// http://www.northstardatasheet.com/datasheet/APN1100-01.pdf
// https://www.datasheet.directory/index.php?title=Special:PdfViewer&url=https%3A%2F%2Fdatasheet.iiic.cc%2Fdatasheets-0%2Fnoritake%2FCU20025ECPB-W1J.pdf
//
// CU20025 pinout:
//  1 - LCD gnd
//  2 - VCC (5v)
//  3 - not connected (assuming JP1 pins 1 and 2 are not shorted)
//  4 - RS Register Select (rs)
//  5 - Read/Write
//  6 - Enable (en)
//  7 - Data 0 (db0) ----
//  8 - Data 1 (db1)     |-------- Not used in 4 bit mode
//  9 - Data 2 (db2)     |
// 10 - Data 3 (db3) ----
// 11 - Data 4 (db4)
// 12 - Data 5 (db5)
// 13 - Data 6 (db6)
// 14 - Data 7 (db7)
//
// ---------------------------------------------------------------------------
//
// The API functionality provided by this library class is compatible
// with the functionality of the Arduino LiquidCrystal library.
//
//
// 2019.11.23  bperrybap - initial creation from hd44780_pinIO i/o class
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#ifndef hd44780_NTCU20025ECPB_pinIO_h
#define hd44780_NTCU20025ECPB_pinIO_h

#include <hd44780.h>
// For STUPID versions of gcc that don't hard error on missing header files
#ifndef hd44780_h
#error Missing hd44780.h header file
#endif
#include <pins_arduino.h> // to get PWM detection macros

class hd44780_NTCU20025ECPB_pinIO : public hd44780
{
public:
// ====================
// === constructors ===
// ====================

// 4 bit mode constructor without r/w control
hd44780_NTCU20025ECPB_pinIO(uint8_t rs,  uint8_t en,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) :
	hd44780(20,2), _rs(rs), _rw(0xff), _en(en),
	_d4(d4), _d5(d5), _d6(d6), _d7(d7) { }

// 4 bit mode constructor with r/w control
hd44780_NTCU20025ECPB_pinIO(uint8_t rs,  uint8_t rw, uint8_t en,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7) :
	hd44780(20,2), _rs(rs), _rw(rw), _en(en),
	_d4(d4), _d5(d5), _d6(d6), _d7(d7) { }

private:
// ====================
// === private data ===
// ====================

// Arduino Pin information
uint8_t _rs;		// hd44780 rs arduino pin
uint8_t _rw;		// hd44780 rw arduino pin
uint8_t _en;		// hd44780 en arduino pin
uint8_t _d4;		// hd44780 d4 arduino pin
uint8_t _d5;		// hd44780 d4 arduino pin
uint8_t _d6;		// hd44780 d4 arduino pin
uint8_t _d7;		// hd44780 d4 arduino pin

// ==================================================
// === hd44780 i/o subclass virtual i/o functions ===
// ==================================================

// ioinit() - initialize the h/w
int ioinit()
{
	// initialize Arduino pins used for hd44780 signals
	// NOTE:
	// _rw and _en pins are set to low even though they should
	// default to this from a an initial reset.
	// the reason is it is possible initilization is called without
	// without resetting the processor. In that case the state of the
	// bit in the port register would be unknown, so it is safest to set
	// to the desired state (LOW).

	pinMode(_rs, OUTPUT);

	if(_rw != 0xff)
	{
		pinMode(_rw, OUTPUT);
		// if r/w is used it be will set to be low,
		// it will only be turned hi during a read and
		// then set back to low. This offers a slight boost
		// for writes since normally reads are not done very often
		digitalWrite(_rw, LOW);
	}

	pinMode(_en, OUTPUT);
	digitalWrite(_en, LOW);

	pinMode(_d4, OUTPUT);
	pinMode(_d5, OUTPUT);
	pinMode(_d6, OUTPUT);
	pinMode(_d7, OUTPUT);
  
	return(hd44780::RV_ENOERR); // all is good
}

// ioread(type) - read a byte from LCD DDRAM
//
// returns:
// 	success:  8 bit value read
// 	failure: negative value: reading not supported
//
int ioread(hd44780::iotype type) 
{
uint8_t data = 0;

	// check if r/w control supported
	if(_rw == 0xff)
		return(hd44780::RV_ENOTSUP);

	waitReady();		// ensure previous instruction finished

	// put all the LCD data pins into input mode.
	pinMode(_d4, INPUT);
	pinMode(_d5, INPUT);
	pinMode(_d6, INPUT);
	pinMode(_d7, INPUT);

	// set RS based on type of read (data or status/cmd)
	if(type == hd44780::HD44780_IOdata) 
	{
		digitalWrite(_rs, HIGH); // RS high to access data reg
	}
	else
	{
		digitalWrite(_rs, LOW); // RS LOW to access status/cmd reg
	}

	// r/w  HIGH for reading
	digitalWrite(_rw, HIGH);

	// raise E to allow reading the data.
	digitalWrite(_en, HIGH);

	// allow for hd44780 tDDR (Data delay time) before reading data
	// this could be much shorter but this is portable for all CPUs.
	// and it will ensure that hd44780 PWEH timing is honored as well.
	delayMicroseconds(1);

	// read pins for d4-d7 into upper nibble of byte
	if(digitalRead(_d4) == HIGH)
		data |= (1 << 4);

	if(digitalRead(_d5) == HIGH)
		data |= (1 << 5);

	if(digitalRead(_d6) == HIGH)
		data |= (1 << 6);

	if(digitalRead(_d7) == HIGH)
		data |= (1 << 7);

	// lower E after reading nibble
	digitalWrite(_en, LOW);

	// allow for hd44780 1/2 of tcycE (Enable cycle time)
	// this could be much shorter but this is portable for all CPUs.
	delayMicroseconds(1);
	
	// raise E to allow reading the lower nibbly of the byte
	digitalWrite(_en, HIGH);

	// allow for hd44780 tDDR (Data delay time) before reading data
	// this could be shorter but this is portable for all CPUs.
	// and it will ensure that hd44780 PWEH timing is honored as well.
	delayMicroseconds(1);

	// read pins for d4-d7 into upper nibble of byte
	if(digitalRead(_d4) == HIGH)
		data |= (1 << 0);

	if(digitalRead(_d5) == HIGH)
		data |= (1 << 1);

	if(digitalRead(_d6) == HIGH)
		data |= (1 << 2);

	if(digitalRead(_d7) == HIGH)
		data |= (1 << 3);

   
	// lower E after reading nibble
	digitalWrite(_en, LOW);

	//
	// put all pins back into state for writing to LCD
	//

	// put all the LCD data pins into input mode.
	pinMode(_d4, OUTPUT);
	pinMode(_d5, OUTPUT);
	pinMode(_d6, OUTPUT);
	pinMode(_d7, OUTPUT);

	// r/w  LOW for Writing
	digitalWrite(_rw, LOW);

	return(data);
}


// iowrite(type, value) - send either a command or data byte to lcd
// returns zero on success, non zero on failure
int iowrite(hd44780::iotype type, uint8_t value)
{
	if(type == hd44780::HD44780_IOdata)
  		digitalWrite(_rs, HIGH);
	else
  		digitalWrite(_rs, LOW);
  
	// "4 bit commands" are special.
	// They are used only during initalization and
	// are used to reliably get the LCD and host in nibble sync
	// with each other and to get the LCD into 4 bit mode.
	// 8 bit host interfaces don't have to worry about this,
	// but 4 bit host interfaces must only send the upper nibble.

	// note:
	// waitReady() is explicitly called as late as possible *after* the Arduino
	// pins for the LCD control and data signals are set to allow
	// overhead of the digitalWrite() calls to be hidden under execution time.
	
	write4bits(value>>4);	// setup uppper nibble d4-d7 lcd pins
	waitReady();			// ensure previous instruction finished
	pulseEnable();			// send upper nibble to LCD

	// send lower nibble if not a 4 bit command
	// (sends nibble for both data and "normal" commands)
	if (type != hd44780::HD44780_IOcmd4bit )
	{
		write4bits((value & 0x0F));// setup lower nibble on d4-d7 lcd pins
		pulseEnable();				// send lower nibble to LCD
	}
	return(hd44780::RV_ENOERR); // it never fails
}

// iosetBacklight() - set backlight brightness
// Since this display does not use a backlight but can control pixel intensity
// use the backlight brightness to set the pixel intensity.
//
// Note:
// The brightness control byte on the CU20025 is 2 bit value 
// 00 is 100%
// 01 is  75%
// 10 is  50%
// 11 is  25%
// To handle 0% the display will be turned off
//
// dimvalue levels will be mapped as follows:
//       0 -  0% / display off
// 01 - 63 - 25%
// 64 -127 - 50%
// 128-191 - 75%
// 192-255 -100%

int iosetBacklight(uint8_t dimvalue)
{
	if(dimvalue)
	{
		// brightness is set by sending a brightness control byte after a function set
		// Data which follows the FunctionSet command is considered as brightness data.
		
		// send a function set,
		// this is effectively a NOP, since it will set mode to what it already is
		// Data which follows 
		command(HD44780_FUNCTIONSET | _displayfunction);  

		// send the brightness control byte to set brightness level
		_write(3 - dimvalue / 64);

		display(); // turn display on in case it was off

	}
	else
	{
		noDisplay(); // turn off display for 0% brightness
	}

	// can't tell if it really worked with pin i/o, so say it always worked.
	return(hd44780::RV_ENOERR); // it never fails
}

// ================================
// === internal class functions ===
// ================================

// write4bits() - set the 4 hd44780 data lines
void write4bits(uint8_t value)
{
	// write the bits on the LCD data lines but don't send the data
	if(value & 1)
		digitalWrite(_d4, HIGH);
	else
		digitalWrite(_d4, LOW);

	if(value & 2)
		digitalWrite(_d5, HIGH);
	else
		digitalWrite(_d5, LOW);

	if(value & 4)
		digitalWrite(_d6, HIGH);
	else
		digitalWrite(_d6, LOW);

	if(value & 8)
		digitalWrite(_d7, HIGH);
	else
		digitalWrite(_d7, LOW);
}

// pulseEnable() - toggle en to send data to hd44780 module
void pulseEnable(void)
{
	digitalWrite(_en, HIGH);
#if defined (ARDUINO_ARCH_ESP8266)
	// the extra delay here is not for the LCD, it is to allow signal lines time
	// to settle when using 3v esp modules with 5v LCDs.
	// 3v outputs on 5v inputs is already a bit out of spec and
	// without this, the slew rate isn't fast enough to get "reliable"
	// signal levels
	// while it isn't needed for 3v LCDs, the time penalty isn't much
	// to always do it.
	delayMicroseconds(2);    // enable pulse must be >450ns
#else
	delayMicroseconds(1);    // enable pulse must be >450ns
#endif
	digitalWrite(_en, LOW);
}

}; // end of class definition
#endif
