#include "TGM_AD9850.h"

/*--------AD9850-------*/


// #define AD9850_clock (float)125000000 //125MHz
#define AD9850_clock (double)125000000 //125MHz
// const float x = (float)4294967295 / AD9850_clock;
const double x = (double)4294967295 / AD9850_clock;
// #define fq_convert(f) (uint32_t)((float)f*x)
#define fq_convert(f) (uint32_t)((double)f*x)

#ifdef ARDUINO_ARCH_ESP32
//suit for ESP32_PICO_mini_02
// #define AD9850_w_clk 25
// #define AD9850_fq_up  26
// #define AD9850_rest 32
// int AD9850_DATA[8] = {4, 2, 15, 13, 12, 14, 27, 33}; // Suit for teensy3.6Adptor4TGM3.2Ver3
//suit for ESP32_S2
#define AD9850_w_clk 10
#define AD9850_fq_up 11 
#define AD9850_rest 12
int AD9850_DATA[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // Suit for teensy3.6Adptor4TGM3.2Ver3
#else
#define AD9850_w_clk 34 //Suit for teensy3.6Adptor4TGM3.2Ver3
#define AD9850_fq_up 35 //Suit for teensy3.6Adptor4TGM3.2Ver3
#define AD9850_rest 33 //Suit for teensy3.6Adptor4TGM3.2Ver3
int AD9850_DATA[8] = {24, 25, 26, 27, 28, 29, 30, 31}; //Suit for teensy3.6Adptor4TGM3.2Ver3
#endif


volatile uint32_t curFq = 0;

void _ad9850_data_wr(byte data) {
	for (byte i = 0; i < 8; i++) digitalWrite(AD9850_DATA[i], data & _BV(i));
}

void _ad9850_init_port() {
	pinMode(AD9850_w_clk, OUTPUT);
	digitalWrite(AD9850_w_clk, LOW);
	pinMode(AD9850_fq_up, OUTPUT);
	digitalWrite(AD9850_fq_up, LOW);
	pinMode(AD9850_rest, OUTPUT);
	digitalWrite(AD9850_rest, LOW);
	for (int i = 0; i < 8; i++) pinMode(AD9850_DATA[i], OUTPUT);
	_ad9850_data_wr(0);
}

void _ad9850_reset() {
	digitalWrite(AD9850_rest, LOW);
	digitalWrite(AD9850_rest, HIGH);
	delay(100);
	digitalWrite(AD9850_rest, LOW);
	digitalWrite(AD9850_w_clk, LOW);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);
	digitalWrite(AD9850_fq_up, LOW);
	digitalWrite(AD9850_fq_up, HIGH);
	digitalWrite(AD9850_fq_up, LOW);
	_ad9850_data_wr(0);
}

void _ad9850_init() {
	delay(100);
	_ad9850_init_port();
	_ad9850_reset();
}

void _ad9850_wr_parallel(uint32_t fq) {
	uint8_t * data = (uint8_t *)&fq;

	_ad9850_data_wr(0);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[3]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[2]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[1]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[0]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	digitalWrite(AD9850_fq_up, HIGH);
	digitalWrite(AD9850_fq_up, LOW);

	_ad9850_data_wr(0);
}


void _set_fq(uint32_t fq) {
	if (curFq != fq) {
		_ad9850_wr_parallel(fq_convert(fq));
		curFq = fq;
	}
}
