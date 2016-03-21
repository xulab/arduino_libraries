/*
This is a library to display important infomation on a SSD1331 RGB OLED.
To display paras of mouse behavior experiments
In persue of easy use that simply choose what to display and on which line.
only several really useful functions constructed
the first line is 0
96*64resolution, 6*8 each char, 16 chars a line,8 lines available
Must work with two other libraries:SGL, SSD1331

To find out where's been changed to main program,Ctrl+F"SSD1331_S_SHOW"
now displays TrialNum, Stimulation onset signal etc. See in main Program.

*/


#include "SSD1331_S_SHOW.h"
// #include "SSD1331.h"// not original writen by PJW


#define CS     43
#define DC     42
#define MOSI   51
#define SCLK   52
#define RES    40
#define FONT_COLOR COLOR_RED

#define LINE_SIZE 16 //characters of a line

char temp_line_str[LINE_SIZE + 1] = "                ";//the real words that can display
const char empty_line[LINE_SIZE + 1] = "                ";
SSD1331 oled = SSD1331(CS, DC, MOSI, SCLK);  //??

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



//how to write a row of string
void SSD1331_S_SHOW_class::show_line(unsigned int line, const char * str)
{
//    line = line % 8; //overflow donot encounter error
	memcpy(temp_line_str, empty_line, LINE_SIZE +1);//a c++ function.memcpy(destination,source,size)

    int str_len = strlen(str);
    if(str_len < LINE_SIZE) memcpy(temp_line_str, str, str_len +1);
    else memcpy(temp_line_str, str, LINE_SIZE);// now if str is too long there is no new line

	oled.drawString(temp_line_str, 0,line * 8, 1, FONT_COLOR);//(string,x,y,text_size,color) 
}



void SSD1331_S_SHOW_class::erase_line(unsigned int line)
{
//	show_line(line, empty_line);
    oled.fillRectangle(0,line*8,96,8,COLOR_BLACK);//(x,y,width.height,color),attention tiny math error
}


void SSD1331_S_SHOW_class::erase_screen()
{
	oled.fillScreen(COLOR_BLACK);
}

// void SSD1331_S_SHOW_class::draw_char(uint8_t ascii)
// {
//     oled.drawChar(ascii,6,8,1,COLOR_BLUE);
// }

SSD1331_S_SHOW_class SSD1331_S_SHOW;




