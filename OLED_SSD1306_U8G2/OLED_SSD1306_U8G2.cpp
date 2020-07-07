#include <OLED_SSD1306_U8X8.h>
//#include <U8x8lib.h>
#include <U8g2lib.h>
#include <SPI.h>

#define _FONT_ u8x8_font_chroma48medium8_r
#define MAX_TEMP_STR 12
char temp_str[MAX_TEMP_STR]="";
U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI * u8g2;

void OLED_Class_SSD1306::Init(uint8_t cs, uint8_t dc, uint8_t reset, uint8_t rotation )
{
	u8g2 = new U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI(cs, dc, reset);
	u8g2->begin();
	switch (rotation)
	{
	case 0:
	u8g2->setDisplayRotation(U8G2_R0);
		break;
	case 1:
	u8g2->setDisplayRotation(U8G2_R1);
		break;
	case 2:
	u8g2->setDisplayRotation(U8G2_R2);
		break;
	case 3:
	u8g2->setDisplayRotation(U8G2_R3);
		break;
	}
	u8g2->setPowerSave(0);
	u8g2->setFont(u8g2_font_6x10_tf); 
	u8g2->setDrawColor(1)
	u8g2->setInverseFont(0);
}

void OLED_Class_SSD1306::ClearScreen()  
{  
	u8g2->clearDisplay();
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
