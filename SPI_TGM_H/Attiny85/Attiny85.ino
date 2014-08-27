#ifndef _BV(bit)
#define _BV(bit) (1 << (bit))
#endif

#ifndef NOP()
#define NOP() __asm__ __volatile__ ("nop")
#endif

#define _INPUT_ PINB
#define _OUTPUT_ PORTB
#define HOST_REQ _BV(PORTB0)
#define HOST_BUSY _BV(PORTB1)
#define GUEST_REQ _BV(PORTB2)
#define GUEST_BUSY _BV(PORTB3)

#define INPUT_MASK (HOST_REQ | GUEST_REQ)
#define OUTPUT_MASK (HOST_BUSY | GUEST_BUSY)

#define HOST_BUSY_H _OUTPUT_|=HOST_BUSY
#define HOST_BUSY_L _OUTPUT_&=~HOST_BUSY
#define GUEST_BUSY_H _OUTPUT_|=GUEST_BUSY
#define GUEST_BUSY_L _OUTPUT_&=~GUEST_BUSY

void setup() {
  // put your setup code here, to run once:
  DDRB = 0;
  NOP();
  PORTB = 0;
  NOP();
  DDRB |= OUTPUT_MASK;
  NOP();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(1){
  
    while(1){
      if(_INPUT_ & HOST_REQ){
        GUEST_BUSY_L;
        HOST_BUSY_H;
      }else{
        GUEST_BUSY_L;
        HOST_BUSY_L;
        break;
      }
    }
    
    while(1){
      if(_INPUT_ & GUEST_REQ){
        HOST_BUSY_L;
        GUEST_BUSY_H;
      }else{
        GUEST_BUSY_L;
        HOST_BUSY_L;
        break;
      }
    }
    
  }
  
}
