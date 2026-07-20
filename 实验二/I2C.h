#ifndef __I2C_H_
#define __I2C_H_

#include "stc8h.h"
#include <intrins.h>
#include "main.h"
#include "Delay.h"
void I2C_Init(void);
void I2C_Start();
void I2C_SendData(char dat);
void I2C_RecvACK();
uint8_t I2C_RecvData();
void I2C_SendACK();
void I2C_SendNAK();
void I2C_Stop();
void I2C_Delay();
void I2C_tect(void);

#endif
