#include <FAST_PWM.h>

int output_pin = 9;
void setup() {
	delay(100);
	FAST_PWM.enable_pin(output_pin);
	Serial.begin(115200);
	// put your setup code here, to run once:
}

void loop() {

	for (unsigned char i =0; i<255; i++)
	{
		delay(1);
		FAST_PWM.write(output_pin, i);
	}
}


