#include "LCD1602.h"

/****************** 初始化 *********************/
void LCD_Init(void)
{
	LCD_Write_Cmd(0x01); // 清屏00/01  LCD_Write_Cmd(0x00);
	LCD_Write_Cmd(0x06); // AC加1，画面不动
	LCD_Write_Cmd(0x0c); // 显示开，光标关，闪烁关
	LCD_Write_Cmd(0x38); // 8位数据接口，两行显示，5*7点阵
	LCD_Write_Cmd(0x80); // 从第一行第一列开始显示
}
/****************** 忙判断 *********************
LCD_Data 与 0x80 进行或运算,判断 bit7 位标志
@  若 LCD_Data 的 bit7 位是 0,则 LCD 不忙
***********************************************/
void LCD_Check_Busy()
{
	unsigned char temp;
	LCD_Data = 0xff;  //1111 1111
	LCD_RS = 0;  // 0 指令
	LCD_RW = 1;	 // 1 读
	do{
		LCD_EN = 1;		  // 拉高
		temp = LCD_Data;  // 将  LCD 状态保存在 temp 中,判忙
		LCD_EN = 0;		  // 负跳变使能
	}while(temp & 0x80);  // 结果为 1,LCD 忙,继续循环
}
/****************** 写指令 *********************/
void LCD_Write_Cmd(unsigned char Cmd)
{
	LCD_Check_Busy();
	LCD_RS = 0;  // 0 指令
	LCD_RW = 0;  // 0 写操作
	
	LCD_EN = 0;//开始为 0
	LCD_Data = Cmd;  // 送指令
	Delay_ms(2);
	
	LCD_EN = 1;  // 拉高
	Delay_ms(5);//延时5ms,写入指令有效

	LCD_EN = 0;//负跳变，指令写入LCD
}

/****************** 写数据 *********************/
void LCD_Write_Data(unsigned char Data)
{
	LCD_Check_Busy();  // 数据

	LCD_RS = 1; 
	LCD_RW = 0;  // 0 写操作

	LCD_EN = 0;
	LCD_Data = Data; 
	Delay_ms(2);
	
	LCD_EN = 1;  
	Delay_ms(7);//延时7ms,写入数据有效

	LCD_EN = 0;//负跳变，数据写入LCD
}
// 清屏
void LCD_Clear(void)
{
    LCD_Write_Cmd(0x01);
    Delay_ms(1);
}
//光标定位:row1-2行,col1-16列
void LCD_SetCursor(unsigned char row, unsigned char col)
{
    unsigned char addr;
    if(row == 1)
        addr = 0x00 + col - 1; // 第一行起始地址0x00
    else
        addr = 0x40 + col - 1; // 第二行起始地址0x40
    LCD_Write_Cmd(addr | 0x80); // 地址+显示使能
}
// 显示字符串
void LCD_ShowString(unsigned char row, unsigned char col, unsigned char *str)
{
    LCD_SetCursor(row, col);
    while(*str != '\0')
    {
        LCD_Write_Data(*str);
        str++;
    }
}


