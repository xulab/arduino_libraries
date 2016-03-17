// SPI_TGM.h
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef _SPI_TGM_H_H
#define _SPI_TGM_H_H
#include "Arduino.h"
#ifndef NOP()
#define NOP() __asm__ __volatile__ ("nop")
#endif

#define TGM 255
#define UNO TGM
#define MEGA2560 1


#define info_string_size 50


/*-----------TGM_config-------------*/
/*-----------version-------------*/
#define TGM_VERSION 1
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
#define SWEEP_ARRAY_ADDR 300
#define PREP_OFF 0
#define PREP_ON 1
#define PREP_ONOFF 2
#define TONE_FLAG_OFF 0
#define TONE_FLAG_ON 1
#define INIT_FQ 0
#define INIT_VOL 192 //gain = 0db
#define STEP_FLAG_OFF 0
#define STEP_FLAG_COS_5MS 1
#define STEP_FLAG_COS_2MS 2
#define MAX_CHORD_NUM 10
/*-----------tone-------------*/
/*-----------TGM_config-------------*/

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
	uint16_t chord_num;
} ton;

typedef struct{
	uint16_t frequency;
	byte volume;
}fqinfo;

class SPI_TGMClass
{
private:
	byte _board;
	ton _EMPTY_TONE;
	ton _QUICK_TONE;

	inline void _TGM_init_error();
	inline void _TGM_init_ver();
	inline void _read_info(TGMinfo * data);
	inline void _write_info(TGMinfo * data);
	inline void _read_error(TMGerror * data);
	inline void _write_error(TMGerror * data);
	inline void _read_tone(ton* data);
	inline void _write_tone(ton* data);
	inline void _set_empty_tone(ton* data);
	inline void _erase_tone();
	inline void _step_down_vol();
	inline void timer1_delay();
	inline void _SPI_pin_clear();

public:

	TGMinfo info;
	TMGerror error;
	ton tone;
	fqinfo fq_info;
	void init(byte boardtype);
	void init_TGM();
	void write(uint32_t addr, uint16_t size, char *data);
	void read(uint32_t addr, uint16_t size, char *data);
	void quick_tone(uint16_t duration, uint16_t frequency);
	void quick_tone_vol(uint16_t duration, uint16_t frequency, byte vol);
	void quick_tone_vol_cosramp_5ms(uint16_t duration, uint16_t frequency, byte vol);
	void quick_tone_vol_cosramp_2ms(uint16_t duration, uint16_t frequency, byte vol);
	void wait_command();
	void set_tone();
	void set_fq(uint32_t fq);
	void set_vol(byte vol);
	void read_tone();
	void test_step();
	void step_vol_up(byte t_vol);
	void step_vol_down(byte t_vol);
	void step_vol_up_2ms(byte t_vol);
	void step_vol_down_2ms(byte t_vol);
	void delay_ms(uint16_t data);
	void SPI_pin_clean();
	void vol_chip_reset();
	void quick_sweep_linear_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol);
	void quick_sweep_exp_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol);
	void quick_sweep_peak_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, uint16_t fq2, byte vol);
	void quick_noise_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol, byte mode);
	void quick_chord_cosramp_5ms(uint16_t duration, uint16_t * fq, uint16_t fq_num, byte vol);
};

extern SPI_TGMClass SPI_TGM;

#endif

