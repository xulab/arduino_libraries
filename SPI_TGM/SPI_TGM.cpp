/* version 12
 * modified in 20200707
 * only suit for teensy 3.6
 * change: Can control multiple TGM.
 * Reaction time: 700us.
 */

#include <SPI_TGM.h>

#pragma pack(1) // align to 1 byte.

#define MINITERVAL 500 * 0.0625 // 25us
#define RAWISISIZE 192
uint16_t MAXCLOCKS = 0xFFFF; // 25us

void SPI_TGMClass::init(byte boardtype)
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

	if (this == &SPI_TGM)
	{
		REQ = Mega2560_REQ;
		PER = Mega2560_PER;
	}
	else if (this == &SPI_TGM2)
	{
		REQ = Mega2560_REQ2;
		PER = Mega2560_PER2;
	}
	INFO = Mega2560_INFO;
	WR = Mega2560_WR;
	CACHE_Class::init(MEGA2560);
}

inline int SPI_TGMClass::_read_info(TGMinfo *data, long timeout)
{
	// q_read(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data);
	return q_read(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data, timeout);
}

inline int SPI_TGMClass::_write_info(TGMinfo *data, long timeout)
{
	// q_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data);
	return q_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data, timeout);
}

inline int SPI_TGMClass::_read_error(TMGerror *data, long timeout)
{
	// q_read(ERROR_ADDR, sizeof(TMGerror), (char *)data);
	return q_read(ERROR_ADDR, sizeof(TMGerror), (char *)data, timeout);
}

inline int SPI_TGMClass::_write_error(TMGerror *data, long timeout)
{
	// q_write(ERROR_ADDR, sizeof(TMGerror), (char *)data);
	return q_write(ERROR_ADDR, sizeof(TMGerror), (char *)data, timeout);
}

inline int SPI_TGMClass::_read_tone(ton *data, long timeout)
{
	// q_read(TONE_ADDR, sizeof(ton), (char *)data);
	return q_read(TGM_INFO_ADDR, sizeof(TGMinfo), (char *)data, timeout);
}

inline int SPI_TGMClass::_write_tone(ton *data, long timeout)
{
	return q_write(TONE_ADDR, sizeof(ton), (char *)data, timeout);
}

inline int SPI_TGMClass::_erase_tone(long timeout)
{
	return q_write(TONE_ADDR, sizeof(ton), (char *)&_EMPTY_TONE, timeout);
}

inline void SPI_TGMClass::_set_empty_tone(ton *data)
{
	memmove(data, &_EMPTY_TONE, sizeof(ton));
}

int SPI_TGMClass::write(uint32_t addr, uint16_t size, char *data, long timeout)
{
	return q_write(addr, size, data, timeout);
}

// int SPI_TGMClass::read(uint32_t addr, uint16_t size, char *data)
// {
// 	read(addr, size, data);
// }

int SPI_TGMClass::quick_tone(uint32_t duration, uint32_t frequency, uint32_t pre_sound_delay, long timeout)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.durationL = duration;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_vol(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_vol_cosramp_5ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_vol_cosramp_2ms(uint32_t duration, uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_clicks(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_clicks_cosramp_2ms(uint32_t duration, uint32_t carrierFq, uint32_t clicksDur, uint32_t clicksPeriod, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_tone_AM(uint32_t duration, uint32_t carrierFq, uint32_t AMFq, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::tone_vol_rampup(uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::tone_vol_rampdown(uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::set_tone_fq(uint32_t frequency, uint32_t pre_sound_delay, long timeout)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.pre_sound_delay = pre_sound_delay;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.frequencyL0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_OFF;
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::tone_cancel(long timeout)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = 0;
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::tone_cancel_cosramp_5ms(long timeout)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.step_down_flag = STEP_FLAG_CANCEL_COS_5MS;
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::tone_cancel_cosramp_2ms(long timeout)
{
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.version = TGM_VERSION;
	_QUICK_TONE.duration = 0;
	_QUICK_TONE.durationL = 0;
	_QUICK_TONE.step_down_flag = STEP_FLAG_CANCEL_COS_2MS;
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::set_tone_fq_vol(uint32_t frequency, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_sweep_linear_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_sweep_exp_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_sweep_peak_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, uint32_t fq2, byte vol, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_noise_cosramp_5ms(uint32_t duration, uint32_t fq0, uint32_t fq1, byte vol, byte mode, uint32_t pre_sound_delay, long timeout)
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
	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::quick_chord_cosramp_5ms(uint32_t duration, uint32_t *fq, uint16_t fq_num, byte vol, uint32_t pre_sound_delay, long timeout)
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

	int res = _write_tone(&_QUICK_TONE, timeout);
	_set_empty_tone(&_QUICK_TONE);
	return res;
}

int SPI_TGMClass::read_tone(long timeout)
{
	return q_read(TONE_ADDR, sizeof(ton), (char *)&tone, timeout);
}

SPI_TGMClass SPI_TGM;
SPI_TGMClass SPI_TGM2;
