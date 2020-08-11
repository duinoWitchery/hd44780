//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_pinIO.h - hd44780_pinIO i/o subclass for hd44780 library
//  Copyright (c) 2016-2020  Bill Perry
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
// 2020.08.01  bperrybap - removed calls to analogWrite() on ESP32 core (not supported)
// 2019.08.11  bperrybap - fixed bug introduced by broken backlight check tweak
// 2016.12.26  bperrybap - tweak to broken backlight check code
// 2016.11.12  bperrybap - added code to safely handle broken backlight circuits
// 2016.09.08  bperrybap - changed param order of iowrite() to match ioread()
// 2016.08.06  bperrybap - added ioread()
// 2016.08.06  bperrybap - changed iosend() to iowrite()
// 2016.07.27  bperrybap - added return status for iosend()
// 2016.07.21  bperrybap - merged all class code into header
// 2016.07.20  bperrybap - merged into hd44780 library
// 2016.06.03  bperrybap - added smart execution delays
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
#include <pins_arduino.h> // to get PWM detection macros

#define HIGHZ 0xfe // value is not critical but it cannot be the same as LOW or HIGH, or 0xff

class hd44780_pinIO : public hd44780
{
public:
// ====================
// === constructors ===
// ====================

// 4 bit mode constructor without r/w control, without backlight control
hd44780_pinIO(uint8_t rs,  uint8_t en,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	_rs = rs;
	_en = en;
	_rw = 0xff; // not used
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;
	_bl = 0xff; // not used
	_blLevel = 0xff; // not used
}

// 4 bit mode constructor without r/w control, with backlight control
hd44780_pinIO(uint8_t rs,  uint8_t en,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
			uint8_t bl, uint8_t blLevel)
{
	_rs = rs;
	_en = en;
	_rw = 0xff; // not used
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;
	_bl = bl;
	_blLevel = blLevel;
}


// 4 bit mode constructor with r/w control, without backlight control
hd44780_pinIO(uint8_t rs,  uint8_t rw, uint8_t en,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7)
{
	_rs = rs;
	_en = en;
	_rw = rw;
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;
	_bl = 0xff; // not used
	_blLevel = 0xff; // not used
}

// 4 bit mode constructor with w/2 control, with backlight control
hd44780_pinIO(uint8_t rs,  uint8_t rw, uint8_t en,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
			uint8_t bl, uint8_t blLevel)
{
	_rs = rs;
	_en = en;
	_rw = rw;
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;
	_bl = bl;
	_blLevel = blLevel;
}


private:
// ====================
// === private data ===
// ====================

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
					// (HIGHZ is input mode for ON, LOW for off)


// ==================================================
// === hd44780 i/o subclass virtual i/o functions ===
// ==================================================

// ioinit() - initialize the h/w
int ioinit()
{
	// initialize Arduino pins used for hd44780 signals
	// NOTE:
	// _rw and _en pins are set to low even though they should
	// default to this from a an initial reset.
	// the reason is it is possible initilization is called without
	// without resetting the processor. In that case the state of the
	// bit in the port register would be unknown, so it is safest to set
	// to the desired state (LOW).

	pinMode(_rs, OUTPUT);

	if(_rw != 0xff)
	{
		pinMode(_rw, OUTPUT);
		// if r/w is used it be will set to be low,
		// it will only be turned hi during a read and
		// then set back to low. This offers a slight boost
		// for writes since normally reads are not done very often
		digitalWrite(_rw, LOW);
	}

	pinMode(_en, OUTPUT);
	digitalWrite(_en, LOW);

	pinMode(_d4, OUTPUT);
	pinMode(_d5, OUTPUT);
	pinMode(_d6, OUTPUT);
	pinMode(_d7, OUTPUT);
  
	if(_bl != 0xff)
	{
		// check for broken backlight circuit on LCDkeypads
		// and protect Arduino if it appears to be broken
		// code will do "safe" backlight control
		
		if((_blLevel == HIGH) && blPinTest(_bl))
			_blLevel = HIGHZ;

		pinMode(_bl, OUTPUT);
	}

	return(hd44780::RV_ENOERR); // all is good
}

// ioread(type) - read a byte from LCD DDRAM
//
// returns:
// 	success:  8 bit value read
// 	failure: negative value: reading not supported
//
int ioread(hd44780::iotype type) 
{
uint8_t data = 0;

	// check if r/w control supported
	if(_rw == 0xff)
		return(hd44780::RV_ENOTSUP);

	waitReady();		// ensure previous instruction finished

	// put all the LCD data pins into input mode.
	pinMode(_d4, INPUT);
	pinMode(_d5, INPUT);
	pinMode(_d6, INPUT);
	pinMode(_d7, INPUT);

	// set RS based on type of read (data or status/cmd)
	if(type == hd44780::HD44780_IOdata) 
	{
		digitalWrite(_rs, HIGH); // RS high to access data reg
	}
	else
	{
		digitalWrite(_rs, LOW); // RS LOW to access status/cmd reg
	}

	// r/w  HIGH for reading
	digitalWrite(_rw, HIGH);

	// raise E to allow reading the data.
	digitalWrite(_en, HIGH);

	// allow for hd44780 tDDR (Data delay time) before reading data
	// this could be much shorter but this is portable for all CPUs.
	// and it will ensure that hd44780 PWEH timing is honored as well.
	delayMicroseconds(1);

	// read pins for d4-d7 into upper nibble of byte
	if(digitalRead(_d4) == HIGH)
		data |= (1 << 4);

	if(digitalRead(_d5) == HIGH)
		data |= (1 << 5);

	if(digitalRead(_d6) == HIGH)
		data |= (1 << 6);

	if(digitalRead(_d7) == HIGH)
		data |= (1 << 7);

	// lower E after reading nibble
	digitalWrite(_en, LOW);

	// allow for hd44780 1/2 of tcycE (Enable cycle time)
	// this could be much shorter but this is portable for all CPUs.
	delayMicroseconds(1);
	
	// raise E to allow reading the lower nibbly of the byte
	digitalWrite(_en, HIGH);

	// allow for hd44780 tDDR (Data delay time) before reading data
	// this could be shorter but this is portable for all CPUs.
	// and it will ensure that hd44780 PWEH timing is honored as well.
	delayMicroseconds(1);

	// read pins for d4-d7 into upper nibble of byte
	if(digitalRead(_d4) == HIGH)
		data |= (1 << 0);

	if(digitalRead(_d5) == HIGH)
		data |= (1 << 1);

	if(digitalRead(_d6) == HIGH)
		data |= (1 << 2);

	if(digitalRead(_d7) == HIGH)
		data |= (1 << 3);

   
	// lower E after reading nibble
	digitalWrite(_en, LOW);

	//
	// put all pins back into state for writing to LCD
	//

	// put all the LCD data pins into input mode.
	pinMode(_d4, OUTPUT);
	pinMode(_d5, OUTPUT);
	pinMode(_d6, OUTPUT);
	pinMode(_d7, OUTPUT);

	// r/w  LOW for Writing
	digitalWrite(_rw, LOW);

	return(data);
}


// iowrite(type, value) - send either a command or data byte to lcd
// returns zero on success, non zero on failure
int iowrite(hd44780::iotype type, uint8_t value)
{
	if(type == hd44780::HD44780_IOdata)
  		digitalWrite(_rs, HIGH);
	else
  		digitalWrite(_rs, LOW);
  
	// "4 bit commands" are special.
	// They are used only during initalization and
	// are used to reliably get the LCD and host in nibble sync
	// with each other and to get the LCD into 4 bit mode.
	// 8 bit host interfaces don't have to worry about this,
	// but 4 bit host interfaces must only send the upper nibble.

	// note:
	// waitReady() is explicitly called as late as possible *after* the Arduino
	// pins for the LCD control and data signals are set to allow
	// overhead of the digitalWrite() calls to be hidden under execution time.
	
	write4bits(value>>4);	// setup uppper nibble d4-d7 lcd pins
	waitReady();			// ensure previous instruction finished
	pulseEnable();			// send upper nibble to LCD

	// send lower nibble if not a 4 bit command
	// (sends nibble for both data and "normal" commands)
	if (type != hd44780::HD44780_IOcmd4bit )
	{
		write4bits((value & 0x0F));// setup lower nibble on d4-d7 lcd pins
		pulseEnable();				// send lower nibble to LCD
	}
	return(hd44780::RV_ENOERR); // it never fails
}

// iosetBacklight() - set backlight brightness
// if dimming not supported, any non zero dimvalue turns backlight on
int iosetBacklight(uint8_t dimvalue)
{
	if (_bl == 0xff )
		return(hd44780::RV_ENOTSUP); // no backlight pin so nothing to do

	// Check for HIGHZ active level which is used on LCDkeypads with broken backlight circuits
	// In HIGHZ active level, the output is is never driven high, instead the pin is put
	// into input mode to turn on the backlight.
	// To turn off the backlight, the pin is flipped to output mode.
	// because the pin is never set to HIGH, when the pin is set to output mode it will be LOW.

	if(_blLevel == HIGHZ)
	{
		if(dimvalue)
			pinMode(_bl, INPUT);
		else
			pinMode(_bl, OUTPUT);
		return(hd44780::RV_ENOERR);
	}

	// Check if backlight pin has PWM support
	// The reason this has to be done is that Arduino analogWrite()
	// sets a pin to LOW if there is no PWM support and the PWM value is
	// less than half of maximum. This is not desired behavior.
	// The desired behavior is that if dimming is not supported, then the
	// output is full on until the PWM value is zero to indicate "off".
	// So we have to bypass the goofy code in analogWrite()
	// by detecting if the pin supports PWM and controlling the output manually
	// when no PWM is available.

	// NOTE: the ESP32 core does not have analogWrite() PWM support !
	// so on ESP32 core, all you get is on/off
#if !defined(ARDUINO_ARCH_ESP32)

#if defined(digitalPinHasPWM)
   	// Newer 1.5x Arduino has a macro to check for PWM capability on a pin
	if(digitalPinHasPWM(_bl))
#elif defined(digitalPinToTimer)
	// On 1.x Arduino have to check differently
	if(digitalPinToTimer(_bl) != NOT_ON_TIMER)
#else 
	if(0) // no way to tell so assume no PWM
#endif
	{
		// set PWM signal appropriately for active level
		if(_blLevel == HIGH)
		{
			analogWrite(_bl, dimvalue);
		}
		else 
		{
			analogWrite(_bl, 255 - dimvalue); // active low is inverse PWM
		}
	}

	// No PWM support on pin, so
	// dimvalue 0 is off, any other value is on
	else
#endif
	if(((dimvalue) && (_blLevel == HIGH)) ||
			((dimvalue == 0) && (_blLevel == LOW)))
	{
		digitalWrite(_bl, HIGH);
	}
	else
	{
		digitalWrite(_bl, LOW);
	}
	return(hd44780::RV_ENOERR);
}

// ================================
// === internal class functions ===
// ================================

// write4bits() - set the 4 hd44780 data lines
void write4bits(uint8_t value)
{
	// write the bits on the LCD data lines but don't send the data
	if(value & 1)
		digitalWrite(_d4, HIGH);
	else
		digitalWrite(_d4, LOW);

	if(value & 2)
		digitalWrite(_d5, HIGH);
	else
		digitalWrite(_d5, LOW);

	if(value & 4)
		digitalWrite(_d6, HIGH);
	else
		digitalWrite(_d6, LOW);

	if(value & 8)
		digitalWrite(_d7, HIGH);
	else
		digitalWrite(_d7, LOW);
}

// pulseEnable() - toggle en to send data to hd44780 module
void pulseEnable(void)
{
	digitalWrite(_en, HIGH);
#if defined (ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32)
	// the extra delay here is not for the LCD, it is to allow signal lines time
	// to settle when using 3v esp modules with 5v LCDs.
	// 3v outputs on 5v inputs is already a bit out of spec and
	// without this, the slew rate isn't fast enough to get "reliable"
	// signal levels
	// while it isn't needed for 3v LCDs, the time penalty isn't much
	// to always do it.
	delayMicroseconds(2);    // enable pulse must be >450ns
#else
	delayMicroseconds(1);    // enable pulse must be >450ns
#endif
	digitalWrite(_en, LOW);
}

//
// Function to test a backlight pin
// Returns non-zero if test fails (bad circuit design)
int blPinTest(int pin)
{
int val;

	/*
	 * Check to see if there
	 * is a problem in the backlight circuit
	 * So far, the "broken" designs connected D10
	 * directly to the base of a NPN transistor,
	 * this will cause a short when D10 is set to HIGH
	 * as there is no current limiting resistor in the path
	 * between D10 to the base and the emitter to ground.
	 */

	/*
	 * Set the  pin to an input with pullup disabled.
	 * This should be safe on all shields.
	 * The reason for the digitalWrite() first is that
	 * only the newer Arduino cores disable the pullup
	 * when setting the pin to INPUT.
	 * On boards that have a pullup on the transistor base,
	 * this should cause the backlight to be on.
	 */
	digitalWrite(pin, LOW);
	pinMode(pin, INPUT);

	/*
	 * Set the backlight pin to an output.
	 * since the pullup was turned off above by
	 * setting the pin to input mode,
	 * it should drive the pin LOW which should
	 * be safe given the known design flaw.
	 */
	pinMode(pin, OUTPUT);

	/*
 	 * Set the backlight pin to HIGH
	 * NOTE: This is NOT a safe thing to do
	 * on the broken designs. The code will minimize
	 * the time this is done to prevent any potential damage.
	 */

	digitalWrite(pin, HIGH);

	
	/*
	 * Now read back the pin value to
	 * See if a short is pulling down the HIGH output.
	 */

	delayMicroseconds(5); // give some time for the signal to droop

	val = digitalRead(pin); // read the level on the pin

	/*
 	 * Restore the pin to a safe state
	 * Input with pullup turned off
	 */
	digitalWrite(pin, LOW);
	pinMode(pin, INPUT);

	/*
	 * If the level read back is not HIGH
	 * Then there is a problem because the pin is
	 * being driven HIGH by the AVR.
	 */
	if (val != HIGH)
		return(-1); // test failed
	else
		return(0); // all is ok.
}


}; // end of class definition
#endif
