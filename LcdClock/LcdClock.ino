//Arduino 1.0+ Only
//Arduino 1.0+ Only

#include "Wire.h"
#include "serLCD.h"
#define DS1307_ADDRESS 0x68



#include <SoftwareSerial.h>
#include <serLCD.h>
 
// Set pin to the LCD's rxPin
int pin = 4;
 
serLCD lcd(pin);


void setup(){
  Wire.begin();
  Serial.begin(9600);
}

void loop(){
  printDate();
  delay(1000);
}

byte bcdToDec(byte val)  {
// Convert binary coded decimal to normal decimal numbers
  return ( (val/16*10) + (val%16) );
}

void printDate(){

  // Reset the register pointer
  Wire.beginTransmission(DS1307_ADDRESS);

  byte zero = 0x00;
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date EG   3/1/11 23:59:59
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);


  // print the same thing to the lcd module
 
 
  lcd.clear();
  lcd.print(month);
  lcd.print("/");
  lcd.print(monthDay);
  lcd.print("/");
  lcd.print(year);
  lcd.print(" ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);

 // mySerial.write("Hello, world!");
}





