//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_pinIO.h - hd44780_pinIO i/o subclass for hd44780 library
//  Copyright (c) 2016  Bill Perry
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_pinIO is free software: you can redistribute it and/or
//  modify it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_pinIO is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_pinIO.
//  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements all the hd44780 library i/o methods to control an LCD based
// on the Hitachi HD44780 and compatible chipsets using direct Arduino
// pin connections.
//
// The API functionality provided by this library class is compatible
// with the functionality of the Arduino LiquidCrystal library.
//
//
// 2016.07.20  bperrybap - merged into hd44780 library
// 2016.04.01  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#ifndef hd44780_pinIO_h
#define hd44780_pinIO_h

#include <hd44780.h>
// For STUPID versions of gcc that don't hard error on missing header files
#ifndef hd44780_h
#error Missing hd44780.h header file
#endif

class hd44780_pinIO : public hd44780
{
public:
   // 4 bit mode constructor with optional backlight control
   hd44780_pinIO(uint8_t rs, uint8_t en,
                         uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
	                     uint8_t bl=0xff, uint8_t blLevel=0xff); // optional
private:
	// hd44780 class virtual i/o functions
	int  ioinit();
	void iosend(uint8_t value, hd44780::iosendtype type);
	void iosetBacklight (uint8_t dimvalue);

	// internal functions
	void write4bits(uint8_t value);
	void pulseEnable();

	// Arduino Pin information
	uint8_t _rs;		// hd44780 rs arduino pin
	uint8_t _rw;		// hd44780 rw arduino pin
	uint8_t _en;		// hd44780 en arduino pin
	uint8_t _d4;		// hd44780 d4 arduino pin
	uint8_t _d5;		// hd44780 d4 arduino pin
	uint8_t _d6;		// hd44780 d4 arduino pin
	uint8_t _d7;		// hd44780 d4 arduino pin
	uint8_t _bl;		// arduino pin to control backlight
	uint8_t _blLevel;	// backlight active control level HIGH/LOW
};
#endif
