# `hd44780` Extensible hd44780 LCD library ![hd44780Version](https://img.shields.io/badge/Version-1.3.1-green.svg?style=plastic)

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

While the library is API compatible with the Arduino IDE bundled LiquidCrystal library, hd44780 is not a direct drop in replacement for the LiquidCrystal library.

The API functionality provided by the hd44780 base class, when combined
with an hd44780 library i/o subclass, is compatible with most of the API
functionality of the Arduino LiquidCrystal library as well as
with most of the LCD API 1.0 Specification (some of which is nearly obsolete).

The hd44780 API also provides some addtional extensions and all the API
functions provided by hd44780 are common across all i/o subclasses.
The most most significant extensions being:
- ability to enable automatic line wrapping
- ability to modify the libraries expected command execution times.
- API functions return a status to indicate whether successful
- automatic detection and s/w work around for LCD keypad shields with bad backlight circuit
- self configuration of i2c address and pin mappings for i2c backpacks
- ability to tell if lcd initialization failed
- ability to read data or status from the LCD (requires r/w control)
- ability to tell if sending a raw command to the LCD failed

S/W requirements
================
- ### IDE version 1.0.1 or later (versions 1.6.9 and higher are recommended)

- ### IDE versions that should be avoided:
	- IDE versions 1.5 to 1.55 (unnecessary file name restrictions, breaks many libraries)
	- IDE version 1.6.6 (has function prototyping issues that can break some sketches)
	- IDE version 1.6.8 (has serial port issues that breaks on certain boards)

H/W support
===========
Library should work on all Arduino boards including boards with AVR, pic32, arm, and ESP8266 processors.

**WARNING**<br>
Use caution when using 3v only processors like arm and ESP8266 processors when interfacing with 5v modules
as not doing proper level shifting or incorrectly hooking things up can damage the processor.

The library currenly comes with the following i/o subclasses:

* `hd44780_HC1627_I2C` control LCD with native i2c interface (Tsingtek Display HC1627)

* `hd44780_I2Cexp` control LCD using i2c i/o exapander backpack (PCF8574 or MCP23008)

* `hd44780_I2Clcd` control LCD with native i2c interface (PCF2116, PCF2119x, etc...)

* `hd44780_NTCU165ECPB` control Noritake CU165ECBP-T2J LCD display over SPI

* `hd44780_NTCU20025ECPB_pinIO` control Noritake CU20025ECPB using direct Arduino pin connections

* `hd44780_NTCUUserial` control Noritake CU-U Series VFD display in serial mode

* `hd44780_pinIO` control LCD using direct Arduino Pin connections

Installation
============
For generic information about Arduino libraries and how to isntall them consult the Arduino Libraries page:
https://www.arduino.cc/en/Guide/Libraries

For ease of proper installation, the preferred method of installing the library is to use the library manager.
The use of importing a .zip file is highly discouraged unless using IDE versions prior to version 1.6.2

### Installation using Library manager (IDE 1.6.2 and later)
In the IDE, Simply click on:<br>
**[Sketch]->Include Library->Manage Libraries...**<br>
or<br>
**[Tools]->Manage Libraries...**<br>
Then search for "Extensible hd44780" to locate the library and install it.<br>
There is no need to uninstall or modify any other LCD libary that may have already been installed.

#### Installation using zip file w/o library manager (IDE 1.6.1 and earlier)
Installation requires downloading a zip image and
then, depending on the version of the IDE you can either install it from the IDE or install it manually by unziping the file into your sketchbook libraries.

**NOTE**: due to the way github creates its zip files, there is a manual step that must be done to rename the library directory created in your sketchbook libraries area.

First, dowload a library zip image from the github repository releases area:<br>
https://github.com/duinoWitchery/hd44780/releases <br>
Download the image by clicking the zip icon for the desired release.

#### Installation of zip file library on IDE 1.0.6 to 1.6.1
The library can be installed from the zip image using the IDE.
Install it by clicking on:
[Sketch]->Include Library->Add .ZIP library...
then, simply select the hd44780.zip image you downloaded.

The hd44780 sketchbook library must now be renamed. (see below)

#### Installation of zip file library on IDE 1.0 to 1.0.5
On these versions of the IDE, the install must be done manually.
To install the library simply extract it into your sketchbook/libraries directory.
If you don't know where you sketchbook/libraries directory is simply click on:
[File]->Prefernces
or from the keyboard type: &lt;ctrl&gt;comma (hold ctrl and press comma)
The location of your sketchbook directory will be in the text box.
The zip image must be installed in a directory called "libraries" under that directory.<br>

After the zip file has been extracted and the hd44780 sketchbook library has been created,
the library must now be renamed. (see below)

### Renaming hd44780 libary directory name after manual installation w/o library manager
When not using the library manager, the hd44780 library directory created in your sketchbook libary area will not be the correct name.
This is due to the way github creates its zip files and the way the IDE library manager works.
The downloaded zip file and internal directory will have a name like hd44780-X.Y.Z which causes the sketchbook library name to also be hd44780-X.Y.Z
While the IDE will usually allow this to work, the proper name should be simply "hd44780" and not using its real name can cause issues in the future.<br>
To rename the library directory, simply go to the sketchbook library location and rename it.
If you don't know where you sketchbook/libraries directory is simply click on:
[File]->Prefernces
or from the keyboard type: &lt;ctrl&gt;comma (hold ctrl and press comma)
The location of your sketchbook directory will be in the text box.<br>
Use your favorite tool to rename it.

API Summary
===========
The table below is a summary of all the available API functions in the hd44780 library.

Additional information about the API functions and how to use them
can be found in the examples.


|Function                               | Description                           |
| --------------------------------------| --------------------------------------|
| **LiquidCrystal API**                 | https://www.arduino.cc/en/Reference/LiquidCrystal |
| init(...)                             | **not supported**
| begin(cols, rows)                     | initialize communication interface and LCD<br> **hd44780 extension**: returns zero on success |
| clear()                               | clear the display and home the cursor<br> **hd44780 extension**: returns zero on success |
| home()	                        | home the cursor<br> **hd44780 extension**: returns zero on success |
| setCursor(col, row)                   | set cursor position<br> **hd44780 extension**: returns zero on success |
| write(data)	                        | send data byte to the display<br>returns 1 on success |
| write(*str)	                        | send C string to the display<br>returns characters written |
| write(*buffer, size)                  | send size bytes to the display<br>returns characters written<br>returns size on success |
| print(...)                            | print formatted data on the display<br>(from Print class)<br>returns characters output |
| println(...)                          | **not supported** (from Print class) |
| cursor()                              | turn on underline cursor<br> **hd44780 extension**: returns zero on success |
| noCursor()                            | turn off/hide cursor<br> **hd44780 extension**: returns zero on success |
| blink()                               | enable blinking at cursor position<br> **hd44780 extension**: returns zero on success |
| noBlink()                             | disable blinking at cursor position<br> **hd44780 extension**: returns zero on success |
| display()                             | enable pixels on display<br> **hd44780 extension**: returns zero on success |
| noDisplay()                           | disable/hide pixels on display<br> **hd44780 extension**: returns zero on success |
| scrollDisplayLeft()                   | shift display contents left<br> **hd44780 extension**: returns zero on success |
| scrollDisplayRight()                  | shift display contents right<br> **hd44780 extension**: returns zero on success |
| autoscroll()                          | enable left/right autoshifting for new characters<br> **hd44780 extension**: returns zero on success |
| noAutoscroll()                        | disable left/right autoshifting<br> **hd44780 extension**: returns zero on success |
| leftToRight()                         | write left to right, set autoshift to left<br> **hd44780 extension**: returns zero on success |
| rightToLeft()                         | write right to left, set autoshift to right<br> **hd44780 extension**: returns zero on success |
| createChar(charval, charmap[])        | create a custom character<br> **hd44780 extension**: returns zero on success |
| setRowOffsets(row0)                   | set address for start of line  <br> **hd44780 extension**: returns zero on success |
| setRowOffsets(row0, row1)             | set address for start of each line  <br> **hd44780 extension**: returns zero on success |
| setRowOffsets(row0, row1, row2, row3) | set address for start of each line  <br> **hd44780 extension**: returns zero on success |
| command(cmd)                          | send raw 8bit hd44780 command to LCD<br> **hd44780 extension**: returns zero on success |
|                                       ||
| **hd44780 extensions**<br>Included in hd44780 but not part of LiquidCrytal or LCD 1.0 API ||
| createChar(charval, charmap[])        | create a custom character<br>(supports charmap in AVR PROGMEM)<br>returns zero on success |
| backlight()	                        | turn on backlight (max brightness)<br>returns zero on success |
| noBacklight()                         | turn off backlight<br>returns zero on success    |
| lineWrap()                            | turn on automatic line wrapping<br>(wraps lines but does not scroll display)<br>returns zero on success
| noLineWrap()                          | turn off automatic line wrapping<br>returns zero on success
| moveCursorLeft()                      | move cursor one space to right<br>returns zero on success |
| moveCursorRight()                     | move cursor one space to left<br>returns zero on success |
| read()                                | read data byte from LCD<br>(requires r/w signal control)<br>returns negative value on failure |
| setExecTimes(chUs, insUs)             | configure clear/home and instruction/data times |
|                                       ||
| **LCD API 1.0 Functions**             | http://playground.arduino.cc/Code/LCDAPI
| setBacklight(dimvalue)                | set backlight brightness (0-255)<br> **hd44780 extension**: returns zero on success |
| setContrast(contvalue)                | set contrast (0-255)<br> **hd44780 extension**: returns zero on success |
| on()                                  | turn on LCD pixels and backlight<br> **hd44780 extension**: returns zero on success |
| off()                                 | turn off LCD pixels and backlight<br> **hd44780 extension**: returns zero on success |
| status()                              | read hd44780 status byte (busy flag & address)<br>(requires r/w signal control)<br> returns negative value on failure |
|                                       ||
| **Deprecated LCD API 1.0 Functions**<br>These exist in hd44780 but are deprecated||
| cmdDelay(CmdDelay, CharDelay)         | use setExecTimes() instead |
| cursor_on()                           | use cursor() instead<br> **hd44780 extension**: returns zero on success |
| cursor_off()                          | use noCursor() instead<br> **hd44780 extension**: returns zero on success |
| blink_on()                            | use blink() instead<br> **hd44780 extension**: returns zero on success |
| blink_off()                           | use noBlink() instead<br> **hd44780 extension**: returns zero on success |
| load_custom_character(char_num, Rows[]) | use createChar() instead<br> **hd44780 extension**: returns zero on success |
| **BROKEN LCD API 1.0 Functions**      ||
| setCursor(row, col)                   | row,col is backwards from Liquidcrystal;<br>therefore cannot be supported|

Examples
========
Examples for the included hd44780 i/o classes are provided to demonstrate how to use LCDs with various h/w interfaces.<br>
Each i/o class included in the hd44780 library package has its own examples.<br>
You can access i/o class examples from the IDE from [File]->Examples->hd44780->ioClass and then choose the desired i/o class.<br>
hd44780 also includes some examples that use other 3rd party LCD libraries rather than hd44780 which can be found under otherLibraries.<br>
**The sketches in the hd44780examples area are special purpose sketches that are used as include files for the i/o class examples.
They should not be looked at as examples for how to use the library.**
While the hd44780examples sketches can be modified to work standalone with any LiquidCrystal API compatible library, their primary purpose, as shipped in the hd44780 library package, is to act as an include file for the i/o class wrapper sketches and are not intended to be used directly.  
**See the ioClass specific subdirectories for the examples for each ioClass.**

See **hd44780examples** sketch along with the readme files under the examples directory and the wiki for
additional information about the examples.<br>
If browsing on the hd44780 github repository, you can click on the examples
directory and its subdirectories to see each readme file.

Additional Information & Documentation
======================================
You can find additional information by using the included **Documentation** sketch.<br>
Once the hd44780 library package is installed. It can be found in the IDE from [File]->Examples->Documentation<br>
It provides hd44780 library information as well as links to further information including
* links to the hd44780 repository, hd44780 wiki.
* hd44780 custom font tools
* links to various Arduino boards pinout information and diagrams
* datasheets for various LCD h/w



CHANGELOG
----------------------

[1.3.1] - 2020-08-10
 * hd44780_pinIO examples updated to support lcdkeypad on espduino32
 * hd44780_I2Cexp SoftwareWire example has link to version 1.5.1 issue
 * hd44780_pinIO updates for esp32 platform. ESP32 does not support analogWrite()
 * Minor documentation upates
 * fixed typo in main README file

[1.3.0] - 2020-07-01
 * update version info for 1.3.0 release
 * added hd44780_NTCU20025ECPB i/o class
 * LCDlibTest example prints message if backlight/noBacklight/setBacklight not supported
 * Added hd44780_HC1627_I2C i/o class
 * Added Serial2LCD example
 * update version info for 1.2.1 release
 * I2CexpDiag - report initialization failure status code
 * hd44780_I2Cexp: tweak to MCP23008 auto config for Adafruit #292 board
 * hd44780_I2Cexp: fixed constructor issue for MCP23008 canned entries

[1.2.0] - 2020-06-03
 * update version info for 1.2.0 release
 * added SDA/SCL pin output decodes for STM32 platform
 * Fixes to LCDiSpeed examples for LiquidCrystal_PCF8574
 * Added LiquidCrystalIO support for LCDiSpeed benchmarks
 * Tweak to LCDiSpeed examples for newLiquidCrystal LiquidCrystal_I2C to fix compilation issue
 * LCDiSpeed tweak to ensure backlight is on for LiquidCrystal_I2C library.
 * I2CexpDiag reduced the maximum default lcd devices to 4 to save RAM
 * I2CexpDiag hack workaround for RogerClarks STM32 platform
 * I2CexpDiag print "undefined" for F_CPU when F_CPU not defined
 * Removed INPUT_PULLUP check I2CexpDiag

[1.1.1] - 2020-03-28
 * update version info for 1.1.1 release
 * Minor tweak to I2CexpDiag for ESP32 core
 * Tweak to work around LED_BUILTIN define issue in ESP32 core
 * examples no longer alter status code sent to fatalError() as it isn't necessary
 * Detect incompatible version of SoftWareWire (version 1.5.1 broke things)
 * Added LiquidCrystalFast and LiquidCrystal_PCF8574 to "otherLibraries" benchmark sketches.
 * tweaked newLiquidCrystal LCDiSpeed sketch to better find NewLiquidCrystal library.
 * updated library properties paragraph to mention noritake displays
 * Updated copyright year
 * Added examples and benchmarks for using SoftwareWire soft i2c library
 * fixed return status bug in NTCUUserial iosetBacklight()
 * Examples no longer modify/adjust return status values sent to fatalError()

[1.1.0] - 2019-08-12
 * update version info for 1.1.0 release
 * updates for reinitalization using begin() & init() and use of "new"
 * Added support for 1 and 2 lines/rows to setRowOffsets()
 * Added build tool note about using grip tool for viewing README.md file
 * hd44780_pinIO fixed backlight check tweak that broke backlight check
 * Clarified define to disable ESP8266 specific pin decoding
 * updates to support use of "new" for lcd objects

[1.0.2] - 2019-05-21
 * update version info for 1.0.2 release
 * Corrected table alignment issue from removing "Optional".
 * Removed "Optional" word from LCD API functions section title (was confusing)
 * fixed typo in constructor in hd44780_pinIO HelloWorld example
 * fatalError(errcode) now accepts negative errcode values
 * Better SDA/SCL pin shorted together testing
 * updated comments in LCDKaypadCheck to better explain BL circuit bad

[1.0.1] - 2018-08-06
 * update version info for 1.0.1 release
 * Removed I2C TinyWireM work around (TinyWireM was fixed)  works around Wire bug in ESP32 core

[1.0.0] - 2018-06-24
 * update version info for 1.0.0 release
 * change readme version badge to green
 * hd44780examples special sketch documentation updates
 * Updates to readme for non hd44780 library examples
 * readme cleanup for release
 * I2CexpDiag slight change to SDA and SCL digital pin output message
 * I2CexpDiag now checks for SDA and SCL being shorted together

[0.9.4] - 2018-06-12
 * update version info for 0.9.4 release
 * README tweaks for release
 * readme and sketch documentation/comment updates
 * Added hd44780examples sketch to document examples and hide hd44780examples sketches
 * loops that wait on Serial now call delay() which calls yield()
 * hd44780_I2Cexp now returns RV_ENXIO error if device not at specified address
 * I2CexpDiag prints messsage about potential poor solder joints on memory errors
 * added delay(1) to error output which calls yield() to avoid watchdog on some cores
 * bumped default instruction time from 37us to 38us
 * LineWrap example explains limitations when using 16x1 / 8x2 displays
 * Correct indention issue in LCDCustomChars example

[0.9.3] - 2018-02-11
 * update version info for 0.9.3 release
 * I2CexpDiag updates to output esp8266 specific i2c pin information.
 * mkreleasePatcher tool updates
 * Documenation sketch updates
 * README, comment, and error message udpates for hd44780examples
 * Added README for the Documentation.ino sketch

[0.9.2] - 2018-02-04
 * update version info for 0.9.2 release
 * Bug fix to mkreleasePostPatcher
 * Added initial README.html file
 * updates to various readme files
 * TAB/space alignment correction in comments of many sketches.
 * update mkrelease tools to auto update version info in Documentation files
 * Added Documenation sketch to provide links to documenation
 * Updated Copyright year
 * updated documenation link in hd44780_i2Clcd.h
 * tweaks to hd44780_pinIO class code and examples for esp8266 based boards.
 * LCDCustomChars - added additional custom character builder website link

[0.9.1] - 2018-01-06
 * update version info for 0.9.1 release
 * added missing LCDcharset sketch to hd44780_NTCU165ECPB i/o class
 * updated keyword file, readme files, and added comments to some sketches
 * LCDiSpeed prints actual i2c clock rate when core supports access to it
 * fixed timing issue in status()
 * Updated LCDCustomChars sketch - no longer shows full lcd character set
 * added LCDcharset template sketch to hd44780examples
 * report read data mismatch in ReadWrite Sketch
 * setRowOffsets() can now be called before begin()
 * Added LiquidCrystal_I2C compatible constructor to hd44780_I2Cexp
 * Added LCD API 1.0 init() function
 * 3rd party otherLibraries now only have LCDiSpeed example
 * Fixed bug in LiquidCrystal hd44780examples
 * fixed bug in a hd44780 constructor
 * overload to allow write(0) without having to use a cast
 * createChar() support for AVR PROGMEM data
 * I2Cexp fixed issue with handling of E signal on read of 2nd nibble
 * readme updates
 * Use h/w SPI pins for examples, when possible
 * Have to stop SPI for s/w read
 * Use hardware SPI for CU-U iowrite
 * add CU-U examples
 * add CU-U to READMEs
 * Cleanup and add documentation for Noritake CU-U series
 * Add io class for Noritake CU-U Series VFD (in serial mode)
 * removed old hd44780examples/CustomChars wrapper scketch from all i/o classes
 * updated Arduino IDE keywords
 * move hd44780 example CustomChars to each i/o class as LCDCustomChars
 * removed sprintf() from LCDiSpeed

[0.9.0] - 2017-05-12
 * update version info for 0.9.0 release
 * Simplification of HelloWorld sketch examples
 * comment clarifications to UpTime example sketches
 * Updates to work around TinyWireM library bugs
 * Added UpTime example sketch for all bundled i/o classes
 * linewrap examples updates for better redibility
 * setCursor() wraps when auto linewrap enabled and col beyond end of line
 * turn on cursor during LineWrap examples
 * linewrap tweak for better visual cursor positio

[0.8.6] - 2017-05-11
 * update version info for 0.8.6 release
 * updated examples README
 * renamed examples/OtherLibraries to examples/otherLibraries
 * Added LineWrap examples
 * added automatic linewrap functionality and API
 * Added section about Examples to main readme

[0.8.5] - 2017-04-29
 * update version info for 0.8.5 release
 * Tweaks to comments in examples and readme files
 * Added 'OtherLibraries' wrapper example sketches for some non hd44780 libraries
 * updates to readme files in examples

[0.8.4] - 2017-03-05
 * update version info for 0.8.4 release
 * I2CexpDiag udpates for ESP8266 and to blink backlight 3 times after initialization
 * Comment typo corrections & Word Smithing.
 * Tweaks to allow LCDiSpeed & LCDlibTest to run with either LiquidCrystal_I2C or fm's New LiquidCrystal
 * Tweak to I2Cexp header to allow it to be included in multiple compilation modules.
 * Added canned backpack entry to I2Cexp i/o class for SY1622 backpacks
 * added i/o class readme files
 * Added readme files for examples and ioClass directories

[0.8.3] - 2017-01-08
 * update version info for 0.8.3 release
 * added  keywords to library.properties paragraph for better Arduino IDE library manager searching
 * README updates
 * hd44780_I2Cexp unknown address is now zero vs an enum/define
 * refactored fatalError() code to live in hd44780 class for better support and maintenance.
 * Added clarifying comments to sketches
 * NTCU165ECPB bug fixes & timing fixes in i/o class, fixes for Leonardo, clarifying comments in examples.

[0.8.2] - 2016-12-26
 * update version info for 0.8.2 release
 * I2Cexpdiag: ESP8266 updates, more system info, multi display updates, better i2c checks
 * README updates
 * hd44780 updated comments, added new BUSY status error return value
 * Seperated license information from actual license file for licensee script compatibilty.
 * updated hd44780_I2Cexp.h comments for constructor usage
 * hd44780_pinIO check for broken backlight only on active HIGH backlights
 * hd44780_I2Clcd i2c address auto discovery
 * New I2Cexp constructor for auto locate with canned entry for configuation

[0.8.1] - 2016-11-12
 * update version info for 0.8.1 release
 * added some clarifying comments to various files
 * pinIO class will now auto detect and safely handle broken backlight circuits
 * added Noritake CU165ECBP-T2J to list of supported h/w devices in readme file
 * tweak to hd44780.h to not expect/require Printable class.
 * pinIO class ReadWrite example now atempts to indicate errors on LCD
 * added most common hd44780 pinout to comments in pinIO class helloworld  example
 * fixed bug in pinIO class to properly report status when no read support.
 * added pinIO class LCDKeypadCheck sketch

[0.8.0] - 2016-11-01
 * update version info for 0.8.0 release
 * updated README current status
 * added i/o class and examples for Noritake CU165ECBP-T2J display
 * moved all i/o sub class headers to hd44780ioClass directory
 * added canned entry of sunrom i2c backpack
 * relocated all io class examples to live under examples/ioClass directory

[0.7.3] - 2016-10-23
 * update version info for 0.7.3 release
 * updates to current library status in readme
 * updated non library manager installation instructions
 * display digital pin #s for a4 and a5 on boards that have them.
 * corrected DDRAM address mask in createChar()
 * createChar() will restore DDRAM address on devices that support reads
 * added Custom Character example
 * added I2Cexp canned entry for elecfreaks i2c/spi backpack
 * corrected Serial output formatting issue in LCDlibTest
 * All device API functions now return status
 * updated sketches to call a fatalError() routine to blink the led on fatal errors
 * Corrected ioread in I2Cexp to properly return errors

[0.7.2] - 2016-09-21
 * update version info for 0.7.2 release
 * updated README to indicate println() not supported
 * added automation scripts to aid in building releases and generating changelog
 * update to PCF8574 autoconfig to make it more robust
 * tweaks to hd44780 examples & fixes for single line displays
 * changed order of parameters for iowrite() to be more compatibile with ioread()
 * catch calls to println() and error them off at compile time
 * corrected LCDiSpeed test to properly show FPS when not using 16x2 display

[0.7.1] - 2016-08-20
 * update version info for 0.7.1 release
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
