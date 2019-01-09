// Do not remove the include below
#include "SPI_TGM_H.h"
#include "SPI.h"
//int LED = 13;
//int INFO = A3;
//int REQ = A4;
//int PER = 8;
//int i = 0;
//ISR(PCINT1_vect)
//{
//	Serial.println(i++);
//	if (digitalRead(INFO))
//	{
//		digitalWrite(LED, HIGH);
//		digitalWrite(REQ, HIGH);
//	}
//	else {
//		digitalWrite(LED, LOW);
//		digitalWrite(REQ, LOW);
//	}
//}
void setup()
{

	//pinMode(INFO, INPUT);
	//pinMode(LED, OUTPUT);
	//pinMode(REQ, OUTPUT);
	//pinMode(PER, INPUT);
	////digitalWrite(LED, HIGH);
	//PCMSK1 = _BV(PCINT11);
	//PCICR = _BV(PCINT1);
	////sei();
	//if (digitalRead(INFO))
	//{
	//	digitalWrite(REQ, HIGH);
	//}
	//else {
	//	digitalWrite(REQ, LOW);
	//}

	SPI_TGM.init_TGM();
	//SPI_TGM.set_fq(1908);
	//SPI_TGM.set_vol(192);
}


void loop(){
	SPI_TGM.wait_command();
	SPI_TGM.read_tone();
	//SPI_TGM.testTone();
	SPI_TGM.set_tone();
	//while (1);
}













