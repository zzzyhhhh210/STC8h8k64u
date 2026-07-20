#include "Led.h"
#include "Delay.h"
#include "intrins.h"

void LED_Init(void)
{
    P_SW2 |= 0x80; 
	
    P6M1 = 0x00;   
    P6M0 = 0xFF;   // P6 推挽输出

    P4M1 &= ~0x01; // P4.0 推挽输出(控制LED电源)
    P4M0 |= 0x01;

    P40 = 0;       // Led Power On
}
