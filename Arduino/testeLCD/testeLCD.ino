                    /*
  LiquidCrystal Library - display() and noDisplay()

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD and uses the
 display() and noDisplay() functions to turn on and off
 the display.

 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalDisplay

 */

// include the library code:
#include <LiquidCrystal.h>
char grettings[] = "hello, world!";

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(28, 27, 26, 25, 24, 23);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(grettings);
}

void loop() {
  // Turn off the display:
  lcd.noDisplay();
  delay(2000);
  // Turn on the display:
  lcd.display();
  delay(2000);
  lcd.clear(); // limpa o display

  lcd.setCursor(0,0); // primeira coluna da primeira linha
  lcd.print("Pc Pl");
  lcd.setCursor(0,1); // primeira coluna da segunda linha
  lcd.print("Pc Sl");
  
  delay(5000); // 5000 milisegundos = 5 segundos de espera
  
}

