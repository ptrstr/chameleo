#pragma once

#include "../uByte/uByte.h"

unsigned char steganograph(unsigned char *targetBuffer, unsigned long long int targetSize, unsigned char *secretBuffer, unsigned long long int secretSize, unsigned char *outputBuffer, unsigned long long int *offsets, uByte activeBits);
unsigned char desteganograph(unsigned char *targetBuffer, unsigned long long int targetSize, unsigned char *secretBuffer, unsigned long long int secretSize, unsigned long long int *offsets, uByte activeBits);
