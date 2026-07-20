#include "main.h"

unsigned char LedBuff[6] = {0xC0,0xC0,0xFF,0xFF,0xC0,0xC0};
// 全局变量定义
unsigned char T0RH = 0;
unsigned char T0RL = 0;
unsigned char i = 0;

uint8_t cnt = 0;
uint8_t clok_time[8] = {0,0,0xbe,0,0,0xbe,0,0}; 

unsigned char ircode[4];
bit irflag = 0;

int main(void)	
{ 	   
	P3M0 = 0x00;P3M1 = 0x10; 
	Nix_Init();
//	Timer1_Init();
	EA = 1;//开总中断

	InitInfrared();//红外初始化
	ConfigTimer0(1);//配置定时器0定时1ms

    while(1)
    {
		if(irflag)
		{
			irflag = 0;
			LedBuff[5] = Nix[ircode[0] >> 4];//用户码显示
			LedBuff[4] = Nix[ircode[0] & 0x0f];
			LedBuff[1] = Nix[ircode[2] >> 4];
			LedBuff[0] = Nix[ircode[2] & 0x0f];

		}

	}
}