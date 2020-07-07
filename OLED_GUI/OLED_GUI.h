#ifndef _OLED_GUI_h
#define _OLED_GUI_h
#include <Arduino.h>
//#include <OLED80XX.h>

#define NOM_DIS 1
#define INVER_DIS 0


#define INT_CTRL 0
#define FLOAT_CTRL 1
#define STRING_CTRL 2
#define CHAR_CTRL 3
#define ENUM_CTRL 4

#define CURSORMODE 0
#define CHANGEMODE 1
#define RETURNYES 3 
#define RETURNNO 4

#define MAXPAGECTRL 50

class OLED_GUI_CLASS
{
public:
	void setPara(int f_resX, int f_resY, int f_fontSizeX, int f_fontSizeY);
	int resX = 128;
	int resY = 8;
	int fontSizeX = 6;
	int fontSizeY = 8;
	int winTitleX = 5 * 6;
	int winTitleY = 0;
	int winValueX = 3 * 6;
	int winValueY = 3;
	int winTitleLen = 10;
	int winValueLen = 10;
	int winYesX = 2 * 6;
	int winYesY = 5;
	int winNoX = 10 * 6;
	int winNoY = 5;

	void(*SmallString_len) (unsigned char, unsigned char, String, char, unsigned char);
	void(*SmallInt)(unsigned char, unsigned char, long, byte, char);
	void(*SmallChar)(unsigned char, unsigned char, unsigned char, char);
	void(*ClearScreen)();

private:

};

class OLED_GUI_CTRL_BASE
{
public:
	byte type;
	String value_s = "";
	long value_l;
	float value_f;
	unsigned char value_c;
	String name = "";
	byte X; //X position (pixel num).  
	byte Y; // Y position (line num).
	byte dispLen; //Olny works on type STRING_CTRL.
	byte inverFlag = NOM_DIS;
	byte showFlag = false; //Flag indicate this object is showing.
	byte cursorFlag = false;

	OLED_GUI_CTRL_BASE();
	OLED_GUI_CTRL_BASE(byte f_type, String v_s, long v_l, float v_f, unsigned char v_c, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor = false);

	void refresh(byte invert = false);
	void hide();
private:
};

class OLED_GUI_CTRL_INT : public OLED_GUI_CTRL_BASE
{
public:
	long value;
	OLED_GUI_CTRL_INT(long f_value, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor = false);
	OLED_GUI_CTRL_INT();
	long maxVal;
	long minVal;
	void operator=(const long A);
private:
};

class OLED_GUI_CTRL_STRING : public OLED_GUI_CTRL_BASE
{
public:
	String value;
	//String value;
	OLED_GUI_CTRL_STRING(String s_value, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor = false);
	OLED_GUI_CTRL_STRING();

	void operator=(const String & A);
private:
};

class OLED_GUI_CTRL_ENUM : public OLED_GUI_CTRL_BASE
{
public:
	//String value;
	int value;
	//String value;
	String * enumNameArray;
	int enumNum;
	OLED_GUI_CTRL_ENUM(int valueIn,String * enumNameArrayIn,int enumNumIn, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor = false);
	OLED_GUI_CTRL_ENUM();

	void operator=(int A);
private:
};


class OLED_GUI_CTRL_FLOAT : public OLED_GUI_CTRL_BASE
{
public:
	float value;
	float maxVal;
	float minVal;
	byte decimal = 2;
	OLED_GUI_CTRL_FLOAT(float f_value, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag, byte f_show, byte dec = 2, byte f_cursor = false);
	OLED_GUI_CTRL_FLOAT();
	void operator=(const float A);
private:
};

class OLED_GUI_PAGE
{
public:
	int ctrlNum;
	int cursorCtrlNum;
	int curCursor;

	byte CtrlType[MAXPAGECTRL];
	void * Ctrl[MAXPAGECTRL];
	byte cursorCtrlType[MAXPAGECTRL];
	void * cursorCtrl[MAXPAGECTRL];

	OLED_GUI_PAGE();

	void append(OLED_GUI_CTRL_INT * ctrl);
	void append(OLED_GUI_CTRL_FLOAT * ctrl);
	void append(OLED_GUI_CTRL_STRING * ctrl);
	void append(OLED_GUI_CTRL_ENUM * ctrl);
	void show();
	void hide();
	void nextCursor();
	void lastCursor();
	void showCursor();
	void hideCursor();
	int enter();
private:
	void dispCtrl(byte type, void * ctrl, byte invert = false);
	void hideCtrl(byte type, void * ctrl);
};

class OLED_GUI_FLOAT_CHANGE_PAGE : public OLED_GUI_PAGE
{
public:
	//OLED_GUI_FLOAT_CHANGE_PAGE(byte f_intNum, byte f_decNum);
	//OLED_GUI_FLOAT_CHANGE_PAGE();
	void init();
	float value;
	//float origValue;
	OLED_GUI_CTRL_FLOAT * origCtrl;
	float maxValue;
	float minValue;
	byte intNum;
	byte decNum;
	byte mode;
	void operator=(const float &A);
	OLED_GUI_CTRL_INT * numUp;
	OLED_GUI_CTRL_INT * numDown;
	byte left();
	byte right();
	byte up();
	byte down();
	byte enter();
	//void setAndShow(float f_value, String title, float max, float min);
	void showCtrl(OLED_GUI_CTRL_FLOAT * pCtrl, String title = "");
private:
	void showNumUpDown();
	void hideNumUpDown();
	void cursorInc();
	void cursorDec();
	void float2ctrl();
	//byte getFloatNum(float val, int pos);
};

class OLED_GUI_INT_CHANGE_PAGE : public OLED_GUI_PAGE 
{
public:
	//OLED_GUI_FLOAT_CHANGE_PAGE(byte f_intNum, byte f_decNum);
	//OLED_GUI_INT_CHANGE_PAGE();
	void init();
	long value;
	OLED_GUI_CTRL_INT * origCtrl;
	int maxValue;
	int minValue;
	byte intNum;
	byte mode;
	void operator=(const long &A);
	OLED_GUI_CTRL_INT * numUp;
	OLED_GUI_CTRL_INT * numDown;
	byte left();
	byte right();
	byte up();
	byte down();
	byte enter();
	//void setAndShow(float f_value, String title, float max, float min);
	void showCtrl(OLED_GUI_CTRL_INT * pCtrl, String title = "");
private:
	void showNumUpDown();
	void hideNumUpDown();
	void cursorInc();
	void cursorDec();
	void int2ctrl();
	//byte getFloatNum(float val, int pos);
};


class OLED_GUI_STRING_CHANGE_PAGE : public OLED_GUI_PAGE
{
public:
	//OLED_GUI_STRING_CHANGE_PAGE();
	void init();
	String value;
	//String origValue;
	OLED_GUI_CTRL_STRING * origCtrl;
	byte mode;
	void operator=(const String &A);
	OLED_GUI_CTRL_STRING * charUp;
	OLED_GUI_CTRL_STRING * charDown;
	byte left();
	byte right();
	byte up();
	byte down();
	byte enter();
	//void setAndShow(String val, String title);
	void showCtrl(OLED_GUI_CTRL_STRING * pCtrl, String title = "");

private:
	void showCharUpDown();
	void hideCharUpDown();
	void cursorInc();
	void cursorDec();
	void float2ctrl();
	byte getFloatNum(float val, int pos);

};

class OLED_GUI_INFO_PAGE : public OLED_GUI_PAGE
{
public:
	void init(int buttonNum = 2);
	byte mode;
	byte left();
	byte right();
	byte up();
	byte down();
	byte enter();
	void showCtrl(String title = "");

private:
	void cursorInc();
	void cursorDec();
	void float2ctrl();
	byte getFloatNum(float val, int pos);

};

class OLED_GUI_ENUM_PAGE : public OLED_GUI_PAGE
{
public:
	void init();
	byte mode;
	byte left();
	byte right();
	byte up();
	byte down();
	byte enter();
	int value;
	OLED_GUI_CTRL_ENUM * origCtrl;
	void showCtrl(OLED_GUI_CTRL_ENUM * pCtrl,String title = "");

private:
	void cursorInc();
	void cursorDec();
	void float2ctrl();
	byte getFloatNum(float val, int pos);

};



extern OLED_GUI_CLASS OLED_GUI;

#endif // !_OLED_GUI_h
