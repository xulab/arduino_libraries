/*
DS3231_test.pde
setup by: Eric Ayars
modified by : Jingwei Pan
4/11
Test/demo of read routines for a DS3231 RTC.
Turn on the serial monitor after loading this to check if things are
working as they should.
*/

#include <Wire.h>
#include <DS3231.h>

DS3231 Clock;

void ReadDS3231()
{
  int second,minute,hour,date,month,year,temperature; 
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour();
  date=Clock.getDate();
  month=Clock.getMonth();
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
  
  Serial.print("20");
  Serial.print(year,DEC);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print(' ');
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}


void setup() {
	// Start the I2C interface
	Wire.begin();
        Clock.setSecond(50);//Set the second 
        Clock.setMinute(55);//Set the minute 
        Clock.setHour(13);  //Set the hour 
        Clock.setDoW(1);    //Set the day of the week
        Clock.setDate(16);  //Set the date of the month
        Clock.setMonth(6);  //Set the month of the year
        Clock.setYear(14);  //Set the year (Last two digits of the year)
	// Start the serial interface
	Serial.begin(9600);
}



void loop() {  
  ReadDS3231();
  delay(1000);
}

