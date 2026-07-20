#include <stc8h.h>
#include "LED.h"
#include "Key.h"
#include "Nix.h"

unsigned char KeyNum;
unsigned char pos_buf[8] = {0};//八位显示缓存
unsigned char pos = 8;//位选状态

int main(void)
{
	unsigned char i;
	Key_Init();
	Nix_Init();//初始化
	while(1)
		{				
		KeyNum=Key_scan();//扫描
		if(KeyNum != 16)
			{
		
			  for(i=8; i>0; i--)
				{
					pos_buf[i] = pos_buf[i-1];//低->高
				}
				pos_buf[0] = KeyNum;
				//Delay_ms(100);//3.

			}
			Nix_Display(pos_buf);
			
		}
}
