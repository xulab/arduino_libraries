#ifndef _ENCODER_H
#define _ENCODER_H
#include "Arduino.h"

#define NONE_ACT 0
#define ENCODER_R 1
#define ENCODER_L 2
#define ENCODER_PUSH 3

class ENCODER_Class
{
	private:

	public:
		unsigned char opt_type();
		unsigned long opt_time();
		void init();
};


extern ENCODER_Class ENCODER;
#endif
