#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "I2C.h"
#include "main.h"
#include "Delay.h"

#define EEPROM_ADDR_WRITE 0xA0
#define EEPROM_ADDR_READ  0xA1

void eeprom_write(uint8_t add,uint8_t date);
uint8_t eeprom_read(uint8_t add);

#endif
