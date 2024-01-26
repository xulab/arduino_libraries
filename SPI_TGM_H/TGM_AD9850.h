#ifndef _TGM_AD9850_H
#define _TGM_AD9850_H
#include "Arduino.h"

void _ad9850_data_wr(byte data); 
void _ad9850_init_port(); 
void _ad9850_reset(); 
void _ad9850_init(); 
void _ad9850_wr_parallel(uint32_t fq); 
void _set_fq(uint32_t fq); 

#endif

