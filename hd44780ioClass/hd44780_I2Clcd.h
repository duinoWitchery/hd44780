//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_I2Clcd.h - hd44780_I2Clcd i/o subclass for hd44780 library
//  Copyright (c) 2013-2020  Bill Perry
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
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
// Here is a example of such a device: 
// https://seeeddoc.github.io/Grove-LCD_RGB_Backlight/res/JHD1214Y_YG_1.0.pdf
// This device uses address 0x3E and incorrectly responds to read requests
// It overlaps with the addresses for the PCF8574A
// It will lock up module and/or the AVR i2c h/w and require a power cycle to reset.
// NOTE:
//		This device needs pullups on the bus signals.
//		While it will usually work on an AVR platform without external pullups,
//		it doesn't always work.
// 
// PCF2119x chips use this protocol and slave addresses are 0x3A and 0x3B
// This is defined in the allocated i2c address table Group 7 
// It overlaps with the addresses for the PCF8574A
// This device has extended function set commands that are not directly
// supported by the library but can be used through the command() API
//
// PCF2116 chips use this protocol and slave addresses are 0x3A and 0x3B
// It overlaps with the addresses for the PCF8574A
//
// Raystar RC1602B5-LLH-JWV is jumper settable to 0x3c, 0x3d, 0x3e, 0x3f
// and appears to only be able to be written to.
// It overlaps with the addresses for the PCF8574A
//
// Known addresses for this type of device:
// 0x3a PCF2116/PCF2119x
// 0x3b PCF2116/PCF2119x
// 0x3c Raystar
// 0x3d Raystar
// 0x3e Raystar
// 0x3f Raystar
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
// 2018.08.06  bperrybap - removed TinyWireM work around (TinyWireM was fixed)
// 2017.05.12  bperrybap - now requires IDE 1.0.1 or newer
//                         This is to work around TinyWireM library bugs
// 2016.12.26  bperrybap - added auto i2c address location
// 2016.09.08  bperrybap - changed param order of iowrite() to match ioread()
// 2016.08.06  bperrybap - changed iosend() to iowrite()
// 2016.07.27  bperrybap - added return status for iosend()
// 2017.07.21  bperrybap - merged all class code into header
// 2017.07.20  bperrybap - merged into hd44780 library
// 2016.06.09  bperrybap - changed name to hd44780_I2Clcd
// 2016.06.08  bperrybap - removed pre IDE 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2014.02.15  bperrybap - changed to use hd44780 base class library
// 2013.06.01  bperrybap - initial creation (hd44780_IIClcd)
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ---------------------------------------------------------------------------
#ifndef hd44780_I2Clcd_h
#define hd44780_I2Clcd_h

// A bug in TinyWireM is that requestFrom() returns incorrect status
// so its return status can't be used. Instead the code will check the return
// from Wire.read() which will return -1 if there no data was transfered.

#if (ARDUINO <  101) && !defined(MPIDE)
#error hd44780_I2Clcd i/o class requires Arduino 1.0.1 or later
#endif

class hd44780_I2Clcd : public hd44780 
{
public:
// ====================
// === constructors ===
// ====================

hd44780_I2Clcd(uint8_t i2c_addr=0) : _Addr(i2c_addr) {} // zero addres means auto locate

private:
// ====================
// === private data ===
// ====================

// Control byte bit values

// RS signal in HD44780 interface
// set for data writes, clear for cmds
// note: some datasheets call this A0 bit
static const uint8_t I2Clcd_RS  = (1 << 6);

// set if another control byte following data
// clear if last control byte
static const uint8_t  I2Clcd_CO = (1 << 7);

uint8_t _Addr;             // I2C Address of the LCD

// ==================================================
// === hd44780 i/o subclass virtual i/o functions ===
// ==================================================
//
// Note:
// It is not possible to control the backlight nor
// is it possible to read from the device.
// so iosetbacklight() and ioread()
// will not be defined and will use the defaults in
// hd44780

// ioinit() - Returns non zero if initialization failed.
int ioinit()
{
int status;

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
	 * If i2c address was not specified go try to locate device
	 */
	if(!_Addr)
	{
		if( !(_Addr = LocateDevice())) // failed to find device
		{
			return(hd44780::RV_ENXIO);
		}
	}

	/*
	 * Check to see if the device is responding
	 */
	Wire.beginTransmission(_Addr);
	if( (status = Wire.endTransmission()) )
	{
		if(status == 1)
			status = hd44780::RV_EMSGSIZE;
		else if(status == 2)
			status = hd44780::RV_ENXIO;
		else
			status = hd44780::RV_EIO;
	}

	/*
	 * this device only runs in 8 bit mode
	 */
	_displayfunction = HD44780_8BITMODE;
   return ( status );
}

//
// iowrite(type, value) - send either a command or data byte to lcd
// returns zero on success, non zero on failure
int iowrite(hd44780::iotype type, uint8_t value) 
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

	if(Wire.endTransmission())
		return(hd44780::RV_EIO);
	else
		return(hd44780::RV_ENOERR);
}

// ================================
// === internal class functions ===
// ================================
//  LocateDevice() - Locate I2C LCD device
uint8_t LocateDevice()
{
uint8_t error, address;

	// Search for 6 addresses
	for(address = 0x3a; address <= 0x3f; address++ )
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		// chipkit i2c screws up if you do a beginTransmission() too quickly
		// after an endTransmission()
		// below 20us will cause it to fail,
		// we use delay(1) here to accomdate that as well as ensure that
		// systems with watchdog timers like ESP8266 won't have issues.
		delay(1);

		if(error == 0) // if no error we found something
		{
				return(address);
		}
	}
	return(0); // could not locate device address
}


}; // end of class definition
#endif
