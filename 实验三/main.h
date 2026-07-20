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

#include "I2C.h"
#include "EEPROM.h"
#include "ADC.h"

// 显示界面模式
#define DISP_V   0   // 电压显示界面
#define DISP_F      1   // 频率显示界面
// 段码表索引(F/U)
#define CHAR_F          12
#define CHAR_U          13

#define close_mode  0
#define set_mode    1

// 函数声明
void Seg_Scan(void);
void Seg_Run(void);
void Led_Run(void);
void Key_Run(void);
void Adc_run(void);
void Freq_Output_Init(void);
void Freq_Output_Set(uint32_t f);

#endif
