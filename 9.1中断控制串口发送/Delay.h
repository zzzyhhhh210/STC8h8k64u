#ifndef __DELAY_H_
#define __DELAY_H_

#include "stc8h.h"
#include <intrins.h>

#define     MAIN_Fosc       24000000L 
typedef     unsigned char   u8;
typedef     unsigned int    u16;
typedef     unsigned long   u32;

void Delay_ms(u8 ms);
void Delay_us(unsigned int us);
#endif
