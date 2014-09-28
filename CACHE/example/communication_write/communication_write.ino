// Do not remove the include below

#include <SPI.h>
#include <CACHE.h>

#define str_len 20
#define addr 0

char str1[str_len] = "hello world!";
char str2[str_len] = "I am CACHE_Ver2.";
char str3[str_len] = "This is a test.";
char str[str_len];

#define empty_str memset(str, 0 , str_len)

//The setup function is called once at startup of the sketch
void setup()
{
// Add your initialization code here
	CACHE.INFO = 49;
	CACHE.REQ = 47;
	CACHE.WR = 46;
	CACHE.PER = 48;
	CACHE.init(MEGA2560);
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
	CACHE.q_write(addr, str_len, str1);
	delay(1000);

	CACHE.q_write(addr, str_len, str2);
	delay(1000);

	CACHE.q_write(addr, str_len, str3);
	delay(1000);

}
