#include "Delay.h"

void Delay_ms(uint8_t ms) 
{
     uint16_t i;
     do{
          i = MAIN_Fosc / 10000;
          while(--i);   //10T per loop
     }while(--ms);
}

void Delay_us(uint16_t us)	//@12.000MHz
{
	while(us--){
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	}
}
