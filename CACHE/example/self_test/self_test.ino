// Do not remove the include below

#include <SPI.h>
#include <CACHE.h>

#define str_len 20
#define addr1 0
#define addr2 100
#define addr3 200

char str1[str_len] = "hello world!";
char str2[str_len] = "I am CACHE_Ver2.";
char str3[str_len] = "This is a test.";
char str[str_len];

#define empty_str memset(str, 0 , str_len)

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	Serial.begin(9600);
	CACHE.INFO = 49;
	CACHE.REQ = 47;
	CACHE.WR = 46;
	CACHE.PER = 48;
	CACHE.init(MEGA2560);
	empty_str;
	CACHE.q_write(addr1, str_len, str1);
	CACHE.q_write(addr2, str_len, str2);
	CACHE.q_write(addr3, str_len, str3);
	Serial.println(str1);
	Serial.println(str2);
	Serial.println(str3);

}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	CACHE.q_read(addr1,str_len,str);
	Serial.println(str);
	empty_str;
	delay(1000);

	CACHE.q_read(addr2,str_len,str);
	Serial.println(str);
	empty_str;
	delay(1000);

	CACHE.q_read(addr3,str_len,str);
	Serial.println(str);
	empty_str;
	delay(1000);

}

