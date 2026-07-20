#include "EEPROM.h"

/**
 * @brief 向 EEPROM 写入一个字节
 * @param add: 地址
 * @param date: 数据
 */
void eeprom_write(uint8_t add,uint8_t date)
{
	I2C_Start();
	I2C_SendData(EEPROM_ADDR_WRITE);// 设备地址 + 写
	I2C_RecvACK();

	I2C_SendData(add);// 内存地址
	I2C_RecvACK();

	I2C_SendData(date);// 【关键修复】发送要写入的数据
	I2C_RecvACK();

	I2C_Stop();
    // EEPROM 写入周期通常为 5ms - 10ms
    // 必须延时，否则下一次读写会失败
	I2C_Delay();
}

/**
 * @brief 从 EEPROM 读取一个字节
 * @param add: 地址
 * @return: 数据
 */
uint8_t eeprom_read(uint8_t add)
{
	uint8_t date;

	  // 1. 伪写：设置读取地址指针
	I2C_Start();
	I2C_SendData(EEPROM_ADDR_WRITE);// 设备地址 + 写
	I2C_RecvACK();

	I2C_SendData(add); // 内存地址
	I2C_RecvACK();

	 // 2. 重复启动：开始读取
	I2C_Start();
	I2C_SendData(EEPROM_ADDR_READ);// 设备地址 + 读
	I2C_RecvACK();

	 // 3. 接收数据
	date=I2C_RecvData();

	 // 4. 发送 NAK 并停止
	I2C_SendNAK(); // 最后一个字节发送 NAK
	I2C_Stop();

	return date;
}