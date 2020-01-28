#pragma once
#include "../utility.h"

uint8_t detectPNG(uint8_t *buffer, uint64_t bufferSize);
void getPNGOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
void startPNGBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t ***offsets, uint64_t *offsetsSize);
void endPNGBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t **offsets, uint64_t offsetsSize);
