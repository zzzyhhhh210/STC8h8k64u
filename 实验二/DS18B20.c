#include "DS18B20.h"

extern  uint8_t List[];
void DS18B20_Init(void)
{
	P3M1 = 0X00; P3M0 = 0X00;
	P6M1 = 0XFF; P6M0 = 0XFF;
	P7M1 = 0X00; P7M0 = 0X00;
	P5M1 = 0X00; P5M0 = 0X00;
	Start18B20();
	
}
void Temperature_Scan(void)
{
    unsigned char i;  
	uint16_t temp0 = 0; 
    Start18B20();
    temp0 = Get18B20Temp();//152
    // 解析温度，假设是整数部分
	List[1] = temp0/10%10;//个位
	List[0] = temp0/100; //十位

//温度
	for(i = 0; i < 8; i++){
		P7 = _crol_(0xfe, i);
        if (i == 0) 
            P6 = 0xC6; // 'C' 的字模，如果 Nix 数组里没有，直接用字模值
        else if(i == 2)
            P6 = Nix[List[0]]; // 十位
        else if(i == 1)
           P6 = Nix[List[1]]; // 个位
        else
            P6 = 0xFF; // 其余位置8        
        Delay_us(500);
        P6 = 0xFF; // 消隐
	}		

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
		temp = sum*10+0.5;
		
	}

	return temp;
}










