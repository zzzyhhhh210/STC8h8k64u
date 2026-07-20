#include "Nix.h"
#include "Delay.h"

uint_32 Nix[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
				0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E, 0xFF};   //共阳
//      0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
//    0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E
//    	0. 	 1.   2.   3.	4.	 5.   6.   7. 	8. 	 9.   A.   B.   C.   D.   E.   F.
//    0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10,0x08,0x03,0x46,0x21,0x06,0x0E

uint_8 T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  //位码

//P7= 7654 3210
void Nix_Init(void)
{
	P7M0=0xFF; //IO口工作模式,推挽输出
	P7M1=0xFF;
	P6M0=0x00;
	P6M1=0x00;

	P7=0xFF;    //全灭
	P6=Nix[0];  //段码 显示0
}

void Nix_Num(uint_8 digit,Number)     //静态
{
//	Nix_Init();
//	P6=0x00;//消影
	switch(digit)
		{
			case 1: P7=~T_COM[7];break;	
			case 2:	P7=~T_COM[6];break;
			case 3:	P7=~T_COM[5];break;
			case 4:	P7=~T_COM[4];break;
			case 5:	P7=~T_COM[3];break;
			case 6:	P7=~T_COM[2];break;
			case 7:	P7=~T_COM[1];break;
			case 8:	P7=~T_COM[0];break;
		    default:
					break;
		}
//	Delay_ms(1);//位选后，防止程序直接进入段选
	P6=Nix[Number];
	Delay_ms(1);  //确保亮度无闪烁,消影，控制亮度
	P6=0xff;
}   

void Nix_Display(unsigned char *buf)
{
    static unsigned char pos = 0;
	
    P7 = 0x00;// 关闭所有数码管
    P6 = Nix[buf[pos]]; // 输出当前位码
    P7 = ~(0x01 << pos);// 输出当前位码
    pos = (pos + 1) % 8;//循环切换8位
    Delay_ms(1);
}
//人眼分辨频闪的时间大概20ms
