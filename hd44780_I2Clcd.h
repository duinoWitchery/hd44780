//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_I2Clcd.h - hd44780_I2Clcd library
//  Copyright (c) 2013-2016  Bill Perry
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780_I2Clcd library
//
//  hd44780_I2Clcd is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License.
//
//  hd44780_I2Clcd is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_I2Clcd.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements all the hd44780 library i/o methods to control an LCD based
// on the Hitachi HD44780 and compatible chipsets which have
// a native I2C interface rather than use a simple I2C i/o expander chip
// such as a PCF8574 or MCP23008
// Here is a example of such a device: 
// http://www.seeedstudio.com/wiki/images/0/03/JHD1214Y_YG_1.0.pdf
// This device uses address 0x3E
// NOTE:
//		This device needs pullups on the bus signals.
//		While it will usually work on an AVR platform without external pullups,
//		it doesn't always work.
// 
// PCF2119x chips use this protocol and slave addresses are 0x3A and 0x3B
// This is defined in the allocated i2c address table Group 7 
// It overlaps with the addresses for the PCF8574A
//
// Raystar RC1602B5-LLH-JWV is jumper settable to 0x3c, 0x3d, 0x3e, 0x3f
//
// Known addresses for this type of device:
// 0x3a
// 0x3b
// 0x3c
// 0x3d
// 0x3e
// 0x3f
//
// The interface consists of sending 2 bytes over I2C for each hd44780
// data/cmd. 
//  The first byte is a control byte which sets the RS signal
//  The second byte is the data.
//
// The I2C can only control the LCD and does not have the capability
// to control the backlight so the backlight will always remain on.
//
// Attempting to read from some of these devices will lockup the AVR Wire
// library.
//
// 2016.06.09  bperrybap - changed name to hd44780_I2Clcd
// 2016.06.08  bperrybap - removed pre IDE 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2014.02.15  bperrybap - changed to use hd44780 base class library
// 2013.11.15  bperrybap - initial creation (hd44780_IIClcd)
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ---------------------------------------------------------------------------
#ifndef hd44780_I2Clcd_h
#define hd44780_I2Clcd_h

#include <hd44780.h>

class hd44780_I2Clcd : public hd44780 
{
public:
	hd44780_I2Clcd(uint8_t  i2c_addr);
private:
	// Control byte bit values

 	// RS signal in HD44780 interface
	// set for data writes, clear for cmds
	// note: some datasheets call this A0 bit
	static const uint8_t I2Clcd_RS  = (1 << 6);

	// set if another control byte following data
	// clear if last control byte
	static const uint8_t  I2Clcd_CO = (1 << 7);


	// hd44780 class virtual i/o functions
	int  ioinit();
	void iosend(uint8_t value, hd44780::iosendtype type);

	uint8_t _Addr;             // I2C Address of the LCD
};
#endif
