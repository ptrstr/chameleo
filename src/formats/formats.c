#include "formats.h"
#include <stdlib.h>
#include <string.h>
#include "wav/wav.h"

FORMAT getFormat(unsigned char *buffer, unsigned long bufferSize) {
	if (detectWAV(buffer, bufferSize)) return WAV;
	return NULLFORMAT;
}

unsigned long *getFormatOffsets(unsigned char *buffer, unsigned long bufferSize, FORMAT format) {
	unsigned long *offsets = (unsigned long*)calloc(2, sizeof(unsigned long));
	if (!offsets) return NULL;
	
	unsigned long *tmpOffsets;
	switch (format) {
		case WAV:
			tmpOffsets = getWAVOffsets(buffer, bufferSize);
	}
	
	if (!tmpOffsets) {
		return NULL;
	}
	
	memcpy(offsets, tmpOffsets, 2 * sizeof(unsigned long));
	free(tmpOffsets);
	return offsets;
}
