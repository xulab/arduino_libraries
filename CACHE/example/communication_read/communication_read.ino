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
	Serial.begin(115200);
	/**teensy**/
	CACHE.INFO = 2;
	CACHE.REQ = 6;
	CACHE.WR = 7;
	CACHE.PER = 22;
	CACHE.CS = 23;
	CACHE.SCK = 13;
	CACHE.MO = 11;
	CACHE.MI = 12;
	CACHE.SPISPEED = 4000000;
	CACHE.init();

	/**nano**/
	// CACHE.INFO = A3;
	// CACHE.PER = 8;
	// CACHE.REQ = A4;
	// CACHE.WR = A5;
	// CACHE.CS = 10;
	// CACHE.SCK = 13;
	// CACHE.MO = 11;
	// CACHE.MI = 12;
	// CACHE.init(MEGA328);

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
