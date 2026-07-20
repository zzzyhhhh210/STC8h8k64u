#include "Key.h"

#define ROW_0 0xBF
#define ROW_1 0x7F
void Key_Init(void)
{
	P0M0=0x00;
	P0M1=0x00;//工作模式，双向	
}

unsigned char Key_scan(void)
{
	unsigned char row, column, key_volume;
	Key_Init();
	P0 = 0x0F;
		if(P0 != 0x0F)
		{
			Delay_ms(10);
			if(P0 != 0x0F)//0000 1111
			{
				//1.行扫描	

				
				P0 = ROW_0;				
				if(P0 != ROW_0)
				{
					row = 0;//0
					goto column_scan;
				}
				
				
				
				P0 = ROW_1;
				Delay_ms(10);
				if(P0 != ROW_1)
				{
					row = 1;//0
					goto column_scan;
				}				
				//2.列扫描
				column_scan:
							if ((P0&0x01) == 0) column = 0;		
							else if ((P0&0x02) == 0) column = 1;
							else if ((P0&0x04) == 0) column = 2;
							else if	((P0&0x08) == 0) column = 3;						
				//计算按键值
				key_volume = row*4+column;		
				while (P0 != (row == 0 ? ROW_0 : ROW_1));//1.
				Delay_ms(10);

				return (key_volume);

			}
		}
		else return (16);
		P0=0xFF;
}
