// Do not remove the include below
#include "SPI_TGM_H.h"
#include "SPI.h"

void setup()
{
	SPI_TGM.init_TGM();
}


void loop(){
	SPI_TGM.wait_command();
	SPI_TGM.read_tone();
	SPI_TGM.set_tone();
}













