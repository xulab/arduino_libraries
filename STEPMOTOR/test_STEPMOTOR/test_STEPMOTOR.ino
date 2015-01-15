#include <STEPMOTOR.h>

#define step_delay 2

void setup() {
  // put your setup code here, to run once:
  STEPMOTOR.init();
}

void loop() {
	// put your main code here, to run repeatedly:
	for(int i=0; i<1000; i++){
		STEPMOTOR.forward_step(); 
		delay(step_delay);
	}

	delay(1000);

	for(int i=0; i<1000; i++){
		STEPMOTOR.backward_step(); 
		delay(step_delay);
	}

	delay(1000);

}






