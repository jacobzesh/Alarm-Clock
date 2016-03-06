//This code is for an alarm clock build using
//an Arduino Mega 2560, lcd1602, and 1 rotary encoder


#include <LiquidCrystal.h> //including lcd library
LiquidCrystal lcd(43, 45, 47, 49, 51, 53); //declaring pin numbers for lcd
void setup()
{
	lcd.begin(16, 2); 
	lcd.setCursor(0, 0);
	lcd.print("TEST");    //Beginning lcd and testing
	delay(5000);
	lcd.clear();
}

void loop()
{
	lcd.scrollDisplayLeft();
	lcd.print(" Greetings,");
	lcd.setCursor(1, 1);
	lcd.print("my name is Jacob");
	delay(5000);
	lcd.clear();
}
