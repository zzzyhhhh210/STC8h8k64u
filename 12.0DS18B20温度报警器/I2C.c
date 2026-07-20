#include "I2C.h"

sbit SDA = P1^4;
sbit SCL = P1^5;

bit busy;

void I2C_Init(void)
{
	P_SW2|=0x90;//使能访问XFR，没有冲突不用关闭 90
	
	P6M0 = 0X00;
	P6M1 = 0X00;
	P7M0 = 0XFF;
	P7M1 = 0XFF;
	
	I2CCFG = 0xe0;//I2C主机模式；
	I2CMSST = 0x00;
	EA = 1;
	
	
}



void I2C_Isr() interrupt 24
{
	if(I2CMSST & 0x40)
	{
		I2CMSST &= ~0x40;//清除中断标志
		busy = 0;
	}
}

void I2C_Start()
{
	busy = 1;
	I2CMSCR = 0x81;//发送START命令
	while(busy);
}

void I2C_SendData(char dat)
{
	I2CTXD = dat;//写数据到数据缓冲区
	busy = 1;
	I2CMSCR = 0x82;//发送SEND命令
	while(busy);
}

void I2C_RecvACK()
{
	busy = 1;
	I2CMSCR = 0x83;//发送读ACK命令
	while(busy);
}

uint8_t I2C_RecvData()
{
	busy = 1;
	I2CMSCR = 0x84;//发送RECV命令
	while(busy);
	return I2CRXD;
}

void I2C_SendACK()
{
	I2CMSST = 0x00;//设置ACK信号
	busy = 1;
	I2CMSCR = 0x85;//发送ACK命令
	while(busy);
}

void I2C_SendNAK()
{
	I2CMSST = 0x01;//设置NAK信号
	busy = 1;
	I2CMSCR = 0x85;//发送ACK命令
	while(busy);
}

void I2C_Stop()
{
	busy = 1;
	I2CMSCR = 0x86;//发送STOP命令
	while(busy);
}

void I2C_Delay()
{
	int i;
	
	for(i=0;i<3000;i++)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

void I2C_tect(void)
{
	I2C_Init();
	
	P40=0;
	P6=0X01;//亮灯
	
	I2C_Start();
	I2C_SendData(0xa0);//发送设备地+写命令
	I2C_RecvACK();
	I2C_SendData(0x01);//发送存储地址字节10100000
	I2C_RecvACK();
	I2C_SendData(0xfe);//发送数据
	I2C_RecvACK();
	I2C_Stop();
	
	I2C_Delay();
	
	I2C_Start();
	I2C_SendData(0xa0);//发送设备地+写命令
	I2C_RecvACK();
	I2C_SendData(0x01);//发送想要读的存储地址字节
	I2C_RecvACK();
	I2C_SendData(0xa1);//发送设备地+读命令1010 0001
	I2C_RecvACK();
	
//	P6=I2C_RecvData();//数据
	
	I2C_SendNAK();
	I2C_Stop();
	
	while(1);
}









