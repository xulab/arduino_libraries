#include "PS2.h"

#define PS2HOST_NUM 11
#define HOST2PS_NUM 12
#define CLOCK_PIN 2
#define TIME_OUT_MILLIS 5

#define clk_pin 2
#define data_pin 4
#define clk_pulldown_pin 6
#define data_pulldown_pin 5

#define DISENABLE 0
#define ENABLE 1

#define UNFIN 0
#define FINISHED 1

#define HIGH_RES(pin) pinMode(pin, INPUT);\
						digitalWrite(pin, LOW)

#define PULL_DOWN(pin) pinMode(pin, OUTPUT);\
						digitalWrite(pin, LOW)


uint8_t int_mode = NON_READ;
uint8_t bit_num = 0;
uint8_t byte0 = 0;
uint8_t byte1 = 0;
uint8_t byte2 = 0;
uint8_t byte3 = 0;
uint8_t byte4 = 0;
uint8_t temp_byte = 0;
uint8_t t_write_byte = 0;
uint8_t parity = 0;
uint32_t temp_millis = 0;
uint8_t clock_count = 0;


//volatile uint8_t new_byte_flag = NONE_NEW_BYTE;
volatile uint8_t res_flag = UNFIN;
//volatile uint8_t read_mouse_data_flag = DISENABLE;
//volatile uint8_t interrupt_flag = DISENABLE;
volatile uint8_t data_mode = DISENABLE;
volatile uint8_t error_flag = NO_ERROR;
volatile unsigned char temp_res[RES_SIZE] = {};
volatile void * res_addr;
volatile uint8_t res_size = 0;
volatile uint8_t temp_res_index = 0;
//volatile uint8_t read_mouse_data_count = 0;



//inline void read_mouse_data()
//{
//	//Serial.print("PS2.mouse.counts:");
//	//Serial.println(PS2.mouse.counts, HEX);
//	((unsigned char *)(&(PS2.mouse)))[read_mouse_data_count] = temp_byte;
//	read_mouse_data_count = (read_mouse_data_count + 1) %3;
//	if (read_mouse_data_count == 0) PS2.mouse.counts = PS2.mouse.counts + 1;
//}


void clock_interrupt()
{
	if (DISENABLE == data_mode)
	{
		bit_num = 0;
		return;
	}
	uint8_t t_bit;

	bit_num++;
	if (1 == bit_num) //Start bit
	{
		if (HIGH == digitalRead(data_pin))
		{
			error_flag = READ_START_ERROR;
			bit_num = 0;
			return;
		}
		temp_byte = 0;
		parity = 1; //Because PS/2 protocol use odd verify, then set parity to 1.
	}
	else if (9 >= bit_num) //Data bit
	{
		t_bit = digitalRead(data_pin);
		temp_byte |= t_bit<<(bit_num-2);
		parity ^= t_bit;
	}
	else if (10 == bit_num) //Parity bit
	{
		if (parity == digitalRead(data_pin));
		else
		{
			error_flag = READ_PARITY_ERROR;
			//return;
		}
	}
	else if (11 == bit_num)
	{
		bit_num = 0;
		if (LOW == digitalRead(data_pin))
		{
			error_flag = READ_STOP_ERROR;
			//return;
		}
		else if (error_flag == NO_ERROR)
		{
			//temp_res_index = (temp_res_index + 1) % RES_SIZE;
			temp_res[temp_res_index] = temp_byte;
			temp_res_index++;
			if (res_size == temp_res_index)
			{
				temp_res_index = 0;
				memcpy((void *)res_addr, (void *)temp_res, res_size);
				if (data_mode != STREAM)
				{
					data_mode = DISENABLE;
				}
				PS2.mouse.counts = PS2.mouse.counts + 1;
				PS2.mouse.time = millis();
				res_flag = FINISHED;
			}
			//new_byte_flag = NEW_BYTE;

			//if (read_mouse_data_flag == ENABLE) read_mouse_data();
		}
	}

	if (NO_ERROR != error_flag)
	{
		Serial.print("error_code:");
		Serial.println(error_flag);
		Serial.println(bit_num);
		error_flag = NO_ERROR;
	}
	
}


void PS2_Class::read_bytes(void * addr, unsigned char n)
{
	//read_mouse_data_flag = DISENABLE;
	//read_mouse_data_count = 0;
	//interrupt_flag = ENABLE;
	//for (int i = 0; i < n; i++)
	//{
	//	new_byte_flag = NONE_NEW_BYTE;
	//	while (NONE_NEW_BYTE == new_byte_flag);
	//	new_byte_flag = NONE_NEW_BYTE;
	//	res[i] = temp_res[temp_res_index];
	//}
	res_size = n;
	res_addr = addr;
	res_flag = UNFIN;
	data_mode = REMOTE;
	while (res_flag != FINISHED);
	res_flag = UNFIN;
}

//
//void PS2_Class::enable_read_mouse_data()
//{
//	read_mouse_data_flag = ENABLE;
//	read_mouse_data_count = 0;
//	interrupt_flag = ENABLE;
//}

uint8_t PS2_Class::write_byte(uint8_t data)
{
	data_mode = DISENABLE;
	PULL_DOWN(clk_pulldown_pin);

	delayMicroseconds(100);
	PULL_DOWN(data_pulldown_pin);
	delayMicroseconds(50);
	HIGH_RES(clk_pulldown_pin);
	delayMicroseconds(10);
	temp_millis = millis();
	for (int i = 0; i <= 10; i++)
	{
		while (HIGH == digitalRead(clk_pulldown_pin))
		{
			if (TIME_OUT_MILLIS < (millis() - temp_millis))
			{
				error_flag = WRITE_TIME_OUT;
				HIGH_RES(data_pulldown_pin); //stop bit(HIGH);
				//return WRITE_TIME_OUT;
			}
		}

		if (i < 8)
		{
			digitalWrite(data_pulldown_pin, (data >> i) & 0x01);
		}
		else if (i == 8)
		{
			uint8_t temp_parity = 1;
			for (int j = 0; j < 8; j++)
			{
				temp_parity ^= ((data >> j) & 0x01);
			}
			digitalWrite(data_pulldown_pin, temp_parity);
		}
		else if (i == 9)
		{
			HIGH_RES(data_pulldown_pin); //stop bit(HIGH);
			if (LOW == digitalRead(data_pulldown_pin))
			{
				error_flag = WRITE_STOP_ERROR;
				//return WRITE_STOP_ERROR;
			}
		}
		else if (i == 10)
		{
			if (HIGH == digitalRead(data_pulldown_pin))
			{
				error_flag = WRITE_ACK_ERROR;
				//return WRITE_ACK_ERROR;
			}
			//return NO_ERROR;
		}

		temp_millis = millis();
		while (LOW == digitalRead(clk_pulldown_pin))
		{
			if (TIME_OUT_MILLIS < (millis() - temp_millis))
			{
				error_flag = WRITE_TIME_OUT;
				//return WRITE_TIME_OUT;
			}
		}
		if (NO_ERROR != error_flag)
		{
			Serial.print("error_code:");
			Serial.println(error_flag);
			Serial.println(i);
			error_flag = NO_ERROR;
		}
	}
	HIGH_RES( clk_pulldown_pin);
	return NO_ERROR;
}



void PS2_Class::read_mouse()
{
	write_byte(0xEB); //Read Data. Only works in remote mode.
	read_bytes(&(mouse), 4);
}





void init_pin()
{
	data_mode = DISENABLE;
	//int_mode = NON_READ;
	detachInterrupt(0);

	HIGH_RES(clk_pin);
	HIGH_RES(data_pin);
	HIGH_RES(clk_pulldown_pin);
	HIGH_RES(data_pulldown_pin);
	attachInterrupt(0, clock_interrupt, FALLING);
}


void PS2_Class::init_mouse(unsigned char mode)
{
	//read_mouse_data_flag = DISENABLE;
	//read_mouse_data_count = 0;
	res_flag = UNFIN;
	temp_res_index = 0;
	data_mode = DISENABLE;
	delay(100);
	init_pin();
	delay(100);
	write_byte(0xFF); //reset mouse.
	read_bytes(& mouse_config ,1);
	Serial.print("resp of 0xFF(reset mouse.):");
	Serial.println(mouse_config.response,HEX);


	delay(1);
	write_byte(0xE9); //get mouse status().
	read_bytes(& mouse_config, 4);
	Serial.print("resp of 0xE9(get mouse status()):");
	Serial.println(mouse_config.response, HEX);
	Serial.print("Status of 0xE9:");
	Serial.println(*((byte *)(&(mouse_config.byte))), BIN);
	//Serial.println(res[1], HEX);
	//((unsigned char *)(&(mouse_config)))[1] = res[1];
	Serial.print("Resolution:");
	Serial.println(mouse_config.Resolution, HEX);
	//Serial.println(res[2], HEX);
	//mouse_config.Resolution = res[2];
	Serial.print("Sample Rate:");
	Serial.println(mouse_config.Samp_rate, HEX);
	//Serial.println(res[3], HEX);
	//mouse_config.Samp_rate = res[3];


	delay(1);
	write_byte(0xF2); //get mouse ID(mouse type).
	read_bytes(& mouse_ID,2);
	Serial.print("resp of 0xF2(get mouse ID(mouse type):");
	Serial.println(mouse_ID.response, HEX);
	Serial.print("Muse ID:");
	Serial.println(mouse_ID.ID, HEX);


	switch (mode)
	{
	case REMOTE:
		/****Remote mode*****/
		delay(1);
		write_byte(0xF5); //set mouse to remote mode.
		read_bytes(&mouse_config, 1);
		Serial.print("resp of 0xF5 (set mouse to remote mode):");
		Serial.println(mouse_config.response, HEX);
		break;

	case STREAM:
		/****Stream mode*****/
		delay(1);
		write_byte(0xF4); //set mouse to stream mode.
		read_bytes(&mouse_config, 1);
		Serial.print("resp of 0xF4(set mouse to stream mode):");
		Serial.println(mouse_config.response, HEX);
		res_size = 3;
		data_mode = STREAM;
		res_addr = &(mouse.byte);
		mouse.counts = 0;
		break;


	}


	//enable_read_mouse_data();

}

PS2_Class PS2;




