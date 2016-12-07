/* version 02
* create in 20141119
* Modified in 20150623
* This labariry can generate 4 parallel serial triggers on ATmega2560 or 1 parallel serial triggers on ATmega328p(UNO or NANO).
*/
#include "PWM_Pulse.h"

#define PRE_SCALE_64DIV (_BV(CS10) | _BV(CS11)) //4us
#define CTC_ISI 250 //equals to 1ms(250*4us)
#define CTC_FACTOR (1000/4/CTC_ISI)
#define PRE_SCALE_MASK (_BV(CS10) | _BV(CS11) | _BV(CS12))
#define PWM_PERIOD 800
#define PWM_LASER_DELAY 0 //us
#define PWM_PERIOD_OFFSET (PWM_LASER_DELAY * 16)

#define TRG_ON 1
#define TRG_OFF 0
//#define RAMPDOWN_STEPNUM 50
#define _SET(var,bit) var |= _BV(bit)
#define _CLEAR(var,bit) var &= ~_BV(bit)
uint8_t t_TCCRnA;
uint8_t t_TCCRnB;
uint8_t t_TCCRnC;
/* PWM pin defination on ATmega2560 :
*	pin		PWM source		physical pin	operate interruptor
*	44		OC5C			PL5				p1
*	45		OC5B			PL4				p2
*	46		OC5A			PL3				p3
*
*/

/***********t5_pwm***************/
/***********t5_pwm***************/
inline static void t5_PWM_init() {
	_SET(DDRL, DDL3); //Set PL3 to output mode.
	_CLEAR(PORTL, PORTL3); //Set PL3 to low;
	_SET(DDRL, DDL4); //Set PL4 to output mode.
	_CLEAR(PORTL, PORTL4); //Set PL4 to low;
	_SET(DDRL, DDL5); //Set PL5 to output mode.
	_CLEAR(PORTL, PORTL5); //Set PL5 to low;

	t_TCCRnA = TCCR5A;
	_CLEAR(t_TCCRnA, WGM50); _SET(t_TCCRnA, WGM51); //Fast PWM, TOP: ICR5;
	_SET(t_TCCRnA, COM5A1); _CLEAR(t_TCCRnA, COM5A0); // non-inverting mode;
	_SET(t_TCCRnA, COM5B1); _CLEAR(t_TCCRnA, COM5B0); // non-inverting mode;
	_SET(t_TCCRnA, COM5C1); _CLEAR(t_TCCRnA, COM5C0); // non-inverting mode;

	t_TCCRnB = TCCR5B;
	_SET(t_TCCRnB, CS50); _CLEAR(t_TCCRnB, CS51); _CLEAR(t_TCCRnB, CS52); // No prescaling;
																		  //_SET(t_TCCRnB, WGM52); _CLEAR(t_TCCRnB, WGM53); //Fast PWM, 10bit;
	_SET(t_TCCRnB, WGM52); _SET(t_TCCRnB, WGM53); //Fast PWM, TOP: ICR5;

	ICR5H = (PWM_PERIOD + PWM_PERIOD_OFFSET) >> 8;
	ICR5L = PWM_PERIOD + PWM_PERIOD_OFFSET; //period of PWM is seted to 50us;

	OCR5AH = 0;
	OCR5AL = 0;

	OCR5BH = 0;
	OCR5BL = 0;

	OCR5CH = 0;
	OCR5CL = 0;

	TCNT5H = 0; //set TCNT5 to 0
	TCNT5L = 0; //set TCNT5 to 0

	TCCR5A = t_TCCRnA;
	TCCR5B = t_TCCRnB;
	TCCR5C = 0;
}

void PWM_write5A(uint16_t data)
{
	data = data + PWM_PERIOD_OFFSET;
	OCR5AH = data >> 8;
	OCR5AL = data;
}


void PWM_write5B(uint16_t data)
{
	data = data + PWM_PERIOD_OFFSET;
	OCR5BH = data >> 8;
	OCR5BL = data;
}


void PWM_write5C(uint16_t data)
{
	data = data + PWM_PERIOD_OFFSET;
	OCR5CH = data >> 8;
	OCR5CL = data;
}


float ramp_cal(float data, float rampdown_stepnum) //data can't be greater than 800. rampdown_stepnum
{
	return (data / rampdown_stepnum);
}

/***********p1***************/
/***********p1***************/
unsigned long t1_ON_DUR;
unsigned long t1_OFF_DUR;
unsigned long t1_PHASE_COUT_NUM;
unsigned long t1_DUR_COUT_NUM;
unsigned long t1_TIMEPASSED;
unsigned long t1_CUR_STATUS;
unsigned long t1_FLIPTIME;
unsigned long t1_PRETRG_DELAY_COUT_NUM;
float t1_REAL_PW;
float t1_POWER_RAMPSTEP;
unsigned long t1_RAMPCOUNTER;
int t1_RAMPSTEPNUM;
char t1_RAMPDOWN_FLAG;


inline static void _set_t1_isi(uint16_t isi){
	OCR1AH = isi >> 8;
	OCR1AL = isi;
}

inline static void t1_init(){
	t_TCCRnB = TCCR1B;
	_CLEAR(t_TCCRnB, CS10); _CLEAR(t_TCCRnB, CS11); _CLEAR(t_TCCRnB, CS12); //Close clock source.
	_SET(t_TCCRnB, WGM12); _CLEAR(t_TCCRnB, WGM13); //CTC mode, TOP: OCR1A;
	TCCR1B = t_TCCRnB;
	TCCR1A = 0;
	TCCR1C = 0;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
	TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
	_set_t1_isi(CTC_ISI);
}


inline static void _t1_stop(){
	TCCR1B &= ~PRE_SCALE_MASK;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
}

inline void _t1_start(byte pre_scale){
	TCCR1B |= pre_scale; //set prescale
}
void t1_init_global()
{
	t1_ON_DUR = 0;
	t1_OFF_DUR = 0;
	t1_PHASE_COUT_NUM = 0;
	t1_DUR_COUT_NUM = 0;
 	t1_TIMEPASSED = 0;
	t1_PRETRG_DELAY_COUT_NUM = 0;
	t1_FLIPTIME = 0;
	t1_REAL_PW = 0;
	t1_POWER_RAMPSTEP = 0;
	t1_RAMPDOWN_FLAG = 0;
	t1_RAMPCOUNTER = 0;
	t1_RAMPSTEPNUM = 0;
}

void t1_operator()
{
 	t1_TIMEPASSED++;
	if(t1_TIMEPASSED < t1_PRETRG_DELAY_COUT_NUM) return;
	else if (t1_TIMEPASSED == t1_PRETRG_DELAY_COUT_NUM)
	{
		PWM_write5C(t1_REAL_PW);
	}

	if (t1_TIMEPASSED < (t1_PRETRG_DELAY_COUT_NUM + t1_DUR_COUT_NUM))
	{
		if(t1_TIMEPASSED < t1_FLIPTIME) return;
		else if((t1_CUR_STATUS == TRG_ON) && (t1_OFF_DUR > 0))
		{
			t1_CUR_STATUS = TRG_OFF;
			t1_FLIPTIME = t1_FLIPTIME + t1_OFF_DUR;
			PWM_write5C(0);
		}
		else if((t1_CUR_STATUS == TRG_OFF) && (t1_ON_DUR > 0))
		{
			t1_CUR_STATUS = TRG_ON;
			t1_FLIPTIME = t1_FLIPTIME + t1_ON_DUR;
			PWM_write5C(t1_REAL_PW);
		}
	}
	else if (1 == t1_RAMPDOWN_FLAG)
	{
		if ((t1_REAL_PW > t1_POWER_RAMPSTEP)&&(t1_RAMPCOUNTER < t1_RAMPSTEPNUM))
		{
			t1_REAL_PW = t1_REAL_PW - t1_POWER_RAMPSTEP;
			PWM_write5C(t1_REAL_PW);
			t1_RAMPCOUNTER++;
		}
		else
		{
			_t1_stop();
			PWM_write5C(0);
			t1_init_global();
		}
	}
	else
	{
		_t1_stop();
		PWM_write5C(0);
		t1_init_global();
	}
}

ISR(TIMER1_COMPA_vect)
{
	t1_operator();
}

void PWM_PULSE_Class::p1_multipulses(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay,int power)
{
	float CYCLE = 1000 / fq;
	if (power > 100)
	{
		Serial.println("Error in Pulse, power can't be greater than 100.");
		return;
	}
	t1_REAL_PW = power * 8;
	if (duration == 0) return;

	//t1_PIN_ = pin;
	t1_DUR_COUT_NUM = duration * CTC_FACTOR;
	t1_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t1_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR) : 0;
	t1_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR) : 0;

	t1_PHASE_COUT_NUM = t1_ON_DUR;
	t1_CUR_STATUS = TRG_ON;
	t1_FLIPTIME = t1_PRETRG_DELAY_COUT_NUM + t1_ON_DUR;
	if (t1_PRETRG_DELAY_COUT_NUM == 0) PWM_write5C(t1_REAL_PW);
	_t1_start(PRE_SCALE_64DIV);
}



void PWM_PULSE_Class::p1_multipulses_ramp(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t1_RAMPSTEPNUM = rampdown_stepnum;
	t1_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t1_RAMPDOWN_FLAG = 1;
	p1_multipulses(duration, fq, p_width, pre_trg_delay, power);
}

void PWM_PULSE_Class::p1_constant( uint32_t duration, unsigned long pre_trg_delay, int power)
{
	float fq = (float)1000 / 2 / (float)duration;
	p1_multipulses(duration, fq, duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p1_constant_ramp( uint32_t duration, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t1_RAMPSTEPNUM = rampdown_stepnum;
	t1_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t1_RAMPDOWN_FLAG = 1;
	p1_constant(duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p1_cancel()
{
	t1_PRETRG_DELAY_COUT_NUM = 0;
	t1_RAMPDOWN_FLAG = 0;
	t1_DUR_COUT_NUM = 0;
}

void PWM_PULSE_Class::p1_cancel_ramp(int rampdown_stepnum)
{
	t1_PRETRG_DELAY_COUT_NUM = 0;
	t1_RAMPSTEPNUM = rampdown_stepnum;
	t1_POWER_RAMPSTEP = ramp_cal(t1_REAL_PW,rampdown_stepnum);
	t1_RAMPDOWN_FLAG = 1;
	t1_DUR_COUT_NUM = 0;
}

/***********p2***************/
/***********p2***************/
unsigned long t3_ON_DUR;
unsigned long t3_OFF_DUR;
unsigned long t3_PHASE_COUT_NUM;
unsigned long t3_DUR_COUT_NUM;
unsigned long t3_TIMEPASSED;
unsigned long t3_CUR_STATUS;
unsigned long t3_FLIPTIME;
unsigned long t3_PRETRG_DELAY_COUT_NUM;
float t3_REAL_PW;
float t3_POWER_RAMPSTEP;
unsigned long t3_RAMPCOUNTER;
int t3_RAMPSTEPNUM;
char t3_RAMPDOWN_FLAG;


inline static void _set_t3_isi(uint16_t isi){
	OCR3AH = isi >> 8;
	OCR3AL = isi;
}

inline static void t3_init(){
	t_TCCRnB = TCCR3B;
	_CLEAR(t_TCCRnB, CS30); _CLEAR(t_TCCRnB, CS31); _CLEAR(t_TCCRnB, CS32); //Close clock source.
	_SET(t_TCCRnB, WGM32); _CLEAR(t_TCCRnB, WGM33); //CTC mode, TOP: OCR3A;
	TCCR3B = t_TCCRnB;
	TCCR3A = 0;
	TCCR3C = 0;
	TCNT3H = 0; //set TCNT3 to 0
	TCNT3L = 0; //set TCNT3 to 0
	TIMSK3 |= _BV(OCIE3A); // Output Compare A Match Interrupt Enable
	_set_t3_isi(CTC_ISI);
}


inline static void _t3_stop(){
	TCCR3B &= ~PRE_SCALE_MASK;
	TCNT3H = 0; //set TCNT3 to 0
	TCNT3L = 0; //set TCNT3 to 0
}

inline void _t3_start(byte pre_scale){
	TCCR3B |= pre_scale; //set prescale
}
void t3_init_global()
{
	t3_ON_DUR = 0;
	t3_OFF_DUR = 0;
	t3_PHASE_COUT_NUM = 0;
	t3_DUR_COUT_NUM = 0;
 	t3_TIMEPASSED = 0;
	t3_PRETRG_DELAY_COUT_NUM = 0;
	t3_FLIPTIME = 0;
	t3_REAL_PW = 0;
	t3_POWER_RAMPSTEP = 0;
	t3_RAMPDOWN_FLAG = 0;
	t3_RAMPCOUNTER = 0;
	t3_RAMPSTEPNUM = 0;
}

void t3_operator()
{
 	t3_TIMEPASSED++;
	if(t3_TIMEPASSED < t3_PRETRG_DELAY_COUT_NUM) return;
	else if (t3_TIMEPASSED == t3_PRETRG_DELAY_COUT_NUM)
	{
		PWM_write5B(t3_REAL_PW);
	}

	if (t3_TIMEPASSED < (t3_PRETRG_DELAY_COUT_NUM + t3_DUR_COUT_NUM))
	{
		if(t3_TIMEPASSED < t3_FLIPTIME) return;
		else if((t3_CUR_STATUS == TRG_ON) && (t3_OFF_DUR > 0))
		{
			t3_CUR_STATUS = TRG_OFF;
			t3_FLIPTIME = t3_FLIPTIME + t3_OFF_DUR;
			PWM_write5B(0);
		}
		else if((t3_CUR_STATUS == TRG_OFF) && (t3_ON_DUR > 0))
		{
			t3_CUR_STATUS = TRG_ON;
			t3_FLIPTIME = t3_FLIPTIME + t3_ON_DUR;
			PWM_write5B(t3_REAL_PW);
		}
	}
	else if (1 == t3_RAMPDOWN_FLAG)
	{
		if ((t3_REAL_PW > t3_POWER_RAMPSTEP)&&(t3_RAMPCOUNTER < t3_RAMPSTEPNUM))
		{
			t3_REAL_PW = t3_REAL_PW - t3_POWER_RAMPSTEP;
			PWM_write5B(t3_REAL_PW);
			t3_RAMPCOUNTER++;
		}
		else
		{
			_t3_stop();
			PWM_write5B(0);
			t3_init_global();
		}
	}
	else
	{
		_t3_stop();
		PWM_write5B(0);
		t3_init_global();
	}
}

ISR(TIMER3_COMPA_vect)
{
	t3_operator();
}

void PWM_PULSE_Class::p2_multipulses(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay,int power)
{
	float CYCLE = 1000 / fq;
	if (power > 100)
	{
		Serial.println("Error in Pulse, power can't be greater than 100.");
		return;
	}
	t3_REAL_PW = power * 8;
	if (duration == 0) return;

	t3_DUR_COUT_NUM = duration * CTC_FACTOR;
	t3_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t3_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR) : 0;
	t3_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR) : 0;

	t3_PHASE_COUT_NUM = t3_ON_DUR;
	t3_CUR_STATUS = TRG_ON;
	t3_FLIPTIME = t3_PRETRG_DELAY_COUT_NUM + t3_ON_DUR;
	if (t3_PRETRG_DELAY_COUT_NUM == 0) PWM_write5B(t3_REAL_PW);
	_t3_start(PRE_SCALE_64DIV);
}



void PWM_PULSE_Class::p2_multipulses_ramp(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t3_RAMPSTEPNUM = rampdown_stepnum;
	t3_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t3_RAMPDOWN_FLAG = 1;
	p2_multipulses(duration, fq, p_width, pre_trg_delay, power);
}

void PWM_PULSE_Class::p2_constant( uint32_t duration, unsigned long pre_trg_delay, int power)
{
	float fq = (float)1000 / 2 / (float)duration;
	p2_multipulses(duration, fq, duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p2_constant_ramp( uint32_t duration, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t3_RAMPSTEPNUM = rampdown_stepnum;
	t3_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t3_RAMPDOWN_FLAG = 1;
	p2_constant(duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p2_cancel()
{
	t3_PRETRG_DELAY_COUT_NUM = 0;
	t3_RAMPDOWN_FLAG = 0;
	t3_DUR_COUT_NUM = 0;
}

void PWM_PULSE_Class::p2_cancel_ramp(int rampdown_stepnum)
{
	t3_PRETRG_DELAY_COUT_NUM = 0;
	t3_RAMPSTEPNUM = rampdown_stepnum;
	t3_POWER_RAMPSTEP = ramp_cal(t3_REAL_PW,rampdown_stepnum);
	t3_RAMPDOWN_FLAG = 1;
	t3_DUR_COUT_NUM = 0;
}


/***********p3***************/
/***********p3***************/
unsigned long t4_ON_DUR;
unsigned long t4_OFF_DUR;
unsigned long t4_PHASE_COUT_NUM;
unsigned long t4_DUR_COUT_NUM;
unsigned long t4_TIMEPASSED;
unsigned long t4_CUR_STATUS;
unsigned long t4_FLIPTIME;
unsigned long t4_PRETRG_DELAY_COUT_NUM;
float t4_REAL_PW;
float t4_POWER_RAMPSTEP;
unsigned long t4_RAMPCOUNTER;
int t4_RAMPSTEPNUM;
char t4_RAMPDOWN_FLAG;


inline static void _set_t4_isi(uint16_t isi){
	OCR4AH = isi >> 8;
	OCR4AL = isi;
}

inline static void t4_init(){
	t_TCCRnB = TCCR4B;
	_CLEAR(t_TCCRnB, CS40); _CLEAR(t_TCCRnB, CS41); _CLEAR(t_TCCRnB, CS42); //Close clock source.
	_SET(t_TCCRnB, WGM42); _CLEAR(t_TCCRnB, WGM43); //CTC mode, TOP: OCR4A;
	TCCR4B = t_TCCRnB;
	TCCR4A = 0;
	TCCR4C = 0;
	TCNT4H = 0; //set TCNT4 to 0
	TCNT4L = 0; //set TCNT4 to 0
	TIMSK4 |= _BV(OCIE4A); // Output Compare A Match Interrupt Enable
	_set_t4_isi(CTC_ISI);
}


inline static void _t4_stop(){
	TCCR4B &= ~PRE_SCALE_MASK;
	TCNT4H = 0; //set TCNT4 to 0
	TCNT4L = 0; //set TCNT4 to 0
}

inline void _t4_start(byte pre_scale){
	TCCR4B |= pre_scale; //set prescale
}
void t4_init_global()
{
	t4_ON_DUR = 0;
	t4_OFF_DUR = 0;
	t4_PHASE_COUT_NUM = 0;
	t4_DUR_COUT_NUM = 0;
 	t4_TIMEPASSED = 0;
	t4_PRETRG_DELAY_COUT_NUM = 0;
	t4_FLIPTIME = 0;
	t4_REAL_PW = 0;
	t4_POWER_RAMPSTEP = 0;
	t4_RAMPDOWN_FLAG = 0;
	t4_RAMPCOUNTER = 0;
	t4_RAMPSTEPNUM = 0;
}

void t4_operator()
{
 	t4_TIMEPASSED++;
	if(t4_TIMEPASSED < t4_PRETRG_DELAY_COUT_NUM) return;
	else if (t4_TIMEPASSED == t4_PRETRG_DELAY_COUT_NUM)
	{
		PWM_write5A(t4_REAL_PW);
	}

	if (t4_TIMEPASSED < (t4_PRETRG_DELAY_COUT_NUM + t4_DUR_COUT_NUM))
	{
		if(t4_TIMEPASSED < t4_FLIPTIME) return;
		else if((t4_CUR_STATUS == TRG_ON) && (t4_OFF_DUR > 0))
		{
			t4_CUR_STATUS = TRG_OFF;
			t4_FLIPTIME = t4_FLIPTIME + t4_OFF_DUR;
			PWM_write5A(0);
		}
		else if((t4_CUR_STATUS == TRG_OFF) && (t4_ON_DUR > 0))
		{
			t4_CUR_STATUS = TRG_ON;
			t4_FLIPTIME = t4_FLIPTIME + t4_ON_DUR;
			PWM_write5A(t4_REAL_PW);
		}
	}
	else if (1 == t4_RAMPDOWN_FLAG)
	{
		if ((t4_REAL_PW > t4_POWER_RAMPSTEP)&&(t4_RAMPCOUNTER < t4_RAMPSTEPNUM))
		{
			t4_REAL_PW = t4_REAL_PW - t4_POWER_RAMPSTEP;
			PWM_write5A(t4_REAL_PW);
			t4_RAMPCOUNTER++;
		}
		else
		{
			_t4_stop();
			PWM_write5A(0);
			t4_init_global();
		}
	}
	else
	{
		_t4_stop();
		PWM_write5A(0);
		t4_init_global();
	}
}

ISR(TIMER4_COMPA_vect)
{
	t4_operator();
}

void PWM_PULSE_Class::p3_multipulses(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay,int power)
{
	float CYCLE = 1000 / fq;
	if (power > 100)
	{
		Serial.println("Error in Pulse, power can't be greater than 100.");
		return;
	}
	t4_REAL_PW = power * 8;
	if (duration == 0) return;

	//t4_PIN_ = pin;
	t4_DUR_COUT_NUM = duration * CTC_FACTOR;
	t4_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t4_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR) : 0;
	t4_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR) : 0;

	t4_PHASE_COUT_NUM = t4_ON_DUR;
	t4_CUR_STATUS = TRG_ON;
	t4_FLIPTIME = t4_PRETRG_DELAY_COUT_NUM + t4_ON_DUR;
	if (t4_PRETRG_DELAY_COUT_NUM == 0) PWM_write5A(t4_REAL_PW);
	_t4_start(PRE_SCALE_64DIV);
}



void PWM_PULSE_Class::p3_multipulses_ramp(unsigned long duration, float fq, unsigned long p_width, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t4_RAMPSTEPNUM = rampdown_stepnum;
	t4_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t4_RAMPDOWN_FLAG = 1;
	p3_multipulses(duration, fq, p_width, pre_trg_delay, power);
}

void PWM_PULSE_Class::p3_constant( uint32_t duration, unsigned long pre_trg_delay, int power)
{
	float fq = (float)1000 / 2 / (float)duration;
	p3_multipulses(duration, fq, duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p3_constant_ramp( uint32_t duration, unsigned long pre_trg_delay, int power, int rampdown_stepnum)
{
	t4_RAMPSTEPNUM = rampdown_stepnum;
	t4_POWER_RAMPSTEP = ramp_cal(power * 8, rampdown_stepnum);
	t4_RAMPDOWN_FLAG = 1;
	p3_constant(duration, pre_trg_delay, power);
}

void PWM_PULSE_Class::p3_cancel()
{
	t4_PRETRG_DELAY_COUT_NUM = 0;
	t4_RAMPDOWN_FLAG = 0;
	t4_DUR_COUT_NUM = 0;
}

void PWM_PULSE_Class::p3_cancel_ramp(int rampdown_stepnum)
{
	t4_PRETRG_DELAY_COUT_NUM = 0;
	t4_RAMPSTEPNUM = rampdown_stepnum;
	t4_POWER_RAMPSTEP = ramp_cal(t4_REAL_PW,rampdown_stepnum);
	t4_RAMPDOWN_FLAG = 1;
	t4_DUR_COUT_NUM = 0;
}


/***************init********************/
/***************init********************/
void PWM_PULSE_Class::init()
{
	t5_PWM_init();
	t1_init();
	t3_init();
	t4_init();
	sei();

}


PWM_PULSE_Class PWM_PULSE;
