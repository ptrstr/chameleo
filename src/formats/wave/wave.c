#include "wave.h"
#include <stdlib.h>

unsigned char detectWAVE(unsigned char *buffer, unsigned long long int bufferSize) {
	if (buffer[0] != 0x52 ||
			buffer[1] != 0x49 ||
			buffer[2] != 0x46 ||
			buffer[3] != 0x46 ||
			buffer[8] != 0x57 ||
			buffer[9] != 0x41 ||
			buffer[10] != 0x56 ||
			buffer[11] != 0x45) return 0;

	return 1;
}

void getWAVEOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	(*offsetsSize)++;
	*offsets = (unsigned long long int**)calloc(sizeof(unsigned long long int*), *offsetsSize);
	if (!*offsets)
		return;
	*offsets[0] = (unsigned long long int*)calloc(sizeof(unsigned long long int), 2);
	if (!*offsets[0])
		return;

	// Find data section
	*offsets[0][0] = 0;
	for (unsigned long i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0x64 && buffer[i+1] == 0x61 && buffer[i+2] == 0x74 && buffer[i+3] == 0x61) {
			*offsets[0][0] = i + 8;
			break;
		}
	}

	// Spreads across all file
	*offsets[0][1] = bufferSize;

	if (*offsets[0][0] == 0) *offsets[0][1] = 0;

	return;
}
