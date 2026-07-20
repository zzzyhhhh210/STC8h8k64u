#include <stc8h.h>
#include "Nix.h"
#include "delay.h"
#include "Timer.h"
#include "Key.h"

unsigned char cnt;
unsigned char key;
int sec = 0;
/*
99跑马表（0-99）
1.数码管
2.定时器中断
*/
 int main(void)
{  
	Key_Init();
	Timer1_Init();
      while(1)
      {
		key = Key_scan();
		P6=0XFF;
		P7=0xfe;   
		Delay_us(10);
		P6 = Nix[sec%10];
		Delay_ms(5);//保证数码管显示稳定，避免闪烁
		  
		P6=0XFF;
		P7=0xfd;    
		Delay_us(10);
		P6 = Nix[sec/10];
		Delay_ms(5);//保证数码管显示稳定，避免闪烁	
		  
		if(key == 0)TR0 = 0;
		if(key == 1)TR0 = 1;//EA,ET0，IE		
		if(key == 2){sec++;if(sec>99)sec=0;}
		if(key == 3){sec--;if(sec<0)sec=99;}
		if(key == 4)sec = 0;
      }
	  
}


void Timer1(void) interrupt 3//中断服务函数的关键字 1是中断号，定时器1中断  T1  MODE1
{ 
		TL1 = 0x00;				
		TH1 = 0x28;
		cnt ++;
		if(cnt == 200)//定时器0单次计数时间是5ms
		{
			cnt = 0;					
			sec ++;
			if(sec>99)sec=0;
		}

}

//void Timer0(void) interrupt 1//中断服务函数的关键字 1是中断号，定时器0中断  T1  MODE1
//{ 
//		cnt ++;
//		if(cnt == 20)//定时器0单次计数时间是50ms
//		{
//			cnt = 0;					
//			sec ++;
//			if(sec>99)sec=0;
//		}

//}
