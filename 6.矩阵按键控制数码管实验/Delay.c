#include "Delay.h"

void Delay_ms(u8 ms)
 
{
     u16 i;
     do{
          i = MAIN_Fosc / 10000;
          while(--i);   //10T per loop
     }while(--ms);
}

void Delay1us(u8 us)	//@11.0592MHz
{
	u16 i;
     do{
          i = MAIN_Fosc / 10000000;
          while(--i);   //10T per loop
	}while (--us);
}
