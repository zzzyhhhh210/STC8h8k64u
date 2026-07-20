#include "DS18B20.h"
//	void Delay_ms(uint8_t ms);
//void Delay_us(uint16_t us);

void DS18B20_Init(void)
{
	P3M1 = 0X00; P3M0 = 0X00;
	P6M1 = 0XFF; P6M0 = 0XFF;
	P7M1 = 0X00; P7M0 = 0X00;
	P5M1 = 0X00; P5M0 = 0X00;
	Start18B20();
	
}
bit Get18B20Ack(void)
{
	bit ack;    
	IO_18B20 = 0;//单片机将总线电平拉低
	Delay_us(600);//产生延时500us
	IO_18B20 = 1;//总线电平拉高
	Delay_us(60);//延时60us
	ack = IO_18B20;//读取存在脉冲
	while(!IO_18B20);//等待存在脉冲
	Delay_us(40);
	return ack;
}

void Write18B20(uint8_t dat)
{
	unsigned char mask;
	EA = 0;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B20 = 0;
		_nop_();
		_nop_();
		if( (mask & dat) == 0)
			IO_18B20 = 0;
		else
			IO_18B20 = 1;
			Delay_us(60);
			IO_18B20 = 1;
			Delay_us(10);
	}
	EA = 1;
}

uint8_t Read18B20(void)
{ 
	uint8_t dat = 0;
	uint8_t mask = 0;
	EA = 0;
	for(mask = 0x01; mask != 0; mask <<= 1)
	{
		IO_18B20 = 0;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		IO_18B20 = 1;
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		Delay_us(10);
		if(!IO_18B20)
			dat &= ~mask;
		else
			dat |= mask;
		Delay_us(60);
	}
	EA = 1;
	return dat;
}

void Start18B20(void)
{
	bit ack;
	ack = Get18B20Ack();
	if(ack == 0)
	{
		Write18B20(0xcc);
		Write18B20(0x44);

	}
	Delay_us(500);
}

uint16_t Get18B20Temp(void)
{
	uint8_t MSB, LSB;
	int temp;
	float sum;//浮点数
	bit ack;
	ack = Get18B20Ack();
	
	if(ack == 0)
	{
		Write18B20(0xcc);
		Write18B20(0xBE);
		LSB = Read18B20();
		MSB = Read18B20();
		 
		temp = ((int)MSB << 8) + LSB;
		sum = temp*0.0625;
		temp = sum*100+0.5;
		
	}

	return temp;
}










