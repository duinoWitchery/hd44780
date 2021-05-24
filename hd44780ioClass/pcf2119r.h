/*	pcf2119r - rudimentary support for the PCF2119R display controller
 *
 *	(c) David Haworth
 *
 *  pcf2119r is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  pcf2119r is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with pcf2119r.  If not, see <http://www.gnu.org/licenses/>.
*/

/* The 'R' variant of the PCF2119 display controller is fundamentally broken
 * It has a strange character set; most of the normal 7-bit ASCII set are available, but
 * in the wrong position in the mapping (i.e. bitwise ORed with 0x80).
 * That alone would be more-or-less tolerable. However, the built-in home-and-clear functionality
 * assumes that character 0x20 is a space. In the R character set it isn't :-(
 *
 * This class attempts to fix the worst of the behavior by providing a character remap function
 * for a modified hd44780 class, and by providing a clear_row() that clears the row using a loop.
 *
 * https://www.nxp.com/docs/en/data-sheet/PCF2119X.pdf  for details. Character set R is on page 23.
*/

#ifndef PCF2119R_H
#define PCF2119R_H	1

#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Clcd.h>

class pcf2119r : public hd44780_I2Clcd
{
public:
	uint8_t remap(uint8_t value);	// Overrides hd44780::remap(uint8_t value)
	void clear_row(uint8_t row);	// Clear a single row of the display
};

#endif
