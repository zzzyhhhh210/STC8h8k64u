#include "main.h"
#include "Key.h"

uint16_t temp0 = 0;
uint16_t List[] = {0,0,0,0};
sbit BUZZ  = P5^4;
unsigned char KeyNum;
void main(void)
{  
	uint8_t i = 0;
	DS18B20_Init();
	temp0 = Get18B20Temp();
	Delay_ms(40000);
	Nix_Init();
	Key_Init();
	
      while(1)
      {
		  KeyNum=Key_scan();
		 Start18B20();
		 temp0 = Get18B20Temp();//152
		  
//		  List[0] = temp0/1000; //十位
//		  List[1] =	(temp0/100%10)&0x7f;//个位
//		  List[2] = temp0/100;   
//		  List[3] = temp0%100;
		  
		List[0] = temp0 / 1000;          // 十位 (2553 / 1000 = 2)
        List[1] = (temp0 / 100) % 10;    // 个位 (2553 / 100 % 10 = 5)
        List[2] = (temp0 / 10) % 10;     // 十分位 (2553 / 10 % 10 = 5)
        List[3] = temp0 % 10;            // 百分位 (2553 % 10 = 3)
		Nix_Num(5,List[0]);
		Nix_Num(6,List[1]);
		Nix_Num(7,List[2]);
		Nix_Num(8,List[3]);

		  
		   if(temp0>2500){
				BUZZ = 0;
			   if(KeyNum == 0){BUZZ = 1;Delay_ms(10000);
		   }
		   }
		   else
				BUZZ = 1;	   

      }	
	  
}

