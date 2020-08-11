// vi:ts=4
// ----------------------------------------------------------------------------
// SoftwareWire - how to use SoftwareWire library with hd44780_I2Cexp
// Created by Bill Perry 2020-01-01
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// Demonstrates how to use the SoftwareWire library to allow using different
// Arduino pins for SDA and SCL signals.
//
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// WARNING:
//  The SoftWareWire developers made a major change to the library June 2018
//  which is in the subsequent 1.5.1 release that causes it be unusable by
//  the hd44780 library.
//  Until this change is backed out, you will not be able to use 
//  SoftWareWire beyond version 1.5.0 with the hd44780 library.
//  This sketh will do a check to try to detect the incompatibilty issue
//  and will error off with a compiler error when detected.
//  See this github issue for details:
//  https://github.com/Testato/SoftwareWire/issues/28
//
// NOTE:
// Using the SoftwareWire library is not necessary on the espXXXX platform as
// that platform already has the ability to configure the pins used in the
// Wire library.
// On the esp platforms, include <Wire.h> and set the pins by passing them to 
// Wire.begin(sda, scl) *before* calling lcd.begin()
//
//
// ----------------------------------------------------------------------------

// define the Arduino pins you want to use for sda and scl signals
// use any Arduino pins you want,
// this uses same pins as the i2c h/w pins (SDA, SCL)
const int sda=SDA, scl=SCL;

#include <SoftwareWire.h>	// make sure to not use beyond version 1.5.0
// Check for "new" SoftwareWire that breaks things
#if defined(TwoWire_h)
#error incompatible version of SoftwareWire library (use version 1.5.0)
#endif

SoftwareWire Wire(sda,scl); // Create Wire object using desired Arduino pins

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
hd44780_I2Cexp lcd; // declare lcd object and let it auto-configure everything.

void setup()
{
int istatus;

  istatus = lcd.begin(16,2);
  if(istatus)
  {
	// LCD initalization failed.
	// handle it anyway you want
	lcd.fatalError(istatus); // blinks error code on built in LED
  }
  lcd.print("Hello, World!");
  lcd.setCursor(0,1);
  lcd.print("SoftwareWire");
}

void loop() { }
