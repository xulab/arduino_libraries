#include <STEPMOTOR.h>

#define MOTOR_PORT PORTC
#define pos1 B00000011
#define pos2 B00000110
#define pos3 B00001100
#define pos4 B00001001
#define step_delay 2 //ms
#define MOTOR_EN1 0
#define MOTOR_EN2 1 
#define MOTOR_EN3 2 
#define MOTOR_EN4 4 

unsigned char cur_p=pos1;

void STEPMOTOR_Class::forward_step() 
{
  switch(cur_p)
  { 
    case pos1: cur_p=pos2;MOTOR_PORT = cur_p;break;
    case pos2: cur_p=pos3;MOTOR_PORT = cur_p;break;
    case pos3: cur_p=pos4;MOTOR_PORT = cur_p;break;
    case pos4: cur_p=pos1;MOTOR_PORT = cur_p;break;
  }
}

void STEPMOTOR_Class::backward_step()
{
  switch(cur_p)
  {
    case pos1: cur_p=pos4;MOTOR_PORT = cur_p;break;
    case pos2: cur_p=pos1;MOTOR_PORT = cur_p;break;
    case pos3: cur_p=pos2;MOTOR_PORT = cur_p;break;
    case pos4: cur_p=pos3;MOTOR_PORT = cur_p;break;
  }
}

void STEPMOTOR_Class::init()
{
	DDRC = 0x0F;
	pinMode(MOTOR_EN1, OUTPUT);
	pinMode(MOTOR_EN2, OUTPUT);
	pinMode(MOTOR_EN3, OUTPUT);
	pinMode(MOTOR_EN4, OUTPUT);
	digitalWrite(MOTOR_EN1, HIGH);
	digitalWrite(MOTOR_EN2, HIGH);
	digitalWrite(MOTOR_EN3, LOW);
	digitalWrite(MOTOR_EN4, LOW);
  int i;
  for(i=0; i<20; i++){forward_step();delay(step_delay);}
  for(i=0; i<20; i++){backward_step();delay(step_delay);}
}

STEPMOTOR_Class STEPMOTOR;
