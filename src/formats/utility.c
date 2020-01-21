#include "utility.h"
#include <stdlib.h>
#include "../uByte/uByte.h"

void addOffset(unsigned long long int ***offsets, unsigned long long int *offsetsSize, unsigned long long int start, unsigned long long int end) {
	(*offsetsSize)++;
	if (!(*offsets)) {
		*offsets = (unsigned long long int**)malloc((*offsetsSize) * sizeof(unsigned long long int*));
		if (!(*offsets))
			return;
	} else {
		*offsets = (unsigned long long int**)realloc(*offsets, (*offsetsSize) * sizeof(unsigned long long int*));
		if (!(*offsets))
			return;
	}
	
	(*offsets)[(*offsetsSize) - 1] = (unsigned long long int*)calloc(sizeof(unsigned long long int), 2);
	if (!(*offsets)[(*offsetsSize) - 1])
		return;
	
	(*offsets)[(*offsetsSize) - 1][0] = start;
	(*offsets)[(*offsetsSize) - 1][1] = end;
	return;
}

void setOffset(unsigned long long int ***offsets, unsigned long long int *offsetsSize, unsigned long long int index, unsigned long long int newOffset) {
	if (*offsets && (*offsets)[(*offsetsSize) - 1] && (index == 0 || index == 1))
		(*offsets)[(*offsetsSize) - 1][index] = newOffset;
}

void ltob(unsigned int *little) {
	{
		uByte endianness;
		endianness.byte = 1;
		if (endianness.bits.bit7 != 1) {
			unsigned int big = *little;
			*little = ((big & 0x000000ff) << 24u) | ((big & 0x0000ff00) << 8u) | ((big & 0x00ff0000) >> 8u) | ((big & 0xff000000) >> 24u);
		}
	}
}