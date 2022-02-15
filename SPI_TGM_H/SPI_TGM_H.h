/* version 12
 * modified in 20210721
 * add fucntions suit for TGM_NI_Ver01
 * change: can specify the tone loop time now.
 */
/* version 11
 * modified in 20181129
 * only suit for teensy 3.6
 * change: add pre_sound_delay.
 * change: add clicks.
 * change: add amplitude modulation.
 * Reaction time: 700us.
 */


#ifndef _SPI_TGM_H_H
#define _SPI_TGM_H_H
#include "Arduino.h"

#define info_string_size 50

/*-----------TGM_config-------------*/
/*-----------version-------------*/
#define TGM_VERSION 12
#define TGM_INFO_ADDR 0
#define TGM_VERSION_STRING_SIZE info_string_size
/*-----------version-------------*/

/*-----------error-------------*/
#define ERROR_ADDR 100
#define ERROR_STRING_SIZE info_string_size
#define NO_ERROR 0
/*-----------error-------------*/

/*-----------tone-------------*/
#define TONE_ADDR 200
#define VOLUME_OFF 0
#define VOLUME_ON 1
#define VOLUME_ARRAY 2 //not used, reserved parameter.
#define SWEEP_OFF 0 //pure tone, frequencyL0 is the tone frequency.
#define SWEEP_ON 1 //not used, reserved parameter.
#define SWEEP_LINEAR 2 //swept sine wave, the frequency changes linearly, frequencyL0 is start frequency, frequencyL1 is the end frequency.
#define SWEEP_EXP 3 //swept sine wave, the frequency changes exponentially, frequencyL0 is start frequency, frequencyL1 is the end frequency .
#define SWEEP_ARRAY 4 //not used, reserved parameter.
#define SWEEP_NOISE_WHITE 5 //On TGM3.2 it is a pesudo noise. On TGM_NI is true white noise. frequencyL0 is the low cutoff freq, frequencyL1 is high cutoff freq.
#define SWEEP_NOISE_GAUSS1 6 //Only works on TGM_NI. On TGM_NI is true white noise. frequencyL0 is the low cutoff freq, frequencyL1 is high cutoff freq.
#define SWEEP_NOISE_GAUSS2 7 //not used, reserved parameter.
#define SWEEP_NOISE_GAUSS3 8 //not used, reserved parameter.
#define SWEEP_CHORD 9 //chord_num indicate the chord number. frequencyL0~9 indicate the frequency of single tone.
#define SWEEP_PEAK 10
#define SWEEP_CLICKS 11
#define SWEEP_CLICKS_RAMP_2MS 12
#define SWEEP_AM 13
#define SWEEP_WAV_FILE 14 //play wav file on labview TGM, frequencyL0 is the file index.
#define SWEEP_ARRAY_ADDR 300
#define PREP_OFF 0
#define PREP_ON 1
#define PREP_ONOFF 2
#define TONE_FLAG_OFF 0
#define TONE_FLAG_ON 1
#define TONE_LOOP_OFF 0 //0: no loop or TGM decide; 1: use the parameter toneLoopTime.
#define TONE_LOOP_ON 1
#define INIT_FQ 0  //Close DDS
#define INIT_VOL 0 //Minimun volume
#define STEP_FLAG_OFF 0
#define STEP_FLAG_COS_5MS 1
#define STEP_FLAG_COS_2MS 2
#define STEP_FLAG_CANCEL_COS_5MS 3
#define STEP_FLAG_CANCEL_COS_2MS 4
#define MAX_CHORD_NUM 10
/*-----------tone-------------*/
/*-----------TGM_config-------------*/


class SPI_TGMClass
{
private:

public:

	void init();
	void init_TGM();
	void wait_command();
	void set_tone();
	void set_fq(uint32_t fq);
	void set_vol(byte vol);
	void read_tone();
	void testTone();
};

extern SPI_TGMClass SPI_TGM;

#endif

