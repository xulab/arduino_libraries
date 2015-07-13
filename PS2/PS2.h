/*Version:01
 * Create in 20141119
 * This library can read PS2 mouse's x y speed.
 * This library only suit for UNO, NANO and mega2560.
 * Clock pin on the mouse must be connected to pin2. And connect data pin to arbitrary pin you like.
 */


#define RES_SIZE 8

/**********READ/WRITE FLAG DEFINATION*************/
#define NON_READ 0
#define READ 1 
#define WRITE 2 
#define NONE_NEW_BYTE 0
#define NEW_BYTE 1
#define NO_ERROR 0
#define READ_START_ERROR 2
#define READ_PARITY_ERROR 3
#define READ_STOP_ERROR 4
#define READ_TIMEOUT 5 
#define WRITE_TIME_OUT 6
#define WRITE_STOP_ERROR 7
#define WRITE_ACK_ERROR 8

#define REMOTE 2
#define STREAM 3




struct config_byte
{
	volatile unsigned char R_button:1;
	volatile unsigned char M_button:1;
	volatile unsigned char L_button:1;
	volatile unsigned char Always_0_1:1;
	volatile unsigned char Scaling:1;
	volatile unsigned char Enable:1;
	volatile unsigned char Mode:1;
	volatile unsigned char Alway_0_2:1;
};


typedef struct 
{
	volatile unsigned char response;
	volatile struct  config_byte byte;
	volatile unsigned char Resolution;
	volatile unsigned char Samp_rate;
	volatile unsigned char ID;
} mouse_conf;


struct data_byte
{
	volatile unsigned char L_button:1;
	volatile unsigned char R_button:1;
	volatile unsigned char M_button:1;
	volatile unsigned char Always_1:1;
	volatile unsigned char X_signbit:1;
	volatile unsigned char Y_signbit:1;
	volatile unsigned char X_overflow:1;
	volatile unsigned char Y_overflow:1;
};




typedef struct 
{
	volatile unsigned char response;
	volatile struct data_byte byte;
	volatile char X;
	volatile char Y;
	volatile unsigned long counts;
	volatile unsigned long time;
} mouse_data;


typedef struct 
{
	volatile unsigned char response;
	volatile unsigned char ID;
} mouse_id;



#ifndef _PS2_h
#define _PS2_h
#include "Arduino.h"

class PS2_Class
{
private:

	inline void enable_read();
	//volatile uint8_t res[RES_SIZE] = {};
	uint8_t write_byte(uint8_t data);
	void read_bytes(void * addr, unsigned char n);

public:

	mouse_conf mouse_config;
	mouse_id mouse_ID;
	mouse_data mouse;
	void read_mouse();
	void init_mouse(unsigned char mode);
	void enable_read_mouse_data();
	
};

extern PS2_Class PS2;
#endif




