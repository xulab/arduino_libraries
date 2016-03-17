#include "SSD1331_S_SHOW.h"

#define CS     43
#define DC     42
#define MOSI   51
#define SCLK   52
#define RES    41
#define FONT_COLOR COLOR_RED

#define LINE_SIZE 16
char temp_line_str[LINE_SIZE + 1] = "                ";
const char empty_line[LINE_SIZE + 1] = "                ";
SSD1331 oled = SSD1331(CS, DC, MOSI, SCLK);  

void SSD1331_S_SHOW_class::init()
{
	pinMode(RES, OUTPUT);
    digitalWrite(RES, HIGH);
    delay(1);
    digitalWrite(RES, LOW);
    delay(1);
    digitalWrite(RES, HIGH);
    delay(1);
    oled.init();
    oled.fillScreen(COLOR_BLACK);
}


void SSD1331_S_SHOW_class::show_line(int line, const char * str)
{
	memcpy(temp_line_str, empty_line, LINE_SIZE +1);
	memcpy(temp_line_str, str, strlen(str) +1);
	oled.drawString(temp_line_str, 0,line * 8, 1, FONT_COLOR);
}



void SSD1331_S_SHOW_class::erase_line(int line)
{
	show_line(line, empty_line);
}


void SSD1331_S_SHOW_class::erase_screen()
{
	oled.fillScreen(COLOR_BLACK);
}


SSD1331_S_SHOW_class SSD1331_S_SHOW;




