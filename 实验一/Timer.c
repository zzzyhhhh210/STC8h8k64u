#include "Timer.h"

void Timer0_Init(void)      //¶¨Ê±Æ÷0ÖÐ¶Ï  T0  MODE0
{
	P7M0=0XFF;
	P7M1=0XFF;
	P6M0=0X00;
	P6M1=0X00;		
	AUXR = 0X00; //¸¨Öú¼Ä´æÆ÷ÅäÖÃ	
	
	TMOD = 0X00;//¶¨Ê±Æ÷Ä£Ê½¼Ä´æÆ÷ÅäÖÃ
	TCON = 0X10;
	TL0 = 0XB0;
	TH0 = 0X3C;//50ms
	
	EA = 1;
	ET0 = 1;//ÖÐ¶ÏÔÊÐíÎ»	
}
