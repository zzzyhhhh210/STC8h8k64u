#include "main.h"

sbit BUZZ  = P5^4;

// 全局变量定义
volatile uint8_t code time[] = {26,4,20,23,59,59};

uint8_t cnt = 0;
uint8_t clok_time[8] = {0,0,0xbe,0,0,0xbe,0,0}; 

void main(void)	
{ 	   
	unsigned char i;
	unsigned char temp;
	
	P_SW2 |= 0x80;
	P5M1 = 0X00; P5M0 = 0X00;	//蜂鸣器初始化
	Nix_Init();
	RTC_config();
	Timer0_Init();
	
    while(1)
    {
		if(cnt == 0)
		{
			clok_time[0] = HOUR/10;
			clok_time[1] = HOUR%10;
			clok_time[3] = MIN/10;
			clok_time[4] = MIN%10;
			clok_time[6] = SEC/10;
			clok_time[7] = SEC%10;
		}
		for(i=0;i<8;i++)
		{
			P7 = _crol_(0xfe,i);
			
			if(i==2 || i==5)
				P6 = clok_time[i];
			else 
				P6 = Nix[clok_time[7-i]];
			Delay_ms(1);
			P6 = 0xFF;
		}
		if(HOUR == 0 & MIN == 0 & SEC == 0){
			BUZZ = 0;
			}
		else if(HOUR == 0 & MIN == 0 & SEC == 1)
			BUZZ = 1;
//		if (P06 == 0)
//		{
//			Delay_ms(20);
//			if (P06 == 0)
//			{
//				while(P06 == 0);
//				BUZZ = 1;
//			}
//		
//		}
		
		
    }
}


