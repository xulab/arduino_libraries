

#include <RCM.h>
#include <Wire.h>


#define SPEAKER 7
byte _year = 15;
byte _month = 1;
byte _date = 7;
byte _day = 4;
byte _hour =12;
byte _minute = 15;
byte _sec = 50;


void setup()
{
Serial.begin(9600);
	RCM.init();
  pinMode(SPEAKER, OUTPUT);
   digitalWrite(SPEAKER, LOW); 
 //    RCM.setSecond(_sec);//Set the second 
 //    RCM.setMinute(_minute);//Set the minute 
 //    RCM.setHour(_hour);  //Set the hour 
 //    RCM.setDoW(_day);    //Set the day of the week
 //    RCM.setDate(_date);  //Set the date of the month
 //    RCM.setMonth(_month);  //Set the month of the year
 //    RCM.setYear(_year);  //Set the year (Last two digits of the year)
	// // Start the serial interface

}

void loop(){
	// for(int i=0; i<10000; i++){
	// 	RCM.second();
	// }
	Serial.println("***********************");
	Serial.print("year:");
	Serial.println(RCM.year());
	Serial.print("month:");
	Serial.println(RCM.month());
	Serial.print("date:");
	Serial.println(RCM.date());
	Serial.print("day:");
	Serial.println(RCM.day());
	Serial.print("hour:");
	Serial.println(RCM.hour());
	Serial.print("minute:");
	Serial.println(RCM.minute());
	Serial.print("second:");
	Serial.println(RCM.second());	
	Serial.println("***********************");
delay(1000);
} 