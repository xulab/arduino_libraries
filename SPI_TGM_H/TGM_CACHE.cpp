#include <TGM_CACHE.h>


/*-----------23LC1024------------*/

void _SPI_INIT() {
#ifdef ARDUINO_ARCH_ESP32
	CACHE.INFO = ESP32_INFO;
	CACHE.REQ = ESP32_REQ;
	CACHE.WR = ESP32_WR;
	CACHE.PER = ESP32_PER;
	CACHE.CS = ESP32_CS;
	CACHE.SCK = ESP32_SCK;
	CACHE.MO = ESP32_MO;
	CACHE.MI = ESP32_MI;
	CACHE.SPISPEED = 4000000;
	CACHE.init();
#else
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
#endif
}

void cacheRead(uint32_t addr, uint16_t size, char *data)
{
	CACHE.q_read(addr, size, data);
}

void cacheWrite(uint32_t addr, uint16_t size, char *data)
{
	CACHE.q_write(addr, size, data);
}
int cacheNewInfo()
{
	 return CACHE.new_info();
}
