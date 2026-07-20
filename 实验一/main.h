#ifndef __MAIN_H_
#define __MAIN_H_

typedef     unsigned char   uint8_t;
typedef     unsigned int    uint16_t;
typedef     unsigned long   uint32_t;


#include "Delay.h"
#include "Timer.h"
#include "Nix.h"
#include "stc8h.h"
#include "RTC.h"

#include "I2C.h"
#include "EEPROM.h"
#include "DS18B20.h"
#include "Key.h"
#include "LED.h"


// 定义按键值 (请根据你的 Key_scan 实际返回值调整)
#define KEY_TEMP      3   // 假设 S4 对应 KeyNum 3，用于显示温度
#define KEY_DEC       4   // 减
#define KEY_INC       5   // 加
#define KEY_ALARM     6   // 闹钟设置
#define KEY_CLOCK_SET 7   // 时钟设置

#define SHOW_CLOCK       0  // 正常显示时钟
#define SHOW_TEMP        1  // 正常显示温度
#define SET_CLOCK   2  // 设置时钟模式
#define SET_ALARM   3  // 设置闹钟模式

sbit LED_L1 = P2^7; 
sbit BUZZ  = P5^4;     //蜂鸣器0-响；1-停

#endif
