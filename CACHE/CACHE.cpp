#include <CACHE.h>
#include <SPI.h>




#define Mega2560_CS 53
#define Mega2560_SCK 52
#define Mega2560_MO 51
#define Mega2560_MI 50

#define read_code 0x03
#define wr_code 0x02
#define EDIO 0x3B //Enter Dual I/O access
#define EQIO 0x38 //Enter Quad I/O access
#define RSTIO 0xFF //Reset Dual and Quad I/O access
#define RDMR 0x05 //Read Mode Register
#define WRMR 0x01 //Write Mode Register


void CACHE_Class::init(int board_type){
	switch(board_type){
	case MEGA2560:
		CS=Mega2560_CS;
		SCK=Mega2560_SCK;
		MO=Mega2560_MO;
		MI=Mega2560_MI;
		break;
	}

	pinMode(REQ, INPUT);digitalWrite(REQ, LOW);
	pinMode(WR, INPUT);digitalWrite(WR, LOW);
	pinMode(PER, INPUT);digitalWrite(PER, LOW);
	pinMode(INFO, INPUT);digitalWrite(INFO, LOW);
	pinMode(CS, INPUT);digitalWrite(CS, LOW);
	pinMode(SCK, INPUT);digitalWrite(SCK, LOW);
	pinMode(MO, INPUT);digitalWrite(MO, LOW);
	pinMode(MI, INPUT);digitalWrite(MI, LOW);

	pinMode(REQ, OUTPUT);digitalWrite(REQ, LOW);
	pinMode(WR, OUTPUT);digitalWrite(WR, LOW);
	pinMode(CS, OUTPUT);digitalWrite(CS, HIGH);
	pinMode(SCK, OUTPUT);digitalWrite(SCK, LOW);
	pinMode(MO, OUTPUT);digitalWrite(MO, LOW);

	SPI.setDataMode(SPI_MODE0);
	SPI.setClockDivider(SPI_CLOCK_DIV4); //half speed, if the wire is too long may set this to SPI_CLOCK_DIV8
	SPI.begin();
}

int CACHE_Class::new_info(){
	return digitalRead(INFO);
}

void CACHE_Class::request(){
	digitalWrite(REQ, HIGH);
	while(LOW == digitalRead(PER));
}


void CACHE_Class::release(){
	digitalWrite(REQ, LOW);
	digitalWrite(WR, LOW);
}


void CACHE_Class::write(uint32_t addr, uint16_t size, char *data){
	digitalWrite(WR, HIGH);
	digitalWrite(CS, LOW);

	SPI.transfer(wr_code);
	SPI.transfer(addr>>16);
	SPI.transfer(addr>>8);
	SPI.transfer(addr);
	for(uint16_t i = 0; i<size; i++){
		SPI.transfer(data[i]);
	}

	digitalWrite(CS, HIGH);
	digitalWrite(WR, LOW);
}

void CACHE_Class::read(uint32_t addr, uint16_t size, char *data){
	digitalWrite(CS, LOW);

	SPI.transfer(read_code);
	SPI.transfer(addr>>16);
	SPI.transfer(addr>>8);
	SPI.transfer(addr);

	for(uint16_t i = 0; i < size; i++){
		data[i] = SPI.transfer(0);
	}

	digitalWrite(CS, HIGH);
}


void CACHE_Class::q_write(uint32_t addr, uint16_t size, char *data){
	request();
	write(addr, size, data);
	release();
}


void CACHE_Class::q_read(uint32_t addr, uint16_t size, char *data){
	request();
	read(addr, size, data);
	release();
}


CACHE_Class CACHE;
