/* RaspBoardPowCtrl.ino
* arduino board: pro mini 3.3v(8MHz) 
* designed by: Jingwei Pan
* version: 20160720-ver01
*
*/


#define PWR_OFF 0 
#define PWR_ON 1
#define FORCE_SD_TIME 5000
#define button_delay 20

int Caseswitch = 5;
int PWR_5V = 6;
int PS_ON = 2;
int LED_R = A0;
int LED_G = A1;
unsigned long temp_caseswitch_ms = 0;
int led_value = 0;



void setup() {
	pinMode(Caseswitch, INPUT);
	pinMode(PWR_5V, INPUT);
	pinMode(PS_ON, OUTPUT);
	pinMode(LED_G, OUTPUT);
	pinMode(LED_R, OUTPUT);
	digitalWrite(PS_ON, LOW);
	// analogWrite(LED_G, 250);
	delay(1000); //wait for ATX power init.
	analogWrite(LED_R, 255);
}

void loop() {
		if(digitalRead(Caseswitch)) delay(button_delay);
		if(digitalRead(Caseswitch))
		{
			temp_caseswitch_ms = millis();
			while(digitalRead(Caseswitch))
			{
				if((millis() - temp_caseswitch_ms) > FORCE_SD_TIME) 
				{
					digitalWrite(PS_ON, LOW); 
					analogWrite(LED_G, 0);
					// force to shutdonw;
					break;
				}
			}
			while(digitalRead(Caseswitch));

			if((millis() - temp_caseswitch_ms) < FORCE_SD_TIME) digitalWrite(PS_ON, HIGH); 
			// if time of pushing smaller than FORCE_SD_TIME turn on ATX;

			delay(1000);
		}

			// delay(100);
			// analogWrite(LED_R, 0);
			// delay(100);
			// analogWrite(LED_R, 250);

		if(digitalRead(PWR_5V)) analogWrite(LED_G, 255);
		else analogWrite(LED_G, 0);
}


