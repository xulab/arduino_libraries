/* version 06
 * modified in 2014-11-17
 * change: add a new function peak tone.
 */
#include <math.h>
#include <SPI_TGM.h>
#include <SPI.h>
#ifdef watchdog
#include <avr/wdt.h>
#endif


/*-----------23LC1024------------*/
/*-----------23LC1024------------*/
#define Byte_mode B00000000
#define Page_mode B10000000
#define Seq_mode B01000000
#define Reserved_mode B11000000

#define read_code 0x03
#define wr_code 0x02
#define EDIO 0x3B //Enter Dual I/O access
#define EQIO 0x38 //Enter Quad I/O access
#define RSTIO 0xFF //Reset Dual and Quad I/O access
#define RDMR 0x05 //Read Mode Register
#define WRMR 0x01 //Write Mode Register

#define Mega2560_CS 53
#define Mega2560_SCK 52
#define Mega2560_MO 51
#define Mega2560_MI 50
#define Mega2560_REQ 47 
#define Mega2560_H_BUSY 48 
#define Mega2560_G_BUSY 49 

inline static void _SPI_INIT(){
	pinMode(Mega2560_CS, OUTPUT);
	pinMode(Mega2560_SCK , OUTPUT);
	pinMode(Mega2560_MO , OUTPUT);
	pinMode(Mega2560_MI , INPUT);
	pinMode(Mega2560_REQ , OUTPUT);
	pinMode(Mega2560_H_BUSY, INPUT);
	pinMode(Mega2560_G_BUSY, INPUT);

	digitalWrite(Mega2560_CS, HIGH);
	digitalWrite(Mega2560_SCK, LOW);
	digitalWrite(Mega2560_MO , LOW);
	digitalWrite(Mega2560_REQ , LOW);
}


inline static void _REQ_REQUEST(){
	digitalWrite(Mega2560_REQ, HIGH);
}

inline static void _REQ_RELEASE(){
	digitalWrite(Mega2560_REQ, LOW);
}

inline static uint8_t _REQ_GET_H_BUSY(){
	return digitalRead(Mega2560_H_BUSY);
}

inline static uint8_t _REQ_GET_G_BUSY(){
	return digitalRead(Mega2560_G_BUSY);
}

inline static void _GET_PERMISSION(){
	_REQ_REQUEST();
	NOP();
	NOP();
	while(LOW == _REQ_GET_H_BUSY());
}

inline static void _23lc1024_SPI_begin(){
	_GET_PERMISSION();
	//SPI.begin();
	digitalWrite(Mega2560_CS, LOW);
}

inline static void _23lc1024_SPI_end(){
	digitalWrite(Mega2560_CS, HIGH);
	//SPI.end();
	_REQ_RELEASE();
}

//inline static void _23lc1024_reset(){
//	_SPI_REQUEST();
//	_SPI_Mask = _23lc1024_SPI_Mask;
//	_CS = _23lc1024_CS;
//
//	NOP();
//	_SPI_port |= (_MOSI|_MISO);
//	_SPI_port &= ~_SCK;
//	_SPI_port |= _CS;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port &= ~_CS;
//	NOP();
//	NOP();
//	_SPI_port |= _CS;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port |= _SCK;
//	_SPI_port &= ~_SCK;
//	_SPI_port &= ~_CS;
//
//	_SPI_Mask = _temp_SPI_Mask;
//	_CS = _temp_CS;
//	_SPI_REQ_RELEASE();
//}



//inline static void _23lc1024_set(byte mode){
//	_23lc1024_SPI_begin();
//	_SPI_transfer(WRMR);
//	_SPI_transfer(mode);
//	_23lc1024_SPI_end();
//}

inline static void _23lc1024_write(uint32_t addr, uint16_t size, char *data){
	_23lc1024_SPI_begin();
	SPI.transfer(wr_code);
	SPI.transfer(addr>>16);
	SPI.transfer(addr>>8);
	SPI.transfer(addr);
	for(uint16_t i = 0; i<size; i++){
		SPI.transfer(data[i]);
	}
	_23lc1024_SPI_end();
}

inline static void _23lc1024_read(uint32_t addr, uint16_t size, char *data){
	_23lc1024_SPI_begin();
	SPI.transfer(read_code);
	SPI.transfer(addr>>16);
	SPI.transfer(addr>>8);
	SPI.transfer(addr);
	for(uint16_t i = 0; i < size; i++){
		data[i] = SPI.transfer(0);
	}
	_23lc1024_SPI_end();
}
/*-----------23LC1024-------------*/
/*-----------23LC1024------------*/





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
	_SPI_INIT();
	SPI.setDataMode(SPI_MODE0); 
	SPI.setClockDivider(SPI_CLOCK_DIV4); //half speed, if the wire is too long may set this to SPI_CLOCK_DIV8
	SPI.begin();
}


inline void SPI_TGMClass::_read_info(TGMinfo* data){
	_23lc1024_read(TGM_INFO_ADDR, sizeof(TGMinfo), (char*)data);
}

inline void SPI_TGMClass::_write_info(TGMinfo* data){
	_23lc1024_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char*)data);
}

inline void SPI_TGMClass::_read_error(TMGerror* data){
	_23lc1024_read(ERROR_ADDR, sizeof(TMGerror), (char*)data);
}

inline void SPI_TGMClass::_write_error(TMGerror* data){
	_23lc1024_write(ERROR_ADDR, sizeof(TMGerror), (char*)data);
}

inline void SPI_TGMClass::_read_tone(ton* data){
	_23lc1024_read(TONE_ADDR, sizeof(ton), (char*)data);
}

inline void SPI_TGMClass::_write_tone(ton * data){
	_23lc1024_write(TONE_ADDR, sizeof(ton), (char*)data);
}

inline void SPI_TGMClass::_erase_tone(){
	_23lc1024_write(TONE_ADDR, sizeof(ton), (char*)&_EMPTY_TONE);
}

inline void SPI_TGMClass::_set_empty_tone(ton* data){
	memmove(data, &_EMPTY_TONE, sizeof(ton));
}


void SPI_TGMClass::write(uint32_t addr, uint16_t size, char *data){
	_23lc1024_write(addr, size, data);
}



void SPI_TGMClass::read(uint32_t addr, uint16_t size, char *data){
	_23lc1024_read(addr, size, data);
}


void SPI_TGMClass::quick_tone(uint16_t duration, uint16_t frequency){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = frequency;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_vol(uint16_t duration, uint16_t frequency, byte vol){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_tone_vol_cosramp_5ms(uint16_t duration, uint16_t frequency, byte vol){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = frequency;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}


void SPI_TGMClass::quick_sweep_linear_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_LINEAR;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}


void SPI_TGMClass::quick_sweep_exp_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	// _QUICK_TONE.sweep_base = base;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_EXP;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}


void SPI_TGMClass::quick_sweep_peak_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, uint16_t fq2, byte vol){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.frequency2 = fq2;
	// _QUICK_TONE.sweep_base = base;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_PEAK;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}


void SPI_TGMClass::quick_noise_cosramp_5ms(uint16_t duration, uint16_t fq0, uint16_t fq1, byte vol, byte mode){
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.frequency0 = fq0;
	_QUICK_TONE.frequency1 = fq1;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = mode;
	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}

void SPI_TGMClass::quick_chord_cosramp_5ms(uint16_t duration, uint16_t * fq, uint16_t fq_num, byte vol){
	int i=0;
	if(i<fq_num){
		_QUICK_TONE.frequency0 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency1 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency2 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency3 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency4 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency5 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency6 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency7 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency8 = fq[i];
		i++;
	}
	if(i<fq_num){
		_QUICK_TONE.frequency9 = fq[i];
		i++;
	}
	_QUICK_TONE.chord_num = fq_num;
	_QUICK_TONE.tone_flag = TONE_FLAG_ON;
	_QUICK_TONE.duration = duration;
	_QUICK_TONE.volume_mode = VOLUME_ON;
	_QUICK_TONE.volume = vol;
	_QUICK_TONE.step_up_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.step_down_flag = STEP_FLAG_COS_5MS;
	_QUICK_TONE.sweep = SWEEP_CHORD;

	_write_tone(&_QUICK_TONE);
	_set_empty_tone(&_QUICK_TONE);
}


void SPI_TGMClass::read_tone(){
	read(TONE_ADDR, sizeof(ton), (char*)&tone);
}




SPI_TGMClass SPI_TGM;

