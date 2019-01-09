#include "Arduino.h"

#ifndef _CACHE_H
#define _CAHCE_H

//#ifndef NOP()
//#define NOP() __asm__ __volatile__ ("nop")
//#endif

#define NONESPECIFIC 0
#define MEGA2560 1
#define MEGA328 2


class CACHE_Class
{
private:


public:
	int REQ;
	int WR;
	int PER;
	int INFO;
	int CS;
	int SCK;
	int MO;
	int MI;
	uint32_t SPISPEED = 4000000;
	void request();
	void release();
	void init(int board_type = NONESPECIFIC);
	int new_info();
	void write(uint32_t addr, uint16_t size, char *data);
	void read(uint32_t addr, uint16_t size, char *data);
	void q_write(uint32_t addr, uint16_t size, char *data);
	void q_read(uint32_t addr, uint16_t size, char *data);

};

extern CACHE_Class CACHE;

#endif
