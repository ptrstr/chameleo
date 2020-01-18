#include "png.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"
#include <zlib.h>
#include <png.h>

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
			(*offsetsSize)++;
			*offsets = (unsigned long long int**)realloc(*offsets, sizeof(unsigned long long int*) * (*offsetsSize));
			if (!*offsets)
				return;
			(*offsets)[*offsetsSize - 1] = (unsigned long long int*)calloc(sizeof(unsigned long long int), 2);
			if (!(*offsets)[*offsetsSize - 1])
				return;
			unsigned int IDATChunkSize = 0;
			memcpy(&IDATChunkSize, buffer + i - 4, 4);
			{
				uByte endianness;
				endianness.byte = 1;
				if (endianness.bits.bit7 != 1) {
					unsigned int IDATChunkSizeBigEndian = IDATChunkSize;
					IDATChunkSize = ((IDATChunkSizeBigEndian& 0x000000ff) << 24u) | ((IDATChunkSizeBigEndian& 0x0000ff00) << 8u) | ((IDATChunkSizeBigEndian& 0x00ff0000) >> 8u) | ((IDATChunkSizeBigEndian& 0xff000000) >> 24u);
				}
			}
			(*offsets)[*offsetsSize - 1][0] = i + 6;
			(*offsets)[*offsetsSize - 1][1] = i + 4 + IDATChunkSize;
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

}

void endPNGBuffer(unsigned char **buffer, unsigned long long int bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	for (unsigned long long int i = 0; i < offsetsSize; i++) {
		unsigned int hash = crc32(0, (*buffer) + offsets[i][0] - 4, offsets[i][1] - offsets[i][0] + 4);
		memcpy((*buffer) + offsets[i][1] - offsets[i][0], &hash, 4);
	}
}
