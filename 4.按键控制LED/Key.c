#include "Key.h"

void Key_Init(void)
{
	P0M0=0x00;
	P0M1=0x00;//工作模式，双向

	P00 = 0;
	
}


void Key_text1(void)//按key0亮Nix1
{
	Key_Init();
	Nix_Init();
	Delay_ms(2);
		
	if(P06 == 0)//案件是否按下
		{
			
			Nix_Num(1,2);//P7=0X7F;P6=0xA4;

		}
			Delay_ms(2);//加while不需要消影
			P7=0xFF;//对P7进行复位，不加没效果

}


void Key_text2(void)
{
	unsigned char cnt=0;
	Key_Init();
	Nix_Init();
	P7=0x7F;
	while(1)
	{
		if(P06 == 0)//按键是否按下
		{
			Delay_ms(20);//消抖：硬件消抖加电容等，软件消抖加延时跳过 10~20ms

			if(P06 == 0)//再次判断是否有键按下
			{
				if(cnt%2==1)
					P6=0xF9;	
				else
					P6=0xC0;
				cnt++;		
			while(P06==0);
			}
		
		}
//		Delay_ms(2);
//		P7=0xFF;
	}
}

void Key_text3(void)
{
	unsigned char cnt=0;
	Key_Init();
	Nix_Init();	
	Delay_ms(2);

	while(1)
		{
			if( P06 == 0)//再次判断是否有键按下
				{
					Delay_ms(20);//消抖：硬件消抖加电容等，软件消抖加延时跳过

					if( P06 == 0)//再次判断是否有键按下
					{
						Nix_Num( 1, cnt);
						cnt++;
					while( P06 == 0 );
					}
				}

//			P7=0xFF;//对P7进行复位,可不加
			if( cnt > 9 ) cnt = 0;
		}

}