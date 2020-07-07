#include <OLED_SSD1306_ADAFRUIT_S_SHOW.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define _FONT_ u8x8_font_chroma48medium8_r
#define MAX_TEMP_STR 12
char temp_str[MAX_TEMP_STR] = "";
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define FONT_X 6
#define FONT_Y 8 

Adafruit_SSD1306 * display;

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::Init(uint8_t cs, uint8_t mosi, uint8_t clk, uint8_t dc, uint8_t reset, uint8_t rotation)
{
	display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, mosi, clk, dc, reset, cs);
	//u8g2->begin();
	display->begin();
	display->setRotation(rotation);
	display->setTextSize(1);             // Normal 1:1 pixel scale
	display->setTextColor(WHITE, BLACK);        // Draw white text
	display->display();
	delay(1000); // Pause for 2 seconds
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::ClearScreen()
{
	display->clearDisplay();
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallChar(unsigned char x, unsigned char y, unsigned char chr, char mode)
{
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->write(chr);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallString(unsigned char x, unsigned char y, char *chr, char mode)
{
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->print(chr);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallString(unsigned char x, unsigned char y, String chr, char mode)
{
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->print(chr);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallString_len(unsigned char x, unsigned char y, char *chr, char mode, unsigned char len)
{
	String tmp = chr;
	if (tmp.length() > len) tmp.remove(len, tmp.length() - len);
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->print(tmp);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallString_len(unsigned char x, unsigned char y, String chr, char mode, unsigned char len)
{
	if (chr.length() > len) chr.remove(len, chr.length() - len);
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->print(chr);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

void OLED_Class_SSD1306_ADAFRUIT_S_SHOW::SmallInt(unsigned char x, unsigned char y, long data, byte length, char mode)
{
	String tmp = String(abs(data));
	for (int i = length - tmp.length(); i > 0; i--) tmp = String('0') + tmp;
	if (data < 0) tmp = String('-') + tmp;
	if (mode == INVER_DIS) display->setTextColor(BLACK, WHITE); // Draw 'inverse' text
	display->setCursor(x, y * FONT_Y);
	display->print(tmp);
	display->display();
	if (mode == INVER_DIS) display->setTextColor(WHITE, BLACK);        // Draw white text
}

OLED_Class_SSD1306_ADAFRUIT_S_SHOW OLED_SSD1306;
