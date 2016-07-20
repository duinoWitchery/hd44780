//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_I2Clcd.cpp - hd44780_I2Clcd library
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
//  the Free Software Foundation version 3 of the License.
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
//
// 2014.06.09  bperrybap - changed name to hd44780_I2Clcd
// 2016.06.08  bperrybap - removed pre 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2014.02.15  bperrybap - changed to use hd44780 bse class library
// 2013.06.01  bperrybap - initial creation (hd44780_IIClcd)
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

// include the Wire.h header
#include <Wire.h>
#include "hd44780_I2Clcd.h"

// CONSTRUCTOR
// ---------------------------------------------------------------------------
hd44780_I2Clcd::hd44780_I2Clcd(uint8_t i2c_addr)
{
	_Addr = i2c_addr; // Save away the device address in object
}


//
// ioinit
// Returns non zero if initialization failed.
int hd44780_I2Clcd::ioinit()
{
int status = 0;

	/*
	 * First, initialize the i2c (Wire) library.
	 * This really shouldn't be here 
	 * because Wire.begin() should only be called once, but
	 * unfortunately, there is no way to know if anybody
	 * else has called this.
	 * I believe that it is unreasonable to require the the user
	 * sketch code to do it, because all that should change between
	 * interfaces should be the constructor
	 * So we go ahead and call it here.
	 */
	Wire.begin();

	/*
	 * Check to see if the device is responding
	 */
	Wire.beginTransmission(_Addr);
	status = Wire.endTransmission();

	/*
	 * this device only runs in 8 bit mode
	 */
	_displayfunction = HD44780_8BITMODE;
   return ( status );
}

//
// iosend - write either command or data
void hd44780_I2Clcd::iosend(uint8_t value, iosendtype type) 
{
uint8_t ctlbyte;
   
	/*
	 * No need to look for 4 bit commands as all bits are already in
	 * proper upper nibble and unsued bits are zero.
	 * This allows the special 4 bit commands to be sent "As is" when
	 * using an 8 bit interface - which is what this i2c LCD uses.
	 */

	if(type == hd44780::HD44780_IOdata)
	{
		ctlbyte = I2Clcd_RS; // control byte with RS and no continue
	}
	else
	{
		ctlbyte = 0; // control byte with no RS and no continue
	}

	
	/*
	 * ensure that previous LCD instruction finished.
	 * There is a 25us offset since there will be at least 1 byte
	 * (the i2c address)  transmitted over i2c before the next LCD instruction
	 * information could get to the LCD.
	 * At 400Khz (max rate supported by the LCD) 8 bits plus start and stop
	 * bits is 25us.
	 * So there is at least 25us of time overhead in the physical interface.
	 */

	waitReady(-25);

	/*
	 * Send the next LCD instruction
	 */

	Wire.beginTransmission(_Addr);
	Wire.write(ctlbyte);	// send control byte
	Wire.write(value);		// send data/cmd
	Wire.endTransmission();
}
