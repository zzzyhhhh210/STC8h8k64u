#include <STC89C5xRC.H>

#include "LCD1602.h"

int main(void)
{  
	LCD_Init();
//	LCD_Clear();	
    while(1)
	{
		LCD_ShowString(1,1,"HELLO WORLD");	
	}
	  
}

