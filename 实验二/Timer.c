#include "Timer.h"

void Timer0_Init(void)      //¶¨Ê±Æ÷0ÖÐ¶Ï  T0  MODE0
{
		
	AUXR = 0X00;
	
	TMOD = 0X00;
	TCON = 0X10;

//	TL0 = 0XB0;
//	TH0 = 0X3C;//50ms

	TL0 = 0xA4;   // 低8位
	TH0 = 0xFF;   // 高8位//100us
	EA = 1;
	ET0 = 1;	
}
//void timer0() interrupt 1
//{
//	cnt++;
//	sec = led_time[mode -1];
//	if(cnt == sec*1000)
//	{
//		cnt = 0;
//		if(flag !=1)LED_Run();	
//	}
//	//亮度控制
//	pwm_cnt++;//20MS
//	if(pwm_cnt  > 200)pwm_cnt = 0;

//	if(pwm_cnt  <= 30){P6 = 0xff;SegScan();}
//	else if(pwm_cnt  <bright){P6 = 0xff;P7 = 0xff;P40 = 0;P6 = P6_OUT;}
//	else {P40 = 1;}
//	
//	if(Key_Num!=0;&&set_mode ==1)
//	{
//		clock_f == ++;
//		if(clock_f == 8000)
//		{	
//			clock_f = 0;
//			flash_state = ~flash_state;
//			if(flash_state ==1)
//			{
//				LedBuff[5*Key_Num - 3]=0xff;
//				LedBuff[5*Key_Num - 4]=0xff;
//				LedBuff[5*Key_Num - 5]=0xff;
//			}
//			else seg_run();
//		}
//		
//	}
//}