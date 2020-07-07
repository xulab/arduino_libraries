#ifndef _OLED_SSD1306_U8X8_h
#define _OLED_SSD1306_U8X8_h
#include "Arduino.h"

#define NOM_DIS 1
#define INVER_DIS 0

class OLED_Class_SSD1306
{
private:

public:
	void Init(uint8_t cs, uint8_t dc, uint8_t reset, uint8_t rotation = false);
	void Set_Pos(unsigned char x, unsigned char y);
	void ClearLine(unsigned char y);
	void ClearScreen();
	void SmallChar(unsigned char x, unsigned char y, unsigned char chr, char mode);
	void SmallString(unsigned char x, unsigned char y, char *chr, char mode);
	void SmallString(unsigned char x, unsigned char y, String chr, char mode);
	void SmallString_len(unsigned char x, unsigned char y, char *chr, char mode, unsigned char len);
	void SmallString_len(unsigned char x, unsigned char y, String chr, char mode, unsigned char len);
	void SmallInt(unsigned char x, unsigned char y, long data, byte length, char mode);
};

extern OLED_Class_SSD1306 OLED_SSD1306;
#endif
