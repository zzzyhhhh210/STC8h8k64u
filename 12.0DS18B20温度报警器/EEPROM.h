#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "main.h"
#include "I2C.h"
#include "Delay.h"
void eeprom_write(uint8_t add,uint8_t date );

uint8_t eeprom_read(uint8_t add);

#endif
