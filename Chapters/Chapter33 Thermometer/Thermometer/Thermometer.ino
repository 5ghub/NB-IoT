/*

  Copyright 2019, 5G HUB

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
  associated documentation files (the "Software"), to deal in the Software without restriction, including
  without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
  following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial
  portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
  TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  IN THE SOFTWARE.

*/
/*
  LiquidCrystal Library

  The circuit:
   LCD RS pin to digital pin PA6
   LCD Enable pin to digital pin PA7
   LCD D4 pin to digital pin SS
   LCD D5 pin to digital pin MOSI
   LCD D6 pin to digital pin MISO
   LCD D7 pin to digital pin SCK
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal

  R3 is 10K
*/

#include <board.h>
#include <LiquidCrystal.h>
int tempPin = A0;
LiquidCrystal lcd(PA6, PA7, SS, MOSI, MISO, SCK);

void setup()
{
  lcd.begin(16, 2);
}

void loop()
{
  int tempReading = analogRead(tempPin);
  // This is OK
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
  float tempF = (tempC * 9.0) / 5.0 + 32.0; // Convert Celcius to Fahrenheit
  /*  replaced
    float tempVolts = tempReading * 5.0 / 1024.0;
    float tempC = (tempVolts - 0.5) * 10.0;
    float tempF = tempC * 9.0 / 5.0 + 32.0;
  */
  // Display Temperature in C
  lcd.setCursor(0, 0);
  lcd.print("Temp         C  ");
  // Display Temperature in F
  //lcd.print("Temp         F  ");
  lcd.setCursor(6, 0);
  // Display Temperature in C
  lcd.print(tempC);
  // Display Temperature in F
  //lcd.print(tempF);
  delay(500);
}
