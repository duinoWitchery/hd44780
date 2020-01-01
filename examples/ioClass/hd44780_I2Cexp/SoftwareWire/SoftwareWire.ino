// vi:ts=4
// ----------------------------------------------------------------------------
// SoftwareWire - how to use SoftwareWire library with hd44780_I2Cexp
// Created by Bill Perry 2020-01-01
// bperrybap@opensource.billsworld.billandterrie.com
//
// This example code is unlicensed and is released into the public domain
// ----------------------------------------------------------------------------
//
// Sketch prints "Hello, World!" on the lcd
//
// If initialization of the LCD fails and the arduino supports a built in LED,
// the sketch will simply blink the built in LED.
//
// ----------------------------------------------------------------------------

#include <SoftwareWire.h> 
const int sda=SDA, scl=SCL; // use any pins you want, this uses the i2c h/w pins
SoftwareWire Wire(sda,scl);

#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
hd44780_I2Cexp lcd; // declare lcd object and let it auto-configure everything.

int istatus;
void setup()
{

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
