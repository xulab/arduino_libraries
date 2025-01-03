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
uint32_t chord2[] = {3000, 4000, 6000};

void setup()
{
	pinMode(pin, OUTPUT);

	digitalWrite(pin, LOW);
	delay(50);
	SPI_TGM.init(MEGA2560);
	SPI_TGM2.init(MEGA2560);
	Serial.begin(115200);
	Serial.print("random= ");
	random(10);
	Serial.println(random(40000));
	Serial.println(random(40000));
}

void loop()
{
	/*******test timeout*******/
	/*******test timeout*******/
	// long tmpTime = millis();
	// int res = SPI_TGM.quick_tone_vol(duration, fq1, vol, 0, 1000);
	// if (res == -1)
	// {
	// 	Serial.print("start quick_tone at:");
	// 	Serial.println(tmpTime);
	// 	Serial.print("quick_tone res:");
	// 	Serial.println(res);
	// 	Serial.print("quick_tone time:");
	// 	Serial.println(millis() - tmpTime);
	// }
	// else
	// {
	// 	Serial.println("pure tone");
	// }
	// delay(duration + isi);
	/*******test timeout*******/
	/*******test timeout*******/

	// int tmp_fq;
	// tmp_fq = 1000;
	// SPI_TGM.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// delay(2000);

	// tmp_fq = 2000;
	// SPI_TGM2.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM2.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM2.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// tmp_fq = tmp_fq * 1.2;
	// SPI_TGM2.quick_tone_vol(300, tmp_fq, vol);
	// delay(305);
	// delay(2000);

	// SPI_TGM.set_tone_fq_vol(3000, 192);
	// delay(10);
	// SPI_TGM.tone_cancel();
	// delay(300);

	// int tmp_t = 50;
	// for (int i = 0; i < 10; i++)
	// {
	// 	SPI_TGM.set_tone_fq_vol(1000, 192);
	// 	delay(tmp_t);
	// 	SPI_TGM.set_tone_fq_vol(3000, 192);
	// 	delay(tmp_t);
	// 	SPI_TGM.set_tone_fq_vol(2000, 192);
	// 	delay(tmp_t);
	// 	SPI_TGM.set_tone_fq_vol(5000, 192);
	// 	delay(tmp_t);
	// 	SPI_TGM.set_tone_fq_vol(4000, 192);
	// 	delay(tmp_t);
	// }
	// SPI_TGM.tone_cancel();

	// delay(3000);

	int t = 1;
	for (int i = 1000; i < 2000; i = i + t)
	{
		SPI_TGM.set_tone_fq_vol(i, vol);
		delay(t);
	}
	SPI_TGM.tone_cancel();
	delay(2000);

	SPI_TGM.quick_tone_AM(duration, carrierFq, AMFq, vol);
	// SPI_TGM2.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol, 200);
	Serial.println("AM");
	delay(duration + isi);

	SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod, vol);
	Serial.println("clicks");
	delay(duration + isi);

	SPI_TGM.quick_tone_clicks_cosramp_2ms(duration, carrierFq, clicksDur, clicksPeriod1, vol);
	Serial.println("clicks1");
	delay(duration + isi);

	SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq0, fq1, vol);
	Serial.println("exp");
	delay(duration + isi);

	SPI_TGM.quick_sweep_exp_cosramp_5ms(duration, fq1, fq0, vol);
	Serial.println("exp down");
	delay(duration + isi);

	SPI_TGM.quick_sweep_linear_cosramp_5ms(duration, fq0, fq1, vol);
	Serial.println("linear");
	delay(duration + isi);

	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_WHITE);
	Serial.println("SWEEP_NOISE_WHITE");
	delay(duration + isi);

	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS1);
	Serial.println("SWEEP_NOISE_GAUSS1");
	delay(duration + isi);

	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS2);
	Serial.println("SWEEP_NOISE_GAUSS2");
	delay(duration + isi);

	SPI_TGM.quick_noise_cosramp_5ms(duration, fq0, fq1, vol, SWEEP_NOISE_GAUSS3);
	Serial.println("SWEEP_NOISE_GAUSS3");
	delay(duration + isi);

	SPI_TGM.quick_chord_cosramp_5ms(duration, chord, sizeof(chord) / sizeof(uint32_t), vol);
	Serial.println("chord");
	delay(duration + isi);

	SPI_TGM.quick_chord_cosramp_5ms(duration, chord2, sizeof(chord2) / sizeof(uint32_t), vol);
	Serial.println("chord");
	delay(duration + isi);

}