#ifndef __Timer_H_
#define __Timer_H_

#include "stc8h.h"
#include "main.h"

// 声明外部标志位，将在 main.c 中定义
extern volatile bit g_timer_flag; // 1ms 或 10ms 标志

void Timer0_Init(void);

#endif
