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
	P0 = 0x0F;
	if(P0 != 0x0F)
	{
		Delay_ms(10);
		if(P0 != 0x0F)
		{
			// 行扫描
			P0 = ROW_0;
			Delay_ms(1);
			if(P0 != ROW_0)
			{
				row = 0;
			}
			else
			{
				P0 = ROW_1;
				Delay_ms(1);
				if(P0 != ROW_1)
				{
					row = 1;
				}
				else
				{
					// 未检测到按键行，返回无效值
					P0 = 0xFF;
					return 16;
				}
			}

			// 列扫描
			if((P0 & 0x01) == 0) column = 0;
			else if((P0 & 0x02) == 0) column = 1;
			else if((P0 & 0x04) == 0) column = 2;
			else if((P0 & 0x08) == 0) column = 3;
			else column = 4; // 未检测到列

			// 计算按键值
			key_volume = row * 4 + column;

			// 等待按键释放
			while(P0 != (row == 0 ? ROW_0 : ROW_1));
			Delay_ms(10);

			return key_volume;
		}
	}
	return 16; // 无按键按下
}
