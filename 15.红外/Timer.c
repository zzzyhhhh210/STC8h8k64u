#include "Timer.h"
extern unsigned char T0RH;
extern unsigned char T0RL;

void Timer1_Init(void)		//5ms@11.0592MHz ¶¨Ê±Æ÷1ÖÐ¶Ï  T1  MODE1
{
	P7M0=0XFF;
	P7M1=0XFF;
	P6M0=0X00;
	P6M1=0X00;
	AUXR = 0x40;	
	
	TMOD =0x10;  	
	TCON = 0X40;	
	TL1 = 0x00;				
	TH1 = 0x28;								
	TR1 = 1;				
	
//	EA = 1;
	ET1 = 1;
}

void ConfigTimer0(unsigned int ms)
{
	unsigned long tmp;//
	tmp = 11059200 / 12 / 1000 * ms;//计算计数频率
	tmp = 65536 - tmp;//计算重装值
	tmp =  tmp +13;//补偿中断相应延时误差
	T0RH = (unsigned char)(tmp >> 8);
	T0RL = (unsigned char)tmp;
	TMOD &=0xF0;//清零控制位
	TMOD |=0x01;//选择16位模式
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;//打开定时器0中断
	TR0 = 1;//启动定时器0
}

void InterruptTimer0(void) interrupt 1
{
	EXINT1_ISR();
	TH0 = T0RH;
	TL0 = T0RL;
	LedScan();//数码管动态扫描
}
// void Timer1(void) interrupt 3//ÖÐ¶Ï·þÎñº¯ÊýµÄ¹Ø¼ü×Ö 1ÊÇÖÐ¶ÏºÅ£¬¶¨Ê±Æ÷1ÖÐ¶Ï  T1  MODE1
// { 
// 	EXINT1_ISR();
// 	TH0 = T0RH;
// 	TL0 = T0RL;
// 	LedScan();//数码管动态扫描
// }
