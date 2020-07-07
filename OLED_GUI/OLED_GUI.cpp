#include <OLED_GUI.h>
volatile byte refreshFlag = false;

class OLED_GUI_CLASS OLED_GUI;

void OLED_GUI_CTRL_BASE::refresh(byte invert) {
	if (refreshFlag) {
		return;
	}
	else {

		refreshFlag = true;
		byte tmpLen = this->dispLen;
		byte tmpInvert = invert ? !this->inverFlag : this->inverFlag;

		(*OLED_GUI.SmallString_len)(this->X, this->Y, "                      ", NOM_DIS, tmpLen);

		switch (this->type) {
		case STRING_CTRL:
			(*OLED_GUI.SmallString_len)(this->X, this->Y, this->value_s, tmpInvert, tmpLen);
			break;
		case INT_CTRL:
			if (this->value_l >= 0) tmpLen++;
			(*OLED_GUI.SmallInt)(this->X, this->Y, this->value_l, tmpLen, tmpInvert);
			break;
		case FLOAT_CTRL:
			(*OLED_GUI.SmallString_len)(this->X, this->Y, this->value_s, tmpInvert, tmpLen);
			break;
		case CHAR_CTRL:
			(*OLED_GUI.SmallChar)(this->X, this->Y, this->value_c, tmpInvert);
			break;
		case ENUM_CTRL:
			(*OLED_GUI.SmallString_len)(this->X, this->Y, this->value_s, tmpInvert, tmpLen);
			break;
		}
		refreshFlag = false;
	}
}

void OLED_GUI_CTRL_BASE::hide() {
	byte tmpLen = this->dispLen;
	if (this->type == INT_CTRL) tmpLen++;
	(*OLED_GUI.SmallString_len)(this->X, this->Y, "                      ", NOM_DIS, tmpLen);
}

OLED_GUI_CTRL_BASE::OLED_GUI_CTRL_BASE()
{
}

OLED_GUI_CTRL_BASE::OLED_GUI_CTRL_BASE(byte f_type, String v_s, long v_l, float v_f,
	unsigned char v_c, byte f_X, byte f_Y, byte f_dispLen, byte f_inverFlag,
	byte f_show, byte f_cursor)

	: type(f_type), value_s(v_s), value_l(v_l), value_f(v_f), value_c(v_c),
	X(f_X), Y(f_Y), dispLen(f_dispLen), inverFlag(f_inverFlag), showFlag(f_show),
	cursorFlag(f_cursor)
{
}

OLED_GUI_CTRL_INT::OLED_GUI_CTRL_INT(long f_value, byte f_X, byte f_Y,
	byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor)

	: OLED_GUI_CTRL_BASE(INT_CTRL, "", f_value, 0, 0, f_X, f_Y, f_dispLen,
		f_inverFlag, f_show, f_cursor),

	value(f_value)
{
}

OLED_GUI_CTRL_INT::OLED_GUI_CTRL_INT()
{
	this->type = INT_CTRL;
}

void OLED_GUI_CTRL_INT::operator=(const long A)
{
	this->value = A;
	this->value_l = A;
	if (this->showFlag) this->refresh();
}

OLED_GUI_CTRL_STRING::OLED_GUI_CTRL_STRING(String s_value, byte f_X, byte f_Y,
	byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor)

	: OLED_GUI_CTRL_BASE(STRING_CTRL, s_value, 0, 0, 0, f_X, f_Y, f_dispLen,
		f_inverFlag, f_show, f_cursor),

	value(s_value)
{
}

OLED_GUI_CTRL_STRING::OLED_GUI_CTRL_STRING()
{
	this->type = STRING_CTRL;
}

void OLED_GUI_CTRL_STRING::operator=(const String & A)
{
	this->value = A;
	this->value_s = A;
	if (this->showFlag) this->refresh();
}

OLED_GUI_CTRL_ENUM::OLED_GUI_CTRL_ENUM()
{
	this->type = ENUM_CTRL;
}


OLED_GUI_CTRL_ENUM::OLED_GUI_CTRL_ENUM(int valueIn, String * enumNameArrayIn,int enumNumIn, byte f_X, byte f_Y,
	byte f_dispLen, byte f_inverFlag, byte f_show, byte f_cursor)
	: OLED_GUI_CTRL_BASE(ENUM_CTRL, enumNameArrayIn[valueIn], 0, 0, 0, f_X, f_Y, f_dispLen,
		f_inverFlag, f_show, f_cursor),

	value(valueIn), enumNum(enumNumIn),enumNameArray(enumNameArrayIn)
{
	this->type = ENUM_CTRL;
}

void OLED_GUI_CTRL_ENUM::operator=(int A)
{
	this->value = A;
	this->value_s = enumNameArray[value];
	if (this->showFlag) this->refresh();
}

OLED_GUI_CTRL_FLOAT::OLED_GUI_CTRL_FLOAT(float f_value, byte f_X, byte f_Y,
	byte f_dispLen, byte f_inverFlag, byte f_show, byte dec, byte f_cursor)

	: OLED_GUI_CTRL_BASE(FLOAT_CTRL, String(f_value, dec), 0, f_value, 0, f_X, f_Y,
		f_dispLen, f_inverFlag, f_show, f_cursor),

	value(f_value), decimal(dec)
{
}

OLED_GUI_CTRL_FLOAT::OLED_GUI_CTRL_FLOAT()
{
	this->type = FLOAT_CTRL;
}

void OLED_GUI_CTRL_FLOAT::operator=(const float A)
{
	this->value = A;
	this->value_f = A;
	this->value_s = String(A, this->decimal);
	if (this->showFlag) this->refresh();
}


OLED_GUI_PAGE::OLED_GUI_PAGE() : ctrlNum(0), cursorCtrlNum(0), curCursor(0)
{
}


void OLED_GUI_PAGE::append(OLED_GUI_CTRL_INT * ctrl)
{
	if (ctrl != NULL) {
		if (ctrl->cursorFlag)
		{
			cursorCtrlType[this->cursorCtrlNum] = INT_CTRL;
			cursorCtrl[this->cursorCtrlNum] = (void *)ctrl;
			this->cursorCtrlNum++;
		}
		else {
			CtrlType[this->ctrlNum] = INT_CTRL;
			Ctrl[this->ctrlNum] = (void *)ctrl;
			this->ctrlNum++;
		}
	}
}

void OLED_GUI_PAGE::append(OLED_GUI_CTRL_FLOAT * ctrl)
{
	if (ctrl != NULL) {
		if (ctrl->cursorFlag)
		{
			cursorCtrlType[this->cursorCtrlNum] = FLOAT_CTRL;
			cursorCtrl[this->cursorCtrlNum] = (void *)ctrl;
			this->cursorCtrlNum++;
		}
		else {
			CtrlType[this->ctrlNum] = FLOAT_CTRL;
			Ctrl[this->ctrlNum] = (void *)ctrl;
			this->ctrlNum++;
		}
	}
}

void OLED_GUI_PAGE::append(OLED_GUI_CTRL_STRING * ctrl)
{
	if (ctrl != NULL) {
		if (ctrl->cursorFlag)
		{
			cursorCtrlType[this->cursorCtrlNum] = STRING_CTRL;
			cursorCtrl[this->cursorCtrlNum] = (void *)ctrl;
			this->cursorCtrlNum++;
		}
		else {
			CtrlType[this->ctrlNum] = STRING_CTRL;
			Ctrl[this->ctrlNum] = (void *)ctrl;
			this->ctrlNum++;
		}
	}
}

void OLED_GUI_PAGE::append(OLED_GUI_CTRL_ENUM * ctrl)
{
	if (ctrl != NULL) {
		if (ctrl->cursorFlag)
		{
			cursorCtrlType[this->cursorCtrlNum] = ENUM_CTRL;
			cursorCtrl[this->cursorCtrlNum] = (void *)ctrl;
			this->cursorCtrlNum++;
		}
		else {
			CtrlType[this->ctrlNum] = ENUM_CTRL;
			Ctrl[this->ctrlNum] = (void *)ctrl;
			this->ctrlNum++;
		}
	}
}

void OLED_GUI_PAGE::dispCtrl(byte type, void * ctrl, byte invert)
{
	switch (type)
	{
	case INT_CTRL:
		((OLED_GUI_CTRL_INT *)ctrl)->showFlag = true;
		((OLED_GUI_CTRL_INT *)ctrl)->refresh(invert);
		break;
	case FLOAT_CTRL:
		((OLED_GUI_CTRL_FLOAT *)ctrl)->showFlag = true;
		((OLED_GUI_CTRL_FLOAT *)ctrl)->refresh(invert);
		break;
	case STRING_CTRL:
		((OLED_GUI_CTRL_STRING *)ctrl)->showFlag = true;
		((OLED_GUI_CTRL_STRING *)ctrl)->refresh(invert);
		break;
	case ENUM_CTRL:
		((OLED_GUI_CTRL_ENUM *)ctrl)->showFlag = true;
		((OLED_GUI_CTRL_ENUM *)ctrl)->refresh(invert);
		break;
	default:
		break;
	}
}

void OLED_GUI_PAGE::hideCtrl(byte type, void * ctrl)
{
	switch (type)
	{
	case INT_CTRL:
		((OLED_GUI_CTRL_INT *)ctrl)->showFlag = false;
		((OLED_GUI_CTRL_INT *)ctrl)->hide();
		break;
	case FLOAT_CTRL:
		((OLED_GUI_CTRL_FLOAT *)ctrl)->showFlag = false;
		((OLED_GUI_CTRL_FLOAT *)ctrl)->hide();
		break;
	case STRING_CTRL:
		((OLED_GUI_CTRL_STRING *)ctrl)->showFlag = false;
		((OLED_GUI_CTRL_STRING *)ctrl)->hide();
		break;
	case ENUM_CTRL:
		((OLED_GUI_CTRL_ENUM *)ctrl)->showFlag = false;
		((OLED_GUI_CTRL_ENUM *)ctrl)->hide();
		break;
	default:
		break;
	}

}

void OLED_GUI_PAGE::show()
{
	for (int i = 0; i < ctrlNum; i++)
	{
		dispCtrl(CtrlType[i], Ctrl[i]);
	}

	for (int i = 0; i < cursorCtrlNum; i++)
	{
		dispCtrl(cursorCtrlType[i], cursorCtrl[i]);
	}

	if (cursorCtrlNum > 0) showCursor();
}

void OLED_GUI_PAGE::hide()
{
	//(* OLED_GUI.ClearScreen)();

	for (int i = 0; i < ctrlNum; i++)
	{
		hideCtrl(CtrlType[i], Ctrl[i]);
	}

	for (int i = 0; i < cursorCtrlNum; i++)
	{
		hideCtrl(cursorCtrlType[i], cursorCtrl[i]);
	}
}

void OLED_GUI_PAGE::showCursor()
{
	dispCtrl(cursorCtrlType[curCursor], cursorCtrl[curCursor], true);
}

void OLED_GUI_PAGE::hideCursor()
{
	dispCtrl(cursorCtrlType[curCursor], cursorCtrl[curCursor], false);
}

void OLED_GUI_PAGE::nextCursor()
{
	if (cursorCtrlNum > 0)
	{
		hideCursor();
		curCursor++;
		if (curCursor >= cursorCtrlNum) curCursor = 0;
		showCursor();
	}
}

void OLED_GUI_PAGE::lastCursor()
{
	if (cursorCtrlNum > 0)
	{
		hideCursor();
		curCursor--;
		if (curCursor < 0) curCursor = cursorCtrlNum - 1;
		showCursor();
	}
}

int OLED_GUI_PAGE::enter()
{
	return curCursor;
}

void OLED_GUI_FLOAT_CHANGE_PAGE::init()
{
	this->mode = CURSORMODE;
	int x = OLED_GUI.winValueX;
	int y = OLED_GUI.winValueY;

	/********title ctrl********/
	this->append(new OLED_GUI_CTRL_STRING("", OLED_GUI.winTitleX, OLED_GUI.winTitleY, OLED_GUI.winTitleLen, NOM_DIS, false, false));

	/********sign ctrl********/
	this->append(new OLED_GUI_CTRL_STRING(" ", x, y, 1, NOM_DIS, false, false));
	x = x + OLED_GUI.fontSizeX;

	/********value string ctrl********/
	this->append(new OLED_GUI_CTRL_STRING(" ", x, y, OLED_GUI.winValueLen, NOM_DIS, false, false));
	x = x + OLED_GUI.fontSizeX;

	/********value string cursor ctrl********/
	for (int i = 0; i < OLED_GUI.winValueLen; i++)
	{
		this->append(new OLED_GUI_CTRL_STRING("0", x, y, 1, NOM_DIS, false, true));
		x = x + OLED_GUI.fontSizeX;
	}

	numUp = new OLED_GUI_CTRL_INT(0, 0, y - 1, 0, NOM_DIS, false, false);
	numDown = new OLED_GUI_CTRL_INT(0, 0, y + 1, 0, NOM_DIS, false, false);
	this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.winYesY, 3, NOM_DIS, false, true));
	this->append(new OLED_GUI_CTRL_STRING("no", OLED_GUI.winNoX, OLED_GUI.winNoY, 2, NOM_DIS, false, true));
}


void OLED_GUI_FLOAT_CHANGE_PAGE::float2ctrl()
{
	float tmpVal = (this->value < 0) ? -this->value : this->value;

	if (value < 0) *(OLED_GUI_CTRL_STRING *)(this->Ctrl[1]) = "-"; //sign ctrl
	else *(OLED_GUI_CTRL_STRING *)(this->Ctrl[1]) = " ";

	String tmpStr = String(tmpVal, this->decNum);
	int variLen = intNum + decNum - tmpStr.length();
	if (decNum > 0) variLen++;
	Serial.print("intNum =");
	Serial.println(intNum);
	Serial.print("decNum =");
	Serial.println(decNum);
	Serial.print("tmpStr.length() =");
	Serial.println(tmpStr.length());
	Serial.print("variLen =");
	Serial.println(variLen);
	Serial.print("tmpStr =");
	Serial.println(tmpStr);
	Serial.print("String((float)0,1) =");
	Serial.println(String((float)0, 1));
	Serial.print("String(abs((float)0),1)) =");
	Serial.println(String(abs((float)0), 1));

	if (variLen < 0) tmpStr.remove(0, -variLen);
	else for (int i = 0; i < variLen; i++) tmpStr = "0" + tmpStr;

	*(OLED_GUI_CTRL_STRING *)(this->Ctrl[2]) = tmpStr;

	tmpStr.remove(intNum, 1); //remove dot

	for (int i = 0; i < intNum + decNum; i++)
	{
		*(OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i]) = tmpStr[i];
	}

	numUp->value = ((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[curCursor]))->value.toInt();
	numDown->value = numUp->value;
	numUp->value--;
	numDown->value++;

	if (numUp->value < 0) *numUp = 9;
	else *numUp = numUp->value;

	if (numDown->value > 9) *numDown = 0;
	else *numDown = numDown->value;
}

void OLED_GUI_FLOAT_CHANGE_PAGE::showNumUpDown()
{
	numUp->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	//numUp->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y - 1;
	numDown->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	//numDown->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y + 1;
	numUp->showFlag = true;
	numDown->showFlag = true;
	numUp->refresh();
	numDown->refresh();
}

void OLED_GUI_FLOAT_CHANGE_PAGE::hideNumUpDown()
{
	numUp->showFlag = false;
	numDown->showFlag = false;
	numUp->hide();
	numDown->hide();
}

void OLED_GUI_FLOAT_CHANGE_PAGE::operator=(const float &A)
{
	this->value = A;
	this->float2ctrl();
	this->showCursor();
}

void OLED_GUI_FLOAT_CHANGE_PAGE::cursorInc()
{
	float tmpValue = this->value + pow(10, intNum - curCursor - 1);
	if (tmpValue > maxValue) tmpValue = maxValue;
	*this = tmpValue;
}

void OLED_GUI_FLOAT_CHANGE_PAGE::cursorDec()
{
	float tmpValue = this->value - pow(10, intNum - curCursor - 1);
	if (tmpValue < minValue) tmpValue = minValue;
	*this = tmpValue;
}


void OLED_GUI_FLOAT_CHANGE_PAGE::showCtrl(OLED_GUI_CTRL_FLOAT * pCtrl, String title)
{
	String tmpTitle;
	int x = OLED_GUI.winValueX;
	int y = OLED_GUI.winValueY;
	origCtrl = pCtrl;
	if (title.length() != 0) tmpTitle = title;
	else if (origCtrl->name.length() != 0) tmpTitle = origCtrl->name;
	else  tmpTitle = "FLOAT";

	*(OLED_GUI_CTRL_STRING *)(Ctrl[0]) = tmpTitle;

	maxValue = origCtrl->maxVal;
	minValue = origCtrl->minVal;
	intNum = 1 + max(floor(log10(abs(maxValue))), floor(log10(abs(minValue))));
	decNum = origCtrl->decimal;
	if ((intNum + decNum) > OLED_GUI.winValueLen - 1) intNum = OLED_GUI.winValueLen - decNum;

	mode = CURSORMODE;
	cursorCtrlNum = intNum + decNum + 2;

	x = x + OLED_GUI.fontSizeX; //sign position

	for (int i = 0; i < intNum; i++)
	{
		((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i]))->X = x;
		x = x + OLED_GUI.fontSizeX;
	}

	x = x + OLED_GUI.fontSizeX; //dot position

	for (int i = 0; i < decNum; i++)
	{
		((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[intNum + i]))->X = x;
		x = x + OLED_GUI.fontSizeX;
	}

	this->cursorCtrl[intNum + decNum] = this->cursorCtrl[OLED_GUI.winValueLen]; // move yes to positon
	this->cursorCtrl[intNum + decNum + 1] = this->cursorCtrl[OLED_GUI.winValueLen + 1]; // move no to positon

	curCursor = 0;
	//origValue = data.value;
	*this = origCtrl->value;
	show();
}

byte OLED_GUI_FLOAT_CHANGE_PAGE::enter()
{
	if (this->curCursor == intNum + decNum)
	{
		this->mode = RETURNYES;
		*origCtrl = this->value;
		this->hide();
		return this->mode;
	}

	if (this->curCursor == intNum + decNum + 1)
	{
		this->mode = RETURNNO;
		//this->value = this->origValue;
		this->hide();
		return this->mode;
	}
	if (this->mode == CURSORMODE)
	{
		this->mode = CHANGEMODE;
		*this = value;
		this->showNumUpDown();
	}
	else if (this->mode == CHANGEMODE)
	{
		this->mode = CURSORMODE;
		this->hideNumUpDown();
	}

	return this->mode;
}

byte OLED_GUI_FLOAT_CHANGE_PAGE::right()
{
	if (this->mode == CURSORMODE) this->nextCursor();
	else if (this->mode == CHANGEMODE) this->cursorInc();

	return this->mode;
}

byte OLED_GUI_FLOAT_CHANGE_PAGE::left()
{
	if (this->mode == CURSORMODE) this->lastCursor();
	else if (this->mode == CHANGEMODE) this->cursorDec();

	return this->mode;
}

void OLED_GUI_INT_CHANGE_PAGE::init()
{
	this->mode = CURSORMODE;
	int x = OLED_GUI.winValueX;
	int y = OLED_GUI.winValueY;
	/********title ctrl********/
	this->append(new OLED_GUI_CTRL_STRING("", OLED_GUI.winTitleX, OLED_GUI.winTitleY, OLED_GUI.winTitleLen, NOM_DIS, false, false));

	/********sign ctrl********/
	this->append(new OLED_GUI_CTRL_STRING(" ", x, y, 1, NOM_DIS, false, false));
	x = x + OLED_GUI.fontSizeX;

	/********value string ctrl********/
	this->append(new OLED_GUI_CTRL_STRING(" ", x, y, OLED_GUI.winValueLen, NOM_DIS, false, false));
	x = x + OLED_GUI.fontSizeX;

	/********value string cursor ctrl********/
	for (int i = 0; i < OLED_GUI.winValueLen; i++)
	{
		this->append(new OLED_GUI_CTRL_STRING("0", x, y, 1, NOM_DIS, false, true));
		x = x + OLED_GUI.fontSizeX;
	}

	numUp = new OLED_GUI_CTRL_INT(0, 0, y - 1, 0, NOM_DIS, false, false);
	numDown = new OLED_GUI_CTRL_INT(0, 0, y + 1, 0, NOM_DIS, false, false);
	this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.winYesY, 3, NOM_DIS, false, true));
	this->append(new OLED_GUI_CTRL_STRING("no", OLED_GUI.winNoX, OLED_GUI.winNoY, 2, NOM_DIS, false, true));
}


void OLED_GUI_INT_CHANGE_PAGE::int2ctrl()
{
	long tmpVal = (this->value < 0) ? -this->value : this->value;

	if (value < 0) *(OLED_GUI_CTRL_STRING *)(this->Ctrl[1]) = "-"; //sign ctrl
	else *(OLED_GUI_CTRL_STRING *)(this->Ctrl[1]) = " ";

	String tmpStr = String(tmpVal);
	int variLen = intNum - tmpStr.length();

	if (variLen < 0) tmpStr.remove(0, -variLen);
	else for (int i = 0; i < variLen; i++) tmpStr = "0" + tmpStr;

	*(OLED_GUI_CTRL_STRING *)(this->Ctrl[2]) = tmpStr;

	for (int i = 0; i < intNum; i++)
	{
		*(OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i]) = tmpStr[i];
	}

	numUp->value = ((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[curCursor]))->value.toInt();
	numDown->value = numUp->value;
	numUp->value--;
	numDown->value++;

	if (numUp->value < 0) *numUp = 9;
	else *numUp = numUp->value;

	if (numDown->value > 9) *numDown = 0;
	else *numDown = numDown->value;
}

void OLED_GUI_INT_CHANGE_PAGE::operator=(const long &A)
{
	this->value = A;
	this->int2ctrl();
	this->showCursor();
}

void OLED_GUI_INT_CHANGE_PAGE::showNumUpDown()
{
	numUp->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	//numUp->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y - 1;
	numDown->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	//numDown->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y + 1;
	numUp->showFlag = true;
	numDown->showFlag = true;
	numUp->refresh();
	numDown->refresh();
}

void OLED_GUI_INT_CHANGE_PAGE::hideNumUpDown()
{
	numUp->showFlag = false;
	numDown->showFlag = false;
	numUp->hide();
	numDown->hide();
}


void OLED_GUI_INT_CHANGE_PAGE::cursorInc()
{
	long tmpValue = this->value + pow(10, intNum - curCursor - 1);
	if (tmpValue > maxValue) tmpValue = maxValue;
	*this = tmpValue;
}

void OLED_GUI_INT_CHANGE_PAGE::cursorDec()
{
	long tmpValue = this->value - pow(10, intNum - curCursor - 1);
	if (tmpValue < minValue) tmpValue = minValue;
	*this = tmpValue;
}


void OLED_GUI_INT_CHANGE_PAGE::showCtrl(OLED_GUI_CTRL_INT * pCtrl, String title)
{
	String tmpTitle;
	int x = OLED_GUI.winValueX;
	int y = OLED_GUI.winValueY;
	origCtrl = pCtrl;
	if (title.length() != 0) tmpTitle = title;
	else if (origCtrl->name.length() != 0) tmpTitle = origCtrl->name;
	else  tmpTitle = "FLOAT";

	*(OLED_GUI_CTRL_STRING *)(Ctrl[0]) = tmpTitle;

	maxValue = origCtrl->maxVal;
	minValue = origCtrl->minVal;
	intNum = 1 + max(floor(log10(abs(maxValue))), floor(log10(abs(minValue))));
	//decNum = origCtrl->decimal;
	if (intNum > OLED_GUI.winValueLen - 1) intNum = OLED_GUI.winValueLen;
	cursorCtrlNum = intNum + 2;

	mode = CURSORMODE;

	x = x + OLED_GUI.fontSizeX; //sign position

	for (int i = 0; i < intNum; i++)
	{
		((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i]))->X = x;
		x = x + OLED_GUI.fontSizeX;
	}

	this->cursorCtrl[intNum] = this->cursorCtrl[OLED_GUI.winValueLen]; // move yes to positon
	this->cursorCtrl[intNum + 1] = this->cursorCtrl[OLED_GUI.winValueLen + 1]; // move no to positon

	curCursor = 0;
	//origValue = data.value;
	*this = origCtrl->value;
	show();
}


byte OLED_GUI_INT_CHANGE_PAGE::enter()
{
	if (this->curCursor == intNum)
	{
		this->mode = RETURNYES;
		*origCtrl = this->value;
		this->hide();
		return this->mode;
	}

	if (this->curCursor == intNum + 1)
	{
		this->mode = RETURNNO;
		//this->value = this->origValue;
		this->hide();
		return this->mode;
	}
	if (this->mode == CURSORMODE)
	{
		this->mode = CHANGEMODE;
		*this = value;
		this->showNumUpDown();
	}
	else if (this->mode == CHANGEMODE)
	{
		this->mode = CURSORMODE;
		this->hideNumUpDown();
	}

	return this->mode;
}

byte OLED_GUI_INT_CHANGE_PAGE::right()
{
	if (this->mode == CURSORMODE) this->nextCursor();
	else if (this->mode == CHANGEMODE) this->cursorInc();

	return this->mode;
}

byte OLED_GUI_INT_CHANGE_PAGE::left()
{
	if (this->mode == CURSORMODE) this->lastCursor();
	else if (this->mode == CHANGEMODE) this->cursorDec();

	return this->mode;
}



void OLED_GUI_STRING_CHANGE_PAGE::init()
{
	this->mode = CURSORMODE;
	int x = OLED_GUI.winValueX;
	int y = OLED_GUI.winValueY;
	int len = OLED_GUI.winValueLen;

	this->append(new OLED_GUI_CTRL_STRING("", OLED_GUI.winTitleX, OLED_GUI.winTitleY, len, NOM_DIS, false, false));

	for (int i = 0; i < OLED_GUI.winValueLen; i++)
	{
		this->append(new OLED_GUI_CTRL_STRING(" ", x, y, 1, NOM_DIS, false, true));
		x = x + OLED_GUI.fontSizeX;
	}

	charUp = new OLED_GUI_CTRL_STRING(" ", 0, 0, 1, NOM_DIS, false, false);
	charDown = new OLED_GUI_CTRL_STRING(" ", 0, 0, 1, NOM_DIS, false, false);
	this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.winYesY, 3, NOM_DIS, false, true));
	this->append(new OLED_GUI_CTRL_STRING("no", OLED_GUI.winNoX, OLED_GUI.winNoY, 2, NOM_DIS, false, true));
}



void OLED_GUI_STRING_CHANGE_PAGE::showCtrl(OLED_GUI_CTRL_STRING * pCtrl, String title)
{
	String tmpTitle;
	origCtrl = pCtrl;
	if (title.length() != 0) tmpTitle = title;
	else if (origCtrl->name.length() != 0) tmpTitle = origCtrl->name;
	else  tmpTitle = "STRING";

	*this = origCtrl->value;
	//origValue = origCtrl->value;
	*(OLED_GUI_CTRL_STRING *)(this->Ctrl[0]) = tmpTitle;
	this->mode = CURSORMODE;
	this->show();
}


void OLED_GUI_STRING_CHANGE_PAGE::operator=(const String &A)
{
	this->value = A;

	for (int i = A.length(); i < OLED_GUI.winValueLen; i++)
	{
		this->value = this->value + ' ';
	}

	for (int i = 0; i < OLED_GUI.winValueLen; i++)
	{
		*((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i])) = this->value[i];
	}

	charUp->value = this->value[curCursor];
	charDown->value = this->value[curCursor];
	if (charUp->value[0] == ' ') *charUp = '{'; else *charUp = (char)(charUp->value[0] - 1);
	if (charDown->value[0] == '{') *charDown = ' '; else *charDown = (char)(charDown->value[0] + 1);
	this->showCursor();
}


void OLED_GUI_STRING_CHANGE_PAGE::cursorInc()
{
	if (this->value[curCursor] == '{') this->value[curCursor] = ' ';
	else this->value[curCursor]++;
	*this = this->value;
}

void OLED_GUI_STRING_CHANGE_PAGE::cursorDec()
{
	if (this->value[curCursor] == ' ') this->value[curCursor] = '{';
	else this->value[curCursor]--;
	*this = this->value;
}


byte OLED_GUI_STRING_CHANGE_PAGE::right()
{
	if (this->mode == CURSORMODE) this->nextCursor();
	else if (this->mode == CHANGEMODE) this->cursorInc();

	return this->mode;
}

byte OLED_GUI_STRING_CHANGE_PAGE::left()
{
	if (this->mode == CURSORMODE) this->lastCursor();
	else if (this->mode == CHANGEMODE) this->cursorDec();

	return this->mode;
}


byte OLED_GUI_STRING_CHANGE_PAGE::enter()
{
	if (this->curCursor == OLED_GUI.winValueLen)
	{
		this->mode = RETURNYES;
		*origCtrl = this->value.trim();
		this->hide();
		return this->mode;
	}

	if (this->curCursor == OLED_GUI.winValueLen + 1)
	{
		this->mode = RETURNNO;
		//*this = this->origValue.trim();
		this->hide();
		return this->mode;
	}

	if (this->mode == CURSORMODE)
	{
		this->mode = CHANGEMODE;
		*this = value;
		this->showCharUpDown();
	}
	else if (this->mode == CHANGEMODE)
	{
		this->mode = CURSORMODE;
		this->hideCharUpDown();
	}

	return this->mode;
}

void OLED_GUI_STRING_CHANGE_PAGE::showCharUpDown()
{
	charUp->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	charUp->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y - 1;
	charDown->X = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->X;
	charDown->Y = ((OLED_GUI_CTRL_INT *)(this->cursorCtrl[curCursor]))->Y + 1;
	charUp->showFlag = true;
	charDown->showFlag = true;
	charUp->refresh();
	charDown->refresh();
}

void OLED_GUI_STRING_CHANGE_PAGE::hideCharUpDown()
{
	charUp->showFlag = false;
	charDown->showFlag = false;
	charUp->hide();
	charDown->hide();
}


void OLED_GUI_INFO_PAGE::init(int buttonNum)
{
	this->mode = CURSORMODE;
	int len = OLED_GUI.winTitleLen;

	this->append(new OLED_GUI_CTRL_STRING("", OLED_GUI.winTitleX, OLED_GUI.winTitleY, len, NOM_DIS, false, false));
	if (buttonNum < 2)
	{
		this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.winYesY, 3, NOM_DIS, false, true));
	}
	else {
		this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.winYesY, 3, NOM_DIS, false, true));
		this->append(new OLED_GUI_CTRL_STRING("no", OLED_GUI.winNoX, OLED_GUI.winNoY, 2, NOM_DIS, false, true));
	}
}



void OLED_GUI_INFO_PAGE::showCtrl(String title)
{
	String tmpTitle;
	if (title.length() != 0) tmpTitle = title;
	else  tmpTitle = "STRING";

	*(OLED_GUI_CTRL_STRING *)(this->Ctrl[0]) = tmpTitle;
	this->mode = CURSORMODE;
	this->show();
}

byte OLED_GUI_INFO_PAGE::right()
{
	if (this->mode == CURSORMODE) this->nextCursor();
	return this->mode;
}

byte OLED_GUI_INFO_PAGE::left()
{
	if (this->mode == CURSORMODE) this->lastCursor();
	return this->mode;
}


byte OLED_GUI_INFO_PAGE::enter()
{
	if (this->curCursor == 0)
	{
		this->mode = RETURNYES;
		this->hide();
		return this->mode;
	}

	if (this->curCursor == 1)
	{
		this->mode = RETURNNO;
		this->hide();
		return this->mode;
	}

	return this->mode;
}

void OLED_GUI_ENUM_PAGE::init()
{
	this->mode = CURSORMODE;
	int len = OLED_GUI.winTitleLen;

	this->append(new OLED_GUI_CTRL_STRING("", OLED_GUI.winTitleX, OLED_GUI.winTitleY, len, NOM_DIS, false, false));
	for (int i = 1; i <= (OLED_GUI.resY - 2); i++)
	{
		this->append(new OLED_GUI_CTRL_STRING("", 0, i, OLED_GUI.resX / 8, NOM_DIS, false, true));
	}
	this->append(new OLED_GUI_CTRL_STRING("yes", OLED_GUI.winYesX, OLED_GUI.resY - 1, 3, NOM_DIS, false, true));
	this->append(new OLED_GUI_CTRL_STRING("no", OLED_GUI.winNoX, OLED_GUI.resY - 1, 2, NOM_DIS, false, true));
}

void OLED_GUI_ENUM_PAGE::showCtrl(OLED_GUI_CTRL_ENUM * pCtrl,String title)
{
	String tmpTitle;
	int tmpEnumNum;
	origCtrl = pCtrl;
	value = origCtrl->value;
	if (title.length() != 0) tmpTitle = title;
	else if (origCtrl->name.length() != 0) tmpTitle = origCtrl->name;
	else  tmpTitle = "ENUM";

	*(OLED_GUI_CTRL_STRING *)(this->Ctrl[0]) = tmpTitle;
	this->mode = CURSORMODE;

	for (int i = 0; i < (OLED_GUI.resY - 2); i++)
	{
		*((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i])) = String(" ");
	}
	tmpEnumNum = (origCtrl->enumNum < (OLED_GUI.resY - 2)) ? origCtrl->enumNum : (OLED_GUI.resY - 2);
	for (int i = 0; i < tmpEnumNum; i++)
	{
		if (origCtrl->enumNameArray[i].length() > 0)
		{
			*((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[i])) = origCtrl->enumNameArray[i];
		}
	}

	*((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[value])) = String("->") + ((OLED_GUI_CTRL_STRING *)(this->cursorCtrl[value]))->value;

	this->show();
}

byte OLED_GUI_ENUM_PAGE::right()
{
	if (this->mode == CURSORMODE) this->nextCursor();
	return this->mode;
}

byte OLED_GUI_ENUM_PAGE::left()
{
	if (this->mode == CURSORMODE) this->lastCursor();
	return this->mode;
}


byte OLED_GUI_ENUM_PAGE::enter()
{
	if (this->curCursor == (OLED_GUI.resY - 2))
	{
		this->mode = RETURNYES;
		* origCtrl = value;
		this->hide();
		return this->mode;
	}

	if (this->curCursor == (OLED_GUI.resY - 1))
	{
		this->mode = RETURNNO;
		this->hide();
		return this->mode;
	}
	OLED_GUI_CTRL_STRING * tmpStr;
	if (this->curCursor < origCtrl->enumNum)
	{
		tmpStr = (OLED_GUI_CTRL_STRING *)(this->cursorCtrl[value]);
		*tmpStr = tmpStr->value.remove(0,2);
		value = this->curCursor;
		tmpStr = (OLED_GUI_CTRL_STRING *)(this->cursorCtrl[value]);
		*tmpStr = String("->") + tmpStr->value;
	}

	return this->mode;
}



