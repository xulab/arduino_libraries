#include <OLED_SSD1306_U8X8.h>

byte _CS = 10;
byte _RES = 17;
byte _DC = 16;

void setup() {
	//put your setup code here, to run once:
 	OLED_SSD1306.Init(_CS, _DC, _RES, 0);
	OLED_SSD1306.ClearScreen();
	Serial.begin(9600);
  OLED_SSD1306.SmallString(2*6,0,"hello world!",NOM_DIS);
  OLED_SSD1306.SmallString(2*6,2,"hello world!",INVER_DIS);
  OLED_SSD1306.SmallInt(0*6,3,20,2,NOM_DIS);
}

void loop() {
	// put your main code here, to run repeatedly:

}
