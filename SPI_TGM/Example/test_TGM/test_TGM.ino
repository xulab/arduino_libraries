#include <math.h>
#include <SPI.h>
#include <SPI_TGM.h>

uint32_t fq0 = 5000;
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
uint32_t carrierFq = 3000;
uint32_t AMFq = 100;
uint32_t clicksDur = 1;
uint32_t clicksPeriod = 30;
uint32_t clicksPeriod1 = 5;

uint32_t chord[] = {2000, 3000, 4000};
int SPI_TGM_REQ = 44;
int SPI_TGM_PER = 45;

SPI_TGMClass SPI_TGM2;
void setup()
{
	pinMode(pin, OUTPUT);

	digitalWrite(pin, LOW);
	delay(50);
	SPI_TGM.init(MEGA2560);
	SPI_TGM2.init(MEGA2560, SPI_TGM_REQ, SPI_TGM_PER);
	Serial.begin(115200);
	Serial.print("random= ");
	random(10);
	Serial.println(random(40000));
	Serial.println(random(40000));
}

void loop()
{

	// SPI_TGM.set_tone_fq_vol(1000, vol);
	// delay(300);
	// SPI_TGM.set_tone_fq_vol(2000, vol);
	// delay(300);
	// SPI_TGM.set_tone_fq_vol(3000, vol);
	// delay(300);
	// SPI_TGM.set_tone_fq_vol(4000, vol);
	// delay(300);
	// // SPI_TGM.tone_cancel();
	// SPI_TGM.tone_cancel_cosramp_2ms();
	// delay(2000);

	// SPI_TGM.quick_tone_vol(300, 1000, vol);
	// delay(305);
	// SPI_TGM.quick_tone_vol(300, 2000, vol);
	// delay(305);
	// SPI_TGM.quick_tone_vol(300, 3000, vol);
	// delay(305);
	// SPI_TGM.quick_tone_vol(300, 4000, vol);
	// delay(305);
	// delay(2000);

	// for (int i = 1000; i < 2000; i++)
	// {
	// 	SPI_TGM.set_tone_fq_vol(i, vol);
	// 	delay(1);
	// }
	// SPI_TGM.tone_cancel();
	// delay(2000);
	Serial.print("digitalRead(48) = ");
	Serial.println(digitalRead(48));

	long tmpTime = millis();
	Serial.print("start quick_tone at:");
	Serial.println(tmpTime);
	int res = SPI_TGM.quick_tone_vol(duration, fq1, vol, 0, 1000);
	Serial.print("quick_tone res:");
	Serial.println(res);
	Serial.print("finish quick_tone at:");
	Serial.println(millis());
	Serial.print("quick_tone time:");
	Serial.println(millis() - tmpTime);
	Serial.println("pure tone");
	delay(duration + isi);

	// SPI_TGM.quick_tone_AM(duration, carrierFq, AMFq, vol);
	// Serial.println("AM");
	// delay(duration + isi);

	// SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod, vol);
	// Serial.println("clicks");
	// delay(duration + isi);

	// SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod1, vol);
	// Serial.println("clicks1");
	// delay(duration + isi);

	// SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol);
	// Serial.println("exp");
	// delay(duration + isi);

	// SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq1, fq0, vol);
	// Serial.println("exp down");
	// delay(duration + isi);

	// SPI_TGM.quick_sweep_linear_cosramp_5ms(duration, fq0, fq1, vol);
	// Serial.println("linear");
	// delay(duration + isi);

	// SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_WHITE);
	// Serial.println("SWEEP_NOISE_WHITE");
	// delay(duration + isi);

	// SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS1);
	// Serial.println("SWEEP_NOISE_GAUSS1");
	// delay(duration + isi);

	// SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS2);
	// Serial.println("SWEEP_NOISE_GAUSS2");
	// delay(duration + isi);

	// SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS3);
	// Serial.println("SWEEP_NOISE_GAUSS3");
	// delay(duration + isi);

	// SPI_TGM.quick_chord_cosramp_5ms(duration, chord, sizeof(chord) / sizeof(uint32_t), vol);
	// Serial.println("chord");
	// delay(duration + isi);
}