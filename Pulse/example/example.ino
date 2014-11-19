#include <Pulse.h>
int pin1 = 10;
int pin2 = 11;
int pin3 = 12;
int pin4 = 13;
uint16_t dur = 1000; 
void setup() {
	Serial.begin(115200);
  // put your setup code here, to run once:
  PULSE.init();
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  PULSE.p1_sqr_wave(pin1, 1000, 10, 10, TRG_H); 
  PULSE.p2_sqr_wave(pin2, 1000, 20, 10, TRG_H);
  PULSE.p3_sqr_wave(pin3, 1000, 30, 10, TRG_H);
  PULSE.p4_sqr_wave(pin4, 1000, 40, 10, TRG_H);
  delay(200);
  PULSE.p1_cancel();
  delay(200);
  PULSE.p2_cancel();
  delay(200);
  PULSE.p3_cancel();
  delay(200);
  PULSE.p4_cancel();
  delay(500);
  PULSE.p1_pulse(pin1,500,TRG_H);
  PULSE.p2_pulse(pin2,600,TRG_H);
  PULSE.p3_pulse(pin3,700,TRG_H);
  PULSE.p4_pulse(pin4,800,TRG_H);      
  delay(2000);
  Serial.println(1);
}
