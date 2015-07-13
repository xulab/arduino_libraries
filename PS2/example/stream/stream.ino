//#include <Pulse.h>
#include <PS2.h>

unsigned  long temp_count = 0;

void setup() {
	delay(100);
	Serial.begin(115200);
	PS2.init_mouse(STREAM);
	delay(100); 
	temp_count = PS2.mouse.counts;
}

void loop() {
	while ( temp_count == PS2.mouse.counts);
	temp_count = PS2.mouse.counts;
	
	//Serial.print("byte1:");
	//Serial.print("L_button:");
	//Serial.println(PS2.mouse.byte.L_button);
	//Serial.print("R_button:");
	//Serial.println(PS2.mouse.byte.R_button);
	//Serial.print("M_button:");
	//Serial.println(PS2.mouse.byte.M_button);
	//Serial.print("Always_1:");
	//Serial.println(PS2.mouse.byte.Always_1);
	Serial.print("X_signbit:");
	Serial.println(PS2.mouse.byte.X_signbit);
	//Serial.print("Y_signbit:");
	//Serial.println(PS2.mouse.byte.Y_signbit);
	Serial.print("X_overflow:");
	Serial.println(PS2.mouse.byte.X_overflow);
	//Serial.print("Y_overflow:");
	//Serial.println(PS2.mouse.byte.Y_overflow);
	Serial.print("X:");
	Serial.println(PS2.mouse.X + (PS2.mouse.byte.X_overflow*256), DEC);
	//Serial.print("Y:");
	//Serial.println(PS2.mouse.Y, DEC);
	//Serial.print("time:");
	//Serial.println(PS2.mouse.time);
	//Serial.print("counts:");
	//Serial.println(PS2.mouse.counts);

}




