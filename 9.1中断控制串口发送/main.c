#include <stc8h.h>
#include "Usart.h"
#include "Delay.h"
#include "Nix.h"

unsigned char temp;
/*
@利用外部中断发送一个数据至PC端
@解决按下和抬起都有接收到（外部中断触发方式，默认上升沿下降沿都触发）

*/
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

void inter0_nix(void) interrupt 4
{
	temp = SBUF;
	RI=0;	
		
	SBUF = temp;	
	Delay_ms(2);
	while(TI == 1)
		TI = 0;
}

//void inter0_zd(void) interrupt 0//中断服务函数的关键字 0是中断号
//{ 	
//	EX0 = 0;
//	Delay_ms(20);
//	SBUF = 0X25;
//	
//	if(P32 == 1)
//		{			
//			if(P32 == 1)
//			{				
//				while(TI == 1)TI = 0;//TI == 1，发送结束			
//			}
//		}
//	Delay_ms(20);
//	EX0 = 1;
//	SBUF = 0X00;
//}


