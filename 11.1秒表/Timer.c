#include "Timer.h"

// 定义全局标志位
volatile bit g_timer_flag = 0;

static unsigned char timer_cnt = 0; 

extern volatile unsigned char sec; // 引用 main.c 中的 sec

extern volatile unsigned char min,hour;

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

// 定时器0中断服务函数
void Timer0_ISR(void) interrupt 1
{
	TL0 = 0XB0;
	TH0 = 0X3C;
    
    timer_cnt++;
    if (timer_cnt >= 20) 
    {
        timer_cnt = 0;
        sec++;
        if (sec > 59)
        {
            sec = 0;
			min++;
			if(min > 59)
			{
				min=0;
				hour++;
				if(hour > 23)
				{
					hour=0;
				}

			}

        }
    }
    
    g_timer_flag = 1; // 置位标志，通知主循环（可选，用于其他同步任务）
}


