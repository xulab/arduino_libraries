#include <OLED80XX.h>
#include <OLED_GUI.h>

#define NOCHANGE 0
#define LEFT 1
#define RIGHT 2
#define BUTTONDOWN 3
#define BUTTONUP 4

#define ANGLELEN 6 
#define VALUELEN 11
#define SEPARATCHAR " "

#define MAINPAGE 0
#define SETPAGE 1
#define SETPAGECHAN 2


/*********Pin define************/
byte _CS = 38;
byte _RES = 39;
byte _RW = 14;
byte _DC = 13;
byte _RD = 15;
byte _DATAPORT[8] = { 16, 17, 18, 19, 20, 21, 22, 23 };
byte XA_in = 6;
byte XB_in = 7;
byte YA_in = 8;
byte YB_in = 9;
byte ZA_in = 10;
byte ZB_in = 11;
byte VXA = 12;
byte VXB = 24;
byte VYA = 25;
byte VYB = 26;
byte VZA = 27;
byte VZB = 28;
byte rotatorA = 32;
byte rotatorB = 31;
byte rotatorButton = 30;


volatile long rotatorPosition = 0;
volatile byte rotatorFlag = NOCHANGE;


//void readRotator(volatile long * pos, byte A, byte B)
//{
//	*pos = *pos & (~0B11);
//	*pos = *pos | digitalRead(A) | (digitalRead(B) << 1);
//}
//
//void intRotatorA()
//{
//	long lastPos = rotatorPosition;
//	readRotator(&rotatorPosition, rotatorA, rotatorB);
//	long change = rotatorPosition - lastPos;
//	if (change >= 3) rotatorPosition = rotatorPosition - 4;
//	if (change <= -3) rotatorPosition = rotatorPosition + 4;
//	if (change > 0) rotatorFlag = LEFT;
//	if (change < 0) rotatorFlag = RIGHT;
//}
//
void intRotatorA()
{
	byte tmp = digitalRead(rotatorA) + (digitalRead(rotatorB) << 1);
	switch (tmp)
	{
	case 0:
		rotatorPosition++;
		rotatorFlag = RIGHT;
		break;
	case 1:
		rotatorPosition--;
		rotatorFlag = LEFT;
		break;
	case 2:
		rotatorPosition--;
		rotatorFlag = LEFT;
		break;
	case 3:
		rotatorPosition++;
		rotatorFlag = RIGHT;
		break;
	}
}

void intRotatorButton()
{
	if (digitalRead(rotatorButton)) rotatorFlag = BUTTONDOWN;
	else rotatorFlag = BUTTONUP;
}

/*************wrap OLED driver***********/
inline void SmallString_len_wrap(unsigned char x, unsigned char y, String chr, char mode, unsigned char len)
{
	OLED80XX.SmallString_len(x, y, chr, mode, len);
}
inline void SmallInt_wrap(unsigned char x, unsigned char y, long data, byte length, char mode)
{
	OLED80XX.SmallInt(x, y, data, length, mode);
}

inline void SmallChar_wrap(unsigned char x, unsigned char y, unsigned char chr, char mode)
{
	OLED80XX.SmallChar(x, y, chr, mode);
}

inline void ClearScreen_wrap()
{
	OLED80XX.ClearScreen();
}



void setup() {
	//put your setup code here, to run once:

	pinMode(_DC, OUTPUT);
	digitalWrite(_DC, LOW);
	digitalWrite(_DC, HIGH);
	digitalWrite(_DC, LOW);
	digitalWrite(_DC, HIGH);
	digitalWrite(_DC, LOW);

	OLED80XX.Init(_CS, _RES, _RW, _DC, _RD, _DATAPORT, 1);
	OLED80XX.ClearScreen();
	/*OLED_GUI setting*/
	OLED_GUI.resX = 128;
	OLED_GUI.resY = 8;
	OLED_GUI.fontSizeX = 6;
	OLED_GUI.fontSizeY = 8;
	OLED_GUI.winTitleX = 5*6;
	OLED_GUI.winTitleY = 0;
	OLED_GUI.winValueX = 3*6;
	OLED_GUI.winValueY = 3 ;
	OLED_GUI.winTitleLen = 10;
	OLED_GUI.winValueLen = 10;
	OLED_GUI.winYesX = 2*6;
	OLED_GUI.winYesY = 5;
	OLED_GUI.winNoX = 10*6;
	OLED_GUI.winNoY = 5;


	OLED_GUI.ClearScreen = ClearScreen_wrap;
	OLED_GUI.SmallChar = SmallChar_wrap;
	OLED_GUI.SmallInt = SmallInt_wrap;
	OLED_GUI.SmallString_len = SmallString_len_wrap;

	pinMode(rotatorA, INPUT);
	pinMode(rotatorB, INPUT);
	pinMode(rotatorButton, INPUT);
	attachInterrupt(digitalPinToInterrupt(rotatorA), intRotatorA, CHANGE);
	attachInterrupt(digitalPinToInterrupt(rotatorButton), intRotatorButton, CHANGE);

	/*********ctrl test************/
	OLED_GUI_CTRL_STRING testCtrl2("2", 0 * 6, 1, 20, NOM_DIS, false, false);
	testCtrl2.refresh();
	OLED_GUI_CTRL_STRING testCtrl3("3", 1 * 6, 1, 20, NOM_DIS, false, false);
	testCtrl3.refresh();
	OLED_GUI_CTRL_STRING testCtrl4("4", 2 * 6, 1, 20, NOM_DIS, false, false);
	testCtrl4.refresh();
	delay(1000);
	testCtrl2.hide();
	testCtrl3.hide();
	testCtrl4.hide();
	delay(1000);
	OLED_GUI_CTRL_INT testCtrl5(5, 0 * 6, 2, 0, NOM_DIS, false, false);
	testCtrl5.refresh();
	OLED_GUI_CTRL_INT testCtrl6(6, 1 * 6, 2, 0, NOM_DIS, false, false);
	testCtrl6.refresh();
	OLED_GUI_CTRL_INT testCtrl7(7, 2 * 6, 2, 0, NOM_DIS, false, false);
	testCtrl7.refresh();
	delay(1000);
	testCtrl5.hide();
	testCtrl6.hide();
	testCtrl7.hide();
	delay(1000);

	/*********mainpage************/
	//OLED_GUI_CTRL_STRING save("save", 0 * 6, 0, 4, NOM_DIS, false, true);
	OLED_GUI_CTRL_STRING save;
	save = "save";
	save.X = 0*6;
	save.Y = 0;
	save.dispLen = 4;
	save.cursorFlag = true;
	//OLED_GUI_CTRL_STRING load("load", 7 * 6, 0, 4, NOM_DIS, false, true);
	//OLED_GUI_CTRL_STRING reset("reset", 14 * 6, 0, 5, NOM_DIS, false, true);
	//OLED_GUI_CTRL_STRING mainPage_H("H", 0 * 6, 1, 1, NOM_DIS, false, false);
	OLED_GUI_CTRL_FLOAT angle_H(-123.1, 3 * 6, 1, ANGLELEN, NOM_DIS, false, 1, true);
	//OLED_GUI_CTRL_FLOAT angle_H;
	angle_H = -123.1;
	angle_H.X = 3*6;
	angle_H.Y = 1;
	angle_H.decimal = 1;
	angle_H.dispLen = ANGLELEN;
	angle_H.cursorFlag = true;
	angle_H.maxVal = 500;
	angle_H.minVal = -500;
	angle_H.name = "angle_H";
	//OLED_GUI_CTRL_STRING mainPage_V("V", 12 * 6, 1, 1, NOM_DIS, false, false);
	//OLED_GUI_CTRL_FLOAT angle_V(-456.5, 15 * 6, 1, ANGLELEN, NOM_DIS, false, 1, true);
	//OLED_GUI_CTRL_STRING origX_name("origX", 0 * 6, 2, 5, NOM_DIS, false, false);
	//OLED_GUI_CTRL_STRING origY_name("origY", 0 * 6, 3, 5, NOM_DIS, false, false);
	//OLED_GUI_CTRL_STRING origZ_name("origZ", 0 * 6, 4, 5, NOM_DIS, false, false);
	//OLED_GUI_CTRL_STRING virtualX_name("virtualX", 0 * 6, 5, 8, NOM_DIS, false, false);
	//OLED_GUI_CTRL_STRING virtualY_name("virtualY", 0 * 6, 6, 8, NOM_DIS, false, false);
	//OLED_GUI_CTRL_STRING virtualZ_name("virtualZ", 0 * 6, 7, 8, NOM_DIS, false, false);
	//OLED_GUI_CTRL_INT origX(0, 9 * 6, 2, VALUELEN, NOM_DIS, false, false);
	//OLED_GUI_CTRL_INT origY(0, 9 * 6, 3, VALUELEN, NOM_DIS, false, false);
	//OLED_GUI_CTRL_INT origZ(0, 9 * 6, 4, VALUELEN, NOM_DIS, false, false);
	OLED_GUI_CTRL_INT virtualX(0, 9 * 6, 5, VALUELEN, NOM_DIS, false, false);
	virtualX = 987;
	virtualX.X = 9 * 6;
	virtualX.Y = 5;
	virtualX.dispLen = VALUELEN;
	virtualX.cursorFlag = false;
	virtualX.maxVal = 5000;
	virtualX.minVal = -5000;
	angle_H.name = "virtualX";


	//OLED_GUI_CTRL_INT virtualY(0, 9 * 6, 6, VALUELEN, NOM_DIS, false, false);
	//OLED_GUI_CTRL_INT virtualZ(0, 9 * 6, 7, VALUELEN, NOM_DIS, false, false);

	OLED_GUI_PAGE mainPage;
	mainPage.append(&save);
	//mainPage.append(&load);
	//mainPage.append(&reset);
	//mainPage.append(&mainPage_H);
	mainPage.append(&angle_H);
	//mainPage.append(&mainPage_V);
	//mainPage.append(&angle_V);
	//mainPage.append(&origX_name);
	//mainPage.append(&origY_name);
	//mainPage.append(&origZ_name);
	//mainPage.append(&virtualX_name);
	//mainPage.append(&virtualY_name);
	//mainPage.append(&virtualZ_name);
	//mainPage.append(&origX);
	//mainPage.append(&origY);
	//mainPage.append(&origZ);
	mainPage.append(&virtualX);
	//mainPage.append(&virtualY);
	//mainPage.append(&virtualZ);
	mainPage.show();
	delay(1000);
	mainPage.nextCursor();
	delay(1000);
	mainPage.nextCursor();
	delay(1000);
	mainPage.lastCursor();
	delay(1000);
	save = "ABCD";
	delay(1000);
	angle_H = -987.65;
	delay(1000);
	mainPage.hide();
	delay(1000);

	/*********float page************/
	OLED_GUI_FLOAT_CHANGE_PAGE fPage;
	fPage.append(new OLED_GUI_CTRL_STRING("*****", 0*6, 0, 5, NOM_DIS, false, false));
	fPage.append(new OLED_GUI_CTRL_STRING("*****", 16*6, 0, 5, NOM_DIS, false, false));
	fPage.append(new OLED_GUI_CTRL_STRING("*********************", 0*6, 7, 21, NOM_DIS, false, false));
	
	fPage.showCtrl(&angle_H);
	delay(1000);
	fPage.right();
	delay(1000);
	fPage.right();
	delay(1000);
	fPage.right();
	delay(1000);
	fPage.left();
	delay(1000);
	fPage.left();
	delay(1000);
	fPage.enter();
	delay(1000);
	fPage.left();
	delay(1000);
	fPage.left();
	delay(1000);
	fPage.right();
	delay(1000);
	fPage.enter();
	delay(1000);
	fPage.hide();
	delay(1000);

	/*********int page************/
	OLED_GUI_INT_CHANGE_PAGE iPage;
	iPage.append(new OLED_GUI_CTRL_STRING("*****", 0*6, 0, 5, NOM_DIS, false, false));
	iPage.append(new OLED_GUI_CTRL_STRING("*****", 16*6, 0, 5, NOM_DIS, false, false));
	iPage.append(new OLED_GUI_CTRL_STRING("*********************", 0*6, 7, 21, NOM_DIS, false, false));
	
	iPage.showCtrl(&virtualX);
	delay(1000);
	iPage.right();
	delay(1000);
	iPage.right();
	delay(1000);
	iPage.right();
	delay(1000);
	iPage.left();
	delay(1000);
	iPage.left();
	delay(1000);
	iPage.enter();
	delay(1000);
	iPage.left();
	delay(1000);
	iPage.left();
	delay(1000);
	iPage.right();
	delay(1000);
	iPage.enter();
	delay(1000);
	iPage.hide();
	delay(1000);

	/*********String page************/
	OLED_GUI_STRING_CHANGE_PAGE sPage;
	sPage.append(new OLED_GUI_CTRL_STRING("*****", 0*6, 0, 5, NOM_DIS, false, false));
	sPage.append(new OLED_GUI_CTRL_STRING("*****", 16*6, 0, 5, NOM_DIS, false, false));
	sPage.append(new OLED_GUI_CTRL_STRING("*********************", 0*6, 7, 21, NOM_DIS, false, false));
	sPage.showCtrl(&save);
	delay(1000);
	sPage.right();
	delay(1000);
	sPage.right();
	delay(1000);
	sPage.right();
	delay(1000);
	sPage.left();
	delay(1000);
	sPage.left();
	delay(1000);
	sPage.enter();
	delay(1000);
	sPage.left();
	delay(1000);
	sPage.left();
	delay(1000);
	sPage.right();
	delay(1000);
	sPage.enter();
	delay(1000);
	sPage.hide();
	delay(1000);
}


void loop() {

	// put your main code here, to run repeatedly:

}
