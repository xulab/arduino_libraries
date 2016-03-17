#include <SGL.h>
#include <SSD1331.h>
#include <SPI.h>
#include "SSD1331_S_SHOW.h"

void setup() {
	SSD1331_S_SHOW.init();
	SSD1331_S_SHOW.show_line(0, "Hello world!");
	SSD1331_S_SHOW.show_line(1, "This is a demo.");
}

void loop() {

}

