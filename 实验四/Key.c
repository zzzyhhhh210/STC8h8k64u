#include "Key.h"
extern uint8_t led_lec[8];
extern uint8_t key4_num;
extern unsigned char code LedChar[];
extern bit flag, flash_state, mode, led_f;
#define ROW_0 0xBF
#define ROW_1 0x7F

void Key_Init(void)
{
	P0M0=0x00;
	P0M1=0x00;//工作模式，双向	
}

unsigned char Key_scan(void)
{
	unsigned char row,column, key_volume;
	P0 = 0X0F;
	if(P0 !=0x0F)
	{
		Delay_ms(10);
		if(P0 !=0x0F)
		{
			P0 = 0XBF;
			if(P0!=0xBF)
			{
				row=0;
				goto colume_scan;
			}
			P0 = 0X7F;
			Delay_ms(10);
			if(P0!=0x7F)
			{
				row=1;
				goto colume_scan;
			}
			return(16);
			
			colume_scan:
						if((P0&0X01)==0) column=0;
						else if((P0&0X02)==0)column=1;
						else if((P0&0X04)==0)column=2;
						else column=3;
						
						key_volume=row*4+column;
            
						while((P0!=0x7F) && (P0!=0xBF));
						return(key_volume); 
		}
		return(16);
	}
	else 	
		return(16);
}
