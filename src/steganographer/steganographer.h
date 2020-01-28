#pragma once
#include <inttypes.h>

#include "../uByte/uByte.h"

uint8_t steganograph(uint8_t *targetBuffer, uint64_t targetSize, uint8_t *secretBuffer, uint64_t secretSize, uint8_t *outputBuffer, uint64_t **offsets, uint64_t offsetsSize, uByte *activeBits, uint64_t activeBitsSize);
uint8_t desteganograph(uint8_t *targetBuffer, uint64_t targetSize, uint8_t *secretBuffer, uint64_t secretSize, uint64_t **offsets, uint64_t offsetsSize, uByte *activeBits, uint64_t activeBitsSize);
