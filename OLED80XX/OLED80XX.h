#ifndef _OLED80XX_h
#define _OLED80XX_h
#include "Arduino.h"

#define NOM_DIS 1
#define INVER_DIS 0

class OLED80XX_Class
{
  private:
  public:
	// void Init();
	void Init(byte _CS, byte _RES, byte _RW, byte _DC, byte _RD, byte *data, byte reverse);
	void Set_Pos(unsigned char x, unsigned char y);
	void ClearLine(unsigned char y);
	void ClearScreen();
	void BigChar(unsigned char x, unsigned char y, unsigned char chr, char mode);
	void SmallChar(unsigned char x, unsigned char y, unsigned char chr, char mode);
	void SmallString(unsigned char x, unsigned char y, char *chr, char mode);
	void SmallString(unsigned char x, unsigned char y, String chr, char mode);
	void SmallString_len(unsigned char x, unsigned char y, char *chr, char mode, unsigned char len);
	void SmallString_len(unsigned char x, unsigned char y, String chr, char mode, unsigned char len);
	void BigString(unsigned char x, unsigned char y, char *chr, char mode);
	void BigString_len(unsigned char x, unsigned char y, char *chr, char mode, unsigned char len);
	void SmallInt(unsigned char x, unsigned char y, long data, byte length, char mode);
	void BigInt(unsigned char x, unsigned char y, long data, byte length, char mode);
};

extern OLED80XX_Class OLED80XX;
#endif
