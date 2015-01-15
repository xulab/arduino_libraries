#ifndef _STEPMOTOR_H
#define _STEPMOTOR_H
#include <arduino.h>

class STEPMOTOR_Class
{
private:

public:
	void init();
	void forward_step();	
	void backward_step();
};

extern STEPMOTOR_Class STEPMOTOR;

#endif

