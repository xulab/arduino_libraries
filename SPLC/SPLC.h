#ifndef _SPLC_H
#define _SPLC_H
#include "Arduino.h"
#ifndef NOP()
#define NOP() __asm__ __volatile__ ("nop")
#endif


#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif





typedef class SPLCClass
{
private:

public:
	int get_D_SPL(float fq, byte DB);	
	void init();
}SPLCClass;

extern SPLCClass SPLC;


#endif