#ifndef __DELAY_H_
#define __DELAY_H_

#include "stc8h.h"
#include <intrins.h>
#include "main.h"

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;

#define     MAIN_Fosc       24000000L

void Delay_ms(uint8_t ms);
void Delay_us(uint16_t us);

#endif

