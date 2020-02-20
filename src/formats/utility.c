#include "utility.h"
#include <stdlib.h>
#include "../uByte/uByte.h"

void addOffset(uint64_t ***offsets, uint64_t *offsetsSize, uint64_t start, uint64_t end) {
	(*offsetsSize)++;
	if (!(*offsets)) {
		*offsets = (uint64_t**)malloc((*offsetsSize) * sizeof(uint64_t*));
		if (!(*offsets))
			return;
	} else {
		*offsets = (uint64_t**)realloc(*offsets, (*offsetsSize) * sizeof(uint64_t*));
		if (!(*offsets))
			return;
	}

	(*offsets)[(*offsetsSize) - 1] = (uint64_t*)calloc(2, sizeof(uint64_t));
	if (!(*offsets)[(*offsetsSize) - 1])
		return;
	(*offsets)[(*offsetsSize) - 1][0] = start;
	(*offsets)[(*offsetsSize) - 1][1] = end;
	return;
}

void setOffset(uint64_t ***offsets, uint64_t *offsetsSize, uint64_t index, uint64_t newOffset) {
	if (*offsets && (*offsets)[(*offsetsSize) - 1] && (index == 0 || index == 1))
		(*offsets)[(*offsetsSize) - 1][index] = newOffset;
}

void ltob16(uint16_t *little) {
	uByte endianness;
	endianness.byte = 1;
	if (endianness.bits.bit7 != 1) {
		uint32_t big = *little;
		*little = (big>>8) | (big<<8);
	}
}

void btol16(uint16_t *big) {
	uByte endianness;
	endianness.byte = 1;
	if (endianness.bits.bit0 != 1) {
		uint32_t little = *big;
		*big = (little>>8) | (little<<8);
	}
}

void ltob32(uint32_t *little) {
	uByte endianness;
	endianness.byte = 1;
	if (endianness.bits.bit7 != 1) {
		uint32_t big = *little;
		*little = ((big & 0x000000ff) << 24u) | ((big & 0x0000ff00) << 8u) | ((big & 0x00ff0000) >> 8u) | ((big & 0xff000000) >> 24u);
	}
}

void btol32(uint32_t *big) {
	uByte endianness;
	endianness.byte = 1;
	if (endianness.bits.bit0 != 1) {
		uint32_t little = *big;
		*big = ((little & 0x000000ff) << 24u) | ((little & 0x0000ff00) << 8u) | ((little & 0x00ff0000) >> 8u) | ((little & 0xff000000) >> 24u);
	}
}
