#pragma once

#include "../uByte/uByte.h"

unsigned char steganograph(unsigned char *targetBuffer, unsigned long targetSize, unsigned char *secretBuffer, unsigned long secretSize, unsigned char *outputBuffer, unsigned long *offsets, uByte activeBits);
unsigned char desteganograph(unsigned char *targetBuffer, unsigned long targetSize, unsigned char *secretBuffer, unsigned long secretSize, unsigned long *offsets, uByte activeBits);
