#ifndef __RTC_H_
#define __RTC_H

#include "stc8h.h"
#include "main.h"
#include "Nix.h"

//#define     INIYEAR                 (*(unsigned char volatile xdata *)0xfe68)
//#define     INIMONTH                (*(unsigned char volatile xdata *)0xfe69)
//#define     INIDAY                  (*(unsigned char volatile xdata *)0xfe6a)
//#define     INIHOUR                 (*(unsigned char volatile xdata *)0xfe6b)
//#define     INIMIN                  (*(unsigned char volatile xdata *)0xfe6c)
//#define     INISEC                  (*(unsigned char volatile xdata *)0xfe6d)
//#define     INISSEC                 (*(unsigned char volatile xdata *)0xfe6e)
//#define     YEAR                    (*(unsigned char volatile xdata *)0xfe70)
//#define     MONTH                   (*(unsigned char volatile xdata *)0xfe71)
//#define     DAY                     (*(unsigned char volatile xdata *)0xfe72)
//#define     HOUR                    (*(unsigned char volatile xdata *)0xfe73)
//#define     MIN                     (*(unsigned char volatile xdata *)0xfe74)
//#define     SEC                     (*(unsigned char volatile xdata *)0xfe75)
//	
void RTC_config();
void Display_Clock(void);
#endif

