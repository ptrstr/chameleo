#pragma once
#include "../utility.h"

uint8_t detectWAVE(uint8_t *buffer, uint64_t bufferSize);
void getWAVEOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
