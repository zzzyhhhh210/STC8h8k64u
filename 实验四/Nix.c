#include "Nix.h"
#include "Delay.h"

uint_32 Nix[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x48};   
uint_8 T_COM[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};  
uint8_t code LedChar[] = {
    0xC0, //  0: ABCDEF
    0xF9, //  1: BC
    0xA4, //  2: ABDEG
    0xB0, //  3: ABCDG
    0x99, //  4: BCFG
    0x92, //  5: ACD-FG
    0x82, //  6: ACDEFG
    0xF8, //  7: ABC
    0x80, //  8: ABCDEFG
    0x90, //  9: ABCDFG
    0xBF, // 10: '-' (G)
    0xFF, // 11: 熄灭
    0x8E, // 12: 'F' (AEFG)
    0xC1, // 13: 'U' (BCDEF)
    0x88, // 14: 'A' (ABCEFG)
    0x8C, // 15: 'P' (ABEFG)
    0xC6, // 16: 'C' (ABDEG)
};
uint8_t LedBuff[8] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
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
// ================= 数码管动态扫描 =================
void Seg_Scan(void)
{
    static unsigned char i = 0;  //¶¯̬ɨè˷ҽ
    
    P40 = 1;
    P6 = 0xFF;                  //¹رՋùӐ¶Α¡λ£¬ϔʾϻҾ
    P7 = _crol_(0xfe,i);       //λѡ˷ҽֵ¸³ֵµ½P7¿ڵͳλ
    P6 = LedBuff[7-i];            //»º³凸֐˷ҽλփµĊý¾݋͵½P0¿ڍ
    if (i < sizeof(LedBuff)-1)  //˷ҽµݔöѭ»·£¬±逺ջ¸ö»º³凸
        i++;
    else
        i = 0;
}