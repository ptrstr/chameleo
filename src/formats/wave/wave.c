#include "wave.h"
#include <stdlib.h>

uint8_t detectWAVE(uint8_t *buffer, uint64_t bufferSize) {
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

void getWAVEOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize) {
	// Find data section
	uint64_t start = 0;
	for (uint64_t i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0x64 && buffer[i+1] == 0x61 && buffer[i+2] == 0x74 && buffer[i+3] == 0x61) {
			start = i + 8;
			break;
		}
	}

	// Spreads across all file
	addOffset(offsets, offsetsSize, start, bufferSize);

	if (*offsets[0][0] == 0) setOffset(offsets, offsetsSize, 1, 0);

	return;
}
