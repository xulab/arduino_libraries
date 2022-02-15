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

struct CALIB_STCT
{
	float* CALIB_Array;
	float f0;
	float df;
	float calib_vol;

};

// extern float* addr_CALIB;
extern struct CALIB_STCT calib_default_stct1;
// extern float* addr_CALIB2;
extern struct CALIB_STCT calib_default_stct2;


typedef class SPLCClass
{
private:

public:
	// int get_D_SPL(float fq, byte DB, float* addr = addr_CALIB);
	int SPLCClass::get_D_SPL(float fq, byte DB, struct CALIB_STCT param = calib_default_stct1);
	// int get_D_SPL2(float fq, byte DB, float* addr = addr_CALIB2);	
	int SPLCClass::get_D_SPL2(float fq, byte DB, struct CALIB_STCT param = calib_default_stct2);
	void init();
}SPLCClass;

extern SPLCClass SPLC;


#endif
