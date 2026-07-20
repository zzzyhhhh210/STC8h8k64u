#include "RTC.h"

extern uint8_t code time[];

void RTC_config()
{
	X32KCR = 0XC0;
	while(!(X32KCR & 0X01));
	RTCCFG &= ~0X02;
	
	INIYEAR = time[0];
	INIMONTH = time[1];
	INIDAY = time[2];
	INIHOUR = time[3];
	INIMIN = time[4];
	INISEC = time[5];
	INISSEC = 0;
	
	RTCCFG |= 0X01;
	RTCCR = 0X01;
	while(RTCCFG & 0x01);

}