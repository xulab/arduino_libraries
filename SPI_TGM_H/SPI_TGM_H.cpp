/* version 11
 * modified in 20181129
 * only suit for teensy 3.6
 * change: add pre_sound_delay.
 * change: add clicks.
 * change: add amplitude modulation.
 * Reaction time: 700us.
 */

#include <math.h>
#include <SPI_TGM_H.h>
#include <SPI.h>
// #include <CACHE.h>
// #include "TGM_CACHE.h"
#include "TGM_parameter.h"

#define DEBUG 1 
#include "TGM_debug.h"


 /*-----------TGM_config-------------*/
const char VERSION_STRING[] = "VER011_20181207_PJW.";
/*-----------error-------------*/
const char noerror_string[] = "no error";
const char error_string1[] = "no error";
char temp_error_string[ERROR_STRING_SIZE] = "";
/*-----------error-------------*/
/*-----------TGM_config-------------*/


/*--------timer1-------*/
#define TIMER1_STOP 0
#define TIMER1_RUNNING 1
#define TIMER1_INTERRUPTED 2
#define TIMER1_PAUSE 3
#define SWEEP_INTERVAL 100 //change fq interval(us)
#define MININT 10 //10us, about 1800 clocks for teensy3.6 .

#ifdef ARDUINO_ARCH_ESP32
hw_timer_t *timer1 = NULL;
#else
IntervalTimer timer1;
#endif

byte flag_vol_i = 0;
volatile byte _target_vol = 192;
volatile byte timer1_mode = 0;
volatile byte step_index = 0;
volatile uint32_t delay_time = 0;
volatile uint32_t sweep_index = 0;
volatile uint32_t sweep_index_size = 0;
// volatile float sweep_linear_step = 0;
volatile double sweep_linear_step = 0;
// volatile float sweep_linear_step2 = 0;
volatile double sweep_linear_step2 = 0;
// volatile float sweep_base = 0;
volatile double sweep_base = 0;
volatile uint32_t chord_fq_array[MAX_CHORD_NUM];
volatile uint16_t chord_fq_num = 0;
uint32_t sweep_fq0 = 0;
uint32_t sweep_fq1 = 0;
uint32_t sweep_fq2 = 0;
byte sweep_mode = SWEEP_OFF;


toneStep toneStepArray[TONESTEPARRAYSIZE]; //just creat some tonestep struct. 6 steps may satisfy most situation.
toneStep IdleToneStep; //just creat some tonestep struct. 6 steps may satisfy most situation.
toneStep * curToneStep = NULL; //Current addr of tonestep struct.


TGMinfo info;
TMGerror error;
ton _EMPTY_TONE;
ton _QUICK_TONE;
// ton _tone;


void eraseAllToneStep() {

	for (int i = 0; i < TONESTEPARRAYSIZE; i++)
	{
		memset((void*)toneStepArray, 0, sizeof(toneStepArray));
	}
	curToneStep = NULL;
}

#define MaxAMArraySize 192
volatile byte AMArraySize = 0;
// float AMInterval[MaxAMArraySize] = {};
uint64_t AMInterval[MaxAMArraySize] = {};
byte AMVol[MaxAMArraySize] = {};

const uint16_t cosISI_temp[192] = {};
const byte cosVol_temp[192] = {};

uint32_t sweepLinear() {
	// return (uint32_t)((float)sweep_fq0 + (sweep_linear_step * (float)curToneStep->LoopInd));
	return (uint32_t)(sweep_fq0 + (sweep_linear_step * curToneStep->LoopInd));
}
uint32_t sweepLinear2() {
	return (uint32_t)(sweep_fq1 + (sweep_linear_step2 * curToneStep->LoopInd));
}

uint32_t sweepExp() {
	return sweep_fq0 * pow(sweep_base, curToneStep->LoopInd);
}


void _timer1_begin(void (*fn)(void), uint32_t t)
{
	uint32_t tmp_time = (t < MININT) ? MININT : t;
#ifdef ARDUINO_ARCH_ESP32
	timerStop(timer1);
	timerAlarmWrite(timer1, tmp_time, true); // set time in us. if less than 4us, would cause error. autoreload is true
	timerWrite(timer1, 0);
	timerStart(timer1);
#else
	timer1.begin(fn, tmp_time);
#endif
}

void _timer1_stop() {
#ifdef ARDUINO_ARCH_ESP32
	// if(timerStarted(timer1)) timerStop(timer1);
  timerStop(timer1);
#else
	timer1.end();
#endif
}


#ifdef ARDUINO_ARCH_ESP32
void ARDUINO_ISR_ATTR timer1Func()
#else
void timer1Func() 
#endif
{
	timer1_mode = TIMER1_INTERRUPTED;
	if (NULL == curToneStep || 0 == curToneStep->LoopNum) {
		_timer1_stop(); //ToneStepCur equals to NULL(0) means tone is finished.  LoopNum equals to 0 means some errors are accurred.
	}
	else {
		/**********ISI***********/
		uint64_t interTimeTemp = 0;
		switch (curToneStep->ISI.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			interTimeTemp = curToneStep->ISI.drtPar;
			_timer1_begin(timer1Func, interTimeTemp);
			break;
		case TONESTEPARRAY:
			// interTimeTemp = ((float *)(curToneStep->ISI.arrayPar))[curToneStep->LoopInd];
			interTimeTemp = ((uint64_t *)(curToneStep->ISI.arrayPar))[curToneStep->LoopInd];
			_timer1_begin(timer1Func, interTimeTemp);
			break;
		case TONESTEPARRAYREV:
			// interTimeTemp = ((float *)(curToneStep->ISI.arrayPar))[curToneStep->ISI.array_Size - 1 - curToneStep->LoopInd];
			interTimeTemp = ((uint64_t *)(curToneStep->ISI.arrayPar))[curToneStep->ISI.array_Size - 1 - curToneStep->LoopInd];
			_timer1_begin(timer1Func, interTimeTemp);
			break;
		case TONESTEPFNC:
			if (NULL == curToneStep->ISI.fnc) break;
			interTimeTemp = ((uint64_t_fnc)curToneStep->ISI.fnc)();
			_timer1_begin(timer1Func, interTimeTemp);
			break;
		default: break;
		}

		/**********fq***********/
		switch (curToneStep->fq.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			_set_fq(curToneStep->fq.drtPar);
			break;
		case TONESTEPARRAY: break;
		case TONESTEPFNC:
			if (NULL == curToneStep->fq.fnc) break;
			_set_fq((*((uint32_fnc)curToneStep->fq.fnc))());
			break;
		default: break;
		}

		/**********vol***********/
		byte volTemp = 0;
		switch (curToneStep->vol.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			_set_vol(curToneStep->vol.drtPar);
			break;
		case TONESTEPARRAY:
			volTemp = ((byte *)(curToneStep->vol.arrayPar))[curToneStep->LoopInd];
			if (curToneStep->targetVol > volTemp) {
				_set_vol(curToneStep->targetVol - volTemp);
			}
			else {
				_set_vol(0);
			}
			break;
		case TONESTEPARRAYREV:
			volTemp = ((byte *)(curToneStep->vol.arrayPar))[curToneStep->vol.array_Size - 1 - curToneStep->LoopInd];
			if (curToneStep->targetVol > volTemp) {
				_set_vol(curToneStep->targetVol - volTemp);
			}
			else {
				_set_vol(0);
			}
			break;
		case TONESTEPFNC:
			if (NULL == curToneStep->vol.fnc) break;
			_set_vol((*((byte_fnc)curToneStep->vol.fnc))());
			break;
		default: break;
		}


		curToneStep->LoopInd++;

		if (curToneStep->LoopInd >= curToneStep->LoopNum) {
			curToneStep->LoopInd = 0; //prepare for CycleToneStep1.
			if (0 != curToneStep->CycleNum1) {
				curToneStep->CycleNum1--;
				curToneStep = curToneStep->CycleToneStep1;
			}
			else {
				if (0 != curToneStep->CycleNum2) {
					curToneStep->CycleNum2--;
					curToneStep = curToneStep->CycleToneStep2;
				}
				else {
					curToneStep = curToneStep->NextToneStep;
				}
			}
		}
	}
	timer1_mode = TIMER1_RUNNING;
}

void _timer1_init()
{
#ifdef ARDUINO_ARCH_ESP32
	timer1 = timerBegin(1, 80, true);				 // init timer 1, div 80
	timerAttachInterrupt(timer1, &timer1Func, true); // attach callback
	timerAlarmWrite(timer1, MININT, true); // set time in us. if less than 4us, would cause error. autoreload is true
	timerAlarmEnable(timer1);						 // enable interrupt
	// timerWrite(timer1, 0);
	timerStop(timer1);
#endif
}

void _timer1_pause() {
#ifdef ARDUINO_ARCH_ESP32
//  if(timerStarted(timer1)) timerStop(timer1);
	timerStop(timer1);
#else
	timer1.end();
#endif
}

void _timer1_resume() {
#ifdef ARDUINO_ARCH_ESP32
	timerStart(timer1);
	// _timer1_begin(timer1Func, MININT);
#else
	_timer1_begin(timer1Func, MININT);
#endif
}

/*--------SPI_TGMClass-------*/

void SPI_TGMClass::init()
{
	memset(&info, 0, sizeof(TGMinfo));
	info.nSize = sizeof(TGMinfo);
	info.version = TGM_VERSION;
	strcpy(info.version_str, VERSION_STRING);

	memset(&error, 0, sizeof(TMGerror));
	error.nSize = sizeof(TMGerror);
	error.error_code = NO_ERROR;
	strcpy(error.error_string, noerror_string);

	memset(&_tone, 0, sizeof(ton));
	_tone.nSize = sizeof(ton);
	_tone.version = TGM_VERSION;

	memset(&_EMPTY_TONE, 0, sizeof(ton));
	_EMPTY_TONE.nSize = sizeof(ton);
	_EMPTY_TONE.version = TGM_VERSION;

	memset(&_QUICK_TONE, 0, sizeof(ton));
	_QUICK_TONE.nSize = sizeof(ton);
	_QUICK_TONE.version = TGM_VERSION;

	eraseAllToneStep();
	_SPI_INIT();

#ifdef DEBUG
	 Serial.begin(115200);
#endif // DEBUG
}

inline void _write_info(TGMinfo* data) {
	// CACHE.q_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char*)data);
	cacheWrite(TGM_INFO_ADDR, sizeof(TGMinfo), (char*)data);
}

inline void _write_error(TMGerror* data) {
	// CACHE.q_write(ERROR_ADDR, sizeof(TMGerror), (char*)data);
	cacheWrite(ERROR_ADDR, sizeof(TMGerror), (char*)data);
}

int SPI_TGMClass::wait_command(long timeout) {
	// while (LOW == CACHE.new_info());
	long tmp_time = millis();
	while (LOW == cacheNewInfo())
	{
		if(timeout != -1)
		{
			if(((long)millis()-tmp_time) > timeout) return -1;
		}
	}	
	return 0;
}

uint32_t random_noise_fq() {
	uint32_t fq, fq0, fq1;
	if (sweep_fq1 > sweep_fq0) {
		fq0 = sweep_fq0;
		fq1 = sweep_fq1;
	}
	else {
		fq0 = sweep_fq1;
		fq1 = sweep_fq0;
	}
	switch (sweep_mode) {
	case SWEEP_NOISE_WHITE:
		fq = random(fq0, fq1);
		break;

	case SWEEP_NOISE_GAUSS1:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 2;
		break;

	case SWEEP_NOISE_GAUSS2:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 3;
		break;

	case SWEEP_NOISE_GAUSS3:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 4;
		break;
	}
	return fq;
}

uint32_t chord_fq() {
	return chord_fq_array[curToneStep->LoopInd%chord_fq_num];
}

void SPI_TGMClass::init_TGM()
{
	_ad9850_init();
	init();
	_pga2310_init();
	_write_info(&info);
	_write_error(&error);
	_set_vol(INIT_VOL + 1); // Refresh the curVol
	_set_fq(INIT_FQ + 1);	// Refresh the curFq
	_set_vol(INIT_VOL);		// Refresh the curVol
	_set_fq(INIT_FQ);		// Refresh the curFq
	_timer1_init();
}

void SPI_TGMClass::set_fq(uint32_t fq) {
	_set_fq(fq);
}

void SPI_TGMClass::set_vol(byte vol) {
	_set_vol(vol);
}

void SPI_TGMClass::testTone() {
	memset(&_tone, 0, sizeof(ton));
	_tone.tone_flag = TONE_FLAG_ON;
	_tone.volume_mode = VOLUME_ON;
	_tone.version = TGM_VERSION;
	_tone.pre_sound_delay = 0;
	_tone.volume = 192;
	_tone.frequency0 = 3000;
	_tone.frequencyL0 = 3000;
	//_tone.frequency1 = 5000;
	//_tone.frequencyL1 = 5000;
	_tone.sweep = SWEEP_OFF;
	_tone.duration = 10000;
	_tone.durationL = 10000;
	_tone.step_up_flag = STEP_FLAG_COS_2MS;
	_tone.step_down_flag = STEP_FLAG_COS_2MS;
	//_tone.clicks_dur = 1;
	//_tone.clicks_period = 10;
}

void SPI_TGMClass::read_tone() {
	if (TIMER1_STOP != timer1_mode)
	{
		_timer1_pause();
	}
	// CACHE.q_read(TONE_ADDR, sizeof(ton), (char*)&_tone);
	cacheRead(TONE_ADDR, sizeof(ton), (char*)&_tone);
#ifdef DEBUG
	Serial.print("_tone.tone_flag = ");
	Serial.println(_tone.tone_flag);
	Serial.print("_tone.volume_mode = ");
	Serial.println(_tone.volume_mode);
	Serial.print("_tone.version = ");
	Serial.println(_tone.version);
	Serial.print("_tone.pre_sound_delay = ");
	Serial.println(_tone.pre_sound_delay);
	Serial.print("_tone.volume = ");
	Serial.println(_tone.volume);
	Serial.print("_tone.frequency0 = ");
	Serial.println(_tone.frequency0);
	Serial.print("_tone.duration = ");
	Serial.println(_tone.duration);
	Serial.print("_tone.durationL = ");
	Serial.println(_tone.durationL);
	Serial.print("_tone.step_up_flag = ");
	Serial.println(_tone.step_up_flag);
	Serial.print("_tone.step_down_flag = ");
	Serial.println(_tone.step_down_flag);
#endif // DEBUG

}

void setDelayStep(toneStep * ts, uint32_t t) {
	ts->LoopNum = t; //delay ms
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = 1000; // 1000us.
}

void setOneLoopStep(toneStep * ts) {
	ts->LoopNum = 1;
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = MININT;
}

void setSweepLoopStep(toneStep * ts) {
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = SWEEP_INTERVAL;
}

// void setVolArrayStep(toneStep * ts, float * ISIArray, byte * VolArray, uint32_t arraySize, byte targetVol, byte reverse) {
void setVolArrayStep(toneStep * ts, uint64_t * ISIArray, byte * VolArray, uint32_t arraySize, byte targetVol, byte reverse) {
	ts->targetVol = targetVol;
	ts->LoopNum = arraySize;

	ts->ISI.array_Size = arraySize;
	ts->ISI.arrayPar = (void *)ISIArray;

	ts->vol.array_Size = arraySize;
	ts->vol.arrayPar = (void *)VolArray;

	if (0 == reverse) {
		ts->ISI.flag = TONESTEPARRAY;
		ts->vol.flag = TONESTEPARRAY;
	}
	else {
		ts->ISI.flag = TONESTEPARRAYREV;
		ts->vol.flag = TONESTEPARRAYREV;
	}
}


void prepareToneStep(toneStep ** ts, byte * i) {
	toneStep * toneStep_temp = &(toneStepArray[*i]);
	memset(toneStep_temp, 0, sizeof(toneStep));
	if (NULL != (*ts)) {
		(*ts)->NextToneStep = toneStep_temp; //Add current tonestep to last member NextTonStep.
	}
	(*ts) = toneStep_temp; //toneStep_temp point to current struct.
	(*i)++;
}


void SPI_TGMClass::set_tone() {
#ifdef DEBUG
	print_tone(&_tone);
#endif
	uint32_t dur_temp = 0;
	byte tsInd = 0;
	toneStep * toneStep_temp = NULL;
	if (TONE_FLAG_OFF == _tone.tone_flag) // check _tone.tone_flag is a new command.
	{
		if (TIMER1_PAUSE == timer1_mode) _timer1_resume();
	}
	else
	{
		_timer1_stop();
		_target_vol = _tone.volume;
		sweep_mode = _tone.sweep;
		dur_temp = (_tone.version < 10) ? _tone.duration : _tone.durationL; //there is a new variable "_tone.duration_2" (32bit), so you can play sound longer than 6.5s.
		sweep_fq0 = (_tone.version < 11) ? _tone.frequency0 : _tone.frequencyL0;
		sweep_fq1 = (_tone.version < 11) ? _tone.frequency1 : _tone.frequencyL1;
		sweep_fq2 = (_tone.version < 11) ? _tone.frequency2 : _tone.frequencyL2;

		memcpy((void*)chord_fq_array, &(sweep_fq0), sizeof(sweep_fq0)*_tone.chord_num);
		chord_fq_num = _tone.chord_num;
		/************step1 pretrigger delay**************/
		if (_tone.pre_sound_delay > 0)
		{
			prepareToneStep(&toneStep_temp, &tsInd);
			setDelayStep(toneStep_temp, _tone.pre_sound_delay);
		}

		/************step2 set startup fq **************/
//test
		// if (dur_temp != 0) {
//test
			switch (sweep_mode) {
			case SWEEP_OFF:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_LINEAR:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_EXP:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_ARRAY:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_NOISE_WHITE:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS1:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS2:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS3:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_CHORD:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_PEAK:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_CLICKS:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_CLICKS_RAMP_2MS:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_AM:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;
			}
//test
		// }
//test

		/************step3 ramp up**************/
		switch (_tone.step_up_flag) {
		case STEP_FLAG_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), _target_vol, 0);
			break;
		case STEP_FLAG_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 0);
			break;
		}


		/************step4 set target volume**************/
		if (VOLUME_ON == _tone.volume_mode) {
			prepareToneStep(&toneStep_temp, &tsInd);
			setOneLoopStep(toneStep_temp);
			toneStep_temp->LoopNum = 1;
			toneStep_temp->vol.flag = TONESTEPDRT;
			toneStep_temp->vol.drtPar = _target_vol;
		}

		/************step5-9 duration**************/
		uint32_t sweepLoopNum_temp = 0;
		uint32_t clicksDur_temp = _tone.clicks_dur + 4; //ramp up and down cost 4ms.
		uint32_t clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
		uint32_t normalInterval = clicksPeriod_temp - clicksDur_temp;
		uint32_t CycleNum1_temp = dur_temp / clicksPeriod_temp;
		uint32_t lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;
		toneStep * clicksStep_temp = NULL;

		if (dur_temp != 0) {
			switch (sweep_mode) {
			case SWEEP_OFF:
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, dur_temp);
				break;
			case SWEEP_LINEAR:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;
				sweep_linear_step = ((float)sweep_fq1 - (float)sweep_fq0) / ((float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear;
				break;

			case SWEEP_EXP:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;
				sweep_base = exp(log((float)sweep_fq1 / (float)sweep_fq0) / (float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepExp;
				break;

			case SWEEP_ARRAY:
				break;

			case SWEEP_NOISE_WHITE:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS1:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS2:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS3:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_CHORD:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)chord_fq;
				break;

			case SWEEP_PEAK:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL / 2;
				sweep_linear_step = ((float)sweep_fq1 - (float)sweep_fq0) / ((float)sweepLoopNum_temp);
				sweep_linear_step2 = ((float)sweep_fq2 - (float)sweep_fq1) / ((float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear2;
				break;

			case SWEEP_CLICKS:
				clicksDur_temp = _tone.clicks_dur;
				clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
				normalInterval = clicksPeriod_temp - clicksDur_temp;
				CycleNum1_temp = dur_temp / clicksPeriod_temp;
				lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;

				/*clicks duration*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, _tone.clicks_dur);
				clicksStep_temp = toneStep_temp;
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = _target_vol;
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = (0 != normalInterval) ? &(toneStepArray[tsInd]) : clicksStep_temp; //CycleToneStep1 point to interval1 step.
				toneStep_temp->CycleNum2 = 1;
				toneStep_temp->CycleToneStep2 = (0 != lastClickInterval_temp) ? &(toneStepArray[tsInd + 1]) : &(toneStepArray[tsInd + 2]); //CycleToneStep1 point to interval1 step.
				toneStep_temp->NextToneStep = &(toneStepArray[tsInd + 2]); //Stop tone when cycle finished.

				/*normalInterval*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, normalInterval);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->NextToneStep = clicksStep_temp; //NextToneStep point to ramp up step.

				/*lastInterval.*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, lastClickInterval_temp);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->CycleNum1 = 1;
				toneStep_temp->CycleToneStep1 = clicksStep_temp;
				toneStep_temp->NextToneStep = clicksStep_temp; //Useless, this will reset at last step.
				break;

			case SWEEP_CLICKS_RAMP_2MS:
				clicksDur_temp = _tone.clicks_dur + 4; //ramp up and down cost 4ms.
				clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
				normalInterval = clicksPeriod_temp - clicksDur_temp;
				CycleNum1_temp = dur_temp / clicksPeriod_temp;
				lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;

				/*ramp up*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 0);
				clicksStep_temp = toneStep_temp;

				/*clicks duration*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, _tone.clicks_dur);

				/*ramp down*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 1);
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = (0 != normalInterval) ? &(toneStepArray[tsInd]) : clicksStep_temp; //CycleToneStep1 point to interval1 step.
				toneStep_temp->CycleNum2 = 1;
				toneStep_temp->CycleToneStep2 = (0 != lastClickInterval_temp) ? &(toneStepArray[tsInd + 1]) : &(toneStepArray[tsInd + 2]); //CycleToneStep1 point to interval1 step.
				toneStep_temp->NextToneStep = &(toneStepArray[tsInd + 2]); //Stop tone when cycle finished.

				/*normalInterval*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, normalInterval);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->NextToneStep = clicksStep_temp; //NextToneStep point to ramp up step.

				/*lastInterval.*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, lastClickInterval_temp);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->CycleNum1 = 1;
				toneStep_temp->CycleToneStep1 = clicksStep_temp;
				toneStep_temp->NextToneStep = clicksStep_temp; //Useless, this will reset at last step.
				break;

			case SWEEP_AM:
				// float AMperiod = (float)1 / (float)_tone.AMFrequncy * 1000; //miniseconds.
				double AMperiod = (double)1 / (double)_tone.AMFrequncy * 1000; //miniseconds.
				// float rawIntervalScale = AMperiod / 2 * 1000; //microseconds.
				double rawIntervalScale = AMperiod / 2 * 1000; //microseconds.
				// float minRawInt = MININT / rawIntervalScale;
				double minRawInt = MININT / rawIntervalScale;
				// uint32_t CycleNum1_temp = (float)dur_temp / AMperiod;
				uint32_t CycleNum1_temp = (double)dur_temp / AMperiod;
				// float AM_temp = 0;
				double AM_temp = 0;
				byte j = 0;
				for (byte i = 0; i < MaxAMArraySize; i++) {
					AM_temp = AM_temp + rawInterval[i];
					if ( AM_temp >= minRawInt) {
						AMInterval[j] = AM_temp * rawIntervalScale;
						AMVol[j] = 191 - i;
						j++;
						AM_temp = 0;
					}
				}
				if (0 != AM_temp) {
						AMInterval[j] = MININT;
						AMVol[j] = 0;
						j++;
						AM_temp = 0;
				}
				AMArraySize = j;
#ifdef DEBUG
				Serial.print("AMArraySize = ");
				Serial.println(AMArraySize);
				Serial.println("AMInterval[] = ");
				for (byte i = 0; i < j; i++) {
					Serial.print(AMInterval[i]);
					Serial.print(", ");
				}
				Serial.println();
				Serial.println("AMVol[] = ");
				for (byte i = 0; i < j; i++) {
					Serial.print(AMVol[i]);
					Serial.print(", ");
				}
				Serial.println();
				Serial.print("CycleNum1_temp = ");
				Serial.println(CycleNum1_temp);
#endif // DEBUG
				/*ramp up*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, AMInterval, AMVol, AMArraySize, _target_vol, 0);
				clicksStep_temp = toneStep_temp;

				/*ramp down*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, AMInterval, AMVol, AMArraySize, _target_vol, 1);
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = clicksStep_temp; //CycleToneStep1 point to interval1 step.
				break;
			}
		}
		/************step10 ramp down**************/
		switch (_tone.step_down_flag) {
		case STEP_FLAG_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), _target_vol, 1);
			break;

		case STEP_FLAG_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 1);
			break;

		case STEP_FLAG_CANCEL_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), curVol, 1);
			break;

		case STEP_FLAG_CANCEL_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), curVol, 1);
			break;
		}

		/************step11 close fq and vol step**************/
		if (dur_temp != 0) {
			prepareToneStep(&toneStep_temp, &tsInd);
			setOneLoopStep(toneStep_temp);

			toneStep_temp->fq.flag = TONESTEPDRT;
			toneStep_temp->fq.drtPar = 0;

			toneStep_temp->vol.flag = TONESTEPDRT;
			toneStep_temp->vol.drtPar = 0;
		}

		/************start interrupt**************/
		if (NULL != toneStep_temp)
		{
			curToneStep = &toneStepArray[0]; // Add first toneStepArray address to curToneStep.
			toneStep_temp->NextToneStep = NULL;
#ifdef DEBUG
			Serial.println("final tsInd = ");
			Serial.println(tsInd);
			Serial.println("curToneStep = ");
			Serial.println((uint32_t)curToneStep);

			for (int i = 0; i < tsInd; i++)
			{
				Serial.print("toneStep");
				Serial.println(i);
				printToneStep(&toneStepArray[i]);
			}
#endif // DEBUG
		}
#ifdef DEBUG
		else  Serial.println("NULL == toneStep_temp");
#endif // DEBUG
		// timer1.begin(timer1Func, MININT);
		_timer1_begin(timer1Func, MININT);
	}
}

SPI_TGMClass SPI_TGM;

