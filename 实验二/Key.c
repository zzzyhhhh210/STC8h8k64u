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
			else column = 3; 

			// 计算按键值
			key_volume = row * 4 + column;
			
			// 边沿处理: 非设置模式下S3按下显示亮度等级
			
			if(key_volume == 3 && mode == close_mode)
			{
				led_f = 1;
				if(key4_num != 0)
					led_lec[7] = LedChar[key4_num];
				else
					led_lec[7] = LedChar[4];
				Seg_Run();
			}
			// 等待按键释放(根据行号判断松开值)
			if(row == 0)
				while(P0 != ROW_0);   // row0松开→P0=0xBF
			else
				while(P0 != ROW_1);   // row1松开→P0=0x7F
			led_f = 0;
			
			return (key_volume);
		}
		return 16;
	}
	else
		return 16; // 无按键按下
}
