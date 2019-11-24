//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_NTCUUserial.h - hd44780_NTCUUserial i/o subclass for hd44780 library
//  Copyright (c) 2013-2019  Bill Perry
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_NTCUUserial is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_NTCUUserial is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_NTCUUserial.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements all the hd44780 library i/o methods to control a Noritake
// CU-U Series VFD, in serial mode. Some CU-U boards are serial-only, some have
// a jumper that must be set to enable serial mode, and some do not support
// serial communication. See the application note for more information.
//
// This device has extended function set commands that are not directly
// supported by the library but can be used through the command() API
//
// The device uses a kind of 3-wire SPI for communication.
// Pinout:
// 1 VCC
// 2 SI/SO (DATA)
// 3 GND
// 4 STB (CS)
// 5 SCK (CLK)
// 6 NC
//
// The interface consists of sending 2 bytes serially for each hd44780
// data/cmd.
//  The first byte is a control byte which sets the RS signal
//  The second byte is the data.
//
// The application note for CU-U series boards is here:
//     https://www.noritake-elec.com/includes/documents/brochure/CU-U_Application_Note.pdf
// Datasheets for specific boards, code samples, and more can be found here:
//     https://www.noritake-elec.com/products/vfd-display-module/character-display/cu-u-series
//
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ---------------------------------------------------------------------------
#ifndef hd44780_NTCUUserial_h
#define hd44780_NTCUUserial_h


class hd44780_NTCUUserial : public hd44780
{
public:
	// ====================
	// === constructors ===
	// ====================
	
	// no parameters: use h/w spi signals
	// Note: This doesn't work for Leonardo since SS is not available and only drives an LED
	hd44780_NTCUUserial() : _cs(SS), _clk(SCK), _data(MOSI) { }
	
	// supply alternate chip/slave select
	// will use h/w spi with alternate chip select
	hd44780_NTCUUserial(int cs) : _cs(cs), _clk(SCK), _data(MOSI) { }
	
	// supply pins for chip select, clock, and data
	hd44780_NTCUUserial(uint8_t cs, uint8_t clk, uint8_t data) : _cs(cs), _clk(clk), _data(data) { }
	
private:
	// ====================
	// === private data ===
	// ====================
	
	// Arduino Pin information
	
	const uint8_t _cs;   // arduino pin for chip select
	const uint8_t _clk;  // arduino pin for clock
	const uint8_t _data; // arduino pin for data i/o
	
	
	// Control byte values
	
	static const uint8_t CUU_startByte = 0xf8;
	static const uint8_t CUU_RS = (1 << 1); // RS bit. set for data, clear for command
	static const uint8_t CUU_RW = (1 << 2); // RW bit. set for read, clear for write
	
	
	// ==================================================
	// === hd44780 i/o subclass virtual i/o functions ===
	// ==================================================
	//
	
	// ioinit() - Returns non zero if initialization failed.
	int ioinit()
	{
		// App note says "Wait at least 260 ms after VCC > 4.75VDC"
		// See also the delay in hd44780::begin()
		// In practice little or no delay at all may work fine
		delay(260);
		
		// setup pins
		digitalWrite(_cs, HIGH);
		digitalWrite(_clk, HIGH);
		digitalWrite(_data, HIGH);
		
		pinMode(_cs, OUTPUT);
		pinMode(_clk, OUTPUT);
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

		/*
		 * Serial interface of this device is always 8 bit
		 */
		_displayfunction = HD44780_8BITMODE;
		
		return(hd44780::RV_ENOERR); // all is good
	}
	
	
	// ioread(type) - read a byte from VFD DDRAM
	//
	// returns 8 bit value read
	//
	int ioread(hd44780::iotype type)
	{
		uint8_t data = 0;
		
		if(type == hd44780::HD44780_IOdata)
		{
			data = deviceread(true);
		}
		else
		{
			data = deviceread(false);
		}
		
		return data;
	}
	
	
	//
	// iowrite(type, value) - send either a command or data byte to lcd
	//
	// returns zero on success, non zero on failure
	//
	int iowrite(hd44780::iotype type, uint8_t value)
	{
		if(type == hd44780::HD44780_IOdata)
		{
			devicewrite(value, true);
		}
		else
		{
			devicewrite(value, false);
		}
		
		return(hd44780::RV_ENOERR);
	}
	
	
	// iosetBacklight()  - set backlight brightness
	// Since this display does not use a backlight but can control pixel intensity
	// use the backlight brightness to set the pixel intensity.
	//
	// Note:
	// The dimmer value on the CU-U Series is a 2 bit value
	// 00 is 100%
	// 01 is 75%
	// 10 is 50%
	// 11 is 25%
	//
	// Because of this, an API dimvalue of 0 does not turn off the display
	// so the code must check for that and explicitly turn off the display if the
	// API dimvalue is zero.
	int iosetBacklight(uint8_t dimvalue)
	{
		int status = RV_ENOERR;
		
		if(dimvalue)
		{
			// Data which follows the FunctionSet command is considered as brightness data.
			command(HD44780_FUNCTIONSET | _displayfunction);
			// the dim value is 0-255 so divide by 64 to get a value 0-3
			if (_write(3 - dimvalue / 64) == 0)
				status = RV_EIO;
			
			display();
		}
		else
		{
			noDisplay();
		}
		return(status);
	}
	
	// ================================
	// === internal class functions ===
	// ================================
	
	// deviceread() - send control and read data byte from the device
	uint8_t deviceread(bool rs)
	{
		uint8_t	data = CUU_startByte | CUU_RW | (CUU_RS * rs);
		
#if defined(SPI_HAS_TRANSACTION)
		if((_data == MOSI) && (_clk == SCK))
			SPI.end();
#endif

		digitalWrite(_cs,LOW);	// select device
		
		digitalWrite(_clk, LOW);
		shiftOut(_data, _clk, MSBFIRST, data);
		
		pinMode(_data, INPUT);
		delayMicroseconds(1);
		
		digitalWrite(_clk, LOW);
		data = shiftIn(_data, _clk, MSBFIRST);
		
		digitalWrite(_cs,HIGH);	// deselect device
		
		pinMode(_data, OUTPUT);
		
		delayMicroseconds(5);
		
#if defined(SPI_HAS_TRANSACTION)
		if((_data == MOSI) && (_clk == SCK))
			SPI.begin();
#endif

		return data;
	}
	
	
	// devicewrite() - send control and data byte to the device
	void devicewrite(uint8_t data, bool rs)
	{
		uint8_t	startByte = CUU_startByte | (CUU_RS * rs);
		
		// use h/w spi if we can
#if defined(SPI_HAS_TRANSACTION)
		if((_data == MOSI) && (_clk == SCK))
		{
			// NOTE: App note says min 500ns clock cycle, so 2Mhz is max, 4Mhz seems to work.
			SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
			digitalWrite(_cs, LOW);  // select device
			SPI.transfer(startByte);
			SPI.transfer(data);
			delayMicroseconds(1);    // must delay at least 500ns before de-selecting
			digitalWrite(_cs, HIGH); // deselect device
			SPI.endTransaction();
		}
		else
#endif
		{
			digitalWrite(_cs, LOW);  // select device
			
			digitalWrite(_clk, LOW);
			shiftOut(_data, _clk, MSBFIRST, startByte);
			
			digitalWrite(_clk, LOW);
			shiftOut(_data, _clk, MSBFIRST, data);
			
			delayMicroseconds(1);    // must delay at least 500ns before de-selecting
			digitalWrite(_cs, HIGH); // deselect device
			
			delayMicroseconds(5);
		}
	}
	
}; // end of class definition
#endif
