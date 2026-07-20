#ifndef __KEY_H_
#define __KEY_H_


#include "Nix.h"
#include "LED.h"
#include "stc8h.h"
#include "Delay.h"

extern unsigned char KeyNum;

void Key_Init(void);
unsigned char Key_scan(void);
void Key_Num_test(void);



#endif
