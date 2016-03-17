#include <hidboot.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#include "BTD.h"
#include "BTHID.h"
#include "PS3BT.h"
#include "PS3Enums.h"
#include "PS3USB.h"
#include "PS4BT.h"
#include "PS4Parser.h"
#include "PS4USB.h"
#include "PSBuzz.h"
#include "SPP.h"
#include "Usb.h"
#include "UsbCore.h"
#include "Wii.h"
#include "XBOXOLD.h"
#include "XBOXONE.h"
#include "XBOXRECV.h"
#include "XBOXUSB.h"
#include "address.h"
#include "adk.h"
#include "avrpins.h"
#include "cdc_XR21B1411.h"
#include "cdcacm.h"
#include "cdcftdi.h"
#include "cdcprolific.h"
#include "confdescparser.h"
#include "controllerEnums.h"
#include "hexdump.h"
#include "hid.h"
#include "hidboot.h"
#include "hidescriptorparser.h"
#include "hiduniversal.h"
#include "hidusagestr.h"
#include "macros.h"
#include "masstorage.h"
#include "max3421e.h"
#include "max_LCD.h"
#include "message.h"
#include "parsetools.h"
#include "printhex.h"
#include "settings.h"
#include "sink_parser.h"
#include "usb_ch9.h"
#include "usbhost.h"
#include "usbhub.h"
#include "version_helper.h"
#include "xboxEnums.h"

#endif

class MouseRptParser : public MouseReportParser
{
protected:
	void OnMouseMove	(MOUSEINFO *mi);
	void OnLeftButtonUp	(MOUSEINFO *mi);
	void OnLeftButtonDown	(MOUSEINFO *mi);
	void OnRightButtonUp	(MOUSEINFO *mi);
	void OnRightButtonDown	(MOUSEINFO *mi);
	void OnMiddleButtonUp	(MOUSEINFO *mi);
	void OnMiddleButtonDown	(MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
    Serial.print("dx=");
    Serial.print(mi->dX, DEC);
    Serial.print(" dy=");
    Serial.println(mi->dY, DEC);
};
void MouseRptParser::OnLeftButtonUp	(MOUSEINFO *mi)
{
    Serial.println("L Butt Up");
};
void MouseRptParser::OnLeftButtonDown	(MOUSEINFO *mi)
{
    Serial.println("L Butt Dn");
};
void MouseRptParser::OnRightButtonUp	(MOUSEINFO *mi)
{
    Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown	(MOUSEINFO *mi)
{
    Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp	(MOUSEINFO *mi)
{
    Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown	(MOUSEINFO *mi)
{
    Serial.println("M Butt Dn");
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

uint32_t next_time;

MouseRptParser                               Prs;

void setup()
{
    Serial.begin( 115200 );
#if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay( 200 );

    next_time = millis() + 5000;

    HidMouse.SetReportParser(0,(HIDReportParser*)&Prs);
}

void loop()
{
  Usb.Task();
}

