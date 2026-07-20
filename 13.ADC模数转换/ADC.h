#ifndef __ADC_H_
#define __ADC_H_

#include "stc8h.h"
#include "Timer.h"
#include "Nix.h"
#include "Delay.h"

#define ADC_OFFSET 32


void ADC_Iint(void);
unsigned int Get_ADC12bitResult(unsigned char channel);
void CalculateAdcKey(unsigned char adc);



#endif
