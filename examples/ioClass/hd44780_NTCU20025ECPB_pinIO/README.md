hd44780_NTCU20025ECPB_pinIOexamples
===================================

The examples included in this directory are for the hd44780_NTCU20025ECPB_pinIO i/o class.<br>
The hd44780_NTCU20025ECPB_pinIO i/o class is used to control Noritake
CU-U series displays like the Noritake CU20025ECPB-U1J or CU20025ECPB-W1J
The Noritake CU20025ECPB display is a 20x2 line VFD display.
It is not a hd44780 display but is nearly fully compatible with a hd44780
display. The only real difference is the backlight control.
So while this device is supported by the hd44780 library,
the subclass will do its best to emulatate hd44780 functions and map
them to equivalent functionality when possible.

#### The following examples are included:

- `HelloWorld`<br>
Prints "Hello, World!" on the lcd

- `LCDKeypadCheck`<br>
Tests lcd keypad shields for bad backlight circuitry

- `LCDCustomChars`<br>
Demonstrates using custom characters

- `LineWrap`<br>
Demonstrates automatic linewrap functionality

- `ReadWrite`<br>
Demonstrate the ability to read data from the LCD.

- `Serial2LCD`<br>
Displays a message read from the serial port on the lcd.

- `UpTime`<br>
Prints the amount of time since the Arduino has been reset.

- `hd44780examples`<br>
The hd44780examples subdirectory contains
hd44780_NTCU20025ECPB_pinIOclass specific wrapper sketches for sketches under
examples/hd44780examples.
