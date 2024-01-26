#ifndef _TGM_PGA2310_H
#define _TGM_PGA2310_H
#include "Arduino.h"
#include <SPI.h>
#include "TGM_CACHE.h"

#ifdef ARDUINO_ARCH_ESP32
//suit for ESP32_PICO_mini_02
// #define PGA2310_cs 5
//suit for ESP32_S2
#define PGA2310_cs 15 
#else
#define PGA2310_cs 21
#endif
#define PGA2310SPEED 32000000

extern volatile byte curVol;
void _pga2310_init();
void _pga2310_wr(byte LGain, byte RGain);
void _set_vol(byte vol);

#endif
