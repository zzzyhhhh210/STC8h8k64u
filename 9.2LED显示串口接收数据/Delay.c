#include "Delay.h"

void Delay_ms(u8 ms) 
{
     u16 i;
     do{
          i = MAIN_Fosc / 10000;
          while(--i);   //10T per loop
     }while(--ms);
}

void Delay_us(unsigned int us)	//@12.000MHz
{
	while(us--){
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	}
}
