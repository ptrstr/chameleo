#include "formats.h"
#include <stdlib.h>
#include <string.h>
#include "wav/wav.h"
#include "bitmap/bitmap.h"

FORMAT getFormat(unsigned char *buffer, unsigned long bufferSize) {
	if (detectWAV(buffer, bufferSize)) return WAV;
	if (detectBitmap(buffer, bufferSize)) return BITMAP;
	return NULLFORMAT;
}

unsigned long *getFormatOffsets(unsigned char *buffer, unsigned long bufferSize, FORMAT format) {
	unsigned long *offsets = NULL;
	
	switch (format) {
		case WAV:
			offsets = getWAVOffsets(buffer, bufferSize);
		case BITMAP:
			offsets = getBitmapOffsets(buffer, bufferSize);
	}
	
	if (!offsets) return NULL;
	
	return offsets;
}
