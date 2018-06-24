///////////////////////////////////////////////////////////////////////////////
// hd44780 Extensible hd44780 LCD Library
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
//    hd44780_pinIO: control LCD using direct Arduino Pin connections
//    hd44780_I2Cexp: control LCD using i2c i/o exapander backpack (PCF8574 or MCP23008)
//    hd44780_I2Clcd: control LCD with native i2c interface (PCF2116, PCF2119x, etc...)
//    hd44780_NTCU165ECPB: control Noritake CU165ECBP-T2J LCD display over SPI
//    hd44780_NTCUUserial: control Noritake CU-U Series VFD display in serial mode
//
// Examples
// ========
// Because the hd44780 library package is more than a simple library, the
// examples are not all together in a single location. 
// The examples are seperated and grouped in to areas as follows:
//
// ioClass: 
//    All the hd44780 library package examples are under here.
//    ioClass contains subdirectories for each included hd44780 i/o class
//    containing sketches that are specific to the i/o class.
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
//     to allow easy benchmarking of other 3rd party LCD libraries for
//     comparison purposes.
//
// See the Documentation sketch for additional information.
//
// Note:
//     The library package also includes some special purpose demonstration
//     sketches that can be found under ioClass and otherLibraries
//     - LCDcharset
//        Shows the entire character set of the LCD.
//        Miniture digits are used to show the custom characters.
//
//     - LCDisSpeed
//        Shows the speed of writing characters to the display.
//        It shows the transfer time of an individual character as well as
//        updating the full display. If the geometry is different than 16x2,
//        it will also show what the full display timing would be if the
//        display were 16x2.
//
//        Since the sketch can work on many different libraries and h/w,
//        it is useful for ccomparing the speed of various libraries and LCD h/w.
//
/////////////////////////////////////////////////////////////////////////


// dummy sketch functions
void setup(void){}
void loop(void){}
