#include <math.h>
#include <SPI.h>
#include <SPI_TGM.h>

uint16_t fq0 = 4000;
uint16_t fq1 = 8000; 
// double base;
int isi = 1000;
int duration = 1000;
byte vol = 150;


uint16_t chord[]={2000,3000,4000};

void setup() {
	delay(50);
	SPI_TGM.init(MEGA2560);
	// base =  exp(log((double)fq1/(double)fq0) / (double)duration);
	Serial.begin(9600);
	Serial.print("random= ");
	random(10);
	Serial.println(random(40000));
	Serial.println(random(40000));
}

void loop() {

	SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol);
	delay(duration + isi);

	SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq1, fq0, vol);
	// SPI_TGM.quick_sweep_linear_cosramp_5ms(duration, fq0, fq1, vol);
	delay(duration + isi);

	SPI_TGM.quick_tone_vol_cosramp_5ms(duration, fq0, vol);
	delay(duration + isi);

 	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_WHITE);
 	delay(duration + isi);

	SPI_TGM.quick_chord_cosramp_5ms(duration, chord, sizeof(chord)/sizeof(uint16_t), vol);
 	delay(duration + isi);

}

