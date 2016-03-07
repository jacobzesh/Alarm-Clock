//This code is for an alarm clock build using
//an Arduino Mega 2560, lcd1602, and 1 rotary encoder

#define DS3231_I2C_ADDRESS 0x68
#include "Wire.h" //including wire library
#include <LiquidCrystal.h> //including lcd library
LiquidCrystal lcd(43, 45, 47, 49, 51, 53); //declaring pin numbers for lcd
const int clkPin = 2;
const int dtPin = 3;
const int swPin = 4;

int buttonPress = 0;

int minutes = 0;
int hours = 0;
int day = 0;
int date = 0;
int month = 0;
int year = 0;

byte decToBcd(byte val)
{
	return((val / 10 * 16) + (val % 10));
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
	return((val / 16 * 10) + (val % 16));
}

void setup()
{
	Wire.begin();
	lcd.begin(16, 2); 
	lcd.setCursor(0, 0);
	lcd.print("TEST");    //Beginning lcd and testing
	delay(5000);
	lcd.clear();
	setDS3231time(00, 00, 00, 01, 01, 01, 16);
	pinMode(clkPin, INPUT);//set clkPin as INPUT
	pinMode(dtPin, INPUT);
	pinMode(swPin, INPUT);
	digitalWrite(swPin, HIGH);
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)
{
	// sets time and date data to DS3231
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0); // set next input to start at the seconds register
	Wire.write(decToBcd(second)); // set seconds
	Wire.write(decToBcd(minute)); // set minutes
	Wire.write(decToBcd(hour)); // set hours
	Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
	Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
	Wire.write(decToBcd(month)); // set month
	Wire.write(decToBcd(year)); // set year (0 to 99)
	Wire.endTransmission();
}
void readDS3231time(byte *second,
	byte *minute,
	byte *hour,
	byte *dayOfWeek,
	byte *dayOfMonth,
	byte *month,
	byte *year)
{
	Wire.beginTransmission(DS3231_I2C_ADDRESS);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
	// request seven bytes of data from DS3231 starting from register 00h
	*second = bcdToDec(Wire.read() & 0x7f);
	*minute = bcdToDec(Wire.read());
	*hour = bcdToDec(Wire.read() & 0x3f);
	*dayOfWeek = bcdToDec(Wire.read());
	*dayOfMonth = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());
}
void displayTime(){
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	// retrieve data from DS3231
	readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
		&year);
	lcd.clear(); // clear LCD screen
	lcd.setCursor(0, 0);
	lcd.print(" ");
	lcd.print(hour, DEC);
	lcd.print(":");
	if (minute<10)
	{
		lcd.print("0");
	}
	lcd.print(minute, DEC);
	lcd.print(".");
	if (second<10)
	{
		lcd.print("0");
	}
	lcd.print(second, DEC);
	lcd.setCursor(0, 1);
	lcd.print(" ");
	switch (dayOfWeek){
	case 1:
		lcd.print("Sun");
		break;
	case 2:
		lcd.print("Mon");
		break;
	case 3:
		lcd.print("Tue");
		break;
	case 4:
		lcd.print("Wed");
		break;
	case 5:
		lcd.print("Thu");
		break;
	case 6:
		lcd.print("Fri");
		break;
	case 7:
		lcd.print("Sat");
		break;
	}
	lcd.print(" ");
	lcd.print(month, DEC);
	lcd.print("/");
	lcd.print(dayOfMonth, DEC);
	lcd.print("/20");
	lcd.print(year, DEC);
	delay(1000);
}

int getEncoderTurn(void)
{
	static int oldA = HIGH;
	static int oldB = HIGH;
	int result = 0;
	int newA = digitalRead(clkPin);//read the value of clkPin to newA
	int newB = digitalRead(dtPin);//read the value of dtPin to newB
	if (newA != oldA || newB != oldB)
	{
		// something has changed
		if (oldA == HIGH && newA == LOW)
		{
			result = (oldB * 2 - 1);
		}
	}
	oldA = newA;
	oldB = newB;
	return result;
}

void setTime(){
	if (digitalRead(swPin) == LOW){
		buttonPress += 1;
		delay(1000);
	}
	if (buttonPress == 1){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		minutes = minutes + change;
		lcd.print("Minutes = ");
		lcd.print(minutes);
		delay(10);
	}
	if (buttonPress == 2){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		hours = hours + change;
		lcd.print("Hours = ");
		lcd.print(hours);
		delay(10);
	}
	if (buttonPress == 3){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		day = day + change;
		lcd.print("Day = ");
		lcd.print(day);
		delay(10);
	}
	if (buttonPress == 4){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		date = date + change;
		lcd.print("Date = ");
		lcd.print(date);
		delay(10);
	}
	if (buttonPress == 5){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		month = month + change;
		lcd.print("month = ");
		lcd.print(month);
		delay(10);
	}
	if (buttonPress == 6){
		lcd.clear();
		lcd.setCursor(0, 0);
		int change = getEncoderTurn();
		year = year + change;
		lcd.print("year = ");
		lcd.print(year);
		delay(10);
	}
	if (buttonPress == 7){
		setDS3231time(00, minutes, hours, day, date, month, year);
		lcd.clear();
		lcd.print("Press To Finish");
		delay(10);
	}
	if (buttonPress == 8){
		displayTime();
	}
	

}

void loop()
{
	setTime();

}