// bits.h ... interface to functions on bit-strings
// part of signature indexed files
// See bits.c for details of functions
// Written by John Shepherd, March 2019

#ifndef BITS_H
#define BITS_H 1

typedef struct _BitsRep *Bits;

#include "defs.h"
#include "page.h"

Bits newBits(int);
void freeBits(Bits);
Bool bitIsSet(Bits, int);
Bool isSubset(Bits, Bits);
void setBit(Bits, int);
void setAllBits(Bits);
void unsetBit(Bits, int);
void unsetAllBits(Bits);
void andBits(Bits, Bits);
void orBits(Bits, Bits);
void shiftBits(Bits, int);
void getBits(Page, Offset, Bits);
void putBits(Page, Offset, Bits);
void showBits(Bits);

#endif
