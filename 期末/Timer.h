#ifndef __Timer_H_
#define __Timer_H_

#include "stc8h.h"
#include "main.h"

sbit PWM_OUT = P2^3;

void Timer0_Init(void);
void Timer1_Init(void);
void Timer2_Init(void);
void Timer2_SetFreq(unsigned long freq);
#endif
