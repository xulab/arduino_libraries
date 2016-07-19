#include <PWM_Pulse.h>
// int pin1 = 10;
// int pin2 = 11;
// int pin3 = 12;
// int pin4 = 13;

uint16_t dur = 1000; 
float fq = 10; 
uint16_t p_width = 20; 
uint16_t pre_delay = 100; 
uint16_t power = 100; 
void setup() {
	Serial.begin(115200);
  // put your setup code here, to run once:
  PWM_PULSE.init();
  // pinMode(pin1, OUTPUT);
  // pinMode(pin2, OUTPUT);
  // pinMode(pin3, OUTPUT);
  // pinMode(pin4, OUTPUT);
  pinMode(43, OUTPUT);
}

void loop() {

  // digitalWrite(43, HIGH);
  // delay(1);
  // digitalWrite(43, LOW);

  // PWM_PULSE.p1_multipulses(dur, fq, p_width, pre_delay, power);
  // delay(dur*2);


  // digitalWrite(43, HIGH);
  // delay(1);
  // digitalWrite(43, LOW);

  // PWM_PULSE.p1_constant(dur, pre_delay, power);
  // delay(dur*2);


  // digitalWrite(43, HIGH);
  // delay(1);
  // digitalWrite(43, LOW);

  // PWM_PULSE.p1_constant_ramp50ms(dur, pre_delay, power);
  // delay(dur*2);


  digitalWrite(43, HIGH);
  delay(1);
  digitalWrite(43, LOW);

  PWM_PULSE.p3_constant_ramp50ms(dur, pre_delay, power);
  delay(dur/2);
  PWM_PULSE.p3_cancel();

  delay(dur);


  digitalWrite(43, HIGH);
  delay(1);
  digitalWrite(43, LOW);

  PWM_PULSE.p3_constant_ramp50ms(dur, pre_delay, power);
  delay(dur/2);
  PWM_PULSE.p3_cancel_ramp50ms();

  delay(dur);

  // put your main code here, to run repeatedly:
//   PULSE.p1_sqr_wave(pin1, 1000, 10, 10, TRG_H); 
//   PULSE.p2_sqr_wave(pin2, 1000, 20, 10, TRG_H);
//   PULSE.p3_sqr_wave(pin3, 1000, 30, 10, TRG_H);
//   PULSE.p4_sqr_wave(pin4, 1000, 40, 10, TRG_H);
//   PULSE.p1_sqr_wave(pin1, 1000, 10, 10, TRG_H, 100);
//   PULSE.p2_sqr_wave(pin1, 1000, 10, 10, TRG_H, 100);
//   PULSE.p3_sqr_wave(pin1, 1000, 10, 10, TRG_H, 100);
//   PULSE.p4_sqr_wave(pin1, 1000, 10, 10, TRG_H, 100);

//   PULSE.p1_cancel();
//   delay(200);
//   PULSE.p2_cancel();
//   delay(200);
//   PULSE.p3_cancel();
//   delay(200);
//   PULSE.p4_cancel();
//   delay(500);
//   PULSE.p1_pulse(pin1,500,TRG_H);
//   PULSE.p2_pulse(pin2,600,TRG_H);

//  // PULSE.p2_pulse(pin2,600,TRG_H,50);

// float fq = (float)1000 / 2 / (float)500;
//  PULSE.p1_sqr_wave(pin1, fq, 10, 10, TRG_H, 100);

//  // PULSE.p2_pulse(pin2,600,TRG_H,100);

//   PULSE.p3_pulse(pin3,700,TRG_H);
//   PULSE.p4_pulse(pin4,800,TRG_H);      
//   delay(2000);
//   Serial.println(1);
}
