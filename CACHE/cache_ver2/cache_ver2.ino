#ifndef _BV(bit)
#define _BV(bit) (1 << (bit))
#endif

#ifndef NOP()
#define NOP() __asm__ __volatile__ ("nop")
#endif

#define _INPUT_ PINA
#define _OUTPUT_ PORTA

#define R_REQ _BV(PORTA0)
#define R_WR _BV(PORTA1)
#define R_PER _BV(PORTA2)
#define R_INFO _BV(PORTA3)
#define L_REQ _BV(PORTA4)
#define L_WR _BV(PORTA5)
#define L_PER _BV(PORTA6)
#define L_INFO _BV(PORTA7)

#define INPUT_MASK (R_REQ | R_WR | L_REQ | L_WR)
#define OUTPUT_MASK (R_PER | R_INFO | L_PER | L_INFO)

#define R_PER_H _OUTPUT_|=R_PER
#define R_PER_L _OUTPUT_&=~R_PER
#define R_INFO_H _OUTPUT_|=R_INFO
#define R_INFO_L _OUTPUT_&=~R_INFO

#define L_PER_H _OUTPUT_|=L_PER
#define L_PER_L _OUTPUT_&=~L_PER
#define L_INFO_H _OUTPUT_|=L_INFO
#define L_INFO_L _OUTPUT_&=~L_INFO

#define R_REQ_G (_INPUT_&R_REQ)!=0
#define R_WR_G (_INPUT_&R_WR)!=0
#define L_REQ_G (_INPUT_&L_REQ)!=0
#define L_WR_G (_INPUT_&L_WR)!=0


#define SRAM_OUTPUT PORTB
#define SRAM_CS _BV(PORTB1)
#define SRAM_SCK _BV(PORTB2)
#define SRAM_CS_H SRAM_OUTPUT|=SRAM_CS
#define SRAM_CS_L SRAM_OUTPUT&=~SRAM_CS
#define SRAM_SCK_H SRAM_OUTPUT|=SRAM_SCK
#define SRAM_SCK_L SRAM_OUTPUT&=~SRAM_SCK


void setup() {
/*  Initial status port  */
  DDRA = 0;
  NOP();
  PORTA = 0;
  NOP();
  DDRA |= OUTPUT_MASK;
  NOP();
  
/* Reset SRAM */
  DDRB = 0;
  NOP();
  PORTB = 0;
  NOP();
  DDRB |= SRAM_CS | SRAM_SCK;
  NOP();
  SRAM_CS_H;
  SRAM_SCK_L;
  delay(1);
  
  SRAM_CS_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_CS_H;
  
  delay(1);
  SRAM_CS_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_SCK_H;
  delay(1);
  SRAM_SCK_L;
  delay(1);
  SRAM_CS_H;
  delay(1);
  
  NOP();
  DDRB = 0;
  NOP();
  PORTB = 0;
  NOP();
}


void loop() {
  // put your main code here, to run repeatedly:
  while(R_REQ_G){
    R_PER_H;
    R_INFO_L;
    if(R_WR_G) L_INFO_H;
  }
  R_PER_L;
  
  while(L_REQ_G){
    L_PER_H;
    L_INFO_L;
    if(L_WR_G) R_INFO_H;
  }
  L_PER_L;
}
