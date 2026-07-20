#ifndef __MAIN_H_
#define __MAIN_H_

typedef     unsigned char   uint8_t;
typedef     unsigned int    uint16_t;
typedef     unsigned long   uint32_t;


#include "Delay.h"
#include "Timer.h"
#include "Nix.h"
#include "stc8h.h"
#include "Key.h"
#include "LED.h"
#include "GPIO.H"
#include "DS18B20.h"
#include "ADC.h"

// 显示界面模式
#define DISP_C      0   // 温度显示界面
#define DISP_U      2   // 电压输出界面
#define DISP_P      1   // 参数设置界面
// 段码表索引(F/U/A/P/C)
#define CHAR_F          12
#define CHAR_U          13
#define CHAR_A          14
#define CHAR_P          15
#define CHAR_C          16

#define close_mode  0
#define set_mode    1

// 函数声明
void Seg_Scan(void);
void Seg_Run(void);
void Led_Run(void);
void Key_Run(void);
void Adc_run(void);
void DAC_Init(void);
void DAC_Output(uint16_t value);
void Freq_Output_Init(void);
void Freq_Output_Set(uint32_t f);

#endif
