#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

// TODO: Endianess

uint8_t detectBitmap(uint8_t *buffer, uint64_t bufferSize) {
	// Will only check header
	unsigned short headerField;
	memcpy(&headerField, buffer, 2);
	if (headerField == 0x424D ||
			headerField == 0x4241 ||
			headerField == 0x4349 ||
			headerField == 0x4350 ||
			headerField == 0x4943 ||
			headerField == 0x5054) return 0;

	unsigned int headerBitmapSize;
	memcpy(&headerBitmapSize, buffer + 0x02, 4);
	if (headerBitmapSize != bufferSize) return 0;
	return 1;
}

void getBitmapOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize) {
	// Extract pixel data start from header
	uint64_t start;
	memcpy(&start, buffer + 0x0A, 4);

	// Get end offset
	// Get image width and height
	unsigned int width = 0, height = 0;
	memcpy(&width, buffer + 0x12, 4);
	if (width % 4 != 0) width += 4 - (width % 4);
	memcpy(&height, buffer + 0x16, 4);

	uint64_t pixels = width * height;

	// Get bits per pixel
	unsigned short bitsPerPixel = 0;
	memcpy(&bitsPerPixel, buffer + 0x1C, 2);

	// Get end offset
	uint64_t pixelDataBytes = pixels * (bitsPerPixel / 8);

	addOffset(offsets, offsetsSize, start, start + pixelDataBytes);

	return;
}
