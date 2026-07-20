/************

led显示数据（中断控制，串口发送

***************/


#include <stc8h.h>
#include "Usart.h"
#include "Delay.h"
#include "LED.h"

unsigned char temp=0XFF;

int main(void)
{  
	LED_Init();
	Uart1_Init();
    while(1);
	  
}

void inter0() interrupt 4
{
	temp = SBUF;
	RI=0;
	P6=temp;
}

