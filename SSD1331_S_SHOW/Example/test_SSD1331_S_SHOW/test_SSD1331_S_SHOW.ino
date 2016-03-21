/*

*/

#include <SGL.h>
#include <SSD1331.h>
#include <SPI.h>
#include "SSD1331_S_SHOW.h"


// #define CS     43
// #define DC     42
// #define MOSI   51
// #define SCLK   52
// #define RES    41

// SSD1331 Display = SSD1331(CS, DC, MOSI, SCLK);  //

void setup() {
	Serial.begin(9600);
	short TrialNum = 183;

	// Serial.print("TrialNum is ");
	// Serial.println(TrialNum);
	char str_TrialNum[16];
        
	SSD1331_S_SHOW.init();
	// SSD1331_S_SHOW.show_line(0,"hello world!");
	// SSD1331_S_SHOW.show_line(1, const char * TrialNum);
	SSD1331_S_SHOW.show_line(1,"this is a demohasganghahksgh");
    SSD1331_S_SHOW.show_line(2,"3225");
    // SSD1331_S_SHOW.show_line(3,TrialNum.ToString());
	// SSD1331_S_SHOW.show_line(10,"there is no line 10. hingnhaghaashgasgh");


	// delay(2000);
	
    // delay(1500);
    // SSD1331_S_SHOW.erase_line(0);
    // delay(1500);
    // SSD1331_S_SHOW.erase_line(1);


    SSD1331_S_SHOW.erase_line(2);
        for (TrialNum=180;TrialNum<200;TrialNum++){
          SSD1331_S_SHOW.erase_line(2);
          sprintf( str_TrialNum,"%s%3d","Trial",TrialNum);//transform almost any type to string.
          //fill the words you will display
          SSD1331_S_SHOW.show_line(2,str_TrialNum);
          delay(1000);
        }

        //To show other variables?
	// oled.setScolling(Vertical,0,64,0);
	// SSD1331_S_SHOW.draw_char(M);
//	SSD1331_S_SHOW.erase_screen();
}

void loop() {

    // SSD1331_S_SHOW.show_line(0,"hello world!");    
    // SSD1331_S_SHOW.show_line(1,"hello world!");  
    // SSD1331_S_SHOW.show_line(2,"hello world!");  
    // SSD1331_S_SHOW.show_line(3,"hello world!");  
    // SSD1331_S_SHOW.show_line(4,"hello world!");  
    // delay (1000);
    // SSD1331_S_SHOW.erase_line(0);
    // SSD1331_S_SHOW.erase_line(1);
    // SSD1331_S_SHOW.erase_line(2);
    // SSD1331_S_SHOW.erase_line(3);
    // SSD1331_S_SHOW.erase_line(4);
    // delay(1000);

}

