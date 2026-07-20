/************

数码管显示数据（中断控制，串口发送

***************/

#include <stc8h.h>
#include "Usart.h"
#include "Delay.h"
#include "Nix.h"

unsigned char temp;

int main(void)
{  
	Nix_Init();
	Uart1_Init();		  
    while(1)
	{
		P6=0XFF;
		P7=0xfe;   
		P6 = Nix[temp%16];
		Delay_ms(5);//保证数码管显示稳定，避免闪烁
		  
		P6=0XFF;
		P7=0xfd;    
		P6 = Nix[temp/16];
		Delay_ms(5);//保证数码管显示稳定，避免闪烁	
	
	}
	  
}

void inter0() interrupt 4
{
	temp = SBUF;
	RI=0;		
}

