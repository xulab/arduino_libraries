/* version 02
* create in 20141119
* Modified in 20150623
* This labariry can generate 4 parallel serial triggers on ATmega2560 or 1 parallel serial triggers on ATmega328p(UNO or NANO).
*/
#include "Pulse.h"

// #define _PULSE_INTERVAL_ 1000 //change fq interval(us)
// #define PRE_SCALE_0DIV _BV(CS10) //0.0625us
#define PRE_SCALE_64DIV (_BV(CS10) | _BV(CS11)) //4us
#define CTC_ISI 250 //equals to 1ms(250*4us)
#define CTC_FACTOR (1000/4/CTC_ISI)
#define PRE_SCALE_MASK (_BV(CS10) | _BV(CS11) | _BV(CS12))

#define TRG_ON 1
#define TRG_OFF 0

#define _SET(var,bit) var |= _BV(bit)
#define _CLEAR(var,bit) var &= ~_BV(bit)
uint8_t t_TCCRnA;
uint8_t t_TCCRnB;


#if defined __AVR_ATmega328P__

/***********p1***************/
/***********p1***************/
int t1_PIN_;
int t1_TRG_ON_VAL;
int t1_TRG_OFF_VAL;
unsigned long t1_ON_DUR;
unsigned long t1_OFF_DUR;
unsigned long t1_PHASE_COUT_NUM;
unsigned long t1_DUR_COUT_NUM;
unsigned long t1_CUR_STATUS;
unsigned long t1_PRETRG_DELAY_COUT_NUM;

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
}


inline static void _t1_stop(){
	TCCR1B &= ~PRE_SCALE_MASK;
	OCR1AH = 0;
	OCR1AL = 0;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
}

inline void _t1_start(byte pre_scale){
	t1_init();
	TCCR1B |= pre_scale; //set prescale
}

inline static void _set_t1_isi(uint16_t isi){
	OCR1AH = isi >> 8;
	OCR1AL = isi;
}

void t1_init_global()
{
	t1_ON_DUR = 0;
	t1_OFF_DUR = 0;
	t1_PHASE_COUT_NUM = 0;
	t1_DUR_COUT_NUM = 0;
	t1_PRETRG_DELAY_COUT_NUM = 0；
}

void t1_operator()
{
	
	if (t1_PRETRG_DELAY_COUT_NUM>0){
		t1_PRETRG_DELAY_COUT_NUM--;
	}
	else
	{
		t1_DUR_COUT_NUM--;
		if (t1_DUR_COUT_NUM > 0)
		{
			if (t1_PHASE_COUT_NUM > 0)
			{
				t1_PHASE_COUT_NUM--;
			}
			else
			{
				if ((t1_CUR_STATUS == TRG_ON) && (t1_OFF_DUR > 0))
				{
					t1_PHASE_COUT_NUM = t1_OFF_DUR;
					t1_CUR_STATUS = TRG_OFF;
					digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
				}
				else if (t1_ON_DUR > 0)
				{
					t1_PHASE_COUT_NUM = t1_ON_DUR;
					t1_CUR_STATUS = TRG_ON;
					digitalWrite(t1_PIN_, t1_TRG_ON_VAL);
				}
			}
		}
		else
		{
			_t1_stop();
			digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
			t1_init_global();
		}
     }
}

ISR(TIMER1_COMPA_vect)
{
	t1_operator();
}




void PULSE_Class::p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t1_PIN_ = pin;
	t1_DUR_COUT_NUM = duration * CTC_FACTOR;

	t1_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t1_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t1_PHASE_COUT_NUM = t1_ON_DUR;
	t1_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t1_TRG_ON_VAL = LOW;
		t1_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t1_TRG_ON_VAL = HIGH;
		t1_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t1_PIN_, t1_TRG_ON_VAL);

	_set_t1_isi(CTC_ISI);

	_t1_start(PRE_SCALE_64DIV);

}


void PULSE_Class::p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode，unsigned long pre_trg_delay)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t1_PIN_ = pin;
	t1_DUR_COUT_NUM = duration * CTC_FACTOR;
	t1_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t1_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t1_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t1_PHASE_COUT_NUM = t1_ON_DUR;
	t1_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t1_TRG_ON_VAL = LOW;
		t1_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t1_TRG_ON_VAL = HIGH;
		t1_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t1_PIN_, t1_TRG_ON_VAL);

	_set_t1_isi(CTC_ISI);

	_t1_start(PRE_SCALE_64DIV);

}



void PULSE_Class::p1_pulse(int pin, uint32_t duration, int mode)
{
	float fq = (float)1000 / 2 / (float)duration;
	p1_sqr_wave(pin, duration, fq, duration, mode);
}

void PULSE_Class::p1_cancel()
{
	_t1_stop();
	digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
	t1_init_global();
}



#elif defined __AVR_ATmega2560__

/***********p1***************/
/***********p1***************/
int t1_PIN_;
int t1_TRG_ON_VAL;
int t1_TRG_OFF_VAL;
unsigned long t1_ON_DUR;
unsigned long t1_OFF_DUR;
unsigned long t1_PHASE_COUT_NUM;
unsigned long t1_DUR_COUT_NUM;
unsigned long t1_CUR_STATUS;
unsigned long t1_PRETRG_DELAY_COUT_NUM;

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
}


inline static void _t1_stop(){
	TCCR1B &= ~PRE_SCALE_MASK;
	OCR1AH = 0;
	OCR1AL = 0;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
}

inline void _t1_start(byte pre_scale){
	t1_init();
	TCCR1B |= pre_scale; //set prescale
}

inline static void _set_t1_isi(uint16_t isi){
	OCR1AH = isi >> 8;
	OCR1AL = isi;
}

void t1_init_global()
{
	t1_ON_DUR = 0;
	t1_OFF_DUR = 0;
	t1_PHASE_COUT_NUM = 0;
	t1_DUR_COUT_NUM = 0;
	t1_PRETRG_DELAY_COUT_NUM = 0;
}

void t1_operator()
{
	if (t1_PRETRG_DELAY_COUT_NUM > 0){
		t1_PRETRG_DELAY_COUT_NUM--;
	}
	else{
			t1_DUR_COUT_NUM--;
			if (t1_DUR_COUT_NUM > 0)
			{
				if (t1_PHASE_COUT_NUM > 0)
				{
					t1_PHASE_COUT_NUM--;
				}
				else
				{
					if ((t1_CUR_STATUS == TRG_ON) && (t1_OFF_DUR > 0))
					{
						t1_PHASE_COUT_NUM = t1_OFF_DUR;
						t1_CUR_STATUS = TRG_OFF;
						digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
					}
					else if (t1_ON_DUR > 0)
					{
						t1_PHASE_COUT_NUM = t1_ON_DUR;
						t1_CUR_STATUS = TRG_ON;
						digitalWrite(t1_PIN_, t1_TRG_ON_VAL);
					}
				}
			}
			else
			{
				_t1_stop();
				digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
				t1_init_global();
			}
        }
}

ISR(TIMER1_COMPA_vect)
{
	t1_operator();
}




void PULSE_Class::p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t1_PIN_ = pin;
	t1_DUR_COUT_NUM = duration * CTC_FACTOR;

	t1_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t1_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t1_PHASE_COUT_NUM = t1_ON_DUR;
	t1_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t1_TRG_ON_VAL = LOW;
		t1_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t1_TRG_ON_VAL = HIGH;
		t1_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t1_PIN_, t1_TRG_ON_VAL);

	_set_t1_isi(CTC_ISI);

	_t1_start(PRE_SCALE_64DIV);

}
void PULSE_Class::p1_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t1_PIN_ = pin;
	t1_DUR_COUT_NUM = duration * CTC_FACTOR;
	t1_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t1_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t1_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t1_PHASE_COUT_NUM = t1_ON_DUR;
	t1_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t1_TRG_ON_VAL = LOW;
		t1_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t1_TRG_ON_VAL = HIGH;
		t1_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t1_PIN_, t1_TRG_ON_VAL);

	_set_t1_isi(CTC_ISI);

	_t1_start(PRE_SCALE_64DIV);

}

void PULSE_Class::p1_pulse(int pin, uint32_t duration, int mode)
{
	float fq = (float)1000 / 2 / (float)duration;
	p1_sqr_wave(pin, duration, fq, duration, mode);
}

void PULSE_Class::p1_cancel()
{
	_t1_stop();
	digitalWrite(t1_PIN_, t1_TRG_OFF_VAL);
	t1_init_global();
}



/***********p2***************/
/***********p2***************/
int t3_PIN_;
int t3_TRG_ON_VAL;
int t3_TRG_OFF_VAL;
unsigned long t3_ON_DUR;
unsigned long t3_OFF_DUR;
unsigned long t3_PHASE_COUT_NUM;
unsigned long t3_DUR_COUT_NUM;
unsigned long t3_CUR_STATUS;
unsigned long t3_PRETRG_DELAY_COUT_NUM;

inline static void t3_init(){
	t_TCCRnB = TCCR3B;
	_CLEAR(t_TCCRnB, CS30); _CLEAR(t_TCCRnB, CS31); _CLEAR(t_TCCRnB, CS32); //Close clock source.
	_SET(t_TCCRnB, WGM32); _CLEAR(t_TCCRnB, WGM33); //CTC mode, TOP: OCR1A;
	TCCR3B = t_TCCRnB;
	TCCR3A = 0;
	TCCR3C = 0;
	TCNT3H = 0; //set TCNT3 to 0
	TCNT3L = 0; //set TCNT3 to 0
	TIMSK3 |= _BV(OCIE3A); // Output Compare A Match Interrupt Enable
}


inline static void _t3_stop(){
	TCCR3B &= ~PRE_SCALE_MASK;
	OCR3AH = 0;
	OCR3AL = 0;
	TCNT3H = 0; //set TCNT3 to 0
	TCNT3L = 0; //set TCNT3 to 0
}

inline void _t3_start(byte pre_scale){
	t3_init();
	TCCR3B |= pre_scale; //set prescale
}

inline static void _set_t3_isi(uint16_t isi){
	OCR3AH = isi >> 8;
	OCR3AL = isi;
}

void t3_init_global()
{
	t3_ON_DUR = 0;
	t3_OFF_DUR = 0;
	t3_PHASE_COUT_NUM = 0;
	t3_DUR_COUT_NUM = 0;
	t3_PRETRG_DELAY_COUT_NUM = 0;
}

void t3_operator()
{
	if (t3_PRETRG_DELAY_COUT_NUM > 0){
		t3_PRETRG_DELAY_COUT_NUM--;
	}
	else{
		t3_DUR_COUT_NUM--;
		if (t3_DUR_COUT_NUM > 0)
		{
			if (t3_PHASE_COUT_NUM > 0)
			{
				t3_PHASE_COUT_NUM--;
			}
			else
			{
				if ((t3_CUR_STATUS == TRG_ON) && (t3_OFF_DUR > 0))
				{
					t3_PHASE_COUT_NUM = t3_OFF_DUR;
					t3_CUR_STATUS = TRG_OFF;
					digitalWrite(t3_PIN_, t3_TRG_OFF_VAL);
				}
				else if (t3_ON_DUR > 0)
				{
					t3_PHASE_COUT_NUM = t3_ON_DUR;
					t3_CUR_STATUS = TRG_ON;
					digitalWrite(t3_PIN_, t3_TRG_ON_VAL);
				}
			}
		}
		else
		{
			_t3_stop();
			digitalWrite(t3_PIN_, t3_TRG_OFF_VAL);
			t3_init_global();
		}
	}
}

ISR(TIMER3_COMPA_vect)
{
	t3_operator();
}




void PULSE_Class::p2_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t3_PIN_ = pin;
	t3_DUR_COUT_NUM = duration * CTC_FACTOR;

	t3_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t3_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t3_PHASE_COUT_NUM = t3_ON_DUR;
	t3_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t3_TRG_ON_VAL = LOW;
		t3_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t3_TRG_ON_VAL = HIGH;
		t3_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t3_PIN_, t3_TRG_ON_VAL);

	_set_t3_isi(CTC_ISI);

	_t3_start(PRE_SCALE_64DIV);

}
void PULSE_Class::p2_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t3_PIN_ = pin;
	t3_DUR_COUT_NUM = duration * CTC_FACTOR;
	t3_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t3_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t3_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t3_PHASE_COUT_NUM = t3_ON_DUR;
	t3_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t3_TRG_ON_VAL = LOW;
		t3_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t3_TRG_ON_VAL = HIGH;
		t3_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t3_PIN_, t3_TRG_ON_VAL);

	_set_t3_isi(CTC_ISI);

	_t3_start(PRE_SCALE_64DIV);

}
void PULSE_Class::p2_pulse(int pin, uint32_t duration, int mode)
{
	float fq = (float)1000 / 2 / (float)duration;
	p2_sqr_wave(pin, duration, fq, duration, mode);
}

void PULSE_Class::p2_cancel()
{
	_t3_stop();
	digitalWrite(t3_PIN_, t3_TRG_OFF_VAL);
	t3_init_global();
}


/***********p3***************/
/***********p3***************/
int t4_PIN_;
int t4_TRG_ON_VAL;
int t4_TRG_OFF_VAL;
unsigned long t4_ON_DUR;
unsigned long t4_OFF_DUR;
unsigned long t4_PHASE_COUT_NUM;
unsigned long t4_DUR_COUT_NUM;
unsigned long t4_CUR_STATUS;
unsigned long t4_PRETRG_DELAY_COUT_NUM;


inline static void t4_init(){
	t_TCCRnB = TCCR4B;
	_CLEAR(t_TCCRnB, CS40); _CLEAR(t_TCCRnB, CS41); _CLEAR(t_TCCRnB, CS42); //Close clock source.
	_SET(t_TCCRnB, WGM42); _CLEAR(t_TCCRnB, WGM43); //CTC mode, TOP: OCR1A;
	TCCR4B = t_TCCRnB;
	TCCR4A = 0;
	TCCR4C = 0;
	TCNT4H = 0; //set TCNT4 to 0
	TCNT4L = 0; //set TCNT4 to 0
	TIMSK4 |= _BV(OCIE4A); // Output Compare A Match Interrupt Enable
}


inline static void _t4_stop(){
	TCCR4B &= ~PRE_SCALE_MASK;
	OCR4AH = 0;
	OCR4AL = 0;
	TCNT4H = 0; //set TCNT4 to 0
	TCNT4L = 0; //set TCNT4 to 0
}

inline void _t4_start(byte pre_scale){
	t4_init();
	TCCR4B |= pre_scale; //set prescale
}

inline static void _set_t4_isi(uint16_t isi){
	OCR4AH = isi >> 8;
	OCR4AL = isi;
}

void t4_init_global()
{
	t4_ON_DUR = 0;
	t4_OFF_DUR = 0;
	t4_PHASE_COUT_NUM = 0;
	t4_DUR_COUT_NUM = 0;
	t4_PRETRG_DELAY_COUT_NUM = 0;
}

void t4_operator()
{
	if (t4_PRETRG_DELAY_COUT_NUM > 0){
		t4_PRETRG_DELAY_COUT_NUM--;
	}
	else{
		t4_DUR_COUT_NUM--;
		if (t4_DUR_COUT_NUM > 0)
		{
			if (t4_PHASE_COUT_NUM > 0)
			{
				t4_PHASE_COUT_NUM--;
			}
			else
			{
				if ((t4_CUR_STATUS == TRG_ON) && (t4_OFF_DUR > 0))
				{
					t4_PHASE_COUT_NUM = t4_OFF_DUR;
					t4_CUR_STATUS = TRG_OFF;
					digitalWrite(t4_PIN_, t4_TRG_OFF_VAL);
				}
				else if (t4_ON_DUR > 0)
				{
					t4_PHASE_COUT_NUM = t4_ON_DUR;
					t4_CUR_STATUS = TRG_ON;
					digitalWrite(t4_PIN_, t4_TRG_ON_VAL);
				}
			}
		}
		else
		{
			_t4_stop();
			digitalWrite(t4_PIN_, t4_TRG_OFF_VAL);
			t4_init_global();
		}
	}
}

ISR(TIMER4_COMPA_vect)
{
	t4_operator();
}




void PULSE_Class::p3_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t4_PIN_ = pin;
	t4_DUR_COUT_NUM = duration * CTC_FACTOR;
	

	t4_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t4_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t4_PHASE_COUT_NUM = t4_ON_DUR;
	t4_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t4_TRG_ON_VAL = LOW;
		t4_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t4_TRG_ON_VAL = HIGH;
		t4_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t4_PIN_, t4_TRG_ON_VAL);

	_set_t4_isi(CTC_ISI);

	_t4_start(PRE_SCALE_64DIV);

}

void PULSE_Class::p3_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay)
{
	float CYCLE = 1000 / fq;

	if (duration == 0) return;

	t4_PIN_ = pin;
	t4_DUR_COUT_NUM = duration * CTC_FACTOR;
	t4_PRETRG_DELAY_COUT_NUM = pre_trg_delay * CTC_FACTOR;

	t4_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
	t4_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

	t4_PHASE_COUT_NUM = t4_ON_DUR;
	t4_CUR_STATUS = TRG_ON;

	if (mode == TRG_L)
	{
		t4_TRG_ON_VAL = LOW;
		t4_TRG_OFF_VAL = HIGH;
	}
	else
	{
		t4_TRG_ON_VAL = HIGH;
		t4_TRG_OFF_VAL = LOW;
	}

	digitalWrite(t4_PIN_, t4_TRG_ON_VAL);

	_set_t4_isi(CTC_ISI);

	_t4_start(PRE_SCALE_64DIV);

}

void PULSE_Class::p3_pulse(int pin, uint32_t duration, int mode)
{
	float fq = (float)1000 / 2 / (float)duration;
	p3_sqr_wave(pin, duration, fq, duration, mode);
}

void PULSE_Class::p3_cancel()
{
	_t4_stop();
	digitalWrite(t4_PIN_, t4_TRG_OFF_VAL);
	t4_init_global();
}



/***********t5_pwm***************/
/***********t5_pwm***************/
inline static void t5_PWM_init(){
	_SET(DDRL, DDL3); //Set PL3 to output mode.
	_CLEAR(PORTL, PORTL3); //Set PL3 to low;
	_SET(DDRL, DDL4); //Set PL4 to output mode.
	_CLEAR(PORTL, PORTL4); //Set PL4 to low;
	_SET(DDRL, DDL5); //Set PL5 to output mode.
	_CLEAR(PORTL, PORTL5); //Set PL5 to low;

	t_TCCRnA = TCCR5A;
	_SET(t_TCCRnA, WGM50); _SET(t_TCCRnA, WGM51); //Fast PWM, 10bit;
	_SET(t_TCCRnA, COM5A1); _CLEAR(t_TCCRnA, COM5A0); // non-inverting mode;
	_SET(t_TCCRnA, COM5B1); _CLEAR(t_TCCRnA, COM5B0); // non-inverting mode;
	_SET(t_TCCRnA, COM5C1); _CLEAR(t_TCCRnA, COM5C0); // non-inverting mode;

	t_TCCRnB = TCCR5B;
	_SET(t_TCCRnB, CS50); _CLEAR(t_TCCRnB, CS51); _CLEAR(t_TCCRnB, CS52); // No prescaling;
	_SET(t_TCCRnB, WGM52); _CLEAR(t_TCCRnB, WGM53); //Fast PWM, 10bit;

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

	//TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
}

void PULSE_Class::PWM_write5A(uint16_t data)
{
	OCR5AH = data >> 8;
	OCR5AL = data;
}


void PULSE_Class::PWM_write5B(uint16_t data)
{
	OCR5BH = data >> 8;
	OCR5BL = data;
}


void PULSE_Class::PWM_write5C(uint16_t data)
{
	OCR5CH = data >> 8;
	OCR5CL = data;
}

/***********p4***************/
/***********p4***************/
// int t5_PIN_;
// int t5_TRG_ON_VAL;
// int t5_TRG_OFF_VAL;
// unsigned long t5_ON_DUR;
// unsigned long t5_OFF_DUR;
// unsigned long t5_PHASE_COUT_NUM;
// unsigned long t5_DUR_COUT_NUM;
// unsigned long t5_CUR_STATUS;
// unsigned long t5_PRETRG_DELAY_COUT_NUM;



// inline static void t5_init(){
// 	t_TCCRnB = TCCR5B;
// 	_CLEAR(t_TCCRnB, CS50); _CLEAR(t_TCCRnB, CS51); _CLEAR(t_TCCRnB, CS52); //Close clock source.
// 	_SET(t_TCCRnB, WGM52); _CLEAR(t_TCCRnB, WGM53); //CTC mode, TOP: OCR1A;
// 	TCCR5B = t_TCCRnB;
// 	TCCR5A = 0;
// 	TCCR5C = 0;
// 	TCNT5H = 0; //set TCNT5 to 0
// 	TCNT5L = 0; //set TCNT5 to 0
// 	TIMSK5 |= _BV(OCIE5A); // Output Compare A Match Interrupt Enable
// }


// inline static void _t5_stop(){
// 	TCCR5B &= ~PRE_SCALE_MASK;
// 	OCR5AH = 0;
// 	OCR5AL = 0;
// 	TCNT5H = 0; //set TCNT5 to 0
// 	TCNT5L = 0; //set TCNT5 to 0
// }

// inline void _t5_start(byte pre_scale){
// 	t5_init();
// 	TCCR5B |= pre_scale; //set prescale
// }

// inline static void _set_t5_isi(uint16_t isi){
// 	OCR5AH = isi >> 8;
// 	OCR5AL = isi;
// }

// void t5_init_global()
// {
// 	t5_ON_DUR = 0;
// 	t5_OFF_DUR = 0;
// 	t5_PHASE_COUT_NUM = 0;
// 	t5_DUR_COUT_NUM = 0;
// 	t5_PRETRG_DELAY_COUT_NUM = 0;
// }

// void t5_operator()
// {
// 	if (t5_PRETRG_DELAY_COUT_NUM > 0){
// 	t5_PRETRG_DELAY_COUT_NUM--;
// 	}
// 	else{
// 		t5_DUR_COUT_NUM--;
// 		if (t5_DUR_COUT_NUM > 0)
// 		{
// 			if (t5_PHASE_COUT_NUM > 0)
// 			{
// 				t5_PHASE_COUT_NUM--;
// 			}
// 			else
// 			{
// 				if ((t5_CUR_STATUS == TRG_ON) && (t5_OFF_DUR > 0))
// 				{
// 					t5_PHASE_COUT_NUM = t5_OFF_DUR;
// 					t5_CUR_STATUS = TRG_OFF;
// 					digitalWrite(t5_PIN_, t5_TRG_OFF_VAL);
// 				}
// 				else if (t5_ON_DUR > 0)
// 				{
// 					t5_PHASE_COUT_NUM = t5_ON_DUR;
// 					t5_CUR_STATUS = TRG_ON;
// 					digitalWrite(t5_PIN_, t5_TRG_ON_VAL);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			_t5_stop();
// 			digitalWrite(t5_PIN_, t5_TRG_OFF_VAL);
// 			t5_init_global();
// 		}
// 	}
// }

// ISR(TIMER5_COMPA_vect)
// {
// 	t5_operator();
// }




// void PULSE_Class::p4_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode)
// {
// 	float CYCLE = 1000 / fq;

// 	if (duration == 0) return;

// 	t5_PIN_ = pin;
// 	t5_DUR_COUT_NUM = duration * CTC_FACTOR;

// 	t5_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
// 	t5_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

// 	t5_PHASE_COUT_NUM = t5_ON_DUR;
// 	t5_CUR_STATUS = TRG_ON;

// 	if (mode == TRG_L)
// 	{
// 		t5_TRG_ON_VAL = LOW;
// 		t5_TRG_OFF_VAL = HIGH;
// 	}
// 	else
// 	{
// 		t5_TRG_ON_VAL = HIGH;
// 		t5_TRG_OFF_VAL = LOW;
// 	}

// 	digitalWrite(t5_PIN_, t5_TRG_ON_VAL);

// 	_set_t5_isi(CTC_ISI);

// 	_t5_start(PRE_SCALE_64DIV);

// }


// void PULSE_Class::p4_sqr_wave(int pin, unsigned long duration, float fq, unsigned long p_width, int mode, unsigned long pre_trg_delay)
// {
// 	float CYCLE = 1000 / fq;

// 	if (duration == 0) return;

// 	t5_PIN_ = pin;
// 	t5_DUR_COUT_NUM = duration * CTC_FACTOR;
// 	t5_PRETRG_DELAY_COUT_NUM=pre_trg_delay*CTC_FACTOR;

// 	t5_ON_DUR = (p_width > 0) ? (p_width * CTC_FACTOR - 1) : 0;
// 	t5_OFF_DUR = (CYCLE > p_width) ? ((CYCLE - p_width) * CTC_FACTOR - 1) : 0;

// 	t5_PHASE_COUT_NUM = t5_ON_DUR;
// 	t5_CUR_STATUS = TRG_ON;

// 	if (mode == TRG_L)
// 	{
// 		t5_TRG_ON_VAL = LOW;
// 		t5_TRG_OFF_VAL = HIGH;
// 	}
// 	else
// 	{
// 		t5_TRG_ON_VAL = HIGH;
// 		t5_TRG_OFF_VAL = LOW;
// 	}

// 	digitalWrite(t5_PIN_, t5_TRG_ON_VAL);

// 	_set_t5_isi(CTC_ISI);

// 	_t5_start(PRE_SCALE_64DIV);

// }


// void PULSE_Class::p4_pulse(int pin, uint32_t duration, int mode)
// {
// 	float fq = (float)1000 / 2 / (float)duration;
// 	p4_sqr_wave(pin, duration, fq, duration, mode);
// }

// void PULSE_Class::p4_cancel()
// {
// 	_t5_stop();
// 	digitalWrite(t5_PIN_, t5_TRG_OFF_VAL);
// 	t5_init_global();
// }

#else 
#error "FAST_PWM only suit for Mage2560, UNO(mega328) and NANO(mega328)."
#endif



/***************init********************/
/***************init********************/
void PULSE_Class::init()
{
	sei();

}


PULSE_Class PULSE;
