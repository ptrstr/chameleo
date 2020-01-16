#include "wav.h"
#include <stdlib.h>

unsigned char detectWAV(unsigned char *buffer, unsigned long long int bufferSize) {
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

void getWAVOffsets(unsigned long long int *offsets, unsigned char *buffer, unsigned long long int bufferSize) {
	// Find data section
	offsets[0] = 0;
	for (unsigned long i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0x64 && buffer[i+1] == 0x61 && buffer[i+2] == 0x74 && buffer[i+3] == 0x61) {
			offsets[0] = i + 8;
			break;
		}
	}

	// Spreads across all file
	offsets[1] = bufferSize;

	if (offsets[0] == 0) offsets[1] = 0;

	return;
}
