#include "formats.h"
#include <stdlib.h>
#include <string.h>
#include "wave/wave.h"
#include "bitmap/bitmap.h"
#include "jpeg/jpeg.h"
#include "png/png.h"

FORMAT getFormat(unsigned char *buffer, unsigned long long int bufferSize) {
	if (detectWAVE(buffer, bufferSize)) return WAVE;
	if (detectBitmap(buffer, bufferSize)) return BITMAP;
	if (detectJPEG(buffer, bufferSize)) return JPEG;
	if (detectPNG(buffer, bufferSize)) return PNG;
	return NULLFORMAT;
}

void getFormatOffsets(unsigned char *buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	switch (format) {
		case WAVE:
			getWAVEOffsets(buffer, bufferSize, offsets, offsetsSize);
			break;
		case BITMAP:
			getBitmapOffsets(buffer, bufferSize, offsets, offsetsSize);
			break;
		case JPEG:
			getJPEGOffsets(buffer, bufferSize, offsets, offsetsSize);
			break;
		case PNG:
			getPNGOffsets(buffer, bufferSize, offsets, offsetsSize);
			break;
		case NULLFORMAT: // To avoid warning
			break;
	}

	return;
}

void startFormatBuffer(unsigned char **buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	switch (format) {
		case WAVE:
			break;
		case BITMAP:
			break;
		case JPEG:
			break;
		case PNG:
			startPNGBuffer(buffer, bufferSize, offsets, offsetsSize);
			break;
		case NULLFORMAT: // To avoid warning
			break;
	}

	return;
}

void endFormatBuffer(unsigned char **buffer, unsigned long long int bufferSize, FORMAT format, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	switch (format) {
		case WAVE:
			break;
		case BITMAP:
			break;
		case JPEG:
			break;
		case PNG:
			endPNGBuffer(buffer, bufferSize, offsets, offsetsSize);
			break;
		case NULLFORMAT: // To avoid warning
			break;
	}

	return;
}
