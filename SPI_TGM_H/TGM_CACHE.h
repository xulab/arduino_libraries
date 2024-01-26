#ifndef _TGM_CACHE_H_H
#define _TGM_CACHE_H_H
#include <CACHE.h>


/*-----------23LC1024------------*/
//suit for ESP32_PICO_mini_02
// #define ESP32_INFO 34
// #define ESP32_REQ 7
// #define ESP32_WR 8
// #define ESP32_PER 38
// #define ESP32_MI 19
// #define ESP32_CS 20
// #define ESP32_SCK 21
// #define ESP32_MO 22

//suit for ESP32_S2
#define ESP32_INFO 34
#define ESP32_REQ 35
#define ESP32_WR 36
#define ESP32_PER 14
#define ESP32_CS 16
#define ESP32_SCK 39
#define ESP32_MO 17
#define ESP32_MI 18


void cacheRead(uint32_t addr, uint16_t size, char *data);
void cacheWrite(uint32_t addr, uint16_t size, char *data);
void _SPI_INIT();
int cacheNewInfo();

#endif
