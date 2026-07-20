#ifndef __I2C_H_
#define __I2C_H_

#include "stc8h.h"
#include <intrins.h>
#include "main.h"

void I2C_Init(void);
void I2C_Start();
void I2C_Stop();

void I2C_SendData(uint8_t dat);
uint8_t I2C_RecvData();
void I2C_SendACK();
void I2C_RecvACK();
void I2C_SendNAK();
void I2C_Delay();

#endif
