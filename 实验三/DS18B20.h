#ifndef __DS18B20_H_
#define __DS18B20_H_

#include "stc8h.h"
#include <intrins.h>
#include "main.h"
#include "Delay.h"

#define DS18B20_CONVERT 0x44
#define DS18B20_READ 0xBE

sbit IO_18B20 = P3^3;

void DS18B20_Init(void);
void Write18B20(uint8_t dat);
bit Get18B20Ack(void);
uint8_t Read18B20(void);
void Start18B20(void);

uint16_t Get18B20Temp(void);
void Temperature_Scan(void);

#endif
