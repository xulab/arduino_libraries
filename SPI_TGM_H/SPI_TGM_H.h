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
#define TGM_VERSION 11
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
#define VOLUME_ARRAY 2
#define SWEEP_OFF 0
#define SWEEP_ON 1
#define SWEEP_LINEAR 2
#define SWEEP_EXP 3
#define SWEEP_ARRAY 4
#define SWEEP_NOISE_WHITE 5
#define SWEEP_NOISE_GAUSS1 6
#define SWEEP_NOISE_GAUSS2 7
#define SWEEP_NOISE_GAUSS3 8
#define SWEEP_CHORD 9
#define SWEEP_PEAK 10
#define SWEEP_CLICKS 11
#define SWEEP_CLICKS_RAMP_2MS 12
#define SWEEP_AM 13
#define SWEEP_ARRAY_ADDR 300
#define PREP_OFF 0
#define PREP_ON 1
#define PREP_ONOFF 2
#define TONE_FLAG_OFF 0
#define TONE_FLAG_ON 1
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

