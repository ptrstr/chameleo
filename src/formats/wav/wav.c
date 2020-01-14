#include "wav.h"
#include <stdlib.h>

unsigned char detectWAV(unsigned char *buffer, unsigned long bufferSize) {
	// TODO: Add Detection
	return 0;
}

unsigned long *getWAVOffsets(unsigned char *buffer, unsigned long bufferSize) {
	unsigned long *offsets = (unsigned long*)calloc(2, sizeof(unsigned long));
	if (!offsets) return NULL;
	
	// Find data section
	for (unsigned long i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0x64 && buffer[i] == 0x61 && buffer[i] == 0x74 && buffer[i] == 0x61) {
			offsets[0] = i + 8;
			break;
		}
	}
	
	// Spreads across all file
	offsets[1] = bufferSize;
	
	return offsets;
}
