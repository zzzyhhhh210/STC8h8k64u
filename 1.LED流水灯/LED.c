#include "LED.h"
#include "Delay.h"
#include "intrins.h"

unsigned char LED[]={0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
unsigned char i,a,b,temp= 0x01;

//P6每个的意义
void LED_Init(void)
{
    P_SW2 |= 0x80; 

    P0M1 = 0x30;   P0M0 = 0x30;		//0011 0000 P0^4 P0^5 引脚 均为1，配置开漏输出 其余为0
    P1M1 = 0x30;   P1M0 = 0x30;   	//0011 0000 P1^4 P1^5 引脚 均为1，配置开漏输出 其余为0
    P2M1 = 0x3c;   P2M0 = 0x3c;   	//0011 1100 P2^2 P2^3 P2^4 P2^5引脚为1，开漏输出
    P3M1 = 0x50;   P3M0 = 0x50;     //0101 0000	P3^4 模拟输入 P3^5 推挽输出	
    P4M1 = 0x3c;   P4M0 = 0x3c;		//0011 1100 P4^2 P4^3 P4^4 P4^5引脚为1，开漏输出
    P5M1 = 0x0c;   P5M0 = 0x0c;   	//0000 1100	P5^2 P5^3 引脚 均为1，配置开漏输出 
    P6M1 = 0x00;   P6M0 = 0x00;   	//0000 0000 所有PxM1=0,PxM0=0,准双向口 
    P7M1 = 0x00;   P7M0 = 0x00;   	//0000 0000

    P40 = 0;		//LED Power On

}

void LED_Text1(void)
{
	unsigned char time = 0;
	LED_Init();
	P6 = 0xFF;
	for(i = 0 ; i < 8 ; i++)
	{
		P6 = ~(0x01<<i);
		Delay_ms(50);
		P6 = 0xFF;			//所有位设为1
		time += 100;
		Delay_ms(time);
	}
	Delay_ms(500);
}
//怎么实现一个程序内，速度越来越快
void LED_Text2(void)
{
	LED_Init();
	P6 = 0xFF;
	for(b = 0 ; b < 8 ; b++)
	{
		P6 = _cror_(0x7F,b);
		Delay_ms(100);
	}
	Delay_ms(500);

}

void LED_Text3(void)
{
	LED_Init();
	P6 = 0xFF;
	for(a = 0 ; a < 8 ; a++)
	{
		P6 = LED[a];
		Delay_ms(200);
	}
	Delay_ms(500);
}

void LED_Text4(void)
{ 
	LED_Init();
	P6 = 0xFF;//1111 1111
	//0x80 =  1000 0000
        for(i=8; i>=0; i--)
        {
            P6 = 0xFF;          
            P6 = ~(1 << i);  //1.怎么实现右移 
            Delay_ms(200);
        }
	Delay_ms(500);

}