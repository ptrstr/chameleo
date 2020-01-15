#include "formats.h"
#include <stdlib.h>
#include <string.h>
#include "wav/wav.h"
#include "bitmap/bitmap.h"

FORMAT getFormat(unsigned char *buffer, unsigned long long int bufferSize) {
	if (detectWAV(buffer, bufferSize)) return WAV;
	if (detectBitmap(buffer, bufferSize)) return BITMAP;
	return NULLFORMAT;
}

unsigned long long int *getFormatOffsets(unsigned char *buffer, unsigned long long int bufferSize, FORMAT format) {
	unsigned long long int *offsets = (unsigned long long int*)calloc(2, sizeof(unsigned long long int));
	if (!offsets) return NULL;

	switch (format) {
		case WAV:
			getWAVOffsets(offsets, buffer, bufferSize);
			break;
		case BITMAP:
			getBitmapOffsets(offsets, buffer, bufferSize);
			break;
		case NULLFORMAT: // To avoid warning
			offsets = NULL;
			break;
	}

	if (!offsets) return NULL;

	return offsets;
}
