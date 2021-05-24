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

/* See the header file pcf2119r.h for a description.
*/
#include <pcf2119r.h>

static const uint8_t PROGMEM charset_r[128] =
{
//	x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xa    xb    xc    xd    xe    xf
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 	// 0x
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 	// 1x
	0xa0, 0xa1, 0xa2, 0xa3, 0x82, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 	// 2x
	0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 	// 3x
	0x23, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 	// 4x
	0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0x8a, 0x23, 0x54, 0x23, 0x5a, 	// 5x
	0x23, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 	// 6x
	0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0x10, 0x55, 0x20, 0x46, 0x13, 	// 7x
};

// Remap the ASCII character set as far as possible.
// This means that if you want a character from CGROM range 0x00 to 0x7f, you have to OR 0x80 with it.
// This mapping has the effect of moving the CGRAM characters to the range 0x80 to 0x8f as well.
// Some of the characters in the CGROM range aren't available because the table maps ASCII characters
// to their nearest equivalent and uses 0x23 (chequerboard) for ASCII characters that aren't present.
uint8_t pcf2119r::remap(uint8_t value)
{
	if ( value < 0x80 )
		return (uint8_t)pgm_read_byte(&charset_r[value]);
	return value & ~0x80;
}

// Clear a row by filling with spaces.
void pcf2119r::clear_row(uint8_t row)
{
	setCursor(0, row);
	for ( uint8_t i = 0; i < 16; i++ )
		write(' ');
}
