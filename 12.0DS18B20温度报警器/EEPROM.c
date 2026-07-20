#include "EEPROM.h"
void eeprom_write(uint8_t add,uint8_t date)
{
	unsigned int i;
	
	I2C_Start();
	I2C_SendData(0xa0);
	I2C_RecvACK();
	I2C_SendData(add);
	I2C_RecvACK();
	I2C_SendData(date);
	I2C_RecvACK();
	I2C_Stop();

	
    for(i=0; i<5000; i++)
	{_nop_();}
}

uint8_t eeprom_read(uint8_t add)
{
	uint8_t date;
	I2C_Start();
	I2C_SendData(0xa0);
	I2C_RecvACK();
	I2C_SendData(add);
	I2C_RecvACK();
	
	I2C_Start();
	I2C_SendData(0xa1);
	I2C_RecvACK();
	date=I2C_RecvData();
	I2C_SendNAK();
	I2C_Stop();
	I2C_Delay();
	
	return date;
}