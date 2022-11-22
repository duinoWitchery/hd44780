#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Adafruit_BusIO_Register.h>

#include <Adafruit_MCP23X17.h>
Adafruit_MCP23X17 mcp;

//por defeito da biblioteca inicializa o hardware assim que a classe é criada...
#include "hd44780_MCP23X17.h"
#include <hd44780.h>


#define LCD_COLS  20
#define LCD_ROWS  4



#define rs 0
#define rw 1
#define en 2
#define d4 4
#define d5 5
#define d6 6
#define d7 7

hd44780_MCP23X17 lcd(rs, rw, en, d4, d5, d6, d7);

void setup() {
    Serial.begin(115200);
    Serial.flush();
    Serial.println("Starting SPI");
    if (!mcp.begin_SPI(D1)) {
        Serial.println("Error Starting MCP23X17.");
        while (1);
    }
    Serial.println("MCP23X17 OK.");
    int status = lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.clear();
    lcd.print("Ola Mundo ");
    lcd.print(status);
    delay(500);
}

void loop() {
    lcd.clear();
    lcd.print(millis());
    delay(100);
}
