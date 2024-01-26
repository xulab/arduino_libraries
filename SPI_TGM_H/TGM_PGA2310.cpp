#include "TGM_PGA2310.h"
/*--------PGA2310-------*/

volatile byte curVol = 0;

void _pga2310_init() {
	pinMode(PGA2310_cs, OUTPUT);
	digitalWrite(PGA2310_cs, HIGH);
#ifdef ARDUINO_ARCH_ESP32
  	SPI.begin(ESP32_SCK, ESP32_MI, ESP32_MO, ESP32_CS) ; // SPI.begin(sck,miso,mosi,ss);
#else
#endif

}

void _pga2310_wr(byte LGain, byte RGain) {
	digitalWrite(PGA2310_cs, LOW);
	SPI.beginTransaction(SPISettings(PGA2310SPEED, MSBFIRST, SPI_MODE0));
	SPI.transfer(LGain);//left channel
	SPI.transfer(RGain);//right channel
	SPI.endTransaction();
	digitalWrite(PGA2310_cs, HIGH);
}

void _set_vol(byte vol) {
	if (curVol != vol) {
		_pga2310_wr(vol, vol);
		curVol = vol;
	}
}
