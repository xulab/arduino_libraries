// Do not remove the include below
#include "SPI_TGM_H.h"
#include "SPI.h"

void setup()
{
	SPI_TGM.init_TGM();
  Serial.begin(115000);
  Serial.println("hello");
}


void loop(){
  /****test control chips****/
  /****test control chips****/
  // SPI_TGM.set_fq(3000);
  // Serial.println("hello1");
  // delay(1000);
  // SPI_TGM.set_vol(192);
  // Serial.println("hello2");
  // delay(1000);
  // SPI_TGM.set_fq(6000);
  // Serial.println("hello3");
  // delay(1000);
  // SPI_TGM.set_vol(180);
  // Serial.println("hello4");
  // delay(1000);
  // SPI_TGM.set_vol(200);
  // Serial.println("200");
  // delay(1000);
  // SPI_TGM.set_vol(210);
  // Serial.println("210");
  // delay(1000);
  // SPI_TGM.set_vol(220);
  // Serial.println("220");
  // delay(1000);
  // SPI_TGM.set_vol(230);
  // Serial.println("230");
  // delay(1000);

  /****test tone****/
  /****test tone****/
	// SPI_TGM.testTone();
  Serial.print("sizeof(ton) = ");
  Serial.println(sizeof(ton));
	memset(&SPI_TGM._tone, 0, sizeof(ton));
	SPI_TGM._tone.tone_flag = TONE_FLAG_ON;
	SPI_TGM._tone.volume_mode = VOLUME_ON;
	SPI_TGM._tone.version = TGM_VERSION;
	SPI_TGM._tone.pre_sound_delay = 0;
	SPI_TGM._tone.volume = 192;
	SPI_TGM._tone.frequency0 = 1000;
	SPI_TGM._tone.frequencyL0 = 1000;
//SPI_TGM._tone.frequency1 = 5000;
//SPI_TGM._tone.frequencyL1 = 5000;
	SPI_TGM._tone.sweep = SWEEP_OFF;
	SPI_TGM._tone.duration = 1000;
	SPI_TGM._tone.durationL = 1000;
	SPI_TGM._tone.step_up_flag = STEP_FLAG_COS_2MS;
	SPI_TGM._tone.step_down_flag = STEP_FLAG_COS_2MS;
	SPI_TGM.set_tone();
  delay(5000);



  

  /****standard ****/
  /****standard ****/
	// SPI_TGM.wait_command();
	// SPI_TGM.read_tone();
	// SPI_TGM.set_tone();
}
