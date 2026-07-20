#include "I2C.h"

sbit SDA = P1^4;
sbit SCL = P1^5;

bit busy;

void I2C_Isr() interrupt 24
{
	if(I2CMSST & 0x40)
	{
		I2CMSST &= ~0x40;// 清除中断标志
		// 检查是否收到 ACK (0: ACK, 1: NAK)
        // 注意：STC8H I2C 主机模式下，I2CMSST 的位可能不同，具体参考手册
        // 通常在中断中处理状态机，这里简化为仅清除busy
		busy = 0;
	}
}

void I2C_Init(void)
{
	P_SW2|=0x90;//ʹ�ܷ���XFR��û�г�ͻ���ùر� 90
	
	P0M0 = 0X00;
	P0M1 = 0X00;
	P1M0 = 0X00;
	P1M1 = 0X00;
	P2M0 = 0X00;
	P2M1 = 0X00;
	P3M0 = 0X00;
	P3M1 = 0X00;
	P4M0 = 0X00;
	P4M1 = 0X00;
	P5M0 = 0X00;
	P5M1 = 0X00;
	P6M0 = 0X00;
	P6M1 = 0X00;
	
	I2CCFG = 0xE0;// 使能 I2C 主机模式，设置时钟
	I2CMSST = 0x00;
	EA = 1;
	
	
}

void I2C_Start()
{
	busy = 1;
	I2CMSCR = 0x81;//����START����
	while(busy);
}

void I2C_SendData(char dat)
{
	I2CTXD = dat;//д���ݵ����ݻ�����
	busy = 1;
	I2CMSCR = 0x82;//����SEND����
	while(busy);
}

void I2C_RecvACK()
{
	busy = 1;
	I2CMSCR = 0x83;//���Ͷ�ACK����
	while(busy);
}

uint8_t I2C_RecvData()
{
	busy = 1;
	I2CMSCR = 0x84;//����RECV����
	while(busy);
	return I2CRXD;
}

void I2C_SendACK()
{
	I2CMSST = 0x00;//����ACK�ź�
	busy = 1;
	I2CMSCR = 0x85;//����ACK����
	while(busy);
}

void I2C_SendNAK()
{
	I2CMSST = 0x01;//����NAK�ź�
	busy = 1;
	I2CMSCR = 0x85;//����ACK����
	while(busy);
}

void I2C_Stop()
{
	busy = 1;
	I2CMSCR = 0x86;//����STOP����
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










