/* version 12
 * modified in 20200707
 * only suit for teensy 3.6
 * change: Can control multiple TGM.
 * Reaction time: 700us.
 */

#ifndef _SPI_TGM_h
#define _SPI_TGM_h
#include "Arduino.h"
#include <SPI.h>
#include <CACHE.h>

#define TGM 255
#define UNO TGM
#define MEGA2560 1

#define info_string_size 50

#define Mega2560_CS 53
#define Mega2560_SCK 52
#define Mega2560_MO 51
#define Mega2560_MI 50
#define Mega2560_REQ 47
#define Mega2560_PER 48
#define Mega2560_INFO 49
#define Mega2560_WR 46

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
#define INIT_FQ 0
#define INIT_VOL 192 // gain = 0db
#define STEP_FLAG_OFF 0
#define STEP_FLAG_COS_5MS 1
#define STEP_FLAG_COS_2MS 2
#define STEP_FLAG_CANCEL_COS_5MS 3
#define STEP_FLAG_CANCEL_COS_2MS 4
#define MAX_CHORD_NUM 10
/*-----------tone-------------*/
/*-----------TGM_config-------------*/

typedef struct
{
	uint16_t nSize;
	byte version;
	char version_str[TGM_VERSION_STRING_SIZE];
} TGMinfo;

typedef struct
{
	uint16_t nSize;
	byte error_code;
	char error_string[ERROR_STRING_SIZE];
} TMGerror;

typedef struct
{
	uint16_t nSize;
	byte version;
	byte tone_flag;
	byte sweep;
	byte volume_mode; // use gaven volume
	byte volume;
	byte prepare_mode;
	byte step_up_flag;
	byte step_down_flag;
	uint16_t duration;
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
	uint16_t chord_num; // This should less than 10.
	// uint16_t sweep_int_us;
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
	uint32_t toneLoopMode; // 0: no loop or TGM decide; 1: use the parameter toneLoopTime.
	uint32_t toneLoopTime; // ms. every tone loop time.
	uint32_t reserve1;
	uint32_t reserve2;
	uint32_t reserve3;
	uint32_t reserve4;
	uint32_t reserve5;
	uint32_t reserve6;
	uint32_t reserve7;
	uint32_t reserve8;
} ton;

typedef struct
{
	uint16_t frequency;
	byte volume;
} fqinfo;

class SPI_TGMClass : public CACHE_Class
{
private:
	byte _board;
	ton _EMPTY_TONE;
	ton _QUICK_TONE;
	// inline void _read_info(TGMinfo * data);
	inline int _read_info(TGMinfo *data, long timeout = -1);
	// inline void _write_info(TGMinfo * data);
	inline int _write_info(TGMinfo *data, long timeout = -1);
	// inline void _read_error(TMGerror * data);
	inline int _read_error(TMGerror *data, long timeout = -1);
	// inline void _write_error(TMGerror * data);
	inline int _write_error(TMGerror *data, long timeout = -1);
	// inline void _read_tone(ton* data);
	inline int _read_tone(ton *data, long timeout = -1);
	// inline void _write_tone(ton* data);
	inline int _write_tone(ton *data, long timeout = -1);
	// inline void _erase_tone();
	inline int _erase_tone(long timeout = -1);
	inline void _set_empty_tone(ton *data);
	inline void _SPI_pin_clear();
	inline void SPI_TGMClass::_SPI_INIT();

public:
	TGMinfo info;
	TMGerror error;
	ton tone;
	fqinfo fq_info;
	// void init(byte boardtype);
	void init(byte boardtype, int req_pin = Mega2560_REQ, int per_pin = Mega2560_PER, int info_pin = Mega2560_INFO, int wr_pin = Mega2560_WR);
	// void write(uint32_t addr, uint16_t size, char *data);
	int write(uint32_t addr, uint16_t size, char *data, long timeout = -1);
	// void read(uint32_t addr, uint16_t size, char *data);
	// void quick_tone(uint32_t duration, uint32_t frequency, uint32_t pre_sound_delay = 0 );
	int quick_tone(uint32_t duration, uint32_t frequency, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_vol(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0);
	int quick_tone_vol(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_vol_cosramp_5ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0 );
	int quick_tone_vol_cosramp_5ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_vol_cosramp_2ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0);
	int quick_tone_vol_cosramp_2ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_clicks(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0);
	int quick_tone_clicks(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_clicks_cosramp_2ms(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0);
	int quick_tone_clicks_cosramp_2ms(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_tone_AM(uint32_t duration, uint32_t carrierFq, uint32_t AMFq, byte vol, uint32_t pre_sound_delay = 0);
	int quick_tone_AM(uint32_t duration, uint32_t carrierFq, uint32_t AMFq, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void wait_command();
int wait_command(long timeout = -1);
	// void read_tone();
int read_tone(long timeout = -1);
	void SPI_pin_clean();
	// void quick_sweep_linear_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0);
	int quick_sweep_linear_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_sweep_peak_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, uint32_t fq2, byte vol, uint32_t pre_sound_delay = 0);
	int quick_sweep_peak_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, uint32_t fq2, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_sweep_exp_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0);
	int quick_sweep_exp_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_noise_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, byte mode, uint32_t pre_sound_delay = 0);
	int quick_noise_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, byte mode, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void quick_chord_cosramp_5ms(uint32_t duration, uint32_t * fq, uint16_t fq_num, byte vol, uint32_t pre_sound_delay = 0);
	int quick_chord_cosramp_5ms(uint32_t duration, uint32_t *fq, uint16_t fq_num, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void tone_vol_rampup(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0);
	int tone_vol_rampup(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void tone_vol_rampdown(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0);
	int tone_vol_rampdown(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void set_tone_fq(uint32_t frequency, uint32_t pre_sound_delay = 0);
	int set_tone_fq(uint32_t frequency, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void set_tone_fq_vol(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0);
	int set_tone_fq_vol(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0, long timeout = -1);
	// void tone_cancel();
	int tone_cancel(long timeout = -1);
	// void tone_cancel_cosramp_5ms();
	int tone_cancel_cosramp_5ms(long timeout = -1);
	// void tone_cancel_cosramp_2ms();
	int tone_cancel_cosramp_2ms(long timeout = -1);
};

extern SPI_TGMClass SPI_TGM;

#endif
