#ifndef _SSD1331_S_SHOW_H
#define _SSD1331_S_SHOW_H
#include "Arduino.h"
#include <SGL.h>
#include <SSD1331.h>
#include <SPI.h>




class SSD1331_S_SHOW_class
{
private:


public:
	void init(void);
	void show_line(int line, const char * str);
	void erase_line(int line);
	void erase_screen(void);
};

extern SSD1331_S_SHOW_class SSD1331_S_SHOW;

#endif