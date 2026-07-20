#include "Nix.h"
#include "Delay.h"

uint_32 Nix[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x48};   
uint_8 T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  

void Nix_Init(void)
{
    // 设置为推挽输出，增强驱动能力
    P7M0 = 0xFF; 
    P7M1 = 0x00; 
    P6M0 = 0xFF;
    P6M1 = 0x00;

    P7 = 0xFF;    // 关闭位选
//    P6 = Nix[0];  // 初始显示0
}

void Nix_Num(uint_8 digit, uint_8 Number)     
{
    P6 = 0x00; // 消影
    
    if(digit < 1 || digit > 8) return;

    // 位选：低电平有效
    P7 = T_COM[digit]; 
    
    // 段选
    if(Number <= 9)
        P6 = Nix[Number];
    else
        P6 = 0xFF; 
        
    Delay_ms(1);  
    P6 = 0x00; // 消影
    P7 = 0xFF; // 关闭位选
}   

void Nix_Display(unsigned char *buf)
{
    static unsigned char pos = 0;
    
    P7 = 0xFF; // 消影
    P6 = Nix[buf[pos]]; 
    P7 = ~(0x01 << pos); 
    pos = (pos + 1) % 8;
    Delay_ms(1); 
}
//// ================= 数码管动态扫描 =================
//void SegScan(void)
//{
//	static uint8_t pos = 0;
//	P7 = 0xFF;       // 消影: 关闭所有位选
//	if(LedBuff[pos] != 0xff)
//		P6 = Nix[LedBuff[pos]];  // 输出段码(通过Nix表索引)
//	else
//		P6 = 0xFF;      // 熄灭
//	P7 = ~(0x01 << pos);  // 位选
//	Delay_ms(1);       // 延时
//	pos = (pos + 1) % 8;  // 切换到下一位
//}
//void SegScan()
//{
//	unsigned char i;
//	for(i = 0; i < 8; i++)
//		{
//			P7 = _crol_(0xfe, i);
//			
//			if(i == 2 || i == 5)
//				P6 = T_COM[i]; // 显示分隔符 '-'
//			else 
//				P6 = Nix[T_COM[7 - i]]; // 显示数字
//			
//			Delay_ms(1);
//			P6 = 0xFF; // 消隐
//		}
//}