// Do not remove the include below

#include <SPI.h>
#include <CACHE.h>

#define str_len 20
#define addr 0
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
}

// The loop function is called in an endless loop
void loop()
{
//Add your repeated code here
        while(LOW==CACHE.new_info());
	CACHE.q_read(addr,str_len,str);
	Serial.println(str);
	empty_str;
}
