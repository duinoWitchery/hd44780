//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_NTCUUserial.h - hd44780_NTCUUserial i/o subclass for hd44780 library
//  Copyright (c) 2013-2017  Bill Perry
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
// CU-U Series VFD, in serial mode. Serial mode must be enabled on the board by
// setting a jumper.
//
// This device has extended function set commands that are not directly
// supported by the library but can be used through the command() API
//
// The interface consists of sending 2 bytes serially for each hd44780
// data/cmd.
//  The first byte is a control byte which sets the RS signal
//  The second byte is the data.
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
	
	// supply pins for chip select, clock, and data
	hd44780_NTCUUserial(uint8_t cs, uint8_t clk, uint8_t data):
		_cs(cs), _clk(clk), _data(data)
	{ }
	
private:
	// ====================
	// === private data ===
	// ====================
	
	// Arduino Pin information
	
	const uint8_t _cs;	// arduino pin for chip select
	const uint8_t _clk;	// arduino pin for clock
	const uint8_t _data; // arduino pin for data i/o
	
	
	// ==================================================
	// === hd44780 i/o subclass virtual i/o functions ===
	// ==================================================
	//
	
	// ioinit() - Returns non zero if initialization failed.
	int ioinit()
	{
		delay(500);      // wait for device to power up

		// setup pins
		digitalWrite(_cs, HIGH);
		digitalWrite(_clk, HIGH);
		digitalWrite(_data, HIGH);

		pinMode(_cs, OUTPUT);
		pinMode(_clk, OUTPUT);
		pinMode(_data, OUTPUT);
		
		/*
		 * this device only runs in 8 bit mode
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
	// The dimmer value on the CUU is 2 bit value
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
		if(dimvalue)
		{
			devicewrite(0x30, false);
			// the dim value is 0-255 so devide by 64 to get a value 0-3
			devicewrite(3 - (dimvalue / 64), true);

			display();
		}
		else
		{
			noDisplay();
		}
		return(RV_ENOERR);
	}
	
	// ================================
	// === internal class functions ===
	// ================================
	
	// deviceread() - send control and read data byte from the device
	uint8_t deviceread(bool rs)
	{
		uint8_t	data = 0xfc + 2*rs;
		
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
		
		return data;
	}
	
	
	// devicewrite() - send control and data byte to the device
	void devicewrite(uint8_t data, bool rs)
	{
		uint8_t	x = 0xf8 + 2*rs;
		
		digitalWrite(_cs,LOW);	// select device
		
		digitalWrite(_clk, LOW);
		shiftOut(_data, _clk, MSBFIRST, x);
		
		digitalWrite(_clk, LOW);
		shiftOut(_data, _clk, MSBFIRST, data);
		
		digitalWrite(_cs,HIGH);	// deselect device
		
		delayMicroseconds(5);
	}
	
}; // end of class definition
#endif
