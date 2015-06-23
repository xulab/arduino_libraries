/*
1.	px_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)

	This function return immediately and generate serial pulse in background. 

	pin: Indicate the pin you want to generate pulses.

	duratuon: Unit is ms. It indicate lasting time of the generated serial pusles.

	fq: Unit is Hz .You should notice that it means fq pusles per second(not a ms).

	p_width: Unit is ms. It indicate the period of phase "ON" which setted by variable "mode".

	mode: Must set this variable to one of two constants:"TRG_L" and "TRG_H". 
			TRG_L indicate the phase "ON" to be a LOW voltage. And phase "OFF" to be HIGH. 
			TRG_H indicate the phase "ON" to be a HIGH voltage. And phase "OFF" to be LOW. 


2.	px_pulse(int pin, uint32_t duration, int mode);

	this function return immediately and generate one pulse in background. 

	pin: Indicate the pin you want to generate pulses.

	duratuon: Unit is ms. It indicate lasting time of the generated serial pusles.

	mode: Must set this variable to one of two constants:"TRG_L" and "TRG_H". 
			TRG_L indicate the phase "ON" to be a LOW voltage. And phase "OFF" to be HIGH. 
			TRG_H indicate the phase "ON" to be a HIGH voltage. And phase "OFF" to be LOW. 

3.	void px_cancel(); 

	This function return immediately. 
	It aims to shorten p1_sqr_wave and p1_pulse when you change you mind. 
 
 * Caution 1: This program will terminate the timer of arduino. And it will cause	delay() delayMicroseconds() millis() and micros() don't work properly.
 *
 * Caution 2: There are some confilcts when using both "FAST_PWM" and "Pulse". So don't use these simultaneously.

*/
#ifndef _PULSE_h
#define _PULSE_h
#include "Arduino.h"

#define TRG_L 0
#define TRG_H 1 


class PULSE_Class
{
private:

public:
	void init();
	void p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode); 
	void p1_pulse(int pin, uint32_t duration, int mode); 
	void p1_cancel(); 
	void p2_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	void p2_pulse(int pin, uint32_t duration, int mode);
	void p2_cancel();
	void p3_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	void p3_pulse(int pin, uint32_t duration, int mode);
	void p3_cancel();
	void p4_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	void p4_pulse(int pin, uint32_t duration, int mode);
	void p4_cancel();
};

extern PULSE_Class PULSE;

#endif
