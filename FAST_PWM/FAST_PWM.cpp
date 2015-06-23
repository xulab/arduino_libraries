

#include "FAST_PWM.h"

#define _SET(var,bit) var |= _BV(bit)
#define _CLEAR(var,bit) var &= ~_BV(bit)
uint8_t t_TCCRnA;
uint8_t t_TCCRnB;

#if defined __AVR_ATmega328P__
/* PWM pin defination on ATmega328p :
 *	pin		PWM source		physical pin
 *	3		OC2B			PD3
 *	9		OC1A			PB1
 *	10		OC1B			PB2
 *	11		OC2A			PB3
 *
 */


void FAST_PWM_Class::enable_pin(int pin)
{
	pinMode(pin, OUTPUT);

	switch (pin)
	{
	case 3:
		t_TCCRnA = TCCR2A;
		t_TCCRnB = TCCR2B;
		_SET(t_TCCRnA, WGM20), _SET(t_TCCRnA, WGM21), _CLEAR(t_TCCRnB, WGM22); //Enable Fast PWM, 8-bit on Timer/Counter2
		_SET(t_TCCRnB, CS20), _CLEAR(t_TCCRnB, CS21), _CLEAR(t_TCCRnB, CS22); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter2.
		_CLEAR(t_TCCRnA, COM2B0), _SET(t_TCCRnA, COM2B1); //Enable FAST PWM on pin 3(OC2B). Non-inverting mode.
		TCCR2A = t_TCCRnA;
		TCCR2B = t_TCCRnB;
		OCR2B = 0;
		break;

	case 9:
		t_TCCRnA = TCCR1A;
		t_TCCRnB = TCCR1B;
		_SET(t_TCCRnA, WGM10), _CLEAR(t_TCCRnA, WGM11), _SET(t_TCCRnB, WGM12), _CLEAR(t_TCCRnB, WGM13); //Enable Fast PWM, 8-bit on Timer/Counter1
		_SET(t_TCCRnB, CS10), _CLEAR(t_TCCRnB, CS11), _CLEAR(t_TCCRnB, CS12); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter1.
		_CLEAR(t_TCCRnA, COM1A0), _SET(t_TCCRnA, COM1A1); //Enable FAST PWM on pin 9(OC1A). Non-inverting mode.
		TCCR1A = t_TCCRnA;
		TCCR1B = t_TCCRnB;
		TCCR1C = 0;
		OCR1AH = 0;
		break;

	case 10:
		t_TCCRnA = TCCR1A;
		t_TCCRnB = TCCR1B;
		_SET(t_TCCRnA, WGM10), _CLEAR(t_TCCRnA, WGM11), _SET(t_TCCRnB, WGM12), _CLEAR(t_TCCRnB, WGM13); //Enable Fast PWM, 8-bit on Timer/Counter1
		_SET(t_TCCRnB, CS10), _CLEAR(t_TCCRnB, CS11), _CLEAR(t_TCCRnB, CS12); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter1.
		_CLEAR(t_TCCRnA, COM1B0), _SET(t_TCCRnA, COM1B1); //Enable FAST PWM on pin 10(OC1B). Non-inverting mode.
		TCCR1A = t_TCCRnA;
		TCCR1B = t_TCCRnB;
		TCCR1C = 0;
		OCR1BH = 0;
		break;

	case 11:
		t_TCCRnA = TCCR2A;
		t_TCCRnB = TCCR2B;
		_SET(t_TCCRnA, WGM20), _SET(t_TCCRnA, WGM21), _CLEAR(t_TCCRnB, WGM22); //Enable Fast PWM, 8-bit on Timer/Counter2
		_SET(t_TCCRnB, CS20), _CLEAR(t_TCCRnB, CS21), _CLEAR(t_TCCRnB, CS22); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter2.
		_CLEAR(t_TCCRnA, COM2A0), _SET(t_TCCRnA, COM2A1); //Enable FAST PWM on pin 11(OC2A). Non-inverting mode.
		TCCR2A = t_TCCRnA;
		TCCR2B = t_TCCRnB;
		OCR2A = 0;
		break;

	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 3,9,10,11 on ATmega328p.");
		return;
	}
	sei(); //enable global interrupter
}



void FAST_PWM_Class::disable_pin(int pin)
{

	switch (pin)
	{
	case 3:
		t_TCCRnA = TCCR2A;
		_CLEAR(t_TCCRnA, COM2B0), _CLEAR(t_TCCRnA, COM2B1); //Set pin 3 to normal mode.
		TCCR2A = t_TCCRnA;
		break;

	case 9:
		t_TCCRnA = TCCR1A;
		_CLEAR(t_TCCRnA, COM1A0), _CLEAR(t_TCCRnA, COM1A1); //Set pin 9 to normal mode.
		TCCR1A = t_TCCRnA;
		break;

	case 10:
		t_TCCRnA = TCCR1A;
		_CLEAR(t_TCCRnA, COM1B0), _CLEAR(t_TCCRnA, COM1B1); //Set pin 10 to normal mode.
		TCCR1A = t_TCCRnA;
		break;

	case 11:
		t_TCCRnA = TCCR2A;
		_CLEAR(t_TCCRnA, COM2A0), _CLEAR(t_TCCRnA, COM2A1); //Set pin 3 to normal mode.
		TCCR2A = t_TCCRnA;
		break;



	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 3,9,10,11 on ATmega328p.");
		return;
	}
}



void FAST_PWM_Class::write(int pin, uint8_t val)
{
	switch (pin)
	{
	case 3:
		OCR2B = val;
		break;

	case 9:
		OCR1AL = val;
		break;

	case 10:
		OCR1BL = val;
		break;

	case 11:
		OCR2A = val;
		break;

	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 3,9,10,11 on ATmega328p.");
		break;
	}
}


#elif defined __AVR_ATmega2560__

/* PWM pin defination on ATmega328p :
 *	pin		PWM source		physical pin
 *	2		OC3B			PE4
 *	3		OC3C			PE5
 *	5		OC3A			PE3
 *	6		OC4A			PH3
 *	7		OC4B			PH4
 *	8		OC4C			PH5
 *	9		OC2B			PH6
 *	10		OC2A			PB4
 *	11		OC1A			PB5
 *	12		OC1B			PB6
 *	13		OC1C			PB6
 *	44		OC5C			PL5
 *	45		OC5B			PL4
 *	46		OC5A			PL3
 *
 */


void FAST_PWM_Class::enable_pin(int pin)
{
	pinMode(pin, OUTPUT);
	switch (pin)
	{
	case 2:
		t_TCCRnA = TCCR3A;
		t_TCCRnB = TCCR3B;
		_SET(t_TCCRnA, WGM30), _CLEAR(t_TCCRnA, WGM31), _SET(t_TCCRnB, WGM32), _CLEAR(t_TCCRnB, WGM33); //Enable Fast PWM, 8-bit on Timer/Counter3
		_SET(t_TCCRnB, CS30), _CLEAR(t_TCCRnB, CS31), _CLEAR(t_TCCRnB, CS32); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter3.
		_CLEAR(t_TCCRnA, COM3B0), _SET(t_TCCRnA, COM3B1); //Enable FAST PWM on pin 2(OC3B). Non-inverting mode.
		TCCR3A = t_TCCRnA;
		TCCR3B = t_TCCRnB;
		TCCR3C = 0;
		OCR3BH = 0;
		break;

	case 3:
		t_TCCRnA = TCCR3A;
		t_TCCRnB = TCCR3B;
		_SET(t_TCCRnA, WGM30), _CLEAR(t_TCCRnA, WGM31), _SET(t_TCCRnB, WGM32), _CLEAR(t_TCCRnB, WGM33); //Enable Fast PWM, 8-bit on Timer/Counter3
		_SET(t_TCCRnB, CS30), _CLEAR(t_TCCRnB, CS31), _CLEAR(t_TCCRnB, CS32); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter3.
		_CLEAR(t_TCCRnA, COM3C0), _SET(t_TCCRnA, COM3C1); //Enable FAST PWM on pin 3(OC3C). Non-inverting mode.
		TCCR3A = t_TCCRnA;
		TCCR3B = t_TCCRnB;
		TCCR3C = 0;
		OCR3CH = 0;
		break;

	case 5:
		t_TCCRnA = TCCR3A;
		t_TCCRnB = TCCR3B;
		_SET(t_TCCRnA, WGM30), _CLEAR(t_TCCRnA, WGM31), _SET(t_TCCRnB, WGM32), _CLEAR(t_TCCRnB, WGM33); //Enable Fast PWM, 8-bit on Timer/Counter3
		_SET(t_TCCRnB, CS30), _CLEAR(t_TCCRnB, CS31), _CLEAR(t_TCCRnB, CS32); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter3.
		_CLEAR(t_TCCRnA, COM3A0), _SET(t_TCCRnA, COM3A1); //Enable FAST PWM on pin 5(OC3A). Non-inverting mode.
		TCCR3A = t_TCCRnA;
		TCCR3B = t_TCCRnB;
		TCCR3C = 0;
		OCR3AH = 0;
		break;

	case 6:
		t_TCCRnA = TCCR4A;
		t_TCCRnB = TCCR4B;
		_SET(t_TCCRnA, WGM40), _CLEAR(t_TCCRnA, WGM41), _SET(t_TCCRnB, WGM42), _CLEAR(t_TCCRnB, WGM43); //Enable Fast PWM, 8-bit on Timer/Counter4.
		_SET(t_TCCRnB, CS40), _CLEAR(t_TCCRnB, CS41), _CLEAR(t_TCCRnB, CS42); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter4.
		_CLEAR(t_TCCRnA, COM4A0), _SET(t_TCCRnA, COM4A1); //Enable FAST PWM on pin 6(OC4A). Non-inverting mode.
		TCCR4A = t_TCCRnA;
		TCCR4B = t_TCCRnB;
		TCCR4C = 0;
		OCR4AH = 0;
		break;

	case 7:
		t_TCCRnA = TCCR4A;
		t_TCCRnB = TCCR4B;
		_SET(t_TCCRnA, WGM40), _CLEAR(t_TCCRnA, WGM41), _SET(t_TCCRnB, WGM42), _CLEAR(t_TCCRnB, WGM43); //Enable Fast PWM, 8-bit on Timer/Counter4.
		_SET(t_TCCRnB, CS40), _CLEAR(t_TCCRnB, CS41), _CLEAR(t_TCCRnB, CS42); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter4.
		_CLEAR(t_TCCRnA, COM4B0), _SET(t_TCCRnA, COM4B1); //Enable FAST PWM on pin 7(OC4B). Non-inverting mode.
		TCCR4A = t_TCCRnA;
		TCCR4B = t_TCCRnB;
		TCCR4C = 0;
		OCR4BH = 0;
		break;

	case 8:
		t_TCCRnA = TCCR4A;
		t_TCCRnB = TCCR4B;
		_SET(t_TCCRnA, WGM40), _CLEAR(t_TCCRnA, WGM41), _SET(t_TCCRnB, WGM42), _CLEAR(t_TCCRnB, WGM43); //Enable Fast PWM, 8-bit on Timer/Counter4.
		_SET(t_TCCRnB, CS40), _CLEAR(t_TCCRnB, CS41), _CLEAR(t_TCCRnB, CS42); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter4.
		_CLEAR(t_TCCRnA, COM4C0), _SET(t_TCCRnA, COM4C1); //Enable FAST PWM on pin 8(OC4C). Non-inverting mode.
		TCCR4A = t_TCCRnA;
		TCCR4B = t_TCCRnB;
		TCCR4C = 0;
		OCR4CH = 0;
		break;

	case 9:
		t_TCCRnA = TCCR2A;
		t_TCCRnB = TCCR2B;
		_SET(t_TCCRnA, WGM20), _SET(t_TCCRnA, WGM21), _CLEAR(t_TCCRnB, WGM22); //Enable Fast PWM, 8-bit on Timer/Counter2.
		_SET(t_TCCRnB, CS20), _CLEAR(t_TCCRnB, CS21), _CLEAR(t_TCCRnB, CS22); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter2.
		_CLEAR(t_TCCRnA, COM2B0), _SET(t_TCCRnA, COM2B1); //Enable FAST PWM on pin 9(OC2B). Non-inverting mode.
		TCCR2A = t_TCCRnA;
		TCCR2B = t_TCCRnB;
		OCR2B = 0;
		break;

	case 10:
		t_TCCRnA = TCCR2A;
		t_TCCRnB = TCCR2B;
		_SET(t_TCCRnA, WGM20), _SET(t_TCCRnA, WGM21), _CLEAR(t_TCCRnB, WGM22); //Enable Fast PWM, 8-bit on Timer/Counter2
		_SET(t_TCCRnB, CS20), _CLEAR(t_TCCRnB, CS21), _CLEAR(t_TCCRnB, CS22); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter2.
		_CLEAR(t_TCCRnA, COM2A0), _SET(t_TCCRnA, COM2A1); //Enable FAST PWM on pin 10(OC2A). Non-inverting mode.
		TCCR2A = t_TCCRnA;
		TCCR2B = t_TCCRnB;
		OCR2A = 0;
		break;

	case 11:
		t_TCCRnA = TCCR1A;
		t_TCCRnB = TCCR1B;
		_SET(t_TCCRnA, WGM10), _CLEAR(t_TCCRnA, WGM11), _SET(t_TCCRnB, WGM12), _CLEAR(t_TCCRnB, WGM13); //Enable Fast PWM, 8-bit on Timer/Counter1.
		_SET(t_TCCRnB, CS10), _CLEAR(t_TCCRnB, CS11), _CLEAR(t_TCCRnB, CS12); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter1.
		_CLEAR(t_TCCRnA, COM1A0), _SET(t_TCCRnA, COM1A1); //Enable FAST PWM on pin 11(OC1A). Non-inverting mode.
		TCCR1A = t_TCCRnA;
		TCCR1B = t_TCCRnB;
		TCCR1C = 0;
		OCR1AH = 0;
		break;

	case 12:
		t_TCCRnA = TCCR1A;
		t_TCCRnB = TCCR1B;
		_SET(t_TCCRnA, WGM10), _CLEAR(t_TCCRnA, WGM11), _SET(t_TCCRnB, WGM12), _CLEAR(t_TCCRnB, WGM13); //Enable Fast PWM, 8-bit on Timer/Counter1.
		_SET(t_TCCRnB, CS10), _CLEAR(t_TCCRnB, CS11), _CLEAR(t_TCCRnB, CS12); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter1.
		_CLEAR(t_TCCRnA, COM1B0), _SET(t_TCCRnA, COM1B1); //Enable FAST PWM on pin 12(OC1B). Non-inverting mode.
		TCCR1A = t_TCCRnA;
		TCCR1B = t_TCCRnB;
		TCCR1C = 0;
		OCR1BH = 0;
		break;

	case 13:
		t_TCCRnA = TCCR1A;
		t_TCCRnB = TCCR1B;
		_SET(t_TCCRnA, WGM10), _CLEAR(t_TCCRnA, WGM11), _SET(t_TCCRnB, WGM12), _CLEAR(t_TCCRnB, WGM13); //Enable Fast PWM, 8-bit on Timer/Counter1.
		_SET(t_TCCRnB, CS10), _CLEAR(t_TCCRnB, CS11), _CLEAR(t_TCCRnB, CS12); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter1.
		_CLEAR(t_TCCRnA, COM1C0), _SET(t_TCCRnA, COM1C1); //Enable FAST PWM on pin 13(OC1C). Non-inverting mode.
		TCCR1A = t_TCCRnA;
		TCCR1B = t_TCCRnB;
		TCCR1C = 0;
		OCR1CH = 0;
		break;

	case 44:
		t_TCCRnA = TCCR5A;
		t_TCCRnB = TCCR5B;
		_SET(t_TCCRnA, WGM50), _CLEAR(t_TCCRnA, WGM51), _SET(t_TCCRnB, WGM52), _CLEAR(t_TCCRnB, WGM53); //Enable Fast PWM, 8-bit on Timer/Counter5.
		_SET(t_TCCRnB, CS50), _CLEAR(t_TCCRnB, CS51), _CLEAR(t_TCCRnB, CS52); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter5.
		_CLEAR(t_TCCRnA, COM5C0), _SET(t_TCCRnA, COM5C1); //Enable FAST PWM on pin 44(OC5C). Non-inverting mode.
		TCCR5A = t_TCCRnA;
		TCCR5B = t_TCCRnB;
		TCCR5C = 0;
		OCR5CH = 0;
		break;

	case 45:
		t_TCCRnA = TCCR5A;
		t_TCCRnB = TCCR5B;
		_SET(t_TCCRnA, WGM50), _CLEAR(t_TCCRnA, WGM51), _SET(t_TCCRnB, WGM52), _CLEAR(t_TCCRnB, WGM53); //Enable Fast PWM, 8-bit on Timer/Counter5.
		_SET(t_TCCRnB, CS50), _CLEAR(t_TCCRnB, CS51), _CLEAR(t_TCCRnB, CS52); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter5.
		_CLEAR(t_TCCRnA, COM5B0), _SET(t_TCCRnA, COM5B1); //Enable FAST PWM on pin 45(OC5B). Non-inverting mode.
		TCCR5A = t_TCCRnA;
		TCCR5B = t_TCCRnB;
		TCCR5C = 0;
		OCR5BH = 0;
		break;

	case 46:
		t_TCCRnA = TCCR5A;
		t_TCCRnB = TCCR5B;
		_SET(t_TCCRnA, WGM50), _CLEAR(t_TCCRnA, WGM51), _SET(t_TCCRnB, WGM52), _CLEAR(t_TCCRnB, WGM53); //Enable Fast PWM, 8-bit on Timer/Counter5.
		_SET(t_TCCRnB, CS50), _CLEAR(t_TCCRnB, CS51), _CLEAR(t_TCCRnB, CS52); //Set clock with no presaling(same as cpu clock 12MHz) on Timer/Counter5.
		_CLEAR(t_TCCRnA, COM5A0), _SET(t_TCCRnA, COM5A1); //Enable FAST PWM on pin 46(OC5A). Non-inverting mode.
		TCCR5A = t_TCCRnA;
		TCCR5B = t_TCCRnB;
		TCCR5C = 0;
		OCR5AH = 0;
		break;


	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46 on ATmega2560.");
		return;
	}
	sei(); //enable global interrupter
}



void FAST_PWM_Class::disable_pin(int pin)
{

	switch (pin)
	{
	case 2:
		t_TCCRnA = TCCR3A;
		_CLEAR(t_TCCRnA, COM3B0), _CLEAR(t_TCCRnA, COM3B1); //Set pin 2 to normal mode.
		TCCR3A = t_TCCRnA;
		break;

	case 3:
		t_TCCRnA = TCCR3A;
		_CLEAR(t_TCCRnA, COM3C0), _CLEAR(t_TCCRnA, COM3C1); //Set pin 3 to normal mode.
		TCCR3A = t_TCCRnA;
		break;

	case 5:
		t_TCCRnA = TCCR3A;
		_CLEAR(t_TCCRnA, COM3A0), _CLEAR(t_TCCRnA, COM3A1); //Set pin 5 to normal mode.
		TCCR3A = t_TCCRnA;
		break;

	case 6:
		t_TCCRnA = TCCR4A;
		_CLEAR(t_TCCRnA, COM4A0), _CLEAR(t_TCCRnA, COM4A1); //Set pin 6 to normal mode.
		TCCR4A = t_TCCRnA;
		break;

	case 7:
		t_TCCRnA = TCCR4A;
		_CLEAR(t_TCCRnA, COM4B0), _CLEAR(t_TCCRnA, COM4B1); //Set pin 7 to normal mode.
		TCCR4A = t_TCCRnA;
		break;

	case 8:
		t_TCCRnA = TCCR4A;
		_CLEAR(t_TCCRnA, COM4C0), _CLEAR(t_TCCRnA, COM4C1); //Set pin 8 to normal mode.
		TCCR4A = t_TCCRnA;
		break;

	case 9:
		t_TCCRnA = TCCR2A;
		_CLEAR(t_TCCRnA, COM2B0), _CLEAR(t_TCCRnA, COM2B1); //Set pin 9 to normal mode.
		TCCR2A = t_TCCRnA;
		break;

	case 10:
		t_TCCRnA = TCCR2A;
		_CLEAR(t_TCCRnA, COM2A0), _CLEAR(t_TCCRnA, COM2A1); //Set pin 10 to normal mode.
		TCCR2A = t_TCCRnA;
		break; 

	case 11:
		t_TCCRnA = TCCR1A;
		_CLEAR(t_TCCRnA, COM1A0), _CLEAR(t_TCCRnA, COM1A1); //Set pin 11 to normal mode.
		TCCR1A = t_TCCRnA;
		break; 

	case 12:
		t_TCCRnA = TCCR1A;
		_CLEAR(t_TCCRnA, COM1B0), _CLEAR(t_TCCRnA, COM1B1); //Set pin 12 to normal mode.
		TCCR1A = t_TCCRnA;
		break; 

	case 13:
		t_TCCRnA = TCCR1A;
		_CLEAR(t_TCCRnA, COM1C0), _CLEAR(t_TCCRnA, COM1C1); //Set pin 13 to normal mode.
		TCCR1A = t_TCCRnA;
		break; 

	case 44:
		t_TCCRnA = TCCR5A;
		_CLEAR(t_TCCRnA, COM5C0), _CLEAR(t_TCCRnA, COM5C1); //Set pin 44 to normal mode.
		TCCR5A = t_TCCRnA;
		break; 

	case 45:
		t_TCCRnA = TCCR5A;
		_CLEAR(t_TCCRnA, COM5B0), _CLEAR(t_TCCRnA, COM5B1); //Set pin 45 to normal mode.
		TCCR5A = t_TCCRnA;
		break; 

	case 46:
		t_TCCRnA = TCCR5A;
		_CLEAR(t_TCCRnA, COM5A0), _CLEAR(t_TCCRnA, COM5A1); //Set pin 46 to normal mode.
		TCCR5A = t_TCCRnA;
		break; 


	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46 on ATmega2560.");
		return;
	}
}



void FAST_PWM_Class::write(int pin, uint8_t val)
{
	switch (pin)
	{
	case 2:
		OCR3BL = val;
		break;

	case 3:
		OCR3CL = val;
		break;

	case 5:
		OCR3AL = val;
		break;

	case 6:
		OCR4AL = val;
		break;

	case 7:
		OCR4BL = val;
		break;

	case 8:
		OCR4CL = val;
		break;

	case 9:
		OCR2B = val;
		break;

	case 10:
		OCR2A = val;
		break;

	case 11:
		OCR1AL = val;
		break;

	case 12:
		OCR1BL = val;
		break;

	case 13:
		OCR1CL = val;
		break;

	case 44:
		OCR5CL = val;
		break;

	case 45:
		OCR5BL = val;
		break;

	case 46:
		OCR5AL = val;
		break;



	default:
		Serial.println("Error! FAST_PWM can only manipulate pin 2, 3, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46 on ATmega2560.");
		break;
	}
}


#endif



FAST_PWM_Class FAST_PWM;





