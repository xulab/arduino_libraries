/* version 12
 * modified in 20200707
 * only suit for teensy 3.6
 * change: Can control multiple TGM.
 * Reaction time: 700us.
 */

#include <SPI_TGM.h>

#pragma pack (1) //align to 1 byte.


#define MINITERVAL 500*0.0625 //25us
#define RAWISISIZE 192
uint16_t MAXCLOCKS=0xFFFF; //25us

inline static void _SPI_INIT()
{
	CACHE.INFO = Mega2560_INFO;
	CACHE.REQ = Mega2560_REQ;
	CACHE.WR = Mega2560_WR;
	CACHE.PER = Mega2560_PER;
	CACHE.init(MEGA2560);
}

void SPI_TGMClass::init(byte boardtype, int req_pin, int per_pin, int info_pin, int wr_pin)
{
	memset(&info, 0, sizeof(TGMinfo));
	info.nSize = sizeof(TGMinfo);

	memset(&error, 0, sizeof(TMGerror));
	error.nSize = sizeof(TMGerror);

	memset(&tone, 0, sizeof(ton));
	tone.nSize = sizeof(ton);
	tone.version = TGM_VERSION;

	memset(&_EMPTY_TONE, 0, sizeof(ton));
	_EMPTY_TONE.nSize = sizeof(ton);
	_EMPTY_TONE.version = TGM_VERSION;

	memset(&_QUICK_TONE, 0, sizeof(ton));
	_QUICK_TONE.nSize = sizeof(ton);
	_QUICK_TONE.version = TGM_VERSION;

	memset(&fq_info, 0, sizeof(fq_info));
	// _SPI_INIT();
	// INFO = Mega2560_INFO;
	// REQ = Mega2560_REQ;
	// WR = Mega2560_WR;
	// PER = Mega2560_PER;
	INFO = info_pin;
	REQ = req_pin;
	WR = wr_pin;
	PER = per_pin;
	CACHE_Class::init(MEGA2560);
}

inline void SPI_TGMClass::_read_info(TGMinfo *data)
{
	q_read(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data);
}

inline void SPI_TGMClass::_write_info(TGMinfo *data)
{
	q_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data);
}

inline void SPI_TGMClass::_read_error(TMGerror *data)
{
	q_read(ERROR_ADDR, sizeof(TMGerror), (char *)data);
}

inline void SPI_TGMClass::_write_error(TMGerror *data)
{
	q_write(ERROR_ADDR, sizeof(TMGerror), (char *)data);
}

inline void SPI_TGMClass::_read_tone(ton *data)
{
	q_read(TONE_ADDR, sizeof(ton), (char *)data);
}

inline void SPI_TGMClass::_write_tone(ton *data)
{
	q_write(TONE_ADDR, sizeof(ton), (char *)data);
}

inline void SPI_TGMClass::_erase_tone()
{
	q_write(TONE_ADDR, sizeof(ton), (char *)&_EMPTY_TONE);
}

inline void SPI_TGMClass::_set_empty_tone(ton *data)
{
	memmove(data, &_EMPTY_TONE, sizeof(ton));
}

void SPI_TGMClass::write(uint32_t addr, uint16_t size, char *data)
{
	q_write(addr, size, data);
}

// void SPI_TGMClass::read(uint32_t addr, uint16_t size, char *data)
// {
// 	read(addr, size, data);
// }

void SPI_TGMClass::quick_tone(uint32_t duration, uint32_t frequency, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_vol(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_vol_cosramp_5ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_vol_cosramp_2ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_2MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_2MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_clicks(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = carrierFq;
	_QUICK_TONE.frequencyL0 = carrierFq;
	// _QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.sweep = SWEEP_CLICKS;
	_QUICK_TONE.clicks_dur = clicksDur;
	_QUICK_TONE.clicks_period = clicksPeriod;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_clicks_cosramp_2ms(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = carrierFq;
	_QUICK_TONE.frequencyL0 = carrierFq;
	//_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.sweep = SWEEP_CLICKS_RAMP_2MS;
	_QUICK_TONE.clicks_dur = clicksDur;
	_QUICK_TONE.clicks_period = clicksPeriod;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_AM(uint32_t duration, uint32_t carrierFq, uint32_t AMFq, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = carrierFq;
	_QUICK_TONE.frequencyL0 = carrierFq;
	_QUICK_TONE.AMFrequncy = AMFq;
	//_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.sweep = SWEEP_AM;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::tone_vol_rampup(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::tone_vol_rampdown(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::set_tone_fq(uint32_t frequency, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_OFF;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::tone_cancel()
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = 0;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::tone_cancel_cosramp_5ms()
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.step_down_flag = STEP_FLAG_CANCEL_COS_5MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::tone_cancel_cosramp_2ms()
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.step_down_flag = STEP_FLAG_CANCEL_COS_2MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::set_tone_fq_vol(uint32_t frequency, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_sweep_linear_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequencyL0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.frequencyL1 = fq1;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_LINEAR;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_sweep_exp_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequencyL0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.frequencyL1 = fq1;
	// _QUICK_TONE.sweep_base = base;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_EXP;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_sweep_peak_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, uint32_t fq2, byte vol, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequencyL0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.frequencyL1 = fq1;
	_QUICK_TONE.frequency2 = fq2;
	_QUICK_TONE.frequencyL2 = fq2;
	// _QUICK_TONE.sweep_base = base;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_PEAK;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_noise_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, byte mode, uint32_t pre_sound_delay = 0)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequencyL0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.frequencyL1 = fq1;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = mode;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_chord_cosramp_5ms(uint32_t duration, uint32_t *fq, uint16_t fq_num, byte vol, uint32_t pre_sound_delay = 0)
{
	int i = 0;
	if (i < fq_num)
	{
		_QUICK_TONE.frequency0 = fq[i];
		_QUICK_TONE.frequencyL0 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency1 = fq[i];
		_QUICK_TONE.frequencyL1 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency2 = fq[i];
		_QUICK_TONE.frequencyL2 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency3 = fq[i];
		_QUICK_TONE.frequencyL3 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency4 = fq[i];
		_QUICK_TONE.frequencyL4 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency5 = fq[i];
		_QUICK_TONE.frequencyL5 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency6 = fq[i];
		_QUICK_TONE.frequencyL6 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency7 = fq[i];
		_QUICK_TONE.frequencyL7 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency8 = fq[i];
		_QUICK_TONE.frequencyL8 = fq[i];
		i++;
	}
	if (i < fq_num)
	{
		_QUICK_TONE.frequency9 = fq[i];
		_QUICK_TONE.frequencyL9 = fq[i];
		i++;
	}
	_QUICK_TONE.chord_num = fq_num;
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_CHORD;

	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::read_tone()
{
	read(TONE_ADDR, sizeof(ton), (char *)&tone);
}

SPI_TGMClass SPI_TGM;
