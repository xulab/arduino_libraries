/* version 11
 * modified in 20181129
 * only suit for teensy 3.6
 * change: add pre_sound_delay.
 * change: add clicks.
 * change: add amplitude modulation.
 * Reaction time: 700us.
 */

#include <math.h>
#include <SPI_TGM_H.h>
#include <SPI.h>
#include <CACHE.h>
#pragma pack (1) //align to 1 byte.

//#define DEBUG 1 

 /*-----------TGM_config-------------*/
const char VERSION_STRING[] = "VER011_20181207_PJW.";
/*-----------error-------------*/
const char noerror_string[] = "no error";
const char error_string1[] = "no error";
char temp_error_string[ERROR_STRING_SIZE] = "";
/*-----------error-------------*/
/*-----------TGM_config-------------*/


/*-----------23LC1024------------*/
inline void _SPI_INIT() {
	CACHE.INFO = 2;
	CACHE.REQ = 6;
	CACHE.WR = 7;
	CACHE.PER = 22;
	CACHE.CS = 23;
	CACHE.SCK = 13;
	CACHE.MO = 11;
	CACHE.MI = 12;
	CACHE.SPISPEED = 4000000;
	CACHE.init();
}

/*--------AD9850-------*/
#define AD9850_clock (float)125000000 //125MHz
const float x = (float)4294967295 / AD9850_clock;
#define fq_convert(f) (uint32_t)((float)f*x)
//#define AD9850_w_clk 4 //Suit for teensy3.6Adptor4TGM3.2Ver2
#define AD9850_w_clk 34 //Suit for teensy3.6Adptor4TGM3.2Ver3
//#define AD9850_fq_up 5 //Suit for teensy3.6Adptor4TGM3.2Ver2
#define AD9850_fq_up 35 //Suit for teensy3.6Adptor4TGM3.2Ver3
//#define AD9850_rest 24 //Suit for teensy3.6Adptor4TGM3.2Ver2
#define AD9850_rest 33 //Suit for teensy3.6Adptor4TGM3.2Ver3
//int AD9850_DATA[8] = { 39, 38, 14, 16, 10, 9, 19, 20 }; //Suit for teensy3.6Adptor4TGM3.2Ver2
int AD9850_DATA[8] = {24, 25, 26, 27, 28, 29, 30, 31}; //Suit for teensy3.6Adptor4TGM3.2Ver3
volatile uint32_t curFq = 0;

void _ad9850_data_wr(byte data) {
	for (byte i = 0; i < 8; i++) digitalWrite(AD9850_DATA[i], data & _BV(i));
}

inline void _ad9850_init_port() {
	pinMode(AD9850_w_clk, OUTPUT);
	digitalWrite(AD9850_w_clk, LOW);
	pinMode(AD9850_fq_up, OUTPUT);
	digitalWrite(AD9850_fq_up, LOW);
	pinMode(AD9850_rest, OUTPUT);
	digitalWrite(AD9850_rest, LOW);
	for (int i = 0; i < 8; i++) pinMode(AD9850_DATA[i], OUTPUT);
	_ad9850_data_wr(0);
}

inline void _ad9850_reset() {
	digitalWrite(AD9850_rest, LOW);
	digitalWrite(AD9850_rest, HIGH);
	delay(100);
	digitalWrite(AD9850_rest, LOW);
	digitalWrite(AD9850_w_clk, LOW);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);
	digitalWrite(AD9850_fq_up, LOW);
	digitalWrite(AD9850_fq_up, HIGH);
	digitalWrite(AD9850_fq_up, LOW);
	_ad9850_data_wr(0);
}

inline void _ad9850_init() {
	delay(100);
	_ad9850_init_port();
	_ad9850_reset();
}

inline void _ad9850_wr_parallel(uint32_t fq) {
	uint8_t * data = (uint8_t *)&fq;

	_ad9850_data_wr(0);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[3]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[2]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[1]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	_ad9850_data_wr(data[0]);
	digitalWrite(AD9850_w_clk, HIGH);
	digitalWrite(AD9850_w_clk, LOW);

	digitalWrite(AD9850_fq_up, HIGH);
	digitalWrite(AD9850_fq_up, LOW);

	_ad9850_data_wr(0);
}


inline void _set_fq(uint32_t fq) {
	if (curFq != fq) {
		_ad9850_wr_parallel(fq_convert(fq));
		curFq = fq;
	}
}

/*--------PGA2310-------*/

#define PGA2310_cs 21
#define PGA2310SPEED 32000000
volatile byte curVol = 0;

inline void _pga2310_init() {
	pinMode(PGA2310_cs, OUTPUT);
	digitalWrite(PGA2310_cs, HIGH);
}

inline void _pga2310_wr(byte LGain, byte RGain) {
	digitalWrite(PGA2310_cs, LOW);
	SPI.beginTransaction(SPISettings(PGA2310SPEED, MSBFIRST, SPI_MODE0));
	SPI.transfer(LGain);//left channel
	SPI.transfer(RGain);//right channel
	SPI.endTransaction();
	digitalWrite(PGA2310_cs, HIGH);
}

inline void _set_vol(byte vol) {
	if (curVol != vol) {
		_pga2310_wr(vol, vol);
		curVol = vol;
	}
}


/*--------timer1-------*/
#define TIMER1_STOP 0
#define TIMER1_RUNNING 1
#define TIMER1_INTERRUPTED 2
#define TIMER1_PAUSE 3
#define SWEEP_INTERVAL 100 //change fq interval(us)
#define MININT 10 //10us, about 1800 clocks for teensy3.6 .

byte flag_vol_i = 0;
volatile byte _target_vol = 192;
volatile byte timer1_mode = 0;
volatile byte step_index = 0;
volatile uint32_t delay_time = 0;
volatile uint32_t sweep_index = 0;
volatile uint32_t sweep_index_size = 0;
volatile float sweep_linear_step = 0;
volatile float sweep_linear_step2 = 0;
volatile float sweep_base = 0;
volatile uint32_t chord_fq_array[MAX_CHORD_NUM];
volatile uint16_t chord_fq_num = 0;
uint32_t sweep_fq0 = 0;
uint32_t sweep_fq1 = 0;
uint32_t sweep_fq2 = 0;
byte sweep_mode = SWEEP_OFF;

#define TONESTEPARRAYSIZE 12 

#define TONESTEPIDLE 0 //Idle mode. Do nothing.
#define TONESTEPDRT 1 //direct mode. ISI, Fq and Vol will be gaved in struct directly. 
#define TONESTEPARRAY 2 //ISI, Fq and Vol will be gaved in array. Loop number depend on uLoopNum. It would be a good choice to set the uLoopNum to array size.
#define TONESTEPARRAYREV 3 //ISI, Fq and Vol will be gaved in array. But the array will loop reversly.
#define TONESTEPFNC 4 //ISI, Fq and Vol will be gaved by a callback function. Loop number depend on uLoopNum. 

//typedef unsigned int uint16_t;
//typedef unsigned long uint32_t;
//typedef unsigned char byte;
typedef byte(*byte_fnc)();
typedef uint16_t(*uint16_fnc)();
typedef float(*float_fnc)();
typedef uint32_t(*uint32_fnc)();
typedef void(*void_fnc)();
IntervalTimer timer1;

struct stepTonePar {
	byte flag;
	float drtPar; // 32bit unsigned int may compatible to any satuation.
	void * arrayPar; // pointer to a array. Transform it to a specific array type before usage, for instance "((byte *)arrayPar)[2]".
	uint16_t array_Size;
	void_fnc fnc; // Transform return data to the data type needed. For instance, "(*(byte_fnc)fnc)()"
};

struct toneStep {
	uint32_t LoopNum;
	uint32_t LoopInd;
	byte targetVol;

	stepTonePar ISI;
	stepTonePar fq;
	stepTonePar vol;
	uint32_t CycleNum1;
	toneStep * CycleToneStep1;
	uint32_t CycleNum2;
	toneStep * CycleToneStep2;
	toneStep * NextToneStep;
};

toneStep toneStepArray[TONESTEPARRAYSIZE]; //just creat some tonestep struct. 6 steps may satisfy most situation.
toneStep IdleToneStep; //just creat some tonestep struct. 6 steps may satisfy most situation.
toneStep * curToneStep = NULL; //Current addr of tonestep struct.

typedef struct {
	uint16_t nSize;
	byte version;
	char version_str[TGM_VERSION_STRING_SIZE];
} TGMinfo;


typedef struct {
	uint16_t nSize;
	byte error_code;
	char error_string[ERROR_STRING_SIZE];
}TMGerror;

typedef struct {
	uint16_t nSize;
	byte version;
	byte tone_flag;
	byte sweep;
	byte volume_mode; //use gaven volume
	byte volume;
	byte prepare_mode;
	byte step_up_flag;
	byte step_down_flag;
	uint16_t duration;
	uint16_t frequency0;
	uint16_t frequency1;
	uint16_t frequency2;
	uint16_t frequency3;
	uint16_t frequency4;
	uint16_t frequency5;
	uint16_t frequency6;
	uint16_t frequency7;
	uint16_t frequency8;
	uint16_t frequency9;
	uint16_t fq_step;
	float sweep_base;
	uint16_t chord_num;
	uint32_t durationL;
	uint32_t pre_sound_delay;
	uint32_t frequencyL0;
	uint32_t frequencyL1;
	uint32_t frequencyL2;
	uint32_t frequencyL3;
	uint32_t frequencyL4;
	uint32_t frequencyL5;
	uint32_t frequencyL6;
	uint32_t frequencyL7;
	uint32_t frequencyL8;
	uint32_t frequencyL9;
	uint32_t clicks_dur;
	uint32_t clicks_period;
	uint32_t AMFrequncy;
} ton;


TGMinfo info;
TMGerror error;
ton _EMPTY_TONE;
ton _QUICK_TONE;
ton _tone;

#ifdef DEBUG
void printToneStep(toneStep * ts) {
	Serial.print(F("ts->LoopNum = "));
	Serial.println(ts->LoopNum);
	Serial.print(F("ts->LoopInd = "));
	Serial.println(ts->LoopInd);

	Serial.print(F("ts->ISI.arrayPar = "));
	Serial.println((uint32_t)ts->ISI.arrayPar);
	Serial.print(F("ts->ISI.array_Size = "));
	Serial.println(ts->ISI.array_Size);
	Serial.print(F("ts->ISI.drtPra = "));
	Serial.println(ts->ISI.drtPar);
	Serial.print(F("ts->ISI.flag = "));
	Serial.println(ts->ISI.flag);

	Serial.print(F("ts->fq.arrayPar = "));
	Serial.println((uint32_t)ts->fq.arrayPar);
	Serial.print(F("ts->fq.array_Size = "));
	Serial.println(ts->fq.array_Size);
	Serial.print(F("ts->fq.drtPra = "));
	Serial.println(ts->fq.drtPar);
	Serial.print(F("ts->fq.flag = "));
	Serial.println(ts->fq.flag);

	Serial.print(F("ts->vol.arrayPar = "));
	Serial.println((uint32_t)ts->vol.arrayPar);
	Serial.print(F("ts->vol.array_Size = "));
	Serial.println(ts->vol.array_Size);
	Serial.print(F("ts->vol.drtPra = "));
	Serial.println(ts->vol.drtPar);
	Serial.print(F("ts->vol.flag = "));
	Serial.println(ts->vol.flag);
}

void print_tone(ton * t) {
	Serial.print("sizeof(ton) = ");
	Serial.println(sizeof(ton));
	Serial.print(F("t->nSize = "));
	Serial.println(t->nSize);
	Serial.print("t->version = ");
	Serial.println(t->version);
	Serial.print("t->tone_flag = ");
	Serial.println(t->tone_flag);
	Serial.print("t->sweep = ");
	Serial.println(t->sweep);
	Serial.print("t->volume_mode = ");
	Serial.println(t->volume_mode);
	Serial.print("t->volume = ");
	Serial.println(t->volume);
	Serial.print("t->prepare_mode = ");
	Serial.println(t->prepare_mode);
	Serial.print("t->step_up_flag = ");
	Serial.println(t->step_up_flag);
	Serial.print("t->step_down_flag = ");
	Serial.println(t->step_down_flag);
	Serial.print(F("t->duration = "));
	Serial.println(t->duration);
	Serial.print("t->frequency0 = ");
	Serial.println(t->frequency0);
	Serial.print("t->frequency1 = ");
	Serial.println(t->frequency1);
	Serial.print("t->frequency2 = ");
	Serial.println(t->frequency2);
	Serial.print("t->frequency3 = ");
	Serial.println(t->frequency3);
	Serial.print("t->frequency4 = ");
	Serial.println(t->frequency4);
	Serial.print("t->frequency5 = ");
	Serial.println(t->frequency5);
	Serial.print("t->frequency6 = ");
	Serial.println(t->frequency6);
	Serial.print("t->frequency7 = ");
	Serial.println(t->frequency7);
	Serial.print("t->frequency8 = ");
	Serial.println(t->frequency8);
	Serial.print("t->frequency9 = ");
	Serial.println(t->frequency9);
	Serial.print("t->fq_step = ");
	Serial.println(t->fq_step);
	Serial.print("t->sweep_base = ");
	Serial.println(t->sweep_base);
	Serial.print("t->durationL = ");
	Serial.println(t->durationL);
	Serial.print("t->pre_sound_delay = ");
	Serial.println(t->pre_sound_delay);
	Serial.print("t->frequencyL0 = ");
	Serial.println(t->frequencyL0);
	Serial.print("t->frequencyL1 = ");
	Serial.println(t->frequencyL1);
	Serial.print("t->frequencyL2 = ");
	Serial.println(t->frequencyL2);
	Serial.print("t->frequencyL3 = ");
	Serial.println(t->frequencyL3);
	Serial.print("t->frequencyL4 = ");
	Serial.println(t->frequencyL4);
	Serial.print("t->frequencyL5 = ");
	Serial.println(t->frequencyL5);
	Serial.print("t->frequencyL6 = ");
	Serial.println(t->frequencyL6);
	Serial.print("t->frequencyL7 = ");
	Serial.println(t->frequencyL7);
	Serial.print("t->frequencyL8 = ");
	Serial.println(t->frequencyL8);
	Serial.print("t->frequencyL9 = ");
	Serial.println(t->frequencyL9);
	Serial.print("t->clicks_dur = ");
	Serial.println(t->clicks_dur);
	Serial.print("t->clicks_period = ");
	Serial.println(t->clicks_period);
}
#endif // DEBUG

void eraseAllToneStep() {

	for (int i = 0; i < TONESTEPARRAYSIZE; i++)
	{
		memset((void*)toneStepArray, 0, sizeof(toneStepArray));
	}
	curToneStep = NULL;
}

#define MaxAMArraySize 192
volatile byte AMArraySize = 0;
float AMInterval[MaxAMArraySize] = {};
byte AMVol[MaxAMArraySize] = {};

float rawInterval[] = {
	0.00260844292328823, 7.61683536072170e-05, 7.83925498343474e-05, 8.06816973760302e-05, 8.30376931810289e-05,
	8.54624896134584e-05, 8.79580960629492e-05, 9.05265806554439e-05, 9.31700719242253e-05, 9.58907606093571e-05,
	9.86909014682718e-05, 0.000101572815133970, 0.000104538890076286, 0.000107591584555615, 0.000110733428682893,
	0.000113967026516458, 0.000117295058238350, 0.000120720282362720, 0.000124245538065590, 0.000127873747499552,
	0.000131607918276700, 0.000135451145942420, 0.000139406616553630, 0.000143477609355282, 0.000147667499486657,
	0.000151979760815987, 0.000156417968833386, 0.000160985803621408, 0.000165687052947715, 0.000170525615415412,
	0.000175505503724656, 0.000180630848020547, 0.000185905899367097, 0.000191335033276569, 0.000196922753401363,
	0.000202673695287639, 0.000208592630278851, 0.000214684469498019, 0.000220954267999485, 0.000227407228993885,
	0.000234048708234949, 0.000240884218529164, 0.000247919434376610, 0.000255160196764160, 0.000262612518088715,
	0.000270282587247480, 0.000278176774874146, 0.000286301638731626, 0.000294663929289849, 0.000303270595443873,
	0.000312128790442573, 0.000321245877977936, 0.000330629438471713, 0.000340287275562680, 0.000350227422785756,
	0.000360458150477796, 0.000370987972881012, 0.000381825655491868, 0.000392980222635696, 0.000404460965276042,
	0.000416277449089618, 0.000428439522798163, 0.000440957326763858, 0.000453841301874593, 0.000467102198720613,
	0.000480751087075050, 0.000494799365682790, 0.000509258772396045, 0.000524141394633304, 0.000539459680208906,
	0.000555226448528446, 0.000571454902173772, 0.000588158638891317, 0.000605351664008724, 0.000623048403284570,
	0.000641263716231241, 0.000660012909906294, 0.000679311753228447, 0.000699176491807296, 0.000719623863339496,
	0.000740671113583967, 0.000762336012956506, 0.000784636873756371, 0.000807592568088659, 0.000831222546483757,
	0.000855546857281456, 0.000880586166804014, 0.000906361780365760, 0.000932895664173879, 0.000960210468160513,
	0.000988329549810436, 0.00101727699903896, 0.00104707766418399, 0.00107775717918340, 0.00110934199200947,
	0.00114185939444591, 0.00117533755328399, 0.00120980554304609, 0.00124529338032568, 0.00128183205986478,
	0.00131945359247946, 0.00135819104497207, 0.00139807858216878, 0.00143915151123296, 0.00148144632842970,
	0.00152500076851991, 0.00156985385698317, 0.00161604596529148, 0.00166361886946791, 0.00171261581219459,
	0.00176308156875181, 0.00181506251710560, 0.00186860671248147, 0.00192376396680637, 0.00198058593342811,
	0.00203912619757216, 0.00209944037303178, 0.00216158620565007, 0.00222562368420093, 0.00229161515934070,
	0.00235962547138219, 0.00242972208770856, 0.00250197525074963, 0.00257645813753143, 0.00265324703192979,
	0.00273242151088560, 0.00281406464598091, 0.00289826322194099, 0.00298510797380941, 0.00307469384475290,
	0.00316712026669048, 0.00326249146621260, 0.00336091679856272, 0.00346251111280756, 0.00356739515172851,
	0.00367569599042590, 0.00378754751816987, 0.00390309096863919, 0.00402247550441229, 0.00414585886239374,
	0.00427340806782817, 0.00440530022567087, 0.00454172339940123, 0.00468287758889940, 0.00482897582082376,
	0.00498024536706052, 0.00513692910935457, 0.00529928707123822, 0.00546759814197076, 0.00564216202150364,
	0.00582330142066159, 0.00601136455697496, 0.00620672799418359, 0.00640979988265097, 0.00662102366924289,
	0.00684088235911184, 0.00706990342903033, 0.00730866451329454, 0.00755780000995959, 0.00781800878883471,
	0.00809006322529604, 0.00837481983836680, 0.00867323188138514, 0.00898636432404920, 0.00931541178274153,
	0.00966172011156874, 0.0100268125733183, 0.0104124217871605, 0.0108205290267207, 0.0112534129595104,
	0.0117137106379842, 0.0122044945661215, 0.0127293711150134, 0.0132926076674610, 0.0138992989869637,
	0.0145555880036706, 0.0152689634447584, 0.0160486681477892, 0.0169062703775079, 0.0178564813089412,
	0.0189183550869876, 0.0201171034503653, 0.0214869363897509, 0.0230756957103178, 0.0249527981130649,
	0.0272237128601224, 0.0300584861582641, 0.0337540120927323, 0.0388908179683797, 0.0468230376377198,
	0.0619330489621286, 0.152010178458396
};

float rampCosInterval2ms[] = {
10.1138075577600, 10.0665242156311, 10.0157665372491, 10.0739849561148, 10.4318948813948,
11.3251842779095, 11.7043042514671, 11.4282033239377, 10.4095401552840, 11.6873543807027,
13.1242690078743, 10.8935015966778, 11.8869521956133, 12.9733005257656, 14.1619254368629,
10.1568667765621, 10.7713370856308, 11.4246557358438, 12.1196036371246, 12.8592234658062,
13.6468558227406, 14.4861822843088, 15.3812769736181, 16.3366687336121, 17.3574165869981,
18.4492019766013, 19.6184423757685, 10.2738582187091, 10.5985741424764, 10.9351962839415,
11.2843240430073, 11.6466028413232, 12.0227291139499, 12.4134559883672, 12.8195997653019,
13.2420473384858, 13.6817647182237, 14.1398068580607, 14.6173290265891, 15.1156000199192,
15.6360175776694, 16.1801264505921, 16.7496396767336, 17.3464637627703, 17.9727286480984,
18.6308235654831, 19.3234402231375, 20.0536251466367, 20.8248435743210, 21.6410580534414,
22.5068259190208, 23.4274212759684, 24.4089891322430, 25.4587422300269, 26.5852153349221,
27.7985979739274, 29.1111760073411, 30.5379268895168, 32.0973362955784, 33.8125407550157,
35.7129626178825, 37.8367101739752, 40.2342069007307, 42.9738727795017, 46.1513914206355,
49.9055962261299, 54.4474257202447, 60.1169723165282, 67.5080241854647, 77.7816359367594,
93.6460752754396, 123.866097924257, 304.020356916793
};

byte rampCosVol2ms[] = {
168, 144, 130, 120, 112,
105, 99, 94, 90, 86,
82, 79, 76, 73, 70,
68, 66, 64, 62, 60,
58, 56, 54, 52, 50,
48, 46, 45, 44, 43,
42, 41, 40, 39, 38,
37, 36, 35, 34, 33,
32, 31, 30, 29, 28,
27, 26, 25, 24, 23,
22, 21, 20, 19, 18,
17, 16, 15, 14, 13,
12, 11, 10, 9, 8,
7, 6, 5, 4, 3,
2, 1, 0
};

float rampCosInterval5ms[] = {
13.0422146164411, 10.1506274187023, 10.5248817868038, 11.2462380168206, 10.0371841705913,
10.3691806907845, 10.1199190764569, 11.6872564005845, 10.6404314788335, 11.9400680417919,
13.3988243558664, 11.1131549493998, 12.1172599416439, 13.2124740222562, 14.4071784107241,
10.3217733161147, 10.9354958559644, 11.5859847386495, 12.2754946168588, 13.0064282617212,
13.7813481357043, 14.6029891983133, 15.4742731275154, 16.3983241737969, 17.3784869047320,
18.4183461479354, 19.5217495011724, 10.1956309878608, 10.4972018651589, 10.8079310282504,
11.1281184210046, 11.4580757967035, 11.7981273569109, 12.1486104385428, 12.5098762537482,
12.8822906876572, 13.2662351596489, 13.6621075544280, 14.0703232299046, 14.4913161097050,
14.9255398690470, 15.3734692237645, 15.8356013334524, 16.3124573310630, 16.8045839928136,
17.3125555640378, 17.8369757586425, 18.3784799521295, 18.9377375908494, 19.5154548431960,
20.1123775220614, 20.7292943119687, 21.3670403391408, 22.0265011283544, 22.7086169970062,
23.4143879444970, 24.1448791041188, 24.9012268353026, 25.6846455467728, 26.4964353561911,
27.3379907098538, 28.2108101075182, 29.1165071033080, 30.0568227848748, 31.0336399709180,
32.0489994132549, 33.1051183462144, 34.2044117955592, 35.3495171451516, 36.5433225664727,
37.7890000497980, 39.0900439441735, 40.4503161264802, 41.8740991918340, 43.3661594069257,
44.9318216202460, 46.5770589137077, 48.3086005578437, 50.1340628665917, 52.0621089358025,
54.1026451336035, 56.2670647975519, 58.5685531899211, 61.0224728306075, 63.6468555750672,
66.4630383373052, 69.4964949348184, 72.7779400183529, 76.3448172237921, 80.2433407389461,
84.5313518875393, 89.2824065447062, 94.5917754349379, 100.585517251827, 107.434681948754,
115.378478551589, 124.763990565325, 136.118564300612, 150.292430791321, 168.770060463662,
194.454089841898, 234.115188188599, 309.665244810643, 760.050892291981
};

byte rampCosVol5ms[] = {
191, 171, 158, 148, 141,
135, 130, 125, 121, 117,
113, 110, 107, 104, 101,
99, 97, 95, 93, 91,
89, 87, 85, 83, 81,
79, 77, 76, 75, 74,
73, 72, 71, 70, 69,
68, 67, 66, 65, 64,
63, 62, 61, 60, 59,
58, 57, 56, 55, 54,
53, 52, 51, 50, 49,
48, 47, 46, 45, 44,
43, 42, 41, 40, 39,
38, 37, 36, 35, 34,
33, 32, 31, 30, 29,
28, 27, 26, 25, 24,
23, 22, 21, 20, 19,
18, 17, 16, 15, 14,
13, 12, 11, 10, 9,
8, 7, 6, 5, 4,
3, 2, 1, 0
};

const uint16_t cosISI_temp[192] = {};
const byte cosVol_temp[192] = {};

uint32_t sweepLinear() {
	return (uint32_t)((float)sweep_fq0 + (sweep_linear_step * (float)curToneStep->LoopInd));
}
uint32_t sweepLinear2() {
	return (uint32_t)(sweep_fq1 + (sweep_linear_step2 * curToneStep->LoopInd));
}

uint32_t sweepExp() {
	return sweep_fq0 * pow(sweep_base, curToneStep->LoopInd);
}

inline void _timer1_stop() {
	timer1.end();
}


void timer1Func() {
	timer1_mode = TIMER1_INTERRUPTED;
	if (NULL == curToneStep || 0 == curToneStep->LoopNum) {
		_timer1_stop(); //ToneStepCur equals to NULL(0) means tone is finished.  LoopNum equals to 0 means some errors are accurred.
#ifdef DEBUG
		Serial.println(F("NULL == curToneStep, _timer1_stop"));
#endif
	}
	else {
		/**********ISI***********/
		float interTimeTemp = 0;
		switch (curToneStep->ISI.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			timer1.begin(timer1Func, curToneStep->ISI.drtPar);
			break;
		case TONESTEPARRAY:
			interTimeTemp = ((float *)(curToneStep->ISI.arrayPar))[curToneStep->LoopInd];
			timer1.begin(timer1Func, interTimeTemp);
			break;
		case TONESTEPARRAYREV:
			interTimeTemp = ((float *)(curToneStep->ISI.arrayPar))[curToneStep->ISI.array_Size - 1 - curToneStep->LoopInd];
			timer1.begin(timer1Func, interTimeTemp);
			break;
		case TONESTEPFNC:
			if (NULL == curToneStep->ISI.fnc) break;
			interTimeTemp = ((float_fnc)curToneStep->ISI.fnc)();
			timer1.begin(timer1Func, interTimeTemp);
			break;
		default: break;
		}

		/**********fq***********/
		switch (curToneStep->fq.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			_set_fq(curToneStep->fq.drtPar);
			break;
		case TONESTEPARRAY: break;
		case TONESTEPFNC:
			if (NULL == curToneStep->fq.fnc) break;
			_set_fq((*((uint32_fnc)curToneStep->fq.fnc))());
			break;
		default: break;
		}

		/**********vol***********/
		byte volTemp = 0;
		switch (curToneStep->vol.flag) {
		case TONESTEPIDLE: break;
		case TONESTEPDRT:
			_set_vol(curToneStep->vol.drtPar);
			break;
		case TONESTEPARRAY:
			volTemp = ((byte *)(curToneStep->vol.arrayPar))[curToneStep->LoopInd];
			if (curToneStep->targetVol > volTemp) {
				_set_vol(curToneStep->targetVol - volTemp);
			}
			else {
				_set_vol(0);
			}
			break;
		case TONESTEPARRAYREV:
			volTemp = ((byte *)(curToneStep->vol.arrayPar))[curToneStep->vol.array_Size - 1 - curToneStep->LoopInd];
			if (curToneStep->targetVol > volTemp) {
				_set_vol(curToneStep->targetVol - volTemp);
			}
			else {
				_set_vol(0);
			}
			break;
		case TONESTEPFNC:
			if (NULL == curToneStep->vol.fnc) break;
			_set_vol((*((byte_fnc)curToneStep->vol.fnc))());
			break;
		default: break;
		}


		curToneStep->LoopInd++;

		if (curToneStep->LoopInd >= curToneStep->LoopNum) {
			curToneStep->LoopInd = 0; //prepare for CycleToneStep1.
			if (0 != curToneStep->CycleNum1) {
				curToneStep->CycleNum1--;
				curToneStep = curToneStep->CycleToneStep1;
			}
			else {
				if (0 != curToneStep->CycleNum2) {
					curToneStep->CycleNum2--;
					curToneStep = curToneStep->CycleToneStep2;
				}
				else {
					curToneStep = curToneStep->NextToneStep;
				}
			}
		}
	}
	timer1_mode = TIMER1_RUNNING;
}

inline void _timer1_pause() {
	timer1.end();
}

inline void _timer1_resume() {
	timer1.begin(timer1Func, MININT);
}

/*--------SPI_TGMClass-------*/

void SPI_TGMClass::init()
{
	memset(&info, 0, sizeof(TGMinfo));
	info.nSize = sizeof(TGMinfo);
	info.version = TGM_VERSION;
	strcpy(info.version_str, VERSION_STRING);

	memset(&error, 0, sizeof(TMGerror));
	error.nSize = sizeof(TMGerror);
	error.error_code = NO_ERROR;
	strcpy(error.error_string, noerror_string);

	memset(&_tone, 0, sizeof(ton));
	_tone.nSize = sizeof(ton);
	_tone.version = TGM_VERSION;

	memset(&_EMPTY_TONE, 0, sizeof(ton));
	_EMPTY_TONE.nSize = sizeof(ton);
	_EMPTY_TONE.version = TGM_VERSION;

	memset(&_QUICK_TONE, 0, sizeof(ton));
	_QUICK_TONE.nSize = sizeof(ton);
	_QUICK_TONE.version = TGM_VERSION;

	eraseAllToneStep();
	_SPI_INIT();

#ifdef DEBUG
	 Serial.begin(115200);
#endif // DEBUG
}

inline void _write_info(TGMinfo* data) {
	CACHE.q_write(TGM_INFO_ADDR, sizeof(TGMinfo), (char*)data);
}

inline void _write_error(TMGerror* data) {
	CACHE.q_write(ERROR_ADDR, sizeof(TMGerror), (char*)data);
}

void SPI_TGMClass::wait_command() {
	while (LOW == CACHE.new_info());
}

uint32_t random_noise_fq() {
	uint32_t fq, fq0, fq1;
	if (sweep_fq1 > sweep_fq0) {
		fq0 = sweep_fq0;
		fq1 = sweep_fq1;
	}
	else {
		fq0 = sweep_fq1;
		fq1 = sweep_fq0;
	}
	switch (sweep_mode) {
	case SWEEP_NOISE_WHITE:
		fq = random(fq0, fq1);
		break;

	case SWEEP_NOISE_GAUSS1:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 2;
		break;

	case SWEEP_NOISE_GAUSS2:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 3;
		break;

	case SWEEP_NOISE_GAUSS3:
		fq = ((uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1) + \
			(uint32_t)random(fq0, fq1)) / 4;
		break;
	}
	return fq;
}

uint32_t chord_fq() {
	return chord_fq_array[curToneStep->LoopInd%chord_fq_num];
}

void SPI_TGMClass::init_TGM() {
	_ad9850_init();
	_pga2310_init();
	init();
	_write_info(&info);
	_write_error(&error);
	_set_vol(INIT_VOL + 1); //Refresh the curVol
	_set_fq(INIT_FQ + 1); //Refresh the curFq
	_set_vol(INIT_VOL); //Refresh the curVol
	_set_fq(INIT_FQ); //Refresh the curFq
}

void SPI_TGMClass::set_fq(uint32_t fq) {
	_set_fq(fq);
}

void SPI_TGMClass::set_vol(byte vol) {
	_set_vol(vol);
}

void SPI_TGMClass::testTone() {
	memset(&_tone, 0, sizeof(ton));
	_tone.tone_flag = TONE_FLAG_ON;
	_tone.volume_mode = VOLUME_ON;
	_tone.version = TGM_VERSION;
	_tone.pre_sound_delay = 0;
	_tone.volume = 192;
	_tone.frequency0 = 3000;
	_tone.frequencyL0 = 3000;
	//_tone.frequency1 = 5000;
	//_tone.frequencyL1 = 5000;
	_tone.sweep = SWEEP_OFF;
	_tone.duration = 10000;
	_tone.durationL = 10000;
	_tone.step_up_flag = STEP_FLAG_COS_2MS;
	_tone.step_down_flag = STEP_FLAG_COS_2MS;
	//_tone.clicks_dur = 1;
	//_tone.clicks_period = 10;
}

void SPI_TGMClass::read_tone() {
	if (TIMER1_STOP != timer1_mode)
	{
		_timer1_pause();
	}
	CACHE.q_read(TONE_ADDR, sizeof(ton), (char*)&_tone);
#ifdef DEBUG
	Serial.println(_tone.tone_flag);
	Serial.println(_tone.volume_mode);
	Serial.println(_tone.version);
	Serial.println(_tone.pre_sound_delay);
	Serial.println(_tone.volume);
	Serial.println(_tone.frequency0);
	Serial.println(_tone.duration);
	Serial.println(_tone.durationL);
	Serial.println(_tone.step_up_flag);
	Serial.println(_tone.step_down_flag);
#endif // DEBUG

}

void setDelayStep(toneStep * ts, uint32_t t) {
	ts->LoopNum = t; //delay ms
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = 1000; // 1000us.
}

void setOneLoopStep(toneStep * ts) {
	ts->LoopNum = 1;
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = MININT;
}

void setSweepLoopStep(toneStep * ts) {
	ts->ISI.flag = TONESTEPDRT;
	ts->ISI.drtPar = SWEEP_INTERVAL;
}

void setVolArrayStep(toneStep * ts, float * ISIArray, byte * VolArray, uint32_t arraySize, byte targetVol, byte reverse) {
	ts->targetVol = targetVol;
	ts->LoopNum = arraySize;

	ts->ISI.array_Size = arraySize;
	ts->ISI.arrayPar = (void *)ISIArray;

	ts->vol.array_Size = arraySize;
	ts->vol.arrayPar = (void *)VolArray;

	if (0 == reverse) {
		ts->ISI.flag = TONESTEPARRAY;
		ts->vol.flag = TONESTEPARRAY;
	}
	else {
		ts->ISI.flag = TONESTEPARRAYREV;
		ts->vol.flag = TONESTEPARRAYREV;
	}
}


void prepareToneStep(toneStep ** ts, byte * i) {
	toneStep * toneStep_temp = &(toneStepArray[*i]);
	memset(toneStep_temp, 0, sizeof(toneStep));
	if (NULL != (*ts)) {
		(*ts)->NextToneStep = toneStep_temp; //Add current tonestep to last member NextTonStep.
	}
	(*ts) = toneStep_temp; //toneStep_temp point to current struct.
	(*i)++;
}


void SPI_TGMClass::set_tone() {
#ifdef DEBUG
	print_tone(&_tone);
#endif
	uint32_t dur_temp = 0;
	byte tsInd = 0;
	toneStep * toneStep_temp = NULL;
	if (TONE_FLAG_OFF == _tone.tone_flag) // check _tone.tone_flag is a new command.
	{
		if (TIMER1_PAUSE == timer1_mode) _timer1_resume();
	}
	else
	{
		_timer1_stop();
		_target_vol = _tone.volume;
		sweep_mode = _tone.sweep;
		dur_temp = (_tone.version < 10) ? _tone.duration : _tone.durationL; //there is a new variable "_tone.duration_2" (32bit), so you can play sound longer than 6.5s.
		sweep_fq0 = (_tone.version < 11) ? _tone.frequency0 : _tone.frequencyL0;
		sweep_fq1 = (_tone.version < 11) ? _tone.frequency1 : _tone.frequencyL1;
		sweep_fq2 = (_tone.version < 11) ? _tone.frequency2 : _tone.frequencyL2;

		memcpy((void*)chord_fq_array, &(sweep_fq0), sizeof(sweep_fq0)*_tone.chord_num);
		chord_fq_num = _tone.chord_num;

		/************step1 pretrigger delay**************/
		if (_tone.pre_sound_delay > 0)
		{
			prepareToneStep(&toneStep_temp, &tsInd);
			setDelayStep(toneStep_temp, _tone.pre_sound_delay);
		}

		/************step2 set startup fq **************/
		if (dur_temp != 0) {
			switch (sweep_mode) {
			case SWEEP_OFF:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_LINEAR:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_EXP:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_ARRAY:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_NOISE_WHITE:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS1:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS2:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_NOISE_GAUSS3:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = random_noise_fq();
				break;

			case SWEEP_CHORD:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_PEAK:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_CLICKS:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_CLICKS_RAMP_2MS:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;

			case SWEEP_AM:
				prepareToneStep(&toneStep_temp, &tsInd);
				setOneLoopStep(toneStep_temp);
				toneStep_temp->fq.flag = TONESTEPDRT;
				toneStep_temp->fq.drtPar = sweep_fq0;
				break;
			}
		}

		/************step3 ramp up**************/
		switch (_tone.step_up_flag) {
		case STEP_FLAG_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), _target_vol, 0);
			break;
		case STEP_FLAG_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 0);
			break;
		}


		/************step4 set target volume**************/
		if (VOLUME_ON == _tone.volume_mode) {
			prepareToneStep(&toneStep_temp, &tsInd);
			setOneLoopStep(toneStep_temp);
			toneStep_temp->LoopNum = 1;
			toneStep_temp->vol.flag = TONESTEPDRT;
			toneStep_temp->vol.drtPar = _target_vol;
		}

		/************step5-9 duration**************/
		uint32_t sweepLoopNum_temp = 0;
		uint32_t clicksDur_temp = _tone.clicks_dur + 4; //ramp up and down cost 4ms.
		uint32_t clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
		uint32_t normalInterval = clicksPeriod_temp - clicksDur_temp;
		uint32_t CycleNum1_temp = dur_temp / clicksPeriod_temp;
		uint32_t lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;
		toneStep * clicksStep_temp = NULL;

		if (dur_temp != 0) {
			switch (sweep_mode) {
			case SWEEP_OFF:
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, dur_temp);
				break;
			case SWEEP_LINEAR:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;
				sweep_linear_step = ((float)sweep_fq1 - (float)sweep_fq0) / ((float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear;
				break;

			case SWEEP_EXP:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;
				sweep_base = exp(log((float)sweep_fq1 / (float)sweep_fq0) / (float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepExp;
				break;

			case SWEEP_ARRAY:
				break;

			case SWEEP_NOISE_WHITE:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS1:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS2:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_NOISE_GAUSS3:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)random_noise_fq;
				break;

			case SWEEP_CHORD:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)chord_fq;
				break;

			case SWEEP_PEAK:
				sweepLoopNum_temp = dur_temp * 1000 / SWEEP_INTERVAL / 2;
				sweep_linear_step = ((float)sweep_fq1 - (float)sweep_fq0) / ((float)sweepLoopNum_temp);
				sweep_linear_step2 = ((float)sweep_fq2 - (float)sweep_fq1) / ((float)sweepLoopNum_temp);

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear;

				prepareToneStep(&toneStep_temp, &tsInd);
				setSweepLoopStep(toneStep_temp);
				toneStep_temp->LoopNum = sweepLoopNum_temp;
				toneStep_temp->fq.flag = TONESTEPFNC;
				toneStep_temp->fq.fnc = (void_fnc)sweepLinear2;
				break;

			case SWEEP_CLICKS:
				clicksDur_temp = _tone.clicks_dur;
				clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
				normalInterval = clicksPeriod_temp - clicksDur_temp;
				CycleNum1_temp = dur_temp / clicksPeriod_temp;
				lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;

				/*clicks duration*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, _tone.clicks_dur);
				clicksStep_temp = toneStep_temp;
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = _target_vol;
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = (0 != normalInterval) ? &(toneStepArray[tsInd]) : clicksStep_temp; //CycleToneStep1 point to interval1 step.
				toneStep_temp->CycleNum2 = 1;
				toneStep_temp->CycleToneStep2 = (0 != lastClickInterval_temp) ? &(toneStepArray[tsInd + 1]) : &(toneStepArray[tsInd + 2]); //CycleToneStep1 point to interval1 step.
				toneStep_temp->NextToneStep = &(toneStepArray[tsInd + 2]); //Stop tone when cycle finished.

				/*normalInterval*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, normalInterval);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->NextToneStep = clicksStep_temp; //NextToneStep point to ramp up step.

				/*lastInterval.*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, lastClickInterval_temp);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->CycleNum1 = 1;
				toneStep_temp->CycleToneStep1 = clicksStep_temp;
				toneStep_temp->NextToneStep = clicksStep_temp; //Useless, this will reset at last step.
				break;

			case SWEEP_CLICKS_RAMP_2MS:
				clicksDur_temp = _tone.clicks_dur + 4; //ramp up and down cost 4ms.
				clicksPeriod_temp = (_tone.clicks_period > clicksDur_temp) ? _tone.clicks_period : clicksDur_temp;
				normalInterval = clicksPeriod_temp - clicksDur_temp;
				CycleNum1_temp = dur_temp / clicksPeriod_temp;
				lastClickInterval_temp = (dur_temp % clicksPeriod_temp > clicksDur_temp) ? (dur_temp % clicksPeriod_temp - clicksDur_temp) : 0;

				/*ramp up*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 0);
				clicksStep_temp = toneStep_temp;

				/*clicks duration*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setDelayStep(toneStep_temp, _tone.clicks_dur);

				/*ramp down*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 1);
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = (0 != normalInterval) ? &(toneStepArray[tsInd]) : clicksStep_temp; //CycleToneStep1 point to interval1 step.
				toneStep_temp->CycleNum2 = 1;
				toneStep_temp->CycleToneStep2 = (0 != lastClickInterval_temp) ? &(toneStepArray[tsInd + 1]) : &(toneStepArray[tsInd + 2]); //CycleToneStep1 point to interval1 step.
				toneStep_temp->NextToneStep = &(toneStepArray[tsInd + 2]); //Stop tone when cycle finished.

				/*normalInterval*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, normalInterval);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->NextToneStep = clicksStep_temp; //NextToneStep point to ramp up step.

				/*lastInterval.*/
				memset(&(toneStepArray[tsInd]), 0, sizeof(toneStep));
				toneStep_temp = &(toneStepArray[tsInd]); //toneStep_temp point to current struct.
				tsInd++;
				setDelayStep(toneStep_temp, lastClickInterval_temp);
				toneStep_temp->vol.flag = TONESTEPDRT;
				toneStep_temp->vol.drtPar = 0;
				toneStep_temp->CycleNum1 = 1;
				toneStep_temp->CycleToneStep1 = clicksStep_temp;
				toneStep_temp->NextToneStep = clicksStep_temp; //Useless, this will reset at last step.
				break;

			case SWEEP_AM:
				float AMperiod = (float)1 / (float)_tone.AMFrequncy * 1000; //miniseconds.
				float rawIntervalScale = AMperiod / 2 * 1000; //microseconds.
				float minRawInt = MININT / rawIntervalScale;
				uint32_t CycleNum1_temp = (float)dur_temp / AMperiod;
				float AM_temp = 0;
				byte j = 0;
				for (byte i = 0; i < MaxAMArraySize; i++) {
					AM_temp = AM_temp + rawInterval[i];
					if ( AM_temp >= minRawInt) {
						AMInterval[j] = AM_temp * rawIntervalScale;
						AMVol[j] = 191 - i;
						j++;
						AM_temp = 0;
					}
				}
				if (0 != AM_temp) {
						AMInterval[j] = MININT;
						AMVol[j] = 0;
						j++;
						AM_temp = 0;
				}
				AMArraySize = j;
#ifdef DEBUG
				Serial.print("AMArraySize = ");
				Serial.println(AMArraySize);
				Serial.println("AMInterval[] = ");
				for (byte i = 0; i < j; i++) {
					Serial.print(AMInterval[i]);
					Serial.print(", ");
				}
				Serial.println();
				Serial.println("AMVol[] = ");
				for (byte i = 0; i < j; i++) {
					Serial.print(AMVol[i]);
					Serial.print(", ");
				}
				Serial.println();
				Serial.print("CycleNum1_temp = ");
				Serial.println(CycleNum1_temp);
#endif // DEBUG
				/*ramp up*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, AMInterval, AMVol, AMArraySize, _target_vol, 0);
				clicksStep_temp = toneStep_temp;

				/*ramp down*/
				prepareToneStep(&toneStep_temp, &tsInd);
				setVolArrayStep(toneStep_temp, AMInterval, AMVol, AMArraySize, _target_vol, 1);
				toneStep_temp->CycleNum1 = CycleNum1_temp;
				toneStep_temp->CycleToneStep1 = clicksStep_temp; //CycleToneStep1 point to interval1 step.
				break;
			}
		}
		/************step10 ramp down**************/
		switch (_tone.step_down_flag) {
		case STEP_FLAG_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), _target_vol, 1);
			break;

		case STEP_FLAG_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), _target_vol, 1);
			break;

		case STEP_FLAG_CANCEL_COS_5MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval5ms, rampCosVol5ms, sizeof(rampCosVol5ms), curVol, 1);
			break;

		case STEP_FLAG_CANCEL_COS_2MS:
			prepareToneStep(&toneStep_temp, &tsInd);
			setVolArrayStep(toneStep_temp, rampCosInterval2ms, rampCosVol2ms, sizeof(rampCosVol2ms), curVol, 1);
			break;
		}

		/************step11 close fq and vol step**************/
		if (dur_temp != 0) {
			prepareToneStep(&toneStep_temp, &tsInd);
			setOneLoopStep(toneStep_temp);

			toneStep_temp->fq.flag = TONESTEPDRT;
			toneStep_temp->fq.drtPar = 0;

			toneStep_temp->vol.flag = TONESTEPDRT;
			toneStep_temp->vol.drtPar = 0;
		}

		/************start interrupt**************/
		if (NULL != toneStep_temp)
		{
			curToneStep = &toneStepArray[0]; // Add first toneStepArray address to curToneStep.
			toneStep_temp->NextToneStep = NULL;
#ifdef DEBUG
			Serial.println(F("final tsInd = "));
			Serial.println(tsInd);
			Serial.println(F("curToneStep = "));
			Serial.println((uint32_t)curToneStep);

			for (int i = 0; i < tsInd; i++)
			{
				Serial.print(F("toneStep"));
				Serial.println(i);
				printToneStep(&toneStepArray[i]);
			}
#endif // DEBUG
		}
#ifdef DEBUG
		else  Serial.println(F("NULL == toneStep_temp"));
#endif // DEBUG
		timer1.begin(timer1Func, MININT);
	}
}

SPI_TGMClass SPI_TGM;

