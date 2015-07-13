//#include <Pulse.h>
#include <PS2.h>
#include <FAST_PWM.h>

#define POS_MASK 0x0000
#define NEG_MASK 0xFF00
int16_t x_move;



void setup() {
	delay(100);
	FAST_PWM.enable_pin(9);
	Serial.begin(115200);
	PS2.init_mouse(REMOTE);
	delay(100);
}

void loop() {
	delay(10);
	PS2.read_mouse();
	x_move = PS2.mouse.X;
	if(PS2.mouse.byte.X_signbit)
	{
		x_move = PS2.mouse.X | NEG_MASK ;
		if(PS2.mouse.byte.X_overflow)
		{
			x_move -= 0xFF;
		}
	}
	else
	{
		if(PS2.mouse.byte.X_overflow)
		{
			x_move += 0xFF;
		}
	}
	if(x_move>0)
	{
		FAST_PWM.write(9, x_move*2);
	}else 
	{
		FAST_PWM.write(9, 0);
	}
	Serial.print("x_move:");
	Serial.println(x_move);
	//Serial.print("byte1:");
	//Serial.print("L_button:");
	//Serial.println(PS2.mouse.byte.L_button);
	//Serial.print("R_button:");
	//Serial.println(PS2.mouse.byte.R_button);
	//Serial.print("M_button:");
	//Serial.println(PS2.mouse.byte.M_button);
	//Serial.print("Always_1:");
	//Serial.println(PS2.mouse.byte.Always_1);
	// Serial.print("X_signbit:");
	// Serial.println(PS2.mouse.byte.X_signbit);
	//Serial.print("Y_signbit:");
	//Serial.println(PS2.mouse.byte.Y_signbit);
	// Serial.print("X_overflow:");
	// Serial.println(PS2.mouse.byte.X_overflow);
	//Serial.print("Y_overflow:");
	//Serial.println(PS2.mouse.byte.Y_overflow);
	// Serial.print("X:");
	// Serial.println(PS2.mouse.X + (PS2.mouse.byte.X_overflow * 256), DEC);
	//Serial.print("Y:");
	//Serial.println(PS2.mouse.Y, DEC);
	//Serial.print("time:");
	//Serial.println(PS2.mouse.time);
	//Serial.print("counts:");
	//Serial.println(PS2.mouse.counts);
}




