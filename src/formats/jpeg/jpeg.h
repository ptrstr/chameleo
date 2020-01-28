#pragma once
#include "../utility.h"

uint8_t detectJPEG(uint8_t *buffer, uint64_t bufferSize);
void getJPEGOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
