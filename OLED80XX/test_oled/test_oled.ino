// #include <RCM.h>
// #include <Wire.h>
#include <OLED80XX.h>
#include <ENCODER.h>
// #include <STEPMOTOR.h>


#define SPEAKER 7
#define LED1 6
#define LED2 5
unsigned char cur_sec;
byte _CS = 38;
byte _RES = 39;
byte _RW = 14;
byte _DC = 13;
byte _RD = 15;
byte _DATAPORT[8] = {16, 17, 18, 19, 20, 21, 22, 23};
	

// void OLED80XX_REFRESH_TIME()
// {
//   OLED80XX.SmallInt(2*6,0,RCM.year(),2,NOM_DIS);
//   OLED80XX.SmallInt(4*6,0,RCM.month(),2,NOM_DIS);
//   OLED80XX.SmallInt(6*6,0,RCM.date(),2,NOM_DIS);
//   OLED80XX.SmallInt(13*6,0,RCM.hour(),2,NOM_DIS);
//   OLED80XX.SmallInt(16*6,0,RCM.minute(),2,NOM_DIS);
//   OLED80XX.SmallInt(19*6,0,RCM.second(),2,NOM_DIS);
// }


void setup() {
	//put your setup code here, to run once:
	// RCM.init();
  pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	digitalWrite(13, HIGH);
	digitalWrite(13, LOW);
	digitalWrite(13, HIGH);
	digitalWrite(13, LOW);
  OLED80XX.Init(_CS, _RES, _RW,_DC, _RD, _DATAPORT, 1);
	pinMode(SPEAKER, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED1, OUTPUT);
	digitalWrite(SPEAKER, LOW);
	digitalWrite(LED1, LOW);
	digitalWrite(LED2, LOW);
	Serial.begin(9600);
  OLED80XX.SmallString(18*6,0,":",NOM_DIS);
  OLED80XX.SmallString(15*6,0,":",NOM_DIS);
  OLED80XX.SmallInt(0*6,0,-20,2,NOM_DIS);
  // OLED80XX_REFRESH_TIME();
  OLED80XX.BigInt(0,5,123,0,INVER_DIS);
  // STEPMOTOR.init();
  // MOTOR_Init();
  ENCODER.init();
}

void loop() {
	// put your main code here, to run repeatedly:
	
  byte cur_sec, sec_read;
  while(1)
  {
    switch(ENCODER.opt_type())
    {
      case NONE_ACT:
        break;
      case ENCODER_R:
        OLED80XX.SmallString(0,2,"R",NOM_DIS);
        OLED80XX.SmallInt(0,3,ENCODER.opt_time(),8,NOM_DIS);
        // STEPMOTOR.forward_step(); 
        break;

      case ENCODER_L:
        OLED80XX.SmallString(0,2,"L",NOM_DIS);
        OLED80XX.SmallInt(0,3,ENCODER.opt_time(),8,NOM_DIS);
        // STEPMOTOR.backward_step(); 
        break;
      case ENCODER_PUSH:
        OLED80XX.SmallString(0,2,"P",NOM_DIS);
        OLED80XX.SmallInt(0,3,ENCODER.opt_time(),8,NOM_DIS);
        break;
    }

    // sec_read = RCM.second();
    // if (cur_sec != sec_read)
    // {
    // OLED80XX.ClearLine(1);
    // OLED80XX.SmallString(cur_sec,1,"hello world!",INVER_DIS);
    //   cur_sec = sec_read;
    //   if (cur_sec == 0)
    //   {
    //     // OLED80XX_REFRESH_TIME();
    //   }
    //   else
    //   {
    //     OLED80XX.SmallInt(19*6,0,cur_sec,2,NOM_DIS);
    //   }
    // }
  }
}
