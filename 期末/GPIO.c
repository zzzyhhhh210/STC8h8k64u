#include "GPIO.H"

void GPIO()
{
            P0M0 = 0x00;       P0M1 = 0x00;                               
            P1M0 = 0x00;       P1M1 = 0x00;                          
            P2M0 = 0x00;       P2M1 = 0x00;  
            P3M0 = 0x00;       P3M1 = 0x00;  
            P4M0 = 0x00;       P4M1 = 0x00;  
            P5M0 = 0x00;       P5M1 = 0x00;  
            P6M0 = 0x00;       P6M1 = 0x00;  
            P7M0 = 0x00;       P7M1 = 0x00;  
            
            P1M1 |= 0x08;
           
}