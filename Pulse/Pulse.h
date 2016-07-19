/* Version 02
 * create in 20141119
 * Modified in 20150623
 * This labariry can generate 4 parallel serial triggers on ATmega2560 or 1 parallel serial triggers on ATmega328p(UNO or NANO).
 *

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

---------Confliction with FAST_PWM-------
 * Caution: There are some confilcts when using both "FAST_PWM" and "Pulse".

For UNO and NANO usage:

	pin		pulse source
 	3 		none
 	9  		p1
 	10 		p1
 	11 		none


 * For ATmega2560 usage:
 
 	pin  	pulse source
 	2		p2
 	3		p2
 	5		p2
 	6		p3
 	7		p3
 	8		p3
 	9		none
 	10		none
 	11		p1
 	12		p1
 	13		p1
 	44		p4
 	45		p4
 	46		p4
 
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

#if defined __AVR_ATmega328P__
	void init();
	void p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode); 
	void p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay); 
	void p1_pulse(int pin, uint32_t duration, int mode); 
	void p1_cancel(); 

#elif defined __AVR_ATmega2560__
	void init();
	void p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode); 
	void p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay); 
	void p1_pulse(int pin, uint32_t duration, int mode); 
	void p1_cancel(); 
		
	void p2_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	void p2_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay); 
	void p2_pulse(int pin, uint32_t duration, int mode);
	void p2_cancel();

	void p3_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	void p3_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay); 
	void p3_pulse(int pin, uint32_t duration, int mode);
	void p3_cancel();

	// void p4_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode);
	// void p4_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay); 
	// void p4_pulse(int pin, uint32_t duration, int mode);
	// void p4_cancel();
	void PWM_write5A(uint16_t);
	void PWM_write5B(uint16_t);
	void PWM_write5C(uint16_t);
	
#else 
#error "FAST_PWM only suit for Mage2560, UNO(mega328) and NANO(mega328)."
#endif

};

extern PULSE_Class PULSE;

#endif
