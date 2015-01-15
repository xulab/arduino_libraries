#include <ENCODER.h>

#define SPEAKER 7

void setup() {
	pinMode(SPEAKER, OUTPUT);
	digitalWrite(SPEAKER, LOW);
	ENCODER.init();
	Serial.begin(9600);
}

void loop() {
    switch(ENCODER.opt_type())
    {
      case NONE_ACT:
        break;
        
      case ENCODER_R:
        Serial.print("R");
        Serial.println(ENCODER.opt_time());
        break;

      case ENCODER_L:
        Serial.print("L");
        Serial.println(ENCODER.opt_time());
        break;

      case ENCODER_PUSH:
        Serial.print("P");
        Serial.println(ENCODER.opt_time());
        break;
    }
}