#pragma once
#include <inttypes.h>

void addOffset(uint64_t ***offsets, uint64_t *offsetsSize, uint64_t start, uint64_t end);

void setOffset(uint64_t ***offsets, uint64_t *offsetsSize, uint64_t index, uint64_t newOffset);

void ltob16(uint16_t *little);
void btol16(uint16_t *btol);
void ltob32(uint32_t *little);
void btol32(uint32_t *big);