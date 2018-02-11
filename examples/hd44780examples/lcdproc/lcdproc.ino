//  vi:ts=4
// -----------------------------------------------------------------------
// lcdproc hd44780 serial device
//
// lcdproc is the linux equivilent of Windows LCD Smartie
// Information on how to set this up and use it can be found here:
// https://milesburton.com/USD_LCD_Display_(HD44780)_Running_on_Linux_via_Arduino
// http://lcdproc.omnipotent.net/
//
// This sketch implments a serial hd44780 interface device which can be
// used by lcdproc
// This code should work with any LiquidCrystal compatible library.
//
// While you can modify this sketch to use any "LiquidCrystal" library 
// and modify the default lcd parametes,
// wrapper sketches are included in the hd44780 i/o libraries that 
// declare the lcd object and the needed defines to do this for you.
// -----------------------------------------------------------------------

#ifndef HD44780_LCDOBJECT

#error "Special purpose sketch: Use i/o class example wrapper sketch instead."

/*
 * If not using a hd44780 library i/o class example wrapper sketch,
 * you must modify the sketch to include any needed header files for the
 * intended library and define the lcd object.
 *
 * Add your includes and constructor.
 * The lcd object must be named "lcd"
 * and comment out the #error message.
 */

#endif
/*
 * Define your LCD  parameters
 * These must match what you configured in LCDd.conf on the linux host
 */
#ifndef LCD_COLS
#define LCD_COLS 20
#endif

#ifndef LCD_ROWS
#define LCD_ROWS 4
#endif

#ifndef BAUDRATE
#define BAUDRATE 9600
#endif

void setup()
{ 
	Serial.begin(BAUDRATE);
	// set up the LCD's number of columns and columns: 
	lcd.begin(LCD_COLS, LCD_ROWS);

#ifdef WIRECLOCK
#if (ARDUINO > 10507) && !defined(MPIDE)
	Wire.setClock(WIRECLOCK); // set i2c clock bit rate, if asked
#endif
#endif

	lcd.clear();
	// print out a banner that indicates lcd proc device
	// and pramameters like baudrate and geometry
	lcd.print("LCD Proc:");
	lcd.print("hd44780");
	lcd.setCursor(0,1);
	lcd.print(BAUDRATE);
	lcd.print(' ');
	lcd.print(LCD_COLS);
	lcd.print('x');
	lcd.print(LCD_ROWS);
	delay(1000);
}

uint8_t serial_getch()
{
int incoming;  

	while (!Serial.available()){ }
	// read the incoming byte:
	incoming = Serial.read();

	return (incoming & 0xff);
}

void loop()
{
uint8_t rxbyte;

	rxbyte = serial_getch(); // Fetch byte

	if(rxbyte==0xFE) // If command 
		lcd.command(serial_getch()); // Pass through raw hd44780 command
	else
		lcd.write(rxbyte); //Otherwise send it as text character
}
