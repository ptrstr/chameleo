#include "bitmap.h"
#include <stdlib.h>
#include <string.h>

unsigned char detectBitmap(unsigned char *buffer, unsigned long bufferSize) {
	// TODO: Add Detection
	return 0;
}

unsigned long *getBitmapOffsets(unsigned char *buffer, unsigned long bufferSize) {
	unsigned long *offsets = (unsigned long*)calloc(2, sizeof(unsigned long));
	if (!offsets) return NULL;
	
	// Extract pixel data start from header
	memcpy(&offsets[0], buffer + 0x0A, 4);
	
	// Get end offset
	// Get image width and height
	unsigned long width = 0, height = 0;
	memcpy(&width, buffer + 0x12, 4);
	if (width % 4 != 0) width += 4 - (width % 4);
	memcpy(&height, buffer + 0x16, 4);
	
	unsigned long pixels = width * height;
	
	// Get bits per pixel
	unsigned short bitsPerPixel = 0;
	memcpy(&bitsPerPixel, buffer + 0x1C, 2);
	
	// Get end offset
	unsigned long pixelDataBytes = pixels * (bitsPerPixel / 8);
	offsets[1] = offsets[0] + pixelDataBytes;
	
	return offsets;
}
