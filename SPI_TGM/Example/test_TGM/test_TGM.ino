#include <math.h>
#include <SPI.h>
#include <SPI_TGM.h>
#include <CACHE.h>

uint16_t fq0 = 4000;
uint16_t fq1 = 12000; 
// double base;
int isi = 1000;
int sweep_isi = 1;
int duration = 1000;
byte vol = 192; 


uint16_t chord[]={2000,3000,4000};

void setup() {
	delay(50);
	SPI_TGM.init(MEGA2560);
	Serial.begin(9600);
	Serial.print("random= ");
	random(10);
	Serial.println(random(40000));
	Serial.println(random(40000));
}

void loop() {

	// SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol);
	// delay(duration + isi);

	// SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq1, fq0, vol);
	// // SPI_TGM.quick_sweep_linear_cosramp_5ms(duration, fq0, fq1, vol);
	// delay(duration + isi);

	SPI_TGM.quick_tone_vol_cosramp_5ms(duration, fq0, vol);
	delay(duration + isi);

 	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_WHITE);
 	delay(duration + isi);

	SPI_TGM.quick_chord_cosramp_5ms(duration, chord, sizeof(chord)/sizeof(uint16_t), vol);
 	delay(duration + isi);

	// SPI_TGM.tone_vol_rampup(fq0, vol);
	// for(int i; i<1000; i++)
	// 	{
	// 		delay(sweep_isi);
	// 	  	SPI_TGM.set_tone_fq(i*5+fq0);
	// 	}
	// delay(sweep_isi);
	// SPI_TGM.tone_vol_rampdown(1000*5+fq0, vol);
 // 	delay(duration + isi);
	// while(1);
}

