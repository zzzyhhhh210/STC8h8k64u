#include "Nix.h"
#include "Delay.h"

uint_32 Nix[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};   
uint_8 T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  

void Nix_Init(void)
{
    // 设置为推挽输出，增强驱动能力
    P7M0 = 0xFF; 
    P7M1 = 0x00; 
    P6M0 = 0xFF;
    P6M1 = 0x00;

    P7 = 0xFF;    // 关闭位选
    P6 = Nix[0];  // 初始显示0
}

void Nix_Num(uint_8 digit, uint_8 Number)     
{
    P6 = 0x00; // 消影
    
    if(digit < 1 || digit > 8) return;

    // 位选：低电平有效
    P7 = ~T_COM[digit - 1]; 
    
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