//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_HC1627_I2C.h - hd44780_HC1627_I2C i/o subclass for hd44780 library
//  Copyright (c) 2020  Bill Perry
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_HC1627_I2C is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_HC1627_I2C is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_HC1627_I2C.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements all the hd44780 library i/o methods to control an LCD based
// on the Tsingtek Display Co HC1627 LCD devices in i2c mode which have
// a native I2C interface rather than use a simple I2C i/o expander chip
// such as a PCF8574 or MCP23008
// This device uses two i2c address:
// - base address for RS clear
// - one higher for RS set
// The communication interface is very simple:
// 8 bit data is sent using the base address
// command/status use the base address +1
//
// Here are examples of such a device: 
// HC1627-B-LWH-I2C-30
// HC1627_I2C-SYH-I2C-30
// http://dvtecnologia.com.br/imagens/HC1627_I2C-SYH-I2C-30_Eng%2CA.pdf
// This device has 2 address pins to configure the base address.
//
// The configurable base address can be:
// 0x38, 0x3A, 0x3C, 0x3E

// It overlaps with the addresses for the PCF8574A and PCF2116/PCF2119x
// NOTE:
//		This device needs pullups on the bus signals.
//		While it will usually work on an AVR platform without external pullups,
//		it doesn't always work.
// 
// The I2C can only control the LCD and does not have the capability
// to control the backlight so the backlight will always remain on.
//
// 2020.06.26  bperrybap - initial creation (hd44780_IIClcd)
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
//
// ---------------------------------------------------------------------------
#ifndef hd44780_HC1627_I2C_h
#define hd44780_HC1627_I2C_h

#if (ARDUINO <  101) && !defined(MPIDE)
#error hd44780_HC1627_I2C i/o class requires Arduino 1.0.1 or later
#endif

class hd44780_HC1627_I2C : public hd44780 
{
public:
// ====================
// === constructors ===
// ====================

hd44780_HC1627_I2C(uint8_t i2c_addr=0) : _Addr(i2c_addr) {} // zero addres means auto locate

private:
// ====================
// === private data ===
// ====================

uint8_t _Addr;             // I2C base Address of the LCD

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
uint8_t addr;
	/*
	 * No need to look for 4 bit commands as all bits are already in
	 * proper upper nibble and unsued bits are zero.
	 * This allows the special 4 bit commands to be sent "As is" when
	 * using an 8 bit interface - which is what this i2c LCD uses.
	 */

	if(type == hd44780::HD44780_IOdata)
	{
		addr = _Addr + 1; // RS bit is set by using 1 higher address
	}
	else
	{
		addr = _Addr; // RS is clear when using base address
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

	Wire.beginTransmission(addr);
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

	// Search for 4 base address pairs
	for(address = 0x38; address <= 0x3E; address += 2 )
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
