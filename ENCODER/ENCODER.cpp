#include "ENCODER.h"
#define encoder1 3
#define encoder2 19
#define encoder3 18
#define encoder1_interupt 1
#define encoder2_interupt 4
#define encoder3_interupt 5
#define jitter_time 5 
#define max_pulse_time 100

int int_num=0;
int cur_ph=0;
unsigned long ph1_time=0;
unsigned long encoder1_ph1_time=0;
unsigned long encode_time=0;
unsigned char encode_type=NONE_ACT;

inline void attach_23_interrupts();

void encoder2_r_ph2()
{
  if(((millis()-ph1_time)>max_pulse_time)&&(digitalRead(encoder3)==LOW))
  {
    attach_23_interrupts();
    return;
  }
  detachInterrupt(encoder2_interupt);
  cur_ph=1;
  encode_time = millis();
  encode_type=ENCODER_L;
  attach_23_interrupts();
}

void encoder2_f_ph2()
{
  if(((millis()-ph1_time)>max_pulse_time)&&(digitalRead(encoder3)==HIGH))
  {
    attach_23_interrupts();
    return;
  }
  detachInterrupt(encoder2_interupt);
  cur_ph=0;
  encode_time = millis();
  encode_type=ENCODER_L;
  attach_23_interrupts();
}

void encoder3_r_ph2()
{
  if(((millis()-ph1_time)>max_pulse_time)&&(digitalRead(encoder3)==LOW))
  {
    attach_23_interrupts();
    return;
  }
  detachInterrupt(encoder3_interupt);
  cur_ph=1;
  encode_time = millis();
  encode_type=ENCODER_R;
  attach_23_interrupts();
}

void encoder3_f_ph2()
{
  if(((millis()-ph1_time)>max_pulse_time)&&(digitalRead(encoder3)==HIGH))
  {
    attach_23_interrupts();
    return;
  }
  detachInterrupt(encoder3_interupt);
  cur_ph=0;
  encode_time = millis();
  encode_type=ENCODER_R;
  attach_23_interrupts();
}

void encoder2_r_ph1()
{
  if((millis()-encode_time)<jitter_time)return;
  if(digitalRead(encoder3)==HIGH)return;
  ph1_time=millis();
  detachInterrupt(encoder2_interupt);
  detachInterrupt(encoder3_interupt);
  attachInterrupt(encoder3_interupt, encoder3_r_ph2, RISING);
}

void encoder2_f_ph1()
{
  if((millis()-encode_time)<jitter_time)return;
  if(digitalRead(encoder3)==LOW)return;
  ph1_time=millis();
  detachInterrupt(encoder2_interupt);
  detachInterrupt(encoder3_interupt);
  attachInterrupt(encoder3_interupt, encoder3_f_ph2, FALLING);
}

void encoder3_r_ph1()
{
  if((millis()-encode_time)<jitter_time)return;
  if(digitalRead(encoder2)==HIGH)return;
  ph1_time=millis();
  detachInterrupt(encoder3_interupt);
  detachInterrupt(encoder2_interupt);
  attachInterrupt(encoder2_interupt, encoder2_r_ph2, RISING);
}

void encoder3_f_ph1()
{
  if((millis()-encode_time)<jitter_time)return;
  if(digitalRead(encoder2)==LOW)return;
  ph1_time=millis();
  detachInterrupt(encoder3_interupt);
  detachInterrupt(encoder2_interupt);
  attachInterrupt(encoder2_interupt, encoder2_f_ph2, FALLING);
}

inline void attach_23_interrupts()
{
  if(cur_ph==0)
  {
    attachInterrupt(encoder2_interupt, encoder2_r_ph1, RISING);  
    attachInterrupt(encoder3_interupt, encoder3_r_ph1, RISING);  
  }
  else
  {
    attachInterrupt(encoder2_interupt, encoder2_f_ph1, FALLING);
    attachInterrupt(encoder3_interupt, encoder3_f_ph1, FALLING);     
  }
}
void encoder1_ph1();
void encoder1_ph2()
{
  if((millis()-encoder1_ph1_time)<jitter_time)return;
  encode_time=millis();
  encode_type=ENCODER_PUSH;
  attachInterrupt(encoder1_interupt,encoder1_ph1, FALLING);
}

void encoder1_ph1()
{
  attachInterrupt(encoder1_interupt,encoder1_ph2, RISING);
  encoder1_ph1_time = millis();
}

unsigned char ENCODER_Class::opt_type()
{
  unsigned char type_temp = encode_type;
  encode_type=NONE_ACT;
  return type_temp;
}

unsigned long ENCODER_Class::opt_time()
{
  unsigned long time_temp = encode_time;
  encode_time=0;
  return time_temp;
}
void ENCODER_Class::init()
{
  pinMode(encoder1, INPUT);
  pinMode(encoder2, INPUT);
  pinMode(encoder3, INPUT);
  attach_23_interrupts();
  attachInterrupt(encoder1_interupt,encoder1_ph1, FALLING);
}

ENCODER_Class ENCODER;
