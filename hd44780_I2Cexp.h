//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_I2Cexp.h - hd44780_I2Cexp i/o subclass for hd44780 library
//  Copyright (c) 2013-2016  Bill Perry
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780 library
//
//  hd44780_I2Cexp is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  hd44780_I2Cexp is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with hd44780_I2Cexp.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// It implements all the hd44780 library i/o methods to control an LCD based
// on the Hitachi HD44780 and compatible chipsets using I2C extension
// backpacks that use a simple I2C i/o expander chip.
// Currently the PCF8574 or the MCP23008 are supported.
//
// The API functionality provided by this library class is compatible
// with the API functionality of the Arduino LiquidCrystal library.
//
// The hd44780_I2Cexp constructor can specify all the parameters or let the
// library auto configure itself.
// hd44780_I2Cexp constructor can specifiy expander output bit assignments
// or use pre-defined entries
//
// addr can  be I2Cexp_ADDR_UNKNOWN or an instance number (0 to 15) 
//  if you want the library to autolocate the io/expander.
//
// examples:
// hd44780_I2Cexp lcd; // autolocate/autoconfigure everything
// hd44780_I2Cexp lcd(0x20); // autoconfigure for lcd at i2c address 0x20
//
// hd44780_I2Cexp lcdA; // autolocate/autoconfigure for lcd instance 0
// hd44780_I2Cexp lcdB; // autolocate/autoconfigure for lcd instance 1
//
// hd44780_I2Cexp lcd(addr, chiptype,    rs,en,d4,d5,d6,d7,bl,blpol);
// hd44780_I2Cexp lcd(I2Cexp_ADDR_UNKNOWN, I2Cexp_BOARD_SYDZ);
// hd44780_I2Cexp lcd(0x38, I2Cexp_BOARD_SYDZ);
// hd44780_I2Cexp lcd(0x20, I2Cexp_BOARD_ADAFRUIT292);
// hd44780_I2Cexp lcd(I2Cexp_ADDR_UNKNOWN, I2Cexp_MCP23008,1,2,3,4,5,6,7,HIGH);
//
// NOTES:
// It is best to use autoconfigure if possible.
// Intermixing autolocate and specific i2c addresss can create conflicts.
// The library cannot autoconfigure the SYDZ backpack.
// It will correctly identify the pin mapping but incorrectly determine
// the backlight active level control.
//
// 2016.08.06  bperrybap - changed iosend() to iowrite()
// 2016.08.06  bperrybap - added ioread()
// 2016.07.27  bperrybap - added return status for iosend()
// 2016.07.21  bperrybap - merged all class code into header
// 2016.07.20  bperrybap - merged into hd44780 library
// 2016.06.15  bperrybap - added i2c probing delay for chipkit i2c issue
// 2016.06.15  bperrybap - added getProp() diagnostic function
// 2016.06.09  bperrybap - changed name from hd44780_IICexp to hd44780_I2Cexp
// 2016.06.08  bperrybap - removed pre IDE 1.0 support
// 2016.06.03  bperrybap - added smart execution delays
// 2016.05.14  bperrybap - added support for multiple unknown address objects
// 2016.04.01  bperrybap - added auto config & auto detect pin mapping
// 2014.02.15  bperrybap - changed to use hd44780 base class library
// 2013.06.01  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#ifndef hd44780_I2Cexp_h
#define hd44780_I2Cexp_h

#include <Wire.h>
#include <hd44780.h>
// For STUPID versions of gcc that don't hard error on missing header files
#ifndef hd44780_h
#error Missing hd44780.h header file
#endif

// canned i2c board/backpack parameters
// allows using:
// hd44780_I2Cexp lcd({i2c_address}, I2Cexp_BOARD_XXX);
// instead of specifying all individual parameters.
// Note: some boards tie the LCD r/w line directly to ground
// boards that have control of the LCD r/w line will be able to do reads from lcd.
//
// The underlying hd4480_I2Cexp constructors support
// with or without r/w control, and with and without backlight control.
// - If r/w control is not desired, simply leave off the r/w pin from the constructor.
// - If backlight control is not desired/supported, simply leave off backlight pin and active level
//
// Since the library has to drive all 8 output pins, the boards that have
// r/w tied to ground should use an unused output pin for the r/w signal
// which will be set to LOW but ignored by the LCD on those boards.
// This means that the unused pin on the i/o expander cannot be used as an input
//
//									expType, rs[,rw],en,d4,d5,d6,d7[,bl, blpol]
#define I2Cexp_BOARD_LCDXIO        I2Cexp_PCF8574, 4,5,6,0,1,2,3 // ElectroFun default (no backlight control)
#define I2Cexp_BOARD_LCDXIOnBL     I2Cexp_PCF8574, 4,5,6,0,1,2,3,7,LOW // Electrofun & PNP transistor for BL

#define I2Cexp_BOARD_MJKDZ         I2Cexp_PCF8574, 6,5,4,0,1,2,3,7,LOW // mjkdz backpack
#define I2Cexp_BOARD_GYI2CLCD      I2Cexp_PCF8574, 6,5,4,0,1,2,3,7,LOW // GY-I2CLCD backpack

#define I2Cexp_BOARD_LCM1602       I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,LOW // Robot Arduino LCM1602 backpack
                                                                       // (jumper forces backlight on)

// these boards are all the same
// and match/work with the hardcoded Arduino LiquidCrystal_I2C class
#define I2Cexp_BOARD_YWROBOT       I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_DFROBOT       I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_SAINSMART     I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_FUNDUINO      I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_SYDZ          I2Cexp_PCF8574, 0,1,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack 
                                                                        // SYDZ backpack uses a pullup because of the pullup,
                                                                        // backlight active level can not be auto detected

// MCP23008 based boards
// Currently r/w control is disabled since most boards either can't do it, or have it disabled.
#define I2Cexp_BOARD_ADAFRUIT292   I2Cexp_MCP23008,1,2,3,4,5,6,7,HIGH // Adafruit #292 i2c/SPI backpack in i2c mode (lcd RW grounded)
                                                                      // GP0 not connected to r/w so no ability to do LCD reads

#define I2Cexp_BOARD_WIDEHK        I2Cexp_MCP23008,4,7,0,1,2,3,6,HIGH // WIDE.HK mini backpack (lcd r/w hooked to GP5)

#define I2Cexp_BOARD_LCDPLUG       I2Cexp_MCP23008,4,6,0,1,2,3,7,HIGH // JeeLabs LCDPLUG (NOTE: NEVER use the SW jumper)
                                                                      // GP5 is hooked to s/w JP1 jumper, LCD RW is hardwired to gnd
                                                                      // So no ability to do LCD reads.

#define I2Cexp_BOARD_XXX           I2Cexp_MCP23008,7,6,5,4,3,2,1,HIGH // unknown backpack brand

#define I2Cexp_BOARD_MLTBLUE       I2Cexp_MCP23008,1,3,4,5,6,7,0,HIGH // i2c LCD MLT group "Blue Board" backpack
                                                                      // http://www.mlt-group.com/I2C-LCD-Blue-Board-for-Arduino
                                                                      // There is jumper on the board jp6 that controls how the
                                                                      // the board drives r/w.
                                                                      // It looks like by defualt r/w is wired to gnd and
                                                                      // can be changed by changing the solder jumper jp6.
                                                                      // however it isn't clear if that changes to GP2 or to Vcc.
                                                                      // It sounds like it changes to vcc with is REALLY dumb!


//FIXME these can't go in the class unless they are referenced using the classname

enum I2CexpType { I2Cexp_UNKNOWN, I2Cexp_PCF8574, I2Cexp_MCP23008 };
static const int I2Cexp_ADDR_UNKNOWN = 0xff; // auto locate device


class hd44780_I2Cexp : public hd44780
{
public:
// ====================
// === constructors ===
// ====================

//	-- Automagic / auto-detect constructors --

// Auto find next instance and auto config pin mapping
hd44780_I2Cexp(){ _addr = AutoInst++; _expType = I2Cexp_UNKNOWN;}

// Auto config specific addr/instance.
// addr can be i2c address or instance (0 - 15)
hd44780_I2Cexp(uint8_t addr){ _addr = addr; _expType = I2Cexp_UNKNOWN;}


// -- Explicit constructors, specify address & pin mapping information --

// constructor with r/w control without backlight control
hd44780_I2Cexp(uint8_t i2c_addr, I2CexpType type, uint8_t rs, uint8_t rw, uint8_t en,
			 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
{
   config(i2c_addr, type, rs, rw, en, d4, d5, d6, d7);
}

// Constructor with r/w control with backlight control
hd44780_I2Cexp(uint8_t i2c_addr, I2CexpType type, uint8_t rs, uint8_t rw, uint8_t en,
				uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
				uint8_t bl, uint8_t blLevel)
{
   config(i2c_addr, type, rs, rw, en, d4, d5, d6, d7, bl, blLevel);
}

// Constructor without r/w control without backlight control
hd44780_I2Cexp(uint8_t i2c_addr, I2CexpType type, uint8_t rs, uint8_t en,
			 uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 )
{
   config(i2c_addr, type, rs, 0xff, en, d4, d5, d6, d7);
}

// Constructor without r/w control with backlight control
hd44780_I2Cexp(uint8_t i2c_addr, I2CexpType type, uint8_t rs, uint8_t en,
				uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
				uint8_t bl, uint8_t blLevel)
{
   config(i2c_addr, type, rs, 0xff, en, d4, d5, d6, d7, bl, blLevel);
}



// ============================================
// === library specific diagnostic function ===
// ============================================

enum I2CexpProp
{
	Prop_addr,
	Prop_expType,
	Prop_rs,
	Prop_rw,
	Prop_en,
	Prop_d4,
	Prop_d5,
	Prop_d6,
	Prop_d7,
	Prop_bl,
	Prop_blLevel,
};

int mask2bit(uint8_t mask)
{
	for(uint8_t bit = 0; bit < 8; bit++)
		if(mask  & (1 << bit))
			return(bit);

	// didn't find it, return error
	return(-1);
}

int getProp(I2CexpProp propID)
{
	switch(propID)
	{
		case Prop_addr:
			return(_addr);
		case Prop_expType:
			return((int)_expType);
		case Prop_rs:
			return(mask2bit(_rs));
		case Prop_rw:
			return(mask2bit(_rw));
		case Prop_en:
			return(mask2bit(_en));
		case Prop_d4:
			return(mask2bit(_d4));
		case Prop_d5:
			return(mask2bit(_d5));
		case Prop_d6:
			return(mask2bit(_d6));
		case Prop_d7:
			return(mask2bit(_d7));
		case Prop_bl:
			return(mask2bit(_bl));
		case Prop_blLevel:
			return(_blLevel);
		default:
			return(-1);
	}
}

private:
// ====================
// === private data ===
// ====================

// auto instance tracts inst number when creating multiple auto locate objects
// this is static since it is for the entire class not per object.
static uint8_t AutoInst; // declaration only, definition is outside class

// expander pin mapping & state information
uint8_t _addr;			// I2C Address of the IO expander
I2CexpType _expType;	// I2C chip type used on the IO expander
uint8_t _rs;			// I2C chip IO pin mask for Register Select pin
uint8_t _rw;			// I2C chip IO pin mask for r/w pin
uint8_t _en;			// I2C chip IO pin mask for enable pin
uint8_t _d4;			// I2C chip IO pin mask for data d4 pin
uint8_t _d5;			// I2C chip IO pin mask for data d5 pin
uint8_t _d6;			// I2C chip IO pin mask for data d6 pin
uint8_t _d7;			// I2C chip IO pin mask for data d7 pin
uint8_t _bl;			// I2C chip IO pin mask for Backlight
uint8_t _blLevel;		// backlight active control level HIGH/LOW
uint8_t _blCurState;	// Current IO pin state mask for Backlight

// ==================================================
// === hd44780 i/o subclass virtual i/o functions ===
// ==================================================

// ioinit() - initialize the h/w
// Returns non zero if initialization failed.
//
// can't be used from constructors because not everything is initalized yet.
// (maybe interrupts or other library constructors)
// if Wire library is used in constructor, it will hang.
int ioinit()
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
	 * hd44780 i/o interfaces should be the constructor
	 * So we go ahead and call it here.
	 */
	Wire.begin();

	// auto locate i2c expander and magically detect pin mappings

	if(_addr < 0x10) // go locate device instance if index instance specified
		_addr = LocateDevice(_addr);
	else if(_addr == I2Cexp_ADDR_UNKNOWN) // locate next instance
		_addr = LocateDevice(AutoInst++);

	if(_addr == I2Cexp_ADDR_UNKNOWN) // if we couldn't locate it, return error
		return(-1);

	if(_expType == I2Cexp_UNKNOWN) // figure out expander chip if not told
	{
		_expType = IdentifyIOexp(_addr);

		if(_expType == I2Cexp_UNKNOWN) // coudn't figure it out?, return error
			return(-1);

		if(_expType == I2Cexp_PCF8574)
			status = autocfg8574();		// go auto configure the pin mappings
		else
			status = autocfgMCP23008();	// go auto configure the pin mappings
		if(status)
			return(status);
	}

	// initialize IO expander chip

	Wire.beginTransmission(_addr);

	if(_expType == I2Cexp_MCP23008)
	{
		/*
		 * First make sure to put chip into BYTE mode
		 * BYTE mode is used to make a MCP23008 work more like PCF8574
		 * In BYTE mode the address register does not increment so that
		 * once you point it to GPIO you can write to it over and over again
		 * within the same i2c connection by simply sending more bytes.
		 * This is necessary as the code uses back to back writes to perform
		 * the nibble updates as well as the toggling the enable signal.
		 * This methodology offers significant performance gains.
		 */
		Wire.write(5);	// point to IOCON
		Wire.write(0x20);// disable sequential mode (enables BYTE mode)
		Wire.endTransmission();

		/*
		 * Now set up output port
		 */
		Wire.beginTransmission(_addr);
		Wire.write((uint8_t)0); // point to IODIR
		Wire.write((uint8_t)0); // all pins output
		Wire.endTransmission();
	
		/*
		 * point chip to GPIO
		 */
		Wire.beginTransmission(_addr);
		Wire.write(9); // point to GPIO
		
	}
	Wire.write((uint8_t)0);  // Set the entire output port to LOW
	status = Wire.endTransmission();

	return ( status );
}

// ioread(type) - read a byte from LCD DDRAM
//
// returns:
// 	success:  8 bit value read
// 	failure: negative value: error or read not supported
int ioread(hd44780::iotype type) 
{
uint8_t gpioValue =  _blCurState;
uint8_t data = 0;
uint8_t iodata;

	// If address or expander type is unknown, then abort read w/error
	if(_addr == I2Cexp_ADDR_UNKNOWN || _expType == I2Cexp_UNKNOWN)
		return(-1);

	// reads for MCP23008 not yet supported
	if(_expType == I2Cexp_MCP23008)
	{
		return(-1);
	}

	// check if reads supported
	if(!_rw)
		return(-1);

	/*
	 * ensure that previous LCD instruction finished.
	 * There is a 45us offset since there will be at least 2 bytes
	 * (the i2c address and the i/o expander data)  transmitted over i2c
	 * before the i/o expander i/o pins could be seen by the LCD.
	 * (3 bytes on the MCP23008)
	 * At 400Khz (max rate supported by the i/o expanders) 16 bits plus start
	 * and stop bits is 45us.
	 * So there is at least 45us of time overhead in the physical interface.
	 */

	waitReady(-45);
   
	// put all the expander LCD data pins into input mode.
	// PCF8574 psuedo inputs use pullups so setting them to 1
	// makes them suitible for inputs.

	gpioValue |= _d4|_d5|_d6|_d7;

	// set RS based on type of read (data or status/cmd)
	if(type == hd44780::HD44780_IOdata) 
	{
		gpioValue |= _rs; // RS high to read data reg
	}

	gpioValue |= _rw; // r/w high for reading

	// write all the bits to the expander port
	
	Wire.beginTransmission(_addr);
	Wire.write(gpioValue);		// d4-d7 are inputs, RS, r/w high, E LOW
	Wire.endTransmission();


	// raise E to read the data.
	Wire.beginTransmission(_addr);
	Wire.write(gpioValue | _en); // Raises E 
	Wire.endTransmission();

	// read the expander port to get the upper nibble of the byte
	Wire.requestFrom((int)_addr, 1);
	iodata = Wire.read();

	Wire.beginTransmission(_addr);
	Wire.write(gpioValue); // lower E after reading nibble
	Wire.endTransmission();

	// map i/o expander port bits into upper nibble of byte
	if(iodata & _d4)
		data |= (1 << 4);

	if(iodata & _d5)
		data |= (1 << 5);

	if(iodata & _d6)
		data |= (1 << 6);

	if(iodata & _d7)
		data |= (1 << 7);
	
	Wire.beginTransmission(_addr);
	Wire.write(gpioValue | _en); // Raise E to read next nibble
	Wire.endTransmission();

	// read the expander port to get the upper lower of the byte
	Wire.requestFrom((int)_addr, 1);
	iodata = Wire.read();

	// map i/o expander port bits into lower nibble of byte
	if(iodata & _d4)
		data |= (1 << 0);

	if(iodata & _d5)
		data |= (1 << 1);

	if(iodata & _d6)
		data |= (1 << 2);

	if(iodata & _d7)
		data |= (1 << 3);

   
	// put gpio port back to all outputs state with WR signal low for writes
	Wire.beginTransmission(_addr);
	Wire.write(_blCurState);		// with E LOW
	Wire.endTransmission();

	return(data);
}

// iowrite(value, type) - send either command or data byte to lcd
// returns zero on success, non zero on failure
int iowrite(uint8_t value, hd44780::iotype type) 
{
	// If address or expander type is unknown, then drop data
	if(_addr == I2Cexp_ADDR_UNKNOWN || _expType == I2Cexp_UNKNOWN)
		return(-1);

	/*
	 * ensure that previous LCD instruction finished.
	 * There is a 45us offset since there will be at least 2 bytes
	 * (the i2c address and the i/o expander data)  transmitted over i2c
	 * before the i/o expander i/o pins could be seen by the LCD.
	 * (3 bytes on the MCP23008)
	 * At 400Khz (max rate supported by the i/o expanders) 16 bits plus start
	 * and stop bits is 45us.
	 * So there is at least 45us of time overhead in the physical interface.
	 */

	waitReady(-45);
   
	// grab i2c bus
	Wire.beginTransmission(_addr);
	if(_expType == I2Cexp_MCP23008)
	{
		Wire.write(9); // point to GPIO
	}
	// send both nibbles in same i2c connection
	write4bits( (value >> 4), type );  // send upper nibble

	/*
	 * "4 bit commands" are special.
	 * They are used only during initalization and
	 * are used to reliably get the LCD and host in nibble sync
	 * with each other and to get the LCD into 4 bit mode.
	 * When sending a "4 bit command" only the upper nibble of
	 * of the 8 bit byte will presented on LCD signals D4-D7.
	 */
	if(type != hd44780::HD44780_IOcmd4bit)
	{
		write4bits( (value & 0x0F), type); // lower nibble, if not 4bit cmd
	}
	return(Wire.endTransmission());
}

// iosetBacklight()  - set backlight brightness
// Since dimming is not supported, any non zero value
// will turn on the backlight.
void iosetBacklight(uint8_t dimvalue) 
{
	if(_bl) // backlight control?
	{
		// dimvalue 0 is backlight off any other dimvalue is backlight on
		// configure backlight state mask according to active level
		if(((dimvalue) && (_blLevel == HIGH)) || 
				((dimvalue == 0) && (_blLevel == LOW)))
		{
			_blCurState = _bl;
		}
		else 
		{
			_blCurState = 0;
		}
		Wire.beginTransmission(_addr);
		if(_expType == I2Cexp_MCP23008)
		{
			Wire.write(9); // point to GPIO
		}
		Wire.write( _blCurState );
		Wire.endTransmission();
	}
}

// ================================
// === internal class functions ===
// ================================

// config() - save constructor parameters
void config(uint8_t i2c_addr, I2CexpType i2c_type, uint8_t rs, uint8_t rw, uint8_t en, 
						uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
						uint8_t bl=0xff, uint8_t blLevel=0xff )
{
	// Save away config data into object
	_expType = i2c_type;
	_addr = i2c_addr;
   
	_rs = ( 1 << rs );

	if(rw < 8)
		_rw = (1 << rw);
	else
		_rw = 0; // no r/w control

	_en = ( 1 << en );
   
	// Initialise pin mapping
	_d4 = ( 1 << d4 );
	_d5 = ( 1 << d5 );
	_d6 = ( 1 << d6 );
	_d7 = ( 1 << d7 );   
   
	if(bl < 8)
		_bl = ( 1 << bl );
	else
		_bl = 0; // no backlight control
	_blLevel = blLevel;

	// set default bl state to backlight on
	// if no _bl control, the _blCurState values will also be set to zero
	// so it doesn't turn on any other pins.
	// 
	
	if(_bl && (blLevel == HIGH))
		_blCurState = _bl;
	else
		_blCurState = 0;
}

//  LocateDevice() - Locate I2C expander device instance
uint8_t LocateDevice(uint8_t instance)
{
uint8_t error, address;
uint8_t locinst = 0;

	// 8 addresses for PCF8574 or MCP23008
	for(address = 0x20; address <= 0x27; address++ )
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
#ifdef ARDUINO_ARCH_PIC32
		// chipkit stuff screws up if you do beginTransmission() too fast
		// after an endTransmission()
		// below 20us will cause it to fail, so we pad it out a bunch to provide margin
		delayMicroseconds(100);
#endif
		if(error == 0) // if no error we found something
		{
			if(locinst == instance)
			{
#if 0
				if(IdentifyIOexp(address) == I2Cexp_UNKNOWN) // if we can't identify it, keep looking
					continue;
#endif
				return(address);
			}
			locinst++;
		}
	}
	// 8 addresses for PCF8574A
	for(address = 0x38; address <= 0x3f; address++ )
	{
		Wire.beginTransmission(address);
		error = Wire.endTransmission();
#ifdef ARDUINO_ARCH_PIC32
		// chipkit stuff screws up if you do beginTransmission() too fast
		// after an endTransmission()
		// below 20us will cause it to fail, so we pad it out a bunch to provide margin
		delayMicroseconds(20);
#endif
		if(error == 0) // if no error we found something
		{
			if(locinst == instance)
			{
#if 0
				if(IdentifyIOexp(address) == I2Cexp_UNKNOWN) // if we can't identify it, keep looking
					continue;
#endif
				return(address);
			}
			locinst++;
		}
	}
	return(I2Cexp_ADDR_UNKNOWN); // could not locate expander instance
}


// IdentifyIOexp() -  Identify I2C i/o expander device type
// Currently PCF8574 or MCP23008
I2CexpType IdentifyIOexp(uint8_t address)
{
int data;
I2CexpType chiptype;

	/*
	 * Identify PCF8574 vs MCP23008
	 * On a PCF8574 1 bits turn on pullups and make the pin an input.
	 * and 0 bits set the output pin to drive 0.
	 * And a read always reads the port pins.
	 *
	 * Strategy:
	 *	- Write 0xff to MCP23008 IODIR reg (location 0) puts pins in input mode
	 *  - Point MCP23008 to IODIR register (location 0)
	 *	- Read 1 byte
	 *
	 * On a MCP23008 the read will return 0xff because it will read the 
	 * IODIR we just wrote
	 * On a PCF8574 we should read a 0 since we last wrote zeros to all the
	 * PORT bits
	 *
	 * NOTE:
	 * 	The values 0xff and zero were carefully chosen so as to not create a bus
	 *  collision with the LCD data lines.
	 *  On a PCF8574 the 0xff will put all the port pins into input mode and the
	 *  the value 0x0 will ensure that the LCD is write mode even if the R/W
	 *  pin is connected to a PCF8574 pin both of these will ensure that the
	 *  PCF8574 is not ever driving a pin that
	 *  is connected to pin that the LCD is also driving.
	 *
	 *  On a MCP23008, the write of 0xff to IODIR will put the port into input
	 *  mode.
	 */

	/*
	 * First try to write 0xff to MCP23008 IODIR
	 * On a PCF8574 this will end up writing 0 and then ff to output port
	 */
	Wire.beginTransmission(address);
	Wire.write((uint8_t) 0);	// try to point to MCP23008 IODR
	Wire.write((uint8_t) 0xff);	// try to write to MCP23008 IODR
	Wire.endTransmission();

	/*
	 * Now try to point MCP23008 to IODIR for read
	 * On a PCF8574 this will end up writing a 0 to the output port
	 */

	Wire.beginTransmission(address);
	Wire.write((uint8_t) 0);	// try to point to MCP23008 IODR
	Wire.endTransmission();

	/*
	 * Now read a byte
	 * On a MCP23008 we should read the 0xff we wrote to IODIR
	 * On a PCF8574 we should read 0 since the output port was set to 0
	 */
	Wire.requestFrom((int)address, 1);
	data = Wire.read();

	if(data == 0xff)
	{
		chiptype = I2Cexp_MCP23008;
	}
	else if(data == 0x00)
	{
		chiptype = I2Cexp_PCF8574;
	}
	else
	{
		chiptype = I2Cexp_UNKNOWN; // this shouldn't happen
	}
	return(chiptype);
}

/*
 * autocfg8574() - automagically figure out the pcf8574 pin mappings
 *
 * This code can auto detect 6 different 8574 backpack pin mappings.
 * This is all the known pin mappings.
 * Note: it does fail on the SYDZ bapack by incorrectly picking the backlight
 * active level because that backpack uses a pullup on the backlight transistor base.
 * SYDZ backpacks will have to be manually configured.
 *
 * Overview of how this works:
 * 
 * It appears that when you set the i/o port to input and read
 * the port, that the expander input pins with the en, rs, & di connections
 * will all float high from the expander input pullups.
 * Whereas the inputs with the d4-d7 connections will not.
 *
 * Also, 
 * When there is no pullup/pulldown resistor on the backlight transistor base,
 * the 8574 bl pin as an input will be pulled to the direction of the emitter.
 * for active low backlights, the bl input pin will be high
 * for active high backlights, the bl input pin will be low.
 *
 * The MAGIC is knowing how to use all this information to differenciate
 * between backpacks.
 *
 * Since all known 8574 backpacks appear to use either the upper or the lower
 * nibble for the data, a single read can split the devices into those
 * that use the upper nibble vs those that use the lower nibble.
 * All the devices that use the upper nibble use the same bits
 * for rs (0), rw (1), and en (2), and backlight is bit 3
 *
 * The devices that use the lower nibble for data is much more tricky.
 * For that we have to actually guess which signal is EN, and turn it off
 * to get the LCD to stop driving the data lines. This will cause a change
 * on the data lines since the LCD seems to have pull downs on the data lines
 * that over power the weak pullups on the 8574.
 * This can be used to determine between the remaining two types.
 *
 * Determiing backlight active level is easy as mentioned above.
 *
 * Auto active level detect fails when a FET is used or
 * a pullup resistor is used on a NPN transistor.
 * With an FET there is no load betweent the 8574 bl pin and the FET gate,
 * so there is no way to distinguish this from a PNP emitter pulling up a
 * base pin.
 * When using a NPN transistor with a pullup, the pullup on the base will
 * not allow the base to be pulled in the direction of the emitter.
 *
 * As of now, the only known auto config failure is the SYDZ board since
 * it uses a pullup on the base.
 * Because of this, the code will incorrecly pick active low backlight control.
 * Modifying the h/w to disable the pullup can be done, but is difficult due
 * to the way R1 resistors are used and the PCB layout.
 * It requires cuting a trace and dead bugging
 * a new resistor to  connect to the base of the transistor.
 * 
 * As a result the SYDZ backpack cannot use autoconfiguration.
 */

int autocfg8574()
{
uint8_t data;
uint8_t rs, rw, en, d4, d5, d6, d7, bl, blLevel;

	/*
	 * First put a 0xff in the output port
	 */
	Wire.beginTransmission(_addr);
	Wire.write((uint8_t) 0xff);
	Wire.endTransmission();

	/*
	 * now read back from the port
	 */

	Wire.requestFrom((int)_addr, 1);
	data = Wire.read();

	if((data & 0x7) == 7)
	{
		rs = 0;
		rw = 1;
		en = 2;
		d4 = 4;
		d5 = 5;
		d6 = 6;
		d7 = 7;
		bl = 3;
		if(data & 0x8) // bit 3 high so active low
		{
			// LCM1602
			// 0,1,2,4,5,7,3, LOW
			blLevel = LOW;
		}
		else
		{
			// YwRobot/DFRobot/SainSmart/funduino
			// 0,1,2,4,5,6,7,3, HIGH
			blLevel = HIGH;
		}
	}
	else if((data & 0x70) == 0x70)
	{
	uint8_t data2;

		// now we have either Electro fun LCDXIO or MJKDZ board
		// both use bit 7 for backlight control

		// Turn off the en bit which should change the data bits
		// bit 4 on the mjkdz is en so we try that bit
	
		Wire.beginTransmission(_addr);
		Wire.write((uint8_t) 0xef);
		Wire.endTransmission();

		// read back data
		Wire.requestFrom((int)_addr, 1);
		data2 = Wire.read();

		// look at data bits and see if they changed
		// if they changed, then en was bit 4 and it is mjdkz
		if((data2 & 0xf)  != (data & 0xf))
		{
			// mjkdz
			rs = 6;
			rw = 5;
			en = 4;
			d4 = 0;
			d5 = 1;
			d6 = 2;
			d7 = 3;
			bl = 7;
		}
		else
		{
			// electrofun LCDXIO
			rs = 4;
			rw = 5;
			en = 6;
			d4 = 0;
			d5 = 1;
			d6 = 2;
			d7 = 3;
			bl = 7;
		}
		if(data & 0x80) // bit 7 high so active low
		{
			blLevel = LOW;
		}
		else
		{
			blLevel = HIGH;
		}
	}
	else
	{
		// couldn't figure it out
		return(-1);
	}
	config(_addr, _expType, rs, rw, en, d4, d5, d6, d7, bl, blLevel);
	return(0);
}

/*
 * autocfgMCP23008() - automagically figure out the mcp23008 pin mappings
 *
 * - BOARD_ADAFRUIT292	- adafruit #292	I2C/"SPI"
 * - BOARD_WIDEHK 		- WideHK
 * - BOARD_LCDPLUG		- Jeelabs LCDPlug
 * - BOARD_XXX			- unknown vendor
 *
 * WARNING WARNING WARNING:
 * Because the MCP23008 has high drive and sink capabability on its i/o port
 * pins, it is possible that if this auto detection fails, that there could
 * be a bus contention between the MCP23008 and the LCD data lines.
 * Should this occur, it is possible that either one or both could be damaged.
 * 
 * currently this code will not detect the LCDPLUG as it is similar to
 * adafruit #292 board.
 *
 * On the #292 board, the combination of the pullups with the 595 SR chip on
 * the data bus, and the r/w line hard wired to gnd, the data lines are all
 * pulled high, when the port is in input mode with pullups enabled.
 * GP0 is not stable without pullups as it is not connected to anything.
 * GP7 will pull down from the emitter of the NPN transistor.
 *
 * On the WIDEKH board,
 * GP6 should pull down from the emitter of the NPN transistor.
 * GP5 is wired to LCD RW signal
 * 
 * On the BOARD_XXXX,
 * GP1 should pull down from the emitter of the NPN transistor.
 *
 * On the LCDPLUG,
 * GP7 will pull down from the emitter of the NPN transistor.
 * LCD RW signal is hardwired to ground.
 * 
 * Given that the LCDPLUG is the only other board that uses GP7 for the
 * backlight control and all the known boards active HIGH control and GP7
 * is never used for a data line,
 * it is safe to assume that if GP7 is low then the board is either
 * LCDPLUG or ADAFRUIT292.
 * For now, if GP7 is low then assume the board is ADAFRUIT292
 *
 * Other untested probes:
 * If GP6 is low then the board is a WIDEHK
 * if GP1 is low then the board is the BOARD_XXX board
 * The BOARD_XXX may not work as GP1 is used for D5 on WIDEHK and LCDPLUG
 *
 *
 */
int autocfgMCP23008()
{
uint8_t data;
uint8_t rs, en, d4, d5, d6, d7, bl;
uint8_t blLevel;

	/*
	 * Now set up output port
	 * Make no assumptions as to the state of IOCON BYTE mode
	 */
	Wire.beginTransmission(_addr);
	Wire.write((uint8_t)0); // point to IODIR
	Wire.write(0xff); // all pins inputs
	Wire.endTransmission();

	Wire.beginTransmission(_addr);
	Wire.write(6); // point to GPPU
	Wire.write(0xff); // turn on pullups
	Wire.endTransmission();

	/*
	 * read from the GPIO port
	 */

	Wire.beginTransmission(_addr);
	Wire.write(9); // point to GPIO
	Wire.endTransmission();
	Wire.requestFrom((int)_addr, 1);
	data = Wire.read();

	blLevel = HIGH; // known boards are active HIGH bl

	if(data == 0x7f) // bit 7 low, and other control and data lines high
	{
		// board is either ADAFRUIT292 or LCDPLUG
		// for now, assume ADAFRUIT292
		rs = 1;
		// rw = 0; // not used
		en = 2;
		d4 = 3;
		d5 = 4;
		d6 = 5;
		d7 = 6;
		bl = 7;

	} else  if(!(data & (1<<6))) // bit 6 low (untested)
	{
		// WIDEHK
		rs = 4;
		// rw = 5; // not used
		en = 7;
		d4 = 0;
		d5 = 1;
		d6 = 2;
		d7 = 3;
		bl = 6;

	} else if(!(data & (1<<1))) // bit 1 low (untested)
	{
		// BOARD_XXX
		rs = 7;
		// rw = 0; // not used
		en = 6;
		d4 = 5;
		d5 = 4;
		d6 = 3;
		d7 = 2;
		bl = 1;

	}
	else
	{
		return(-1); // could not identify board
	}
	config(_addr, _expType, rs, 0xff, en, d4, d5, d6, d7, bl, blLevel);
	return(0);
}


// write4bits - send a nibble to the LCD through i/o expander port
void write4bits(uint8_t value, hd44780::iotype type ) 
{
uint8_t gpioValue =  _blCurState;
   
	// convert the value to an i/o expander port value
	// based on pin mappings
	if(value & (1 << 0))
		gpioValue |= _d4;

	if(value & (1 << 1))
		gpioValue |= _d5;

	if(value & (1 << 2))
		gpioValue |= _d6;

	if(value & (1 << 3))
		gpioValue |= _d7;


	if(type == hd44780::HD44780_IOdata) 
	{
		gpioValue |= _rs; // set RS high to send to data reg
	}
   
	// Cheat here by raising E at the same time as setting control lines
	// This violates the spec but seems to work realiably.
	Wire.write(gpioValue |_en);	// with E HIGH
	Wire.write(gpioValue);		// with E LOW
}
	
}; // end of class definition

uint8_t hd44780_I2Cexp::AutoInst; // define the static auto instance variable

#endif
