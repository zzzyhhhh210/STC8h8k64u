#ifndef __LCD1602_H_
#define __LCD1602_H_

#include "stc8h.h"
#include "Delay.h"
#include <intrins.h>

/*****不同板子需要换下列定义******/
#define LCD_Data P6  // 数据端口
sbit LCD_RS = P4^5;  // 数据(1)/ 指令(0)
sbit LCD_RW = P4^2;  // 读(1) / 写(0)
sbit LCD_EN = P4^0;	 // 使能(负跳变起作用)
sbit LCD_RST = P3^4;//复位

void LCD_Init(void);//初始化
void LCD_Clear(void);//清屏
void LCD_SetCursor(unsigned char row, unsigned char col);//光标定位:row1-2行,col1-16列
void LCD_ShowString(unsigned char row, unsigned char col, unsigned char *str);// 显示字符串

void LCD_Write_Cmd(unsigned char Cmd);//写指令
void LCD_Write_Data(unsigned char Data);//写数据
void LCD_Check_Busy();//判忙

#endif

