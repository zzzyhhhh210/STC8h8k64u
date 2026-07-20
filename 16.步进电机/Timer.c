#include "Timer.h"

unsigned char cnt = 0;
void Timer_Init(void)
{
	P4M0=0XFF;
	P4M1=0XFF;
	P6M0=0X00;
	P6M1=0X00;
	
	P40=0;//LED阳极。三极管导通
	
	AUXR = 0X00; //辅助寄存器配置
	TMOD &= 0XF0;//定时器模式寄存器配置
	
	TMOD = 0X00;//定时器模式寄存器配置
	TL0 = 0XB0;
	TH0 = 0X3C;
	TR0 = 1;
	while(1)
	{
		if(TF0 == 1)
		{
			TF0 = 0;
			cnt ++;
			if(cnt ==20)						
			P6 =~ P6;
		}	
	}	
}

//中断时，后续功能放主函数
//中断不能传参
//中断没有返回值
//中断函数不用声明
//中断好处1.提高CPU效率2.实现实时处理功能3.实现分时操作
//
void Timer0(void) interrupt 0
{ 
		cnt ++;
		if(cnt == 20)
		{
			cnt = 0;					
			P6 =~ P6;
		}

}
//void Timer0(void) interrupt 0
//{ 
//		if(TF0 == 1)
//		{
//			TF0 = 0;
//			cnt ++;
//			if(cnt ==20)						
//			P6 =~ P6;
//		}

//}
/*查询法，一般定时器不用
void Timer0_Init(void)		//@11.0592MHz
{
	AUXR &= 0x7F;			//12分频  XXXX XXXX & 0111 1111 -> 0XXX XXXX
	TMOD &= 0xF0;			//模式 
	TL0 = 0x66;				
	TH0 = 0xFC;				
	TF0 = 0;				
	TR0 = 1;				
}
*/