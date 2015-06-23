/* Version 1.0 was Modified in 6/8/2015
 * This librairy only suit for Mega2560, UNO and Nano.
 * FAST_PWM will enable the native FAST PWM function of AVR on ATmega328 or ATmega2560.
 * This library may cause some confiction with "Pulse.h"
 * Caution 1: This program will terminate the timer of arduino. And it will cause	delay() delayMicroseconds() millis() and micros() don't work properly.
 *
 * Caution 2: There are some conflicts when using both "FAST_PWM" and "Pulse". So don't use these simultaneously.
 *
 * On UNO (ATmega328p) and NANO (ATmega328p), this library works on pin 3, 9, 10, 11.
 * On Mega2560 (ATmega2560), this library works on pin 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46.
 * For DAC application, adding RC low pass filter (and a optamp?) to output pin is recommended. And suited value are R=1k(metal thin film resister) and C=0.1uF(plastic film capacitor or C0G ceramic capacitor).
 *
 *---------Confliction with FAST_PWM-------
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
 

*
*/

#ifndef _FAST_PWM_h
#define _FAST_PWM_h
#include "Arduino.h"

#if defined __AVR_ATmega328P__
#elif defined __AVR_ATmega2560__
#else 
#error "FAST_PWM only suit for Mage2560, UNO(mega328) and NANO(mega328)."
#endif

class FAST_PWM_Class
{
private:
public:
	void enable_pin(int pin);
	void disable_pin(int pin);
	void write(int pin, uint8_t val);

};

extern FAST_PWM_Class FAST_PWM;

#endif

















