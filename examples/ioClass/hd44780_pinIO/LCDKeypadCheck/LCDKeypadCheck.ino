//
// vi:ts=4
// ----------------------------------------------------------------------------
// LCDKeypadCheck - LCD keypad shield backlight circuitry test
// Copyright 2013-2020 Bill Perry
// bperrybap@opensource.billsworld.billandterrie.com
// ---------------------------------------------------------------------------
//
//  LCDKeypadCheck is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation version 3 of the License.
//
//  LCDKeypadCheck is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with LCDKeypadCheck.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------------
//
// sketch to test lcd keypad shields for bad backlight circuitry
//
// See this Arduino forum thread for a discussion about the isue:
// http://forum.arduino.cc//index.php?topic=96747
//
// NOTE:
//	As of 2016-11-12 in release 0.8.1 the hd44780_pinIO class now
//	automatically detects broken backlight circuits and will use backlight
//	control methods to protect the Arduino processor. The sketch can simply
//	use all the backlight API functions setbacklight(), backlight(), and
//	noBacklight() without concern.
//
// To use this sketch, fill in the proper pin assignements if they are
// different from below.
//
// upload the sketch.
// The LCD display will show whether the backlight circuit is good/bad.
//
// If the LCD shows that the backlight circuit is bad,
// it means that the backlight control circuit on the shield is a bad design
// and has a short circuit issue on the Arduino pin that controls the
// backlight circuit. (usually digital pin 10)
// This means that you need to be careful not to ever set the backlight control
// pin (D10) to HIGH as the high current draw could damage the Arduino processor.
//
// All is not lost, the shield can still be used and the backlight can still
// be controlled. Just keep in mind that unless the hardware is
// modified, some precautions must be taken when controlling the backlight.
// The main thing is that backlight control pin (digital pin 10) should
// never be set to HIGH.
// This means that PWM or analogWrite() cannot be used to dim the backlight.
//
// If you are happy with the backlight on all the time, then nothing needs to
// be done and you can use the LCD without backlight control.
//
// A simple software only solution for backlight on/off control
// is to set the backlight pin to INPUT to turn on the backlight and set the
// backlight pin to OUTPUT mode to turn the backlight off.
// If using hd44780 library 0.8.1 or newer the library will automatically 
// do this for the sketch when the backlight API functions are used.
// 
// History
// 2018.09.18 bperrybap  - updated comments to clarify what
//                         "BL Circuit BAD" means
// 2017.01.07 bperrybap  - updated comments to reflect that library now
//                         automatically detects bad backlight circuits
// 2016.11.08 bperrybap  - updated for inclusion in hd44780 library
// 2013.10.29 bperrybap  - Original creation
//
// ---------------------------------------------------------------------------

#include <hd44780.h>
#include <hd44780ioClass/hd44780_pinIO.h> // Arduino pin i/o class header

// initialize the library with the numbers of the interface pins
// note that ESP8266 based arduinos must use the Dn defines rather than
// raw pin numbers.
#if defined (ARDUINO_ARCH_ESP8266)
const int rs=D8, en=D9, db4=D4, db5=D5, db6=D6, db7=D7; // for esp8266 devices
const int pin_BL = D10; // arduino pin wired to LCD backlight circuit
#elif defined(ARDUINO_ARCH_ESP32)
// esp32 espduino32 D1 R32 (uno form factor)
// note: GPIO12 needs a pulldown resistor
const int rs=12, en=13, db4=17, db5=16, db6=27, db7=14;
const int pin_BL=5; // gpio pin # wired to LCD backlight circuit
#else
const int rs = 8; // arduino pin wired to LCD RS
const int en = 9; // arduino pin wired to LCD EN
const int db4 = 4;// arduino pin wired to LCD db4
const int db5 = 5;// arduino pin wired to LCD db5
const int db6 = 6;// arduino pin wired to LCD db6
const int db7 = 7;// arduino pin wired to LCD db7
const int pin_BL = 10; // arduino pin wired to LCD backlight circuit
#endif

hd44780_pinIO lcd( rs, en, db4,  db5,  db6, db7);


/*
 * Macros to safely turn on the backlight even with back BL hardware
 * These assume that the BL pin is not touched or used after RESET other
 * than by these macros.
 */

#define SafeBLon(pin) pinMode(pin, INPUT)
#define SafeBLoff(pin) pinMode(pin, OUTPUT)

int status;

void setup()
{
	// set up the LCD's number of columns and rows: 
	lcd.begin(16, 2);

	status = pinTest(pin_BL); // only run the actual test once
}

void loop()
{

	lcd.clear();
	if(status)
	{
		/*
		 * Shield has BL circuit issue
	 	 */
		lcd.print("BL Circuit BAD");
		safeBlink(pin_BL, 5); // safely blink the backlight
	}
	else
	{
		/*
		 * Shield is OK (no BL circuit issue)
	 	 */
		lcd.print("BL Circuit GOOD");
		softBlink(pin_BL, 2); // soft blink the backlight (uses PWM)
	}

	delay(1000);
}

/*
 * Function to test a backlight pin
 * Returns non-zero if test fails (bad circuit design)
 */
int pinTest(int pin)
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
	 * being driven HIGH by the Arduino processor.
	 */
	if (val != HIGH)
		return(-1); // test failed
	else
		return(0); // all is ok.
}

void safeBlink(int pin, int count)
{
	/*
	 * Safely blink the backlight on LCD shields that have
	 * broken BL hardware
	 * Uses the SafeBL macros defined above.
	 */

	while(count--)
	{
		delay(200);
		SafeBLoff(pin); // turn on the backlight (safe to use for all shields)
		delay(50);
		SafeBLon(pin); // turn off the backlight (safe to use for all shields)
	}
}

/*
 * soft blink the backlight.
 * NOTE: this should not be used on a shield
 * with a bad backlight circuit
 */
void softBlink(int pin, int count)
{
// note: esp32 core does not support analogWrite()
#if !defined(ARDUINO_ARCH_ESP32)
	// soft blink the backlight by ramping down then back up
	pinMode(pin, OUTPUT);
	for(int times = 0; times < count; times++)
	{
		for(int x = 1; x < 16; x++)
		{
			analogWrite(pin, 256-x * 16);
			delay(50);
		}
		for(int x = 1; x < 16; x++)
		{
			analogWrite(pin, x * 16);
			delay(50);
		}
	}
#endif
}
