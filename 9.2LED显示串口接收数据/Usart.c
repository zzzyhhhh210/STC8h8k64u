#include "Usart.h"
#include "Delay.h"


void Uart1_Init(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据，可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1定时器1
	TMOD &= 0x0F;		//定时器模式
	TMOD |= 0x20;		//定时器模式
	TL1 = 0xFD;			//定时器初值
	TH1 = 0xFD;			//重装置
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计数
	
	EA = 1;//外部中断
	ES = 1;
}

