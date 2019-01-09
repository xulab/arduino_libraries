#include <math.h>
#include <SPI.h>
#include <SPI_TGM.h>
#include <CACHE.h>

uint32_t fq0 = 10000;
uint32_t fq1 = 3000;
uint32_t fq2 = 500;
// double base;
uint32_t isi = 2000;
uint32_t isi1 = 4;
int sweep_isi = 1;
uint32_t duration = 300;
uint32_t duration1 = 1;
uint32_t pre_sound_delay = 1000;
byte vol = 192;
int pin = 13;
uint32_t carrierFq = 10000;
uint32_t AMFq = 100;
uint32_t clicksDur = 1;
uint32_t clicksPeriod = 30;
uint32_t clicksPeriod1 = 5;

uint32_t chord[] = {2000, 3000, 4000};

void setup()
{
	pinMode(pin, OUTPUT);

	digitalWrite(pin, LOW);
	delay(50);
	SPI_TGM.init(MEGA2560);
	Serial.begin(115200);
	Serial.print("random= ");
	random(10);
	Serial.println(random(40000));
	Serial.println(random(40000));
}

void loop()
{

	SPI_TGM.quick_tone_AM(duration, carrierFq, AMFq, vol);
	delay(duration + isi);

	// digitalWrite(pin, HIGH);
	// delayMicroseconds(1);
	// digitalWrite(pin, LOW);
	// SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod, vol);
	// delay(duration + isi);

	// digitalWrite(pin, HIGH);
	// delayMicroseconds(1);
	// digitalWrite(pin, LOW);
	// SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod1, vol);
	// delay(duration / 2);

	// SPI_TGM.tone_cancel_cosramp_2ms();
	// delay(duration + isi);
	//	 SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol);
	//   Serial.println("exp");
	//	 delay(duration + isi);
	//
	//	 SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq1, fq0, vol);
	//   Serial.println("exp down");
	//	 delay(duration + isi);
	//
	// SPI_TGM.quick_sweep_linear_cosramp_5ms(duration, fq0, fq1, vol);
	// Serial.println("linear");
	// delay(duration + isi);
	//  SPI_TGM.quick_sweep_peak_cosramp_5ms(duration * 2 , fq0, fq1, fq2, vol);
	//  Serial.println("peak");
	// delay(duration + isi);

	// digitalWrite(pin, HIGH);
	// delayMicroseconds(1) ;
	// digitalWrite(pin, LOW);

	// SPI_TGM.quick_tone_vol_cosramp_2ms(duration1, fq0, vol);
	// delay(isi1);

	// SPI_TGM.tone_cancel();
	// Serial.println("tone_cancel");
	// delay(duration + isi);

	// digitalWrite(pin, HIGH);
	// delayMicroseconds(1) ;
	// digitalWrite(pin, LOW);

	// SPI_TGM.quick_tone_vol_cosramp_2ms(duration1, fq0, vol);
	// delay(isi1);

	// SPI_TGM.tone_cancel_cosramp_5ms();
	// Serial.println("tone_cancel_cosramp_5ms");
	// delay(duration + isi);

	// digitalWrite(pin, HIGH);
	// delayMicroseconds(1) ;
	// digitalWrite(pin, LOW);

	// SPI_TGM.quick_tone_vol_cosramp_2ms(duration1, fq0, vol);
	// delay(isi1);

	// SPI_TGM.tone_cancel_cosramp_2ms();
	// Serial.println("tone_cancel_cosramp_2ms");
	// delay(duration + isi);

	//
	//SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_WHITE);
	//Serial.println("SWEEP_NOISE_WHITE");
	//delay(duration + isi);
	//
	//SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_GAUSS1);
	//Serial.println("SWEEP_NOISE_GAUSS1");
	//delay(duration + isi);
	//
	//SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_GAUSS2);
	//Serial.println("SWEEP_NOISE_GAUSS2");
	//delay(duration + isi);
	//
	//SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol,SWEEP_NOISE_GAUSS3);
	//Serial.println("SWEEP_NOISE_GAUSS3");
	//delay(duration + isi);
	//
	//SPI_TGM.quick_chord_cosramp_5ms(duration, chord, sizeof(chord)/sizeof(uint32_t), vol);
	//Serial.println("chord");
	//delay(duration + isi);

	// SPI_TGM.quick_tone_vol_cosramp_5ms(100, 5000, vol);
	// delay(duration + isi);

	// SPI_TGM.tone_vol_rampup(fq1, vol);
	// while(1);
	// SPI_TGM.quick_tone_vol_cosramp_5ms(duration, fq0, vol);
	// delay(duration + isi);

	// SPI_TGM.quick_tone_vol_cosramp_5ms(duration, fq1, 160);
	// delay(duration + isi);
	// SPI_TGM.quick_tone_vol_cosramp_5ms(duration, fq1, 120);
	// delay(duration + isi);

	// SPI_TGM.quick_tone_vol(duration, fq1, vol);
	// delay(duration + isi);

	// SPI_TGM.tone_vol_rampup(fq0, vol);
	// for(int i; i<1000; i++)
	// 	{
	// 		delay(sweep_isi);
	// 	  	SPI_TGM.set_tone_fq(i*5+fq0);
	// 	}
	// delay(sweep_isi);
	// SPI_TGM.tone_vol_rampdown(1000*5+fq0, vol);
	// 	delay(duration + isi);
	// // while(1);
}