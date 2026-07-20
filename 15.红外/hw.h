#ifndef __HW_H_
#define __HW_H_

#include "stc8h.h"
#include <intrins.h>
#include "main.h"

void InitInfrared();
unsigned int GetHighTime();
unsigned int GetLowTime();
void EXINT1_ISR();


#endif

