//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_I2Cexp.h - hd44780_I2Cexp library
//  Copyright (c) 2013-2016  Bill Perry
// ---------------------------------------------------------------------------
//
//  This file is part of the hd44780_I2Cexp library
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
// 2016.06.13  bperrybap - added getProp properties
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
// Since the library has to drive all 8 output pins, the boards that have
// r/w tied to ground will be assigned an unused output pin for the r/w signal
// which will be set to LOW but ignored by the LCD on those boards.
// This means that the unused pin on the i/o expander cannot be used as an input
//
//									expType, rs,en,d4,d5,d6,d7,bl, blpol
#define I2Cexp_BOARD_LCDXIO        I2Cexp_PCF8574, 4,6,0,1,2,3 // ElectroFun default (no backlight control)
#define I2Cexp_BOARD_LCDXIOnBL     I2Cexp_PCF8574, 4,6,0,1,2,3,7,LOW // Electrofun & PNP transistor for BL

#define I2Cexp_BOARD_MJKDZ         I2Cexp_PCF8574, 6,4,0,1,2,3,7,LOW // mjkdz backpack
#define I2Cexp_BOARD_GYI2CLCD      I2Cexp_PCF8574, 6,4,0,1,2,3,7,LOW // GY-I2CLCD backpack

#define I2Cexp_BOARD_LCM1602       I2Cexp_PCF8574, 0,2,4,5,6,7,3,LOW // Robot Arduino LCM1602 backpack
                                                                     // (jumper forces backlight on)

// these boards are all the same
// and match/work with the hardcoded Arduino LiquidCrystal_I2C class
#define I2Cexp_BOARD_YWROBOT       I2Cexp_PCF8574, 0,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_DFROBOT       I2Cexp_PCF8574, 0,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_SAINSMART     I2Cexp_PCF8574, 0,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_FUNDUINO      I2Cexp_PCF8574, 0,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack
#define I2Cexp_BOARD_SYDZ          I2Cexp_PCF8574, 0,2,4,5,6,7,3,HIGH // YwRobot/DFRobot/SainSmart/funduino backpack 
																			// SYDZ backpack uses a pullup
																			// because of the pullup, backlight active level can't be auto detected

// MCP23008 based boards
#define I2Cexp_BOARD_ADAFRUIT292   I2Cexp_MCP23008,1,2,3,4,5,6,7,HIGH // Adafruit #292 i2c/SPI backpack in i2c mode (lcd RW grounded)
																			// GP0 not connected
#define I2Cexp_BOARD_WIDEHK        I2Cexp_MCP23008,4,7,0,1,2,3,6,HIGH // WIDE.HK mini backpack (lcd r/w hooked to GP5)
#define I2Cexp_BOARD_LCDPLUG       I2Cexp_MCP23008,4,6,0,1,2,3,7,HIGH // JeeLabs LCDPLUG (NOTE: NEVER use the SW jumper)
																			// GP5 is hooked to s/w JP1 jumper, LCD RW is hardwired to gnd
#define I2Cexp_BOARD_XXX           I2Cexp_MCP23008,7,6,5,4,3,2,1,HIGH // unknown backpack brand

#define I2Cexp_BOARD_MLTBLUE       I2Cexp_MCP23008,1,3,4,5,6,7,0,HIGH // MLT-group "blueboard" backpack


//FIXME these can't go in the class unless they are referenced using the classname

enum I2CexpType { I2Cexp_UNKNOWN, I2Cexp_PCF8574, I2Cexp_MCP23008 };
static const int I2Cexp_ADDR_UNKNOWN = 0xff; // auto locate device

class hd44780_I2Cexp : public hd44780
{
public:
	// Types and constants


	// Constructors

	/*
	 * Automagic / auto-detect constructors
	 */

	// if nothing specified, find next instance & autoconfig pin mapping
	hd44780_I2Cexp();

	// auto config specific i2c addr & autoconfig pin mapping
	hd44780_I2Cexp(uint8_t addr);
   
	/*
	 * Explicit constructors, that specifify everything
	 */
	hd44780_I2Cexp(uint8_t  i2c_addr, I2CexpType exp_type, uint8_t Rs, uint8_t En,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7 );
	// Constructor with backlight control
	hd44780_I2Cexp(uint8_t  i2c_addr, I2CexpType exp_type, uint8_t Rs, uint8_t En,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
		uint8_t Bl, uint8_t BlLevel);

	/*
	 * library specific diagnostic function
	 */
	enum I2CexpProp
	{
		Prop_addr,
		Prop_expType,
		Prop_rs,
		Prop_en,
		Prop_d4,
		Prop_d5,
		Prop_d6,
		Prop_d7,
		Prop_bl,
		Prop_blLevel,
	};
	int getProp(I2CexpProp propID);

private:
	// hd44780 class virtual i/o functions
	int  ioinit();
	void iosend(uint8_t value, hd44780::iosendtype type);
	void iosetBacklight (uint8_t dimvalue);
	
	// internal functions
	void config (uint8_t i2c_Addr, I2CexpType exp_type, uint8_t Rs, uint8_t En,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
		uint8_t bl, uint8_t blLevel);
	
	I2CexpType IdentifyIOexp(uint8_t i2c_addr);

	uint8_t LocateDevice(uint8_t instance);
	int autocfg8574();
	int autocfgMCP23008();
	void write4bits(uint8_t value, hd44780::iosendtype type);
	
	/*
	 * Private data
	 */
	
	uint8_t _addr;			// I2C Address of the IO expander
	I2CexpType _expType;	// I2C chip type used on the IO expander
	uint8_t _rs;			// I2C chip IO pin mask for Register Select pin
	// note: Rw pin is not used
	uint8_t _en;			// I2C chip IO pin mask for enable pin
	uint8_t _d4;			// I2C chip IO pin mask for data d4 pin
	uint8_t _d5;			// I2C chip IO pin mask for data d5 pin
	uint8_t _d6;			// I2C chip IO pin mask for data d6 pin
	uint8_t _d7;			// I2C chip IO pin mask for data d7 pin
	uint8_t _bl;			// I2C chip IO pin mask for Backlight
	uint8_t _blLevel;		// backlight active control level HIGH/LOW
	uint8_t _blCurState;	// Current IO pin state mask for Backlight
};

#endif
