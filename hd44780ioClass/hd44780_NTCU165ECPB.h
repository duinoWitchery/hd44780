//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_NTCU165ECPB.h - hd44780_NTCU165ECPB i/o subclass for hd44780 library
//  Copyright (c) 2016-2020  Bill Perry
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_NTCU165ECPB is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_NTCU165ECPB is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_NTCU165ECPB.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements hd44780 library subclass i/o methods to control a
// Noritake CU165ECBP-T2J display.
// The Noritake CU165ECBP-T2J display is a 16x1 line VFD display.
// It is not a hd44780 display. So while this device is supported by hd44780,
// it will not be capable of supporting all the hd44780 capabilities.
// The subclass will do its best to emulatate hd44780 functions and map
// them to equivalent functionality when possible but certain capabilities
// will not be possible.
//
// Datasheet can be found here:
//	http://www.blog.zapro.dk/wp-content/CU165.pdf
// Full datasheet/release notes:
//	http://www.blog.zapro.dk/wp-content/CU165ECPB_T2J_specification.pdf
//
// The device uses SPI for communication.
// Pinout:
// 1 Vcc
// 2 GND
// 3 /CS  (SS)
// 4 CLK  (SCK)
// 5 DATA (MOSI)
//
// ---------------------------------------------------------------------------
// history
//
// 2017.12.23  bperrybap - added support LCD API 1.0 init()
// 2016.08.22  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ---------------------------------------------------------------------------
#ifndef hd44780_NTCU165ECPB_h
#define hd44780_NTCU165ECPB_h

class hd44780_NTCU165ECPB : public hd44780 
{
public:
// ====================
// === constructors ===
// ====================

// no parameters: use h/w spi signals
// Note: This doesn't work for Leonardo since SS is not available and only drives an LED
hd44780_NTCU165ECPB() : hd44780(16,1), _cs(SS), _clk(SCK), _data(MOSI) { }

// supply alternate chip/slave select
// will use h/w spi with alternate chip select
hd44780_NTCU165ECPB(int cs) : hd44780(16,1), _cs(cs), _clk(SCK), _data(MOSI) { }

// supply pins for chip select, clock, and data
hd44780_NTCU165ECPB(int cs, int clock, int data) : hd44780(16,1), _cs(cs), _clk(clock), _data(data) { }


private:
// ====================
// === private data ===
// ====================

// Arduino Pin information

const uint8_t _cs;	// arduino pint for slave select (chip select)
const uint8_t _clk;	// arduino pin for clock
const uint8_t _data;  // arduino pin for master out slave in (data)

// other internal variables
uint8_t _cgmode; // set when in cgramaddr mode
uint8_t _udf[5]; // udf buffer area
uint8_t _cgindx; // index into cgdata (will be 0 to 7)
uint8_t _cgchar; // custom character (0 to 7, this board goes to f)


// device commands

static const uint8_t CMD_SETDISPLAYLEN = 0x00;	// lower 3 bits sets total digits/cols
                                        // to display
										// 0 is 9 and 7 means all 16 digits/cols

static const uint8_t CMD_SETBRIGHTNESS = 0x08; // lower 3 bits sets brigtness 
                                        // 0 is 1/16 and 7 is 14/16 max
static const uint8_t CMD_SETDIGSCAN160 = 0xF6; // set digit scan time to 160us
static const uint8_t CMD_SETDIGSCAN320 = 0xF7; // set digit scan time to 320us
static const uint8_t CMD_SETDIGPOINTER = 0xE0; // set column, lower 4 bits sets column
static const uint8_t CMD_NOAUTOINCREMENT = 0xF0; // no auto advance to next column
static const uint8_t CMD_AUTOINCREMENT   = 0xF1; // auto advance to next column

static const uint8_t CMD_DISPLAYOFF   = 0xF0; // all digits/dots off
static const uint8_t CMD_DISPLAYON    = 0xF1; // normal display mode
static const uint8_t CMD_DISPLAYALLON = 0xF3; // all digits, all dots on

static const uint8_t CMD_STOREUDF = 0xFC; // begin user defined font (custom char)
                                   // There are 16 custom chars, they are
                                   // accessed/printed using 0x90-0x9F
                                   // they are programmed using locations 0-F
                                   // Each character glyph is 5 bytes.
                                   // Each byte is vertical 7 pixels with the
                                   // MSB at the top and the LSB dropped.
                                   // The first byte is the left most column of
                                   // the glyph. 
                                   // full command data is 7 bytes:
                                   // 0xFC,<location (0-f)> 5 bytes of data

// start of custom user defined font (UDF) characters
// There are 16 UDF characters that start here
static const uint8_t UDF_BASE = 0x90; 


// ==================================================
// === hd44780 i/o subclass virtual i/o functions ===
// ==================================================
//
// Note:
// It is not possible to read from the device.
// so ioread(), and & iostatus()
// will not be defined and will use the defaults in
// hd44780 which will return an apropriate error status

// ioinit() - Returns non zero if initialization failed.
int ioinit()
{
int status = 0;

	// setup SPI signals
	digitalWrite(_cs, HIGH);
	pinMode(_cs, OUTPUT);

	pinMode(_clk, OUTPUT);
	digitalWrite(_clk, LOW);
	pinMode(_data, OUTPUT);

// check to see if SPI has beginTransaction() and endTransaction()
// this was added in IDE 1.06 and so if using an IDE older than that,
// H/W spi will not be used
// This requires that <SPI.h> be included prior to this header 
#if defined(SPI_HAS_TRANSACTION)
	// check to see if h/w spi can be used
	if((_data == MOSI) && (_clk == SCK))
		SPI.begin();
#endif

	// maybe look at cols to see how many should be turned on?
	devicewrite(CMD_SETDISPLAYLEN|0x7);	// show all 16 characters

	devicewrite(CMD_SETDIGSCAN320);		// set digit scan time to 320us
	devicewrite(CMD_AUTOINCREMENT);	// set auto advance to next column on writes

// for now turn on display and "backlight"
// clear and set to "home" address position
	devicewrite(CMD_DISPLAYON);
	devicewrite(CMD_SETBRIGHTNESS | 0x7); // max brightness
	device_clear();

	return ( status );
}

//
// iowrite() - send either a command or data byte to lcd
// returns zero on success, non zero on failure
//
// Things are a bit tricky here since this is not a hd44780 device.
// The code must look at the commands and emulate them or drop them
//
int iowrite(hd44780::iotype type, uint8_t value) 
{

	/*
	 * waitReady() is not called as this module does not need any execution time
	 * delays even when using a 4mHz h/w clock.
	 */
   
	/*
	 * 4 bit commands are part of low level hd44780 initalization.
	 * since this is not a hd44780 display, just throw them away
	 * and say they "worked"
	 */

	if(type == hd44780::HD44780_IOcmd4bit)
		return(0);

	if(type == hd44780::HD44780_IOcmd)
	{
		return(hd44780cmd(value));
	}

	if(_cgmode)
	{
		// in cgram mode so have to process custom character data.
		// hd44780 font data is stored horizontally in 8 bytes
		// with each byte being 1 row of dots
		// with upper 3 bits of each byte ignored.
		// first byte of the 8 bytes is upper row of dots.
		// Noritake UDF format is vertical columns of 5 bytes with 
		// the LSB of each byte ignored as it only has 5x7 dots.
		// the first byte is the right most column of 7 dots of the font.
		//
		// Because of this mismatch, the bits & bytes have to be flipped
		// around and stored in the 5 UDF bytes until the full 8 hd44780 font
		// bytes come in. Once all 8 hd44780 font bytes are in,
		// the 5 byte UDF font definition can be pushed to the device.

		for(int i = 0; i < 5; i++)
		{
			if( (value & (1<< i)))
			{
				_udf[4-i] |= (1 << (7- _cgindx));
			}
			else
			{
				_udf[4-i] &= ~(1 << (7-_cgindx));
			}
		}
		_cgindx++;
		if(_cgindx > 7) // is this the last of the 8 bytes?
		{
         	// 0xFC,<location (0-f)> 5 bytes of data
			devicewrite(CMD_STOREUDF);
			devicewrite(_cgchar);
			for(int i = 0; i < 5; i++)
			{
				devicewrite(_udf[i]);
			}

			// now send it again for the 8-f version
			// to emulate the hd44780 custom chars
			// that duplicate 0-7 at 8-f
			devicewrite(CMD_STOREUDF);
			devicewrite(_cgchar+8);
			for(int i = 0; i < 5; i++)
			{
				devicewrite(_udf[i]);
			}

			// note, cgmode is still in effect.
			// the UDF/font byte index is reset back to 0,
			// and the cgchar location is bumped.
			// this is to allow for the case when the code may
			// be attempting to program more than a single character at once.
			_cgindx = 0;
			_cgchar++;
			_cgchar &= 0xf;
		}
		return(0);
	}

	// upper 32 bytes are for commands so drop chars that are in the 0xE0 to 0xFF range
	if(value >= 0xE0)
	{
		return(0);
	}

	// check for hd44780 custom font characters,
	// The hd44780 character code must be mapped to the UDF characters
	// which start at a different location.
	if(value < 16)
	{
		value += UDF_BASE;
	}

	devicewrite(value);
	return(0);
}

// iosetBacklight()  - set backlight brightness
// Since this display does not use a backlight but can control pixel intensity
// use the backlight brightness to set the pixel intensity.
//
// Note:
// The dimmer value on the CU165 is 3 bit value 
// other than the 000 value it looks like the value is mapped to a 4 bit
// value by adding a 0 msb.
// 000 is 1/16
// 001 is 2/16
// 010 is 4/16
// 011 is 6/16
// 100 is 8/16
// 101 is 10/16
// 110 is 12/16
// 111 is 14/16
//
// Becauswe of this, an API dimvalue of 0 does not turn off the display pixels
// so the code must check for that and explicitly turn off the display if the
// API dimvalue is zero.
int iosetBacklight(uint8_t dimvalue) 
{
	if(dimvalue)
	{
		// the dim value is 0-255 so devide by 32 to get a value 0-7
		devicewrite(CMD_SETBRIGHTNESS | (dimvalue/32));
		devicewrite(CMD_DISPLAYON);
	}
	else
	{
		devicewrite(CMD_DISPLAYOFF);
	}
	return(RV_ENOERR);
}

// ================================
// === internal class functions ===
// ================================

// devicwrite() - send a byte to the device
void devicewrite(uint8_t value)
{

	// use h/w spi if we can
#if defined(SPI_HAS_TRANSACTION)
	if((_data == MOSI) && (_clk == SCK))
	{
		// NOTE: spec says 2Mhz is max, 4Mhz seems to work.
		SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
		digitalWrite(_cs,LOW);	// select device
		SPI.transfer(value);
		delayMicroseconds(1);	// must delay at least 130ns before de-selecting
	 	digitalWrite(_cs,HIGH);	// deselect device
		SPI.endTransaction();
	}
	else
#endif
	{
		digitalWrite(_cs,LOW);	// select device
		shiftOut(_data, _clk, MSBFIRST, value);
		delayMicroseconds(1);	// must delay at least 130ns before de-selecting
	 	digitalWrite(_cs,HIGH);	// deselect device
	}
}

// hd44780cmd() - emulate hd44780 commands best we can
// for now, silently drop unsupported commands
int hd44780cmd(uint8_t value)
{
	// detect the hd44780 command
	if(value & HD44780_SETDDRAMADDR)
	{
		_cgmode = 0; // back to ddram mode
		devicewrite(CMD_SETDIGPOINTER | (value & 0xf));
	}
	else if(value & HD44780_SETCGRAMADDR)
	{
		_cgmode = 1; // enter cgram mode
		_cgindx = 0;
		_cgchar = (value >> 3) & 0x7; // for now limmit to 8 custom like hd44780
	}
	else if(value & HD44780_FUNCTIONSET)
	{
		// not supported
	}
	else if(value & HD44780_CURDISPSHIFT)
	{
		// not supported
	}
	else if(value & HD44780_DISPLAYCONTROL)
	{
		if(value & HD44780_DISPLAYON)
			devicewrite(CMD_DISPLAYON);
		else
			devicewrite(CMD_DISPLAYOFF);
	}
	else if(value & HD44780_ENTRYMODESET)
	{
		// not supported
	}
	else if(value & HD44780_RETURNHOME)
	{
		_cgmode = 0; // back to ddram mode
		devicewrite(CMD_SETDIGPOINTER);
	}
	else if(value & HD44780_CLEARDISPLAY)
	{
		_cgmode = 0; // back to ddram mode
		device_clear();
	}
	
	return(0);
}

void device_clear()
{
	devicewrite(CMD_SETDIGPOINTER);
	for(uint8_t col = 0; col < 16; col++)
	{
		devicewrite(' ');
	}
	devicewrite(CMD_SETDIGPOINTER);
}

}; // end of class definition
#endif
