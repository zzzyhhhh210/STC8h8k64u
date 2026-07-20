#ifndef __NIX_H_
#define __NIX_H_

#include "stc8h.h"

typedef     unsigned char   uint_8;
typedef     unsigned int    uint_16;
typedef     unsigned long   uint_32;

void Nix_Init(void);
void Nix_Num(uint_8 digit,Number);
#endif
