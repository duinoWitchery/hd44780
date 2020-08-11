///////////////////////////////////////////////////////////////////////////////
// hd44780 Extensible hd44780 LCD Library Version 1.3.1
//
// hd44780 is an open source project for open source projects. Those wishing to
// create closed projects should seek an alternate solution.
// hd44780 is licensed under the terms of the GNU General Public License v3.0
// as opposed to the more liberal licensing terms of the
// GNU Lesser General Public License (LGPL), MIT, modified BSD, Apache, etc..
// 
// GPL licensing information can found here: https://www.gnu.org/licenses/
///////////////////////////////////////////////////////////////////////////////
//
// hd44780 Library ReadMe: file://./docs/README.html
//
// Overview
// ========
// The hd44780 arduino library is a library package.
// It is more than a simple library to run a specific h/w.
// It is actually an expandable library package that can communicate with
// many different types of LCD hardware and interfaces.
//
// The library is designed to allow communication and interfacing with hd44780
// LCDs over a variety of h/w interfaces.
// Each h/w communication interface has its own C++ i/o class.
//
// The library currently comes with the following i/o subclasses:
//
//    hd44780_HC1627_I2C: control LCD with native i2c interface (Tsingtek Display HC1627)
//    hd44780_I2Cexp: control LCD using i2c i/o exapander backpack (PCF8574 or MCP23008)
//    hd44780_I2Clcd: control LCD with native i2c interface (PCF2116, PCF2119x, etc...)
//    hd44780_NTCU165ECPB: control Noritake CU165ECBP-T2J LCD display over SPI
//    hd44780_NTCU20025ECPB_pinIO: control Noritake CU20025ECPB using direct Arduino pin connections
//    hd44780_NTCUUserial: control Noritake CU-U Series VFD display in serial mode
//    hd44780_pinIO: control LCD using direct Arduino Pin connections
//
// Examples
// ========
// Because the hd44780 library package is more than a simple library, the examples
// are not all together in a single location. 
// The examples are seperated and grouped in to areas as follows:
//
// ioClass: 
//    All the hd44780 library package examples are under here.
//    ioClass contains subdirectories for each included hd44780 i/o class containing
//    sketches that are specific to the i/o class.
//    The examples for each i/o class are grouped together in a directory by
//    the name of the i/o class.
//    In the Arduino IDE they can be found here:
//        [File]->Examples->hd44780->ioClass
//    For example, the examples for the hd44780_I2Cexp i/o class will be
//    found here:
//        [File]->Examples->hd44780->ioClass->hd44780_I2Cexp
//
// otherLibraries:
//     Contains subdirectories for other (non hd44780) libraries that contain
//     wrapper sketches for various hd44780examples sketches. This is intended
//     to allow easy benchmarking of other 3rd party LCD libraries for comparison purposes.
//
// hd44780examples:
//    These are not really example sketches. They should not be looked at as
//    examples for how to use the library. They are special purpose sketches
//    that are used as include files for the i/o class examples.
//    See the ioClass specific examples for each ioClass.
//
// See the readme files under the examples directory and the wiki for
// additional information about the examples.
//
// Custom Character creation tools
// ===============================
//    https://kakedev.github.io/GlyphGenerator/
//    http://www.quinapalus.com/hd44780udg.html
//    https://omerk.github.io/lcdchargen
//
// Github links
// ============
// Version 1.3.1 repository: https://github.com/duinoWitchery/hd44780/tree/1.3.1
// Latest respository: https://github.com/duinoWitchery/hd44780
// hd44780 wiki: https://github.com/duinoWitchery/hd44780/wiki
// (note: Latest repository & wiki may be different from installed version)
//
// Arduino Links
// =============
//    Arduino LiquidCrystal API: https://www.arduino.cc/en/Reference/LiquidCrystal
//    Arduino: http://arduino.cc
//    Arduino Forum: http://www.arduino.cc/forum/
//    Arduino Software: https://www.arduino.cc/en/Main/Software
//    Installing Libraries: https://www.arduino.cc/en/Guide/Libraries
//    Arduino Playground: http://playground.arduino.cc/
//
// Technical Information Links:
// ============================
//
// https://en.wikipedia.org/wiki/Hitachi_HD44780_LCD_controller
//
// hd44780 datasheet:
//    http://fab.cba.mit.edu/classes/863.06/11.13/44780.pdf
//    https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
//
// hd44780 internal RAM addressing:
//    http://web.alfredstate.edu/faculty/weimandn/lcd/lcd_addressing/lcd_addressing_index.html
//
// Arduino board Pinouts:
//    Various Popular Arduino boards:
//      http://www.pighixxx.com/test/pinoutspg/boards/
//      https://github.com/Bouni/Arduino-Pinout
//    Teensy boards:
//      https://www.pjrc.com/teensy/pinout.html
//    Various esp8266 based boards:
//      https://iotbytes.wordpress.com/esp8266-pinouts/
//      https://iotbytes.wordpress.com/nodemcu-pinout/
//      https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/pinouts
//
// PCF8574 datasheet:
//    https://www.aurel32.net/elec/pcf8574.pdf
//    http://www.ti.com/lit/ds/symlink/pcf8574.pdf
//
// MCP23008 datasheet:
//    http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf
//
/////////////////////////////////////////////////////////////////////////


// dummy sketch functions
void setup(void){}
void loop(void){}
