//  vi:ts=4
// ---------------------------------------------------------------------------
//  hd44780_pinIO.cpp - hd44780_pinIO i/o subclass for hd44780 library
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
// 2016.07.20  bperrybap - merged into hd44780 library
// 2016.06.03  bperrybap - added smart execution delays
// 2016.04.01  bperrybap - initial creation
//
// @author Bill Perry - bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------

#include <hd44780.h>
#include "hd44780_pinIO.h"
#include <pins_arduino.h> // to get PWM detection macros

hd44780_pinIO::hd44780_pinIO(uint8_t rs,  uint8_t en,
			uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
			uint8_t bl, uint8_t blLevel)
{
	_rs = rs;
	_en = en;
	_d4 = d4;
	_d5 = d5;
	_d6 = d6;
	_d7 = d7;
	_bl = bl;
	_blLevel = blLevel;
}


// ioinit - initialize the h/w
int hd44780_pinIO::ioinit()
{
	// initialize Arduino pins used for hd44780 signals
	pinMode(_rs, OUTPUT);
	pinMode(_en, OUTPUT);
	pinMode(_d4, OUTPUT);
	pinMode(_d5, OUTPUT);
	pinMode(_d6, OUTPUT);
	pinMode(_d7, OUTPUT);
  
	if(_bl != 0xff)
		pinMode(_bl, OUTPUT);

	return(0); // all is good
}


// iosend - send either a command or data byte to lcd
void hd44780_pinIO::iosend(uint8_t value, hd44780::iosendtype type)
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
}

// iosetBacklight - set backlight brightness
// if dimming not supported, any non zero dimvalue turns backlight on
void hd44780_pinIO::iosetBacklight(uint8_t dimvalue)
{
	if (_bl == 0xff )
		return; // no backlight pin so nothing to do

	// Check if backlight pin has PWM support
	// The reason this has to be done is that Arduino analogWrite()
	// sets a pin to LOW if there is no PWM support and the PWM value is
	// less than half of maximum. This is not desired behavior.
	// The desired behavior is that if dimming is not supported, then the
	// output is full on until the PWM value is zero to indicate "off".
	// So we have to bypass the goofy code in analogWrite()
	// by detecting if the pin supports PWM and controlling the output manually
	// when no PWM is available.

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
	else if(((dimvalue) && (_blLevel == HIGH)) ||
			((dimvalue == 0) && (_blLevel == LOW)))
	{
		digitalWrite(_bl, HIGH);
	}
	else
	{
		digitalWrite(_bl, LOW);
	}
}

void hd44780_pinIO::pulseEnable(void)
{
	digitalWrite(_en, HIGH);
	delayMicroseconds(1);    // enable pulse must be >450ns
	digitalWrite(_en, LOW);
}

void hd44780_pinIO::write4bits(uint8_t value)
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
