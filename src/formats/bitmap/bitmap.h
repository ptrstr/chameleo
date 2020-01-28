#pragma once
#include "../utility.h"

uint8_t detectBitmap(uint8_t *buffer, uint64_t bufferSize);
void getBitmapOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
