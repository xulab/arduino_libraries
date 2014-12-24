
#define _PORT_ PORTA

#define pos1 B00000011
#define pos2 B00000110
#define pos3 B00001100
#define pos4 B00001001
#define step_delay 2 //ms
#define forward_step() switch(cur_pos){ \
                          case pos1: cur_pos=pos2;_PORT_ = cur_pos;break; \
                          case pos2: cur_pos=pos3;_PORT_ = cur_pos;break; \
                          case pos3: cur_pos=pos4;_PORT_ = cur_pos;break; \
                          case pos4: cur_pos=pos1;_PORT_ = cur_pos;break; \
                        }
                        
#define backward_step() switch(cur_pos){ \
                          case pos1: cur_pos=pos4;_PORT_ = cur_pos;break; \
                          case pos2: cur_pos=pos1;_PORT_ = cur_pos;break; \
                          case pos3: cur_pos=pos2;_PORT_ = cur_pos;break; \
                          case pos4: cur_pos=pos3;_PORT_ = cur_pos;break; \
                        }

byte cur_pos=pos1;
int i=0;

void setup() {
  // put your setup code here, to run once:
  DDRA = B00001111;
}

void loop() {
  // put your main code here, to run repeatedly:
  for(i=0; i<1000; i++){
    forward_step();
    delay(step_delay);
  }
  
  delay(1000);
  
    for(i=0; i<1000; i++){
      backward_step();
      delay(step_delay);
  }
  
  delay(1000);

}
