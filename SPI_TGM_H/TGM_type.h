#ifndef _TGM_TYPE_H
#define _TGM_TYPE_H

// #ifdef ARDUINO_ARCH_ESP32
// #else
#pragma pack (1) //align to 1 byte.
// #endif

#define TONESTEPARRAYSIZE 12 

#define TONESTEPIDLE 0 //Idle mode. Do nothing.
#define TONESTEPDRT 1 //direct mode. ISI, Fq and Vol will be gaved in struct directly. 
#define TONESTEPARRAY 2 //ISI, Fq and Vol will be gaved in array. Loop number depend on uLoopNum. It would be a good choice to set the uLoopNum to array size.
#define TONESTEPARRAYREV 3 //ISI, Fq and Vol will be gaved in array. But the array will loop reversly.
#define TONESTEPFNC 4 //ISI, Fq and Vol will be gaved by a callback function. Loop number depend on uLoopNum. 

#define TGM_VERSION_STRING_SIZE info_string_size
#define ERROR_STRING_SIZE info_string_size

//typedef unsigned int uint16_t;
//typedef unsigned long uint32_t;
//typedef unsigned char byte;
typedef byte(*byte_fnc)();
typedef uint16_t(*uint16_fnc)();
typedef uint64_t(*uint64_t_fnc)();
typedef uint32_t(*uint32_fnc)();
typedef void(*void_fnc)();

struct stepTonePar {
	byte flag;
	// float drtPar; // 32bit unsigned int may compatible to any satuation.
	double drtPar; // double may compatible to any satuation.
	void * arrayPar; // pointer to a array. Transform it to a specific array type before usage, for instance "((byte *)arrayPar)[2]".
	uint16_t array_Size;
	void_fnc fnc; // Transform return data to the data type needed. For instance, "(*(byte_fnc)fnc)()"
};

struct toneStep {
	uint32_t LoopNum;
	uint32_t LoopInd;
	byte targetVol;

	stepTonePar ISI;
	stepTonePar fq;
	stepTonePar vol;
	uint32_t CycleNum1;
	toneStep * CycleToneStep1;
	uint32_t CycleNum2;
	toneStep * CycleToneStep2;
	toneStep * NextToneStep;
};

typedef struct {
	uint16_t nSize;
	byte version;
	char version_str[TGM_VERSION_STRING_SIZE];
} TGMinfo;


typedef struct {
	uint16_t nSize;
	byte error_code;
	char error_string[ERROR_STRING_SIZE];
}TMGerror;

typedef struct {
	uint16_t nSize;
	byte version;
	byte tone_flag;
	byte sweep;
	byte volume_mode; //use gaven volume
	byte volume;
	byte prepare_mode;
	byte step_up_flag;
	byte step_down_flag;
	uint16_t duration; //not used affter version 10.
	uint16_t frequency0;
	uint16_t frequency1;
	uint16_t frequency2;
	uint16_t frequency3;
	uint16_t frequency4;
	uint16_t frequency5;
	uint16_t frequency6;
	uint16_t frequency7;
	uint16_t frequency8;
	uint16_t frequency9;
	uint16_t fq_step;
	float sweep_base;
	uint16_t chord_num; //This should less than 10.
	uint32_t durationL;
	uint32_t pre_sound_delay;
	uint32_t frequencyL0;
	uint32_t frequencyL1;
	uint32_t frequencyL2;
	uint32_t frequencyL3;
	uint32_t frequencyL4;
	uint32_t frequencyL5;
	uint32_t frequencyL6;
	uint32_t frequencyL7;
	uint32_t frequencyL8;
	uint32_t frequencyL9;
	uint32_t clicks_dur;
	uint32_t clicks_period;
	uint32_t AMFrequncy;
	uint32_t toneLoopMode; //0: no loop or TGM decide; 1: use the parameter toneLoopTime. 
	uint32_t toneLoopTime; // ms. every tone loop time.
  uint32_t reserve1;
  uint32_t reserve2;
  uint32_t reserve3;
  uint32_t reserve4;
  uint32_t reserve5;
  uint32_t reserve6;
  uint32_t reserve7;
  uint32_t reserve8;
}ton;

#endif
