#include <OLED_SSD1306_U8X8.h>
#include <U8x8lib.h>
#include <SPI.h>

#define _FONT_ u8x8_font_chroma48medium8_r
#define MAX_TEMP_STR 12
char temp_str[MAX_TEMP_STR]="";
U8X8_SSD1306_128X64_NONAME_4W_HW_SPI * u8x8;

void OLED_Class_SSD1306::Init(uint8_t cs, uint8_t dc, uint8_t reset, uint8_t rotation )
{
	u8x8 = new U8X8_SSD1306_128X64_NONAME_4W_HW_SPI(cs, dc, reset);
	u8x8->begin();
	u8x8->setFlipMode(rotation);
	u8x8->setPowerSave(0);
	u8x8->setFont(u8x8_font_chroma48medium8_r);
	u8x8->setInverseFont(0);
}

void OLED_Class_SSD1306::ClearLine(unsigned char y)
{
	u8x8->clearLine(y);
}

void OLED_Class_SSD1306::ClearScreen()  
{  
	u8x8->clear();
}

  

void OLED_Class_SSD1306::SmallChar(unsigned char x,unsigned char y,unsigned char chr, char mode)
{      	
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawGlyph(x/8, y, chr);
	u8x8->setInverseFont(0);
}

void OLED_Class_SSD1306::SmallString(unsigned char x,unsigned char y, char *chr,char mode)
{
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawString(x / 8, y, chr);
	u8x8->setInverseFont(0);
}


void OLED_Class_SSD1306::SmallString(unsigned char x,unsigned char y, String chr,char mode)
{
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawString(x / 8, y, chr.c_str());
	u8x8->setInverseFont(0);
}

void OLED_Class_SSD1306::SmallString_len(unsigned char x,unsigned char y, char *chr,char mode,unsigned char len)
{
	String tmp = chr;
	if (tmp.length() > len) tmp.remove(len, tmp.length() - len);
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawString(x / 8, y, tmp.c_str());
	u8x8->setInverseFont(0);
}

void OLED_Class_SSD1306::SmallString_len(unsigned char x,unsigned char y, String chr,char mode,unsigned char len)
{
	if (chr.length() > len) chr.remove(len, chr.length() - len);
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawString(x / 8, y, chr.c_str());
	u8x8->setInverseFont(0);
}
void OLED_Class_SSD1306::SmallInt(unsigned char x,unsigned char y,long data, byte length,char mode)
{
	String tmp = String(abs(data));
	for (int i = length - tmp.length(); i > 0; i--) tmp = String('0') + tmp;
	if (data < 0) tmp = String('-') + tmp;
	if (mode == INVER_DIS) u8x8->setInverseFont(1);
	u8x8->drawString(x / 8, y, tmp.c_str());
	u8x8->setInverseFont(0);
}

OLED_Class_SSD1306 OLED_SSD1306;
