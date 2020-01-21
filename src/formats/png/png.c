#include "png.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"
#include <zlib.h>
#include "../utility.h"
#include <stdio.h>

unsigned char detectPNG(unsigned char *buffer, unsigned long long int bufferSize) {
	if (buffer[0] != 0x89 ||
		buffer[1] != 0x50 ||
		buffer[2] != 0x4E ||
		buffer[3] != 0x47 ||
		buffer[4] != 0x0D ||
		buffer[5] != 0x0A ||
		buffer[6] != 0x1A ||
		buffer[7] != 0x0A) return 0;
	return 1;
}

void getPNGOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	for (unsigned long long int i = 0; i < bufferSize; i++) {
		if (buffer[i] == 'I' && buffer[i+1] == 'D' && buffer[i+2] == 'A' && buffer[i+3] == 'T') {
			unsigned int IDATChunkSize = 0;
			memcpy(&IDATChunkSize, buffer + i - 4, 4);
			ltob(&IDATChunkSize);
			addOffset(offsets, offsetsSize, i + 4, i + 4 + IDATChunkSize);
		}
	}
}
/*
unsigned int crc32(unsigned int crc, unsigned char *buffer, unsigned int hashSize) {
	unsigned int table[256];
	unsigned int rem;
	unsigned char octet;
	int i, j;
	unsigned char *p, *q;

	// Calculate CRC table
	for (i = 0; i < 256; i++) {
		rem = i;
		for (j = 0; j < 8; j++) {
			if (rem & 1) {
				rem >>= 1;
				rem ^= 0xedb88320;
			} else
				rem >>= 1;
		}
		table[i] = rem;
	}

	crc = ~crc;
	q = buffer + hashSize;
	for (p = buffer; p < q; p++) {
		crc = (crc >> 8) ^ table[(crc & 0xff) ^ (*p)];
	}
	return ~crc;
}*/

void startPNGBuffer(unsigned char **buffer, unsigned long long int bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char bitDepth;
	unsigned char colorType;
	unsigned char compressionType;
	
	for (unsigned long long int i = 0; i < bufferSize; i++) {
		if ((*buffer)[i] == 'I' && (*buffer)[i+1] == 'H' && (*buffer)[i+2] == 'D' && (*buffer)[i+3] == 'R') {
			memcpy(&width, (*buffer) + i + 4, 4);
			ltob(&width);
			memcpy(&height, (*buffer) + i + 8, 4);
			ltob(&height);
			memcpy(&bitDepth, (*buffer) + i + 12, 1);
			memcpy(&colorType, (*buffer) + i + 13, 1);
			memcpy(&compressionType, (*buffer) + i + 14, 1);
		}
	}
	
	unsigned char channels;
	switch(colorType) {
		case 0:
			channels = 1;
			break;
		case 2:
			channels = 3;
			break;
		case 3:
			channels = 1;
			break;
		case 4:
			channels = 2;
			break;
		case 6:
			channels = 4;
			break;
		default:
			return;
	}
	
	unsigned char bitsPerPixel = channels * bitDepth;
	
	uLong uncompressedSize = (width * height * (bitsPerPixel/8)) + height;
	unsigned char *uncompressedBuffer = (unsigned char*)calloc(uncompressedSize, 1);
	
	uLong compressedSize = 0;
	unsigned char *compressedBuffer = (unsigned char*)calloc(compressedSize, 1);
	
	for (unsigned long long int i = 0; i < offsetsSize; i++) {
		compressedSize += offsets[i][1] - offsets[i][0];
		compressedBuffer = (unsigned char*)realloc(compressedBuffer, compressedSize);
		memcpy(compressedBuffer + compressedSize - (offsets[i][1] - offsets[i][0]), (*buffer) + offsets[i][0], offsets[i][1] - offsets[i][0]);
	}
	
	uLong uncompressedSizeCopy = uncompressedSize;
	
	uncompress((Bytef*)uncompressedBuffer, &uncompressedSizeCopy, (Bytef*)compressedBuffer, compressedSize);
	
	
	
	free(compressedBuffer);
	free(uncompressedBuffer);
}

void endPNGBuffer(unsigned char **buffer, unsigned long long int bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	for (unsigned long long int i = 0; i < offsetsSize; i++) {
		unsigned int hash = crc32(0, (*buffer) + offsets[i][0] - 4, offsets[i][1] - offsets[i][0] + 4);
		ltob(&hash);
		memcpy((*buffer) + offsets[i][1] - offsets[i][0], &hash, 4);
	}
}
