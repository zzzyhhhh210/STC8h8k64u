#include "Key.h"

void Key_Init(void)
{
	P0M0=0x00;
	P0M1=0x00;//工作模式，双向
	
}


unsigned char Key_scan(void)//2行4列
{
	unsigned char row, column, key_volume;
	Key_Init();
	P0 = 0x0F;  //列线P0.0~P0.3=1,行线P0.6~P0.7=1(未选中)
//	P7=0x7F;
		if(P0 != 0x0F)//按键是否按下（拉低）
		{
			Delay_ms(10);//消抖：硬件消抖加电容等，软件消抖加延时跳过
			if(P0 != 0x0F)//按键是否按下
			{
				//1.行扫描

				//第0行扫描，拉低P0.6(P0=0xBF=1011 1111)
				P0 = 0xBF;				
				if(P0 != 0xBF)
				{
					row = 0;//第0行有键被按下
					goto column_scan;
				}


				//第1行扫描，拉低P0.7(P0=0x7F=0111 1111)
				P0 = 0x7F;
				Delay_ms(10);//电平稳定延时,不加不行
				if(P0 != 0x7F)
				{
					row = 1;
					goto column_scan;
				}
				
				//2.列扫描
				column_scan:
							if ((P0&0x01) == 0) column = 0;		//P0.0 拉低 -> 第0列 1011 1111 ---> 1011 1101 &0000 0010 P0=0xBF
							else if ((P0&0x02) == 0) column = 1;	//P0.1 拉低 -> 第1列
							else if ((P0&0x04) == 0) column = 2;	//P0.2 拉低 -> 第2列
							else if	((P0&0x08) == 0) column = 3;					//P0.3 拉低 -> 第3列
						
							//计算按键值
							key_volume = row*4+column;
						
							//等待按键释放（避免长按重复触发）
							while (P0 != (row == 0 ? 0xBF : 0x7F));
							Delay_ms(10);//释放消抖

							return (key_volume);

			}
		}
		else return (16);//无按键按下
		P0=0xFF;
}


void Key_Num_test(void)
{
	unsigned char KeyNum;
	Key_Init();
	while(1)
		{	
			KeyNum = 0;
			KeyNum=Key_scan();
			if(KeyNum != 16)
			{
				Nix_Num(8,KeyNum);
			}
			Delay_ms(100);
		}

}