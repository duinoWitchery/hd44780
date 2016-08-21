# `hd44780` Extensible hd44780 LCD library ![hd44780Version](https://img.shields.io/badge/Version-0.7.1-yellow.svg?style=plastic)

CurrentStatus
=============
The library is currently in an alpha state.

While the API and underlying code is stable and has been tested, there are
likely to be some changes in the constructors for the i/o subclasses that will
not be backward compatible with the existing constructors, particuarly for the
`hd44780_I2Cexp` i/o subclass.

These constructor changes are not anticipated to affect lcd object definitions
that use `hd44780_I2Cexp` autoconfiguration.

After the constructors are finalized, the project will move to a beta state
and should quickly move to a 1.0.0 release candidate followed by a
1.0.0 release.

Licensing
=========
![GPLv3Logo](https://www.gnu.org/graphics/gplv3-127x51.png)

hd44780 is an open source project for open source projects. Those wishing to
create closed projects should seek an alternate solution.
hd44780 is licensed under the terms of the GNU General Public License v3.0
as opposed to the more liberal licensing terms of the
GNU Lesser General Public License (LGPL), MIT, modified BSD, Apache, etc..

GPL licensing information can found here: https://www.gnu.org/licenses/

For full details on the licensing of the hd44780 library and its components, see the included licensing file.

Library Overview
=================
hd44780 is an extensible Arduino LCD library for hd44780 based LCD displays.
The library consists of a hd44780 base class combined with one or more
i/o subclasses to perform the i/o communication between the host and the
hd44780 display interface.

The hd44780 library is not a direct drop in replacement for the the Arduino IDE LiquidCrystal library.

The API functionality provided by the hd44780 base class, when combined
with an hd44780 library i/o subclass, is compatible with most of the API
functionality of the Arduino LiquidCrystal library as well as
with most of the LCD API 1.0 Specification (some of which is nearly obsolete).

The hd44780 API also provides some addtional extensions and all the API
functions provided by hd44780 are common across all i/o subclasses.
The most most significant extensions being:
- ability to modify the libraries expected command execution times.
- ability to tell if lcd initialized failed
- ability to read data or status from the LCD (requires r/w control)
- ability to tell if sending a raw command to the LCD failed

S/W requirements
================
- ### IDE version 1.0 or later

- ### IDE versions that should be avoided:
	- IDE versions 1.5 to 1.55 (unnecessary file name restrictions, breaks many libraries)
	- IDE version 1.6.6 (has function prototyping issues that can break some sketches)
	- IDE version 1.6.8 (has serial port issues that breaks on certain boards)


H/W support
===========
Library should work on all Arduino boards.

The library currenly comes with the following i/o subclasses:

* `hd44780_pinIO` control LCD using direct Arduino Pin connections

* `hd44780_I2Cexp` control LCD using i2c i/o exapander backpack (PCF8574 or MCP23008)

* `hd44780_I2Clcd` control LCD with native i2c interface


Installation
============
For ease of installation it is recommended to use an IDE that supports the library mananager which was implemented in IDE version 1.6.2

### Installation using Library manager (IDE 1.6.2 and later)
In the IDE, Simply click on [Sketch]->Include Library->Manage Libraries...
Then search for "Extensible hd44780" to locate the library and install it.

### Installation using zip file (IDE 1.6.1 and earlier)
Installation requires downloading a zip image and
then, depending on the version of the IDE you can either install it from the IDEor install it manually by unziping the file into your sketchbook libraries.

1) dowload a library zip image from the github repository:
https://github.com/duinoWitchery/hd44780
Download the image by clicking the green box that says [Clone or Download].

2) Rename the downloaded and saved the zip image to hd44780.zip

#### Installation of zip file library on IDE 1.0.6 to 1.6.1
The library can be installed from the zip image using the IDE.
Install it by clicking on:
[Sketch]->Include Library->Add .ZIP library...
then, simply select the hd44780.zip image you downloaded and renamed.

#### Installation of zip file library on IDE 1.0 to 1.0.5
On these versions of the IDE the install must be done manually.
To install the library simply extract it into your sketchbook/libraries directory.
If you don't know where you sketchbook/libraries directory is simply click on:
[File]-Prefernces
or from the keyboard type: &lt;ctrl&gt;comma (hold ctrl and press comma)
The loation of your sketchbook directory will be in the text box.
The zip image must be installed in a directory called "libraries" under that directory.

API Summary
===========
The table below is a summary of all the available API functions in the hd44780 library.

Additional information about the API functions and how to use them
can be found in the included examples.


|Function                               | Description                           |
| --------------------------------------| --------------------------------------|
| **LiquidCrystal API**                 | https://www.arduino.cc/en/Reference/LiquidCrystal |
| begin(cols, rows)                     | initialize communication interface and LCD<br> **hd44780 extension**: non zero return value is failure |
| clear()                               | clear the display and home the cursor |
| home()	                        | home the cursor |
| setCursor(col, row)                   | set cursor position |
| write(data)	                        | send data byte to the display |
| write(*str)	                        | send C string to the display |
| write(*buffer, size)                  | send size bytes to the display |
| print(...)                            | print formatted data on the display (from Print class) |
| cursor()                              | turn on underline cursor |
| noCursor()                            | turn off/hide cursor |
| blink()                               | enable blinking at cursor position |
| noBlink()                             | disable blinking at cursor position |
| display()                             | enable pixels on display |
| noDisplay()                           | disable/hide pixels on display |
| scrollDisplayLeft()                   | shift display contents left |
| scrollDisplayRight()                  | shift display contents right |
| autoscroll()                          | enable left/right autoshifting for new characters |
| noAutoscroll()                        | disable left/right autoshifting |
| leftToRight()                         | write left to right, set autoshift to left |
| rightToLeft()                         | write right to left, set autoshift to right |
| createChar(charval, charmap[])        | create a custom character |
| moveCursorLeft()                      | move cursor one space to right |
| moveCursorRight()                     | move cursor one space to left |
| setRowOffsets(row0, row1, row2, row3) | set address for start of each line                                        |
| command(cmd)                          | send raw 8bit hd44780 command to LCD<br> **hd44780 extension**: non zero return value is failure |
|                                       ||
| **hd44780 extensions**<br>Included in hd44780 but not part of LiquidCrytal or LCD 1.0 API ||
| backlight()	                        | turn on backlight (max brightness) |
| noBacklight()                         | turn off backlight    |
| read()                                | read data byte from LCD<br>returns negative value on failure<br>(requires r/w signal)|
| setExecTimes(chUs, insUs)             | configure cmd and instruction/data times |
|                                       ||
| **Optional LCD API 1.0 Functions**    | http://playground.arduino.cc/Code/LCDAPI
| setBacklight(dimvalue)                | set backlight brightness (0-255) |
| setcontrast(contvalue)                | set contrast (0-255) |
| on()                                  | turn on LCD pixels and backlight |
| off()                                 | turn off LCD pixels and backlight |
| status()                              | read hd44780 status byte (busy flag & address)<br> returns negative value on failure<br>(requires r/w signal) |
|                                       ||
| **Deprecated LCD API 1.0 Functions**<br>These exist in hd44780 but are deprecated||
| cmdDelay(CmdDelay, CharDelay)         | use setExecTimes() instead |
| cursor_on()                           | use cursor() istead |
| cursor_off()                          | use noCursor() instead |
| blink_on()                            | use blink() instead |
| blink_off()                           | use noBlink() instead |
| load_custom_character(char_num, Rows[]) | use createChar() instead |
| **BROKEN LCD API 1.0 Functions**      ||
| setCursor(row, col)                   | row,col is backwards from Liquidcrystal;<br>therefore cannot be supported|


CHANGELOG
----------------------

[0.7.1] - 2016-08-20
 * removed build tools for library manager compability
 * added lcdproc examples
 * update version info for 0.7.0 release
 * added automation scripts to aid in building releases and generating changelog
 * a few more tweaks to PrintUpTime
 * added ReadWrite example
 * removed BACKLIGHT_OFF and BACKLIGHT_ON defines
 * updated PrintUpTime() to not rollover as soon.

[0.6.0] - 2016-08-08
 * update version info for 0.6.0 release
 * I2Cexp constructors optionally support r/w control. PCF8474 canned entries enable r/w
 * I2CexpDIag displays pin configuration on serial port
 * added lcd display memory test to I2CexpDiag
 * added ability to read from display (data & status)
 * better documented hd44780 initialization and iosend() IOcmd4bit commands
 * removed warnings when i/o functions setBacklight() or setContrast() are not used.

[0.5.0-a] - 2016-07-30
