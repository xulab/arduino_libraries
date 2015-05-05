/* version 06
 * modified in 2014-11-17
 * change: add a new function peak tone.
 */
#include <math.h>
#include <SPI_TGM_H.h>
#include <SPI.h>
#ifdef watchdog
#include <avr/wdt.h>
#endif


/*-----------timer1-------------*/
#define TIMER1_CLOCK_DIV8 _BV(CS11) //CS12=0,CS11=1,CS10=0
/*-----------timer1-------------*/

/*-----------TGM_config-------------*/
const char VERSION_STRING[] = "VER001_20140211_PJW.";
/*-----------error-------------*/
const char noerror_string[] = "no error";
const char error_string1[] = "no error";
char temp_error_string[ERROR_STRING_SIZE] = "";
/*-----------error-------------*/
/*-----------TGM_config-------------*/


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

#define TGM_CS 10 
#define TGM_SCK 13 
#define TGM_MO 11 
#define TGM_MI 12 
#define TGM_REQ _BV(PORTC4) 
#define TGM_H_BUSY 8 
#define TGM_G_BUSY _BV(PORTC3)


inline static void _SPI_INIT(){
	pinMode(TGM_CS, OUTPUT);
	pinMode(TGM_SCK , OUTPUT);
	pinMode(TGM_MO , OUTPUT);
	pinMode(TGM_MI , INPUT);

	DDRC |= TGM_REQ;
	pinMode(TGM_H_BUSY, INPUT);
	DDRC &= ~TGM_G_BUSY;

	digitalWrite(TGM_CS, HIGH);
	digitalWrite(TGM_SCK, LOW);
	digitalWrite(TGM_MO , LOW);
	PORTC &= ~TGM_REQ;
}


inline static void _REQ_REQUEST(){
	PORTC |= TGM_REQ;
}

inline static void _REQ_RELEASE(){
	PORTC &= ~TGM_REQ;
}

inline static uint8_t _REQ_GET_H_BUSY(){
	return digitalRead(TGM_H_BUSY);
}

inline static uint8_t _REQ_GET_G_BUSY(){
	return (PINC & TGM_G_BUSY) == TGM_G_BUSY;
}

inline static void _GET_PERMISSION(){
	_REQ_REQUEST();
	NOP();
	NOP();
	while(LOW == _REQ_GET_H_BUSY());
}

inline static void _23lc1024_SPI_begin(){
	_GET_PERMISSION();
	digitalWrite(TGM_CS, LOW);
}

inline static void _23lc1024_SPI_end(){
	digitalWrite(TGM_CS, HIGH);
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



inline static void _23lc1024_set(byte mode){
	_23lc1024_SPI_begin();
	SPI.transfer(WRMR);
	SPI.transfer(mode);
	_23lc1024_SPI_end();
}

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



/*--------AD9850-------*/
/*--------AD9850-------*/
#define AD9850_clock (double)125000000 //125MHz
const double x = (double)4294967295/AD9850_clock;
#define fq_convert(f) (uint32_t)((double)f*x)
#define AD9850_w_clk _BV(PORTC0)// PC0,ArPort01
#define AD9850_fq_up _BV(PORTC1)//PC1,ArPort02
#define AD9850_rest  _BV(PORTC2)//PC2,ArPort03
#define AD9850_mask (AD9850_w_clk | AD9850_fq_up | AD9850_rest)
#define AD9850_para_port PORTC
#define AD9850_para_state DDRC
#define AD9850_data_port PORTD
#define AD9850_data_state DDRD

inline static void _ad9850_init_port(){
	AD9850_para_state |= AD9850_mask;
	NOP();
	AD9850_para_port &= ~AD9850_mask;
	AD9850_data_state = 0xff;
	NOP();
	AD9850_data_port  = 0;
}

inline static void _ad9850_reset(){
	AD9850_para_port &= ~AD9850_mask;
	AD9850_para_port |= AD9850_rest;
	AD9850_para_port &= ~AD9850_mask;
}

inline static void _ad9850_init(){
	_ad9850_init_port();
	_ad9850_reset();
}

inline static void _ad9850_wr_parallel(uint32_t fq){
	uint8_t * data = (uint8_t *)&fq;
	//Ð´w0Êý¾Ý
	AD9850_data_port = 0;//set phase = 0
	AD9850_para_port |= AD9850_w_clk;
	AD9850_para_port &= ~AD9850_w_clk;
	//Ð´w1Êý¾Ý
	AD9850_data_port = data[3];     //w1
	AD9850_para_port |= AD9850_w_clk;
	AD9850_para_port &= ~AD9850_w_clk;
	//Ð´w2Êý¾Ý
	AD9850_data_port = data[2];     //w2
	AD9850_para_port |= AD9850_w_clk;
	AD9850_para_port &= ~AD9850_w_clk;
	//Ð´w3Êý¾Ý
	AD9850_data_port = data[1];     //w3
	AD9850_para_port |= AD9850_w_clk;
	AD9850_para_port &= ~AD9850_w_clk;
	//Ð´w4Êý¾Ý
	AD9850_data_port = data[0];     //w4
	AD9850_para_port |= AD9850_w_clk;
	AD9850_para_port &= ~AD9850_w_clk;
	//ÒÆÈëÊ¼ÄÜ
	AD9850_para_port |= AD9850_fq_up;
	AD9850_para_port &= ~AD9850_fq_up;
	AD9850_data_port = 0;
}


inline static void _set_fq(uint32_t fq){
	_ad9850_wr_parallel(fq_convert(fq));
}
/*--------AD9850-------*/
/*--------AD9850-------*/




/*--------PGA2310-------*/
/*--------PGA2310-------*/
#define PGA2310_cs _BV(PORTB1) // PB1,ArPort09
#define PGA2310_mute _BV(PORTC5) // PC5,ArPort A5
#define PGA2310_mute_port PORTC
#define PGA2310_mute_state DDRC
#define PGA2310_port PORTB
#define PGA2310_state DDRB
#define _set_vol(vol) _pga2310_wr(vol,vol)


inline static void _pga2310_init(){

	PGA2310_mute_state |= PGA2310_mute;
	NOP();
	PGA2310_mute_port |= PGA2310_mute;


	PGA2310_state |= PGA2310_cs;
	NOP();
	PGA2310_port |= PGA2310_cs;
}

inline static void _pga2310_wr(byte LGain, byte RGain){
	//_SPI_begin();
	PGA2310_port &= ~PGA2310_cs;
	SPI.transfer(LGain);//left channel
	SPI.transfer(RGain);//right channel
	PGA2310_port |= PGA2310_cs;
	//_SPI_end();
}
/*--------PGA2310-------*/
/*--------PGA2310-------*/



/*--------timer1-------*/
/*--------timer1-------*/
#define step_size sizeof(vol_step_val_5ms_cos)
#define TIMER1_STOP 0
#define TIMER1_STEP_MODE 1
#define TIMER1_DELAY_MODE 2
#define TIMER1_STEP_UP 3
#define TIMER1_STEP_DOWN 4
#define TIMER1_SWEEP_LINEAR 5
#define TIMER1_SWEEP_EXP 6
#define TIMER1_NOISE 7
#define TIMER1_CHORD 8
#define _SWEEP_INTERVAL_ 500 //change fq interval(us)
#define PRE_SCALE_0DIV _BV(CS10) //0.0625us
#define PRE_SCALE_64DIV (_BV(CS10) | _BV(CS11))
#define PRE_SCALE_64DIV_MUTIPLE 250
#define PRE_SCALE_MASK (_BV(CS10) | _BV(CS11) | _BV(CS12))

byte flag_vol_i = 0;
volatile static byte _target_vol = 192;
volatile static byte timer1_mode = TIMER1_STOP;
volatile static byte step_index = 0;
volatile static uint32_t delay_time = 0;
volatile static uint32_t sweep_index = 0;
volatile static uint32_t sweep_index_size = 0;
volatile static double sweep_linear_step = 0;
volatile static float sweep_base = 0;
volatile static uint16_t chord_fq_array[MAX_CHORD_NUM];
volatile static uint16_t chord_fq_num = 0;
uint32_t sweep_fq0 = 0;
uint32_t sweep_fq1 = 0;
uint32_t sweep_fq2 = 0;
byte sweep_mode = SWEEP_OFF;

const uint16_t vol_step_isi_5ms_cos[] = {
		//0.0625us
		17115,6858,5105,4174,3565,
		3123,2781,2505,2276,2082,
		1914,1765,1636,1518,1413,
		1317,1230,1150,1077,1009,
		947,889,835,785,738,
		694,653,616,579,546,
		514,485,457,431,406,
		383,362,341,321,304,
		286,270,255,467,417,
		371,330,294,263,341,
		286,242,263,254,253,
		254,241,241};

const byte vol_step_val_5ms_cos[] = {
		1,2,3,4,5,
		6,7,8,9,10,
		11,12,13,14,15,
		16,17,18,19,20,
		21,22,23,24,25,
		26,27,28,29,30,
		31,32,33,34,35,
		36,37,38,39,40,
		41,42,43,45,47,
		49,51,53,55,58,
		61,64,68,73,80,
		92,142,255};

inline static void timer1_init(){
	TCCR0B = 0;// close timer0
	TCCR2B = 0;// close timer2
	TCCR1B &= ~PRE_SCALE_MASK; //close clock source
	TCCR1A = 0;
	TCCR1B |= _BV(WGM12); //CTC mode, TOP: OCR1A;
	TCCR1C = 0;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
	OCR1AH = 0;
	OCR1AL = 0;
	TIMSK1 |= _BV(OCIE1A); // Output Compare A Match Interrupt Enable
	sei(); //enable global interrupter
}


inline static void _timer1_stop(){
	TCCR1B &= ~PRE_SCALE_MASK;
	OCR1AH = 0;
	OCR1AL = 0;
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
	timer1_mode = TIMER1_STOP;
}

inline void _timer1_start(byte pre_scale){
	TCNT1H = 0; //set TCNT1 to 0
	TCNT1L = 0; //set TCNT1 to 0
	TCCR1B |= pre_scale; //set prescale
}

inline static void _set_ctc_isi(uint16_t data){
	OCR1AH = data>>8;
	OCR1AL = data;
}

inline static void _set_step_vol(byte i){
    if (_target_vol > vol_step_val_5ms_cos[i]){
	    _set_vol(_target_vol - vol_step_val_5ms_cos[i]);
    }else{
	    _set_vol(0);
    }
}

inline static void _set_step_up(){
	byte temp_index = step_size-1-step_index;
	_set_ctc_isi(vol_step_isi_5ms_cos[temp_index]);
	_set_step_vol(temp_index);
	step_index++;
}

inline static void _sweep_linear(){
	sweep_index++;
	uint32_t fq = (uint32_t) (sweep_fq0 + (sweep_linear_step * sweep_index));
	_set_fq(fq);
}

inline static void _sweep_exp(){
	sweep_index++;
	uint32_t fq = (uint32_t) (sweep_fq0 * pow(sweep_base, sweep_index));
	_set_fq(fq);
}


inline static void _set_step_down(){
	_set_ctc_isi(vol_step_isi_5ms_cos[step_index]);
	_set_step_vol(step_index);
	step_index++;
}

//void SPI_TGMClass::test_step(){
//	step_vol_up(192);
//	delay_ms(10);
//	step_vol_down(192);
//}

void SPI_TGMClass::step_vol_up(byte t_vol){
	_target_vol = t_vol;
	step_index = 0;
	timer1_mode = TIMER1_STEP_UP;
	 _set_step_up();
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	step_index = 0;
}

void SPI_TGMClass::step_vol_down(byte t_vol){
	_target_vol = t_vol;
	step_index = 0;
	timer1_mode = TIMER1_STEP_DOWN;
	_set_step_down();
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	step_index = 0;
}


static void _sweep_tone_linear(uint16_t fq0, uint16_t fq1, uint16_t _time, byte vol){
	uint32_t sweep_interval = _SWEEP_INTERVAL_; //us
	_set_vol(vol);
	_set_fq(fq0);
	sweep_index = 0;
	sweep_index_size = ((uint32_t)_time)*1000/sweep_interval;
	sweep_linear_step = ((double)fq1 - (double)fq0)/((double)sweep_index_size);
	timer1_mode = TIMER1_SWEEP_LINEAR;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
}



static void _sweep_tone_exp(uint16_t fq0, uint16_t fq1, uint16_t _time, byte vol){
	uint32_t sweep_interval = _SWEEP_INTERVAL_; //us
	_set_vol(vol);
	_set_fq(fq0);
	sweep_index = 0;
	sweep_index_size = ((uint32_t)_time)*1000/sweep_interval;
	sweep_base =  exp(log((double)fq1/(double)fq0) / (double)sweep_index_size);
	timer1_mode = TIMER1_SWEEP_EXP;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
}

static void _sweep_tone_peak(uint16_t fq0, uint16_t fq1, uint16_t fq2, uint16_t _time, byte vol){
	uint32_t sweep_interval = _SWEEP_INTERVAL_; //us
	_set_vol(vol);
	_set_fq(fq0);
	sweep_index = 0;
	sweep_index_size = (((uint32_t)_time)*1000/sweep_interval)/2;//based on linear change mode
	sweep_linear_step = ((double)fq1 - (double)fq0)/((double)sweep_index_size);
	timer1_mode = TIMER1_SWEEP_LINEAR;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
	_set_fq(fq1);
	sweep_fq0 = sweep_fq1;
	sweep_index_size = (((uint32_t)_time)*1000/sweep_interval)/2;//based on linear change mode
	sweep_linear_step = ((double)fq2 - (double)fq1)/((double)sweep_index_size);
	timer1_mode = TIMER1_SWEEP_LINEAR;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
}

uint16_t random_noise_fq(){
	uint16_t fq,fq0,fq1;
	if(sweep_fq1>sweep_fq0){
		fq0=sweep_fq0;
		fq1=sweep_fq1;
	}else{
		fq0=sweep_fq1;
		fq1=sweep_fq0;
	}
	switch(sweep_mode){
	case SWEEP_NOISE_WHITE:
		fq = random(fq0, fq1);
	break;

	case SWEEP_NOISE_GAUSS1:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1))/2;
	break;

	case SWEEP_NOISE_GAUSS2:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1))/3;
	break;

	case SWEEP_NOISE_GAUSS3:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)+ \
			(uint32_t)random(fq0, fq1))/4;
	break;
	}
	return fq;
}

static void _noise_tone(uint16_t _time){
	uint32_t sweep_interval = _SWEEP_INTERVAL_; //us
	timer1_mode = TIMER1_NOISE;
	// _set_vol(vol);
	_set_fq(random_noise_fq());
	sweep_index = 0;
	sweep_index_size = ((uint32_t)_time)*1000/sweep_interval;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
}

static void _chord(uint16_t _time){
	uint32_t sweep_interval = _SWEEP_INTERVAL_; //us
	timer1_mode = TIMER1_CHORD;
	sweep_index = 0;
	sweep_index_size = ((uint32_t)_time)*1000/sweep_interval;
	_set_ctc_isi(sweep_interval*16);//16 cycles per us
	_timer1_start(PRE_SCALE_0DIV);
	while(TIMER1_STOP != timer1_mode);
	sweep_index = 0;
}


void SPI_TGMClass::delay_ms(uint16_t data){
	delay_time = data;
	timer1_mode = TIMER1_DELAY_MODE;
	_set_ctc_isi(PRE_SCALE_64DIV_MUTIPLE);
	_timer1_start(PRE_SCALE_64DIV);
	while(TIMER1_STOP != timer1_mode);
}




ISR(TIMER1_COMPA_vect){
	switch(timer1_mode){
	case TIMER1_STEP_UP:
		if (step_index < step_size){
			_set_step_up();
		}else{
			_timer1_stop();
		}
		break;

	case TIMER1_STEP_DOWN:
		if (step_index < step_size){
			_set_step_down();
		}else{
			_timer1_stop();
		}
		break;

	case TIMER1_SWEEP_LINEAR:
		if (sweep_index < sweep_index_size){
			_sweep_linear();
		}else{
			_timer1_stop();
		}
		break;

	case TIMER1_SWEEP_EXP:
		if (sweep_index < sweep_index_size){
			_sweep_exp();
		}else{
			_timer1_stop();
		}
		break;

	case TIMER1_NOISE:
		if (sweep_index < sweep_index_size){
			sweep_index++;
			_set_fq(random_noise_fq());
		}else{
			_timer1_stop();
		}
	break; 

	case TIMER1_CHORD:
		 if (sweep_index < sweep_index_size){
			sweep_index++;
			_set_fq(chord_fq_array[sweep_index%chord_fq_num]);
		}else{
			_timer1_stop();
		}
	break; 

	case TIMER1_DELAY_MODE:
		delay_time--;
		if(0 == delay_time) _timer1_stop();
		break;

	case TIMER1_STOP:
		_timer1_stop();
		break;
	}
}
/*--------timer1-------*/
/*--------timer1-------*/




void SPI_TGMClass::init(byte boardtype)
{
	_board = boardtype;//set UNO or MEGA2560

	memset(&info, 0, sizeof(TGMinfo));
	info.nSize = sizeof(TGMinfo);
	info.version = TGM_VERSION;
	strcpy(info.version_str, VERSION_STRING);

	memset(&error, 0, sizeof(TMGerror));
	error.nSize = sizeof(TMGerror);
	error.error_code = NO_ERROR;
	strcpy(error.error_string, noerror_string);

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

void SPI_TGMClass::wait_command(){
	while(LOW == _REQ_GET_G_BUSY());
}

void SPI_TGMClass::init_TGM(){

	timer1_init();
	_ad9850_init();
	_pga2310_init();
	init(TGM);
	//_23lc1024_reset();
	_write_info(&info);
	_write_error(&error);
	_set_vol(INIT_VOL);
	_set_fq(INIT_FQ);
}

void SPI_TGMClass::vol_chip_reset(){
	//_23lc1024_reset();
}

void SPI_TGMClass::set_vol(byte vol){
	_pga2310_wr(vol, vol);
}

void SPI_TGMClass::set_fq(uint32_t fq){
	_set_fq(fq);
}

void SPI_TGMClass::read_tone(){
	read(TONE_ADDR, sizeof(ton), (char*)&tone);
}





void SPI_TGMClass::set_tone(){
	if (TONE_FLAG_ON == tone.tone_flag){
		_target_vol = tone.volume;
		sweep_mode = tone.sweep;
		sweep_fq0 = tone.frequency0;
		sweep_fq1 = tone.frequency1;
		sweep_fq2 = tone.frequency2;
		memcpy((void* )chord_fq_array, &(tone.frequency0), sizeof(tone.frequency0)*tone.chord_num);
		chord_fq_num = tone.chord_num;
		// _set_vol(0);

		switch(tone.sweep){
			case SWEEP_OFF:
			_set_fq(tone.frequency0);
			break;

			case SWEEP_LINEAR:
			_set_fq(tone.frequency0);
			break;

			case SWEEP_EXP:
			_set_fq(tone.frequency0);
			break;

			case SWEEP_PEAK:
			_set_fq(tone.frequency0);
			break;

			case SWEEP_ARRAY:
			_set_fq(tone.frequency0);
			break;

			case SWEEP_NOISE_WHITE:
			_set_fq(random_noise_fq());
			break;

			case SWEEP_NOISE_GAUSS1:
			_set_fq(random_noise_fq());
			break;

			case SWEEP_NOISE_GAUSS2:
			_set_fq(random_noise_fq());
			break;

			case SWEEP_NOISE_GAUSS3:
			_set_fq(random_noise_fq());
			break;

			case SWEEP_CHORD:
			_set_fq(tone.frequency0);
			break;

		}
		
		switch(tone.step_up_flag){
		case STEP_FLAG_COS_5MS:
			step_vol_up(_target_vol);
			break;
		}

		if(VOLUME_ON == tone.volume_mode){
			_set_vol(_target_vol);
		}

		switch(tone.sweep){
			case SWEEP_OFF:
		if(tone.duration!=0)
			{delay_ms(tone.duration);}
			break;

			case SWEEP_LINEAR:
			_sweep_tone_linear(tone.frequency0,tone.frequency1,tone.duration,_target_vol);
			break;

			case SWEEP_EXP:
			_sweep_tone_exp(tone.frequency0,tone.frequency1,tone.duration,_target_vol);
			break;

			case SWEEP_PEAK:
			_sweep_tone_peak(tone.frequency0,tone.frequency1,tone.frequency2,tone.duration,_target_vol);
			break;

			case SWEEP_ARRAY:
			break;

			case SWEEP_NOISE_WHITE:
			_noise_tone(tone.duration);
			break;

			case SWEEP_NOISE_GAUSS1:
			_noise_tone(tone.duration);
			break;

			case SWEEP_NOISE_GAUSS2:
			_noise_tone(tone.duration);
			break;

			case SWEEP_NOISE_GAUSS3:
			_noise_tone(tone.duration);
			break;

			case SWEEP_CHORD:
			_chord(tone.duration);
			break;
		}


		switch(tone.step_down_flag){
		case STEP_FLAG_COS_5MS:
			step_vol_down(_target_vol);
			break;
		}

		if(tone.duration!=0)
		{
			_set_vol(0);
			_set_fq(0);
		}

		_erase_tone();
	}
}

SPI_TGMClass SPI_TGM;

