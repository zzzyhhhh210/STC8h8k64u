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

#include "I2C.h"
#include "EEPROM.h"
#define Seg_mode0   0  //全灭
#define Seg_mode1   1  //L1-L8左循环
#define Seg_mode2   2  //L8-L1右循环
#define Seg_mode3   3  //L1,L8->L2,L7->L3,L6->L4,L5
#define Seg_mode4   4  //L4,L5->L3,L6->L2,L7->L1,L8


#define close_mode  0
#define set_mode  1

#define mode0   0  //S0”启动/停止“按键，按下后启动/停止led
#define SET_TIME   1  //设置按键，按下后进入流转间隔，0.8s间隔亮灭
void Seg_Run();
//#define SHOW_CLOCK       0  // 正常显示时钟
//#define SHOW_TEMP        1  // 正常显示温度
//#define SET_CLOCK   2  // 设置时钟模式
//#define SET_ALARM   3  // 设置闹钟模式

#endif
