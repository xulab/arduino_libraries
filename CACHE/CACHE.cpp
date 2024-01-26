#include <CACHE.h>
#include <SPI.h>

#define Mega2560_CS 53
#define Mega2560_SCK 52
#define Mega2560_MO 51
#define Mega2560_MI 50

#define Mega328_CS 10
#define Mega328_SCK 13
#define Mega328_MO 11
#define Mega328_MI 12

#define read_code 0x03
#define wr_code 0x02
#define EDIO 0x3B  // Enter Dual I/O access
#define EQIO 0x38  // Enter Quad I/O access
#define RSTIO 0xFF // Reset Dual and Quad I/O access
#define RDMR 0x05  // Read Mode Register
#define WRMR 0x01  // Write Mode Register

// #define DEBUG 1

void CACHE_Class::init(int board_type)
{

#if defined(__AVR_ATmega328P__)
	CS = Mega328_CS;
	SCK = Mega328_SCK;
	MO = Mega328_MO;
	MI = Mega328_MI;
#endif
#if defined(__AVR_ATmega2560__)
	CS = Mega2560_CS;
	SCK = Mega2560_SCK;
	MO = Mega2560_MO;
	MI = Mega2560_MI;
#endif

#ifdef DEBUG
	Serial.print("INFO=");
	Serial.println(INFO);
	Serial.print("REQ=");
	Serial.println(REQ);
	Serial.print("WR=");
	Serial.println(WR);
	Serial.print("PER=");
	Serial.println(PER);
	Serial.print("CS=");
	Serial.println(CS);
	Serial.print("SCK=");
	Serial.println(SCK);
	Serial.print("MO=");
	Serial.println(MO);
	Serial.print("MI=");
	Serial.println(MI);
#endif
	// pinMode(REQ, INPUT);digitalWrite(REQ, LOW);
	// pinMode(WR, INPUT);digitalWrite(WR, LOW);
	// pinMode(CS, INPUT);digitalWrite(CS, LOW);
	// pinMode(SCK, INPUT);digitalWrite(SCK, LOW);
	// pinMode(MO, INPUT);digitalWrite(MO, LOW);

	pinMode(PER, INPUT);  // digitalWrite(PER, LOW);
	pinMode(INFO, INPUT); // digitalWrite(INFO, LOW);
	pinMode(REQ, OUTPUT);
	digitalWrite(REQ, LOW);
	pinMode(WR, OUTPUT);
	digitalWrite(WR, LOW);
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
	pinMode(SCK, OUTPUT);
	digitalWrite(SCK, LOW);
	pinMode(MO, OUTPUT);
	digitalWrite(MO, LOW);
	pinMode(MI, INPUT); // digitalWrite(MI, LOW);

#if defined(ARDUINO_ARCH_ESP32)
	SPI.begin(SCK, MI, MO, CS); // SPI.begin(sck,miso,mosi,ss);
								// SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#else
	SPI.begin();
	// SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#endif

#ifdef DEBUG
	Serial.print("INFO=");
	Serial.println(INFO);
	Serial.print("REQ=");
	Serial.println(REQ);
	Serial.print("WR=");
	Serial.println(WR);
	Serial.print("PER=");
	Serial.println(PER);
	Serial.print("CS=");
	Serial.println(CS);
	Serial.print("SCK=");
	Serial.println(SCK);
	Serial.print("MO=");
	Serial.println(MO);
	Serial.print("MI=");
	Serial.println(MI);
#endif
}

int CACHE_Class::new_info()
{
	return digitalRead(INFO);
}

// time = 0 means timeout is infint.
int CACHE_Class::request(long timeout)
{
	// void CACHE_Class::request(){
	digitalWrite(REQ, HIGH);
#ifdef DEBUG
	Serial.print("digitalRead(PER)=");
	Serial.println(digitalRead(PER));
	Serial.print("LOW == digitalRead(PER) = ");
	Serial.println(LOW == digitalRead(PER));
#endif
	// while(LOW == digitalRead(PER));
	long tmpTime = millis();
	while (LOW == digitalRead(PER))
	{
		if (timeout != -1) //-1 indicate timeout is inifint.
		{
			if (((long)millis() - tmpTime) > timeout)
			{
				return -1; // means timeout.
			}
		}
	}
	return 0; // means request successfully.
}

void CACHE_Class::release()
{
	digitalWrite(REQ, LOW);
	digitalWrite(WR, LOW);
}

void CACHE_Class::write(uint32_t addr, uint16_t size, char *data)
{
	digitalWrite(CS, LOW);
	digitalWrite(WR, HIGH);

#if defined(ARDUINO_ARCH_ESP32)
	// SPI.begin(SCK,MI,MO,CS); // SPI.begin(sck,miso,mosi,ss);
	SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#else
	SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#endif
	SPI.transfer(wr_code);
	SPI.transfer(addr >> 16);
	SPI.transfer(addr >> 8);
	SPI.transfer(addr);
	for (uint16_t i = 0; i < size; i++)
	{
		SPI.transfer(data[i]);
	}

#if defined(ARDUINO_ARCH_ESP32)
	SPI.endTransaction();
#else
	SPI.endTransaction();
#endif
	digitalWrite(CS, HIGH);
	digitalWrite(WR, LOW);
}

void CACHE_Class::read(uint32_t addr, uint16_t size, char *data)
{
	digitalWrite(CS, LOW);
#if defined(ARDUINO_ARCH_ESP32)
	// SPI.begin(SCK,MI,MO,CS); // SPI.begin(sck,miso,mosi,ss);
	SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#else
	SPI.beginTransaction(SPISettings(SPISPEED, MSBFIRST, SPI_MODE0));
#endif
	SPI.transfer(read_code);
	SPI.transfer(addr >> 16);
	SPI.transfer(addr >> 8);
	SPI.transfer(addr);

	for (uint16_t i = 0; i < size; i++)
	{
		data[i] = SPI.transfer(0);
	}

#if defined(ARDUINO_ARCH_ESP32)
	SPI.endTransaction();
#else
	SPI.endTransaction();
#endif
	digitalWrite(CS, HIGH);
}

// void CACHE_Class::q_write(uint32_t addr, uint16_t size, char *data){
int CACHE_Class::q_write(uint32_t addr, uint16_t size, char *data, long timeout)
{
	// request();
	if (0 == request(timeout))
	{
		write(addr, size, data);
		release();
		return 0; // successful
	}
	else
	{
		return -1; //-1 indicate timeout.
	}
}

int CACHE_Class::q_read(uint32_t addr, uint16_t size, char *data, long timeout)
{
	// request();
	// if (-1 == request(timeout)) return -1; //-1 indicate timeout.
	if (0 == request(timeout))
	{
		read(addr, size, data);
		release();
		return 0; // successful
	}
	else
	{
		return -1; //-1 indicate timeout.
	}
}

CACHE_Class CACHE;
