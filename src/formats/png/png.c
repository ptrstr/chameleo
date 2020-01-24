#include "png.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"
#include <zlib.h>
#include "../utility.h"

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

void startPNGBuffer(unsigned char **buffer, unsigned long long int *bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned char bitDepth;
	unsigned char colorType;
	unsigned char compressionType;
	
	for (unsigned long long int i = 0; i < (*bufferSize); i++) {
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
	
	for (unsigned long long int i = 0; i < *offsetsSize; i++) {
		compressedSize += (*offsets)[i][1] - (*offsets)[i][0];
		compressedBuffer = (unsigned char*)realloc(compressedBuffer, compressedSize);
		memcpy(compressedBuffer + compressedSize - ((*offsets)[i][1] - (*offsets)[i][0]), (*buffer) + (*offsets)[i][0], (*offsets)[i][1] - (*offsets)[i][0]);
	}
	
	uLong uncompressedSizeCopy = uncompressedSize;
	
	uncompress((Bytef*)uncompressedBuffer, &uncompressedSizeCopy, (Bytef*)compressedBuffer, compressedSize);
	
	*buffer  = (unsigned char*)realloc((*buffer), (*bufferSize) + uncompressedSize);
	memcpy((*buffer) + (*bufferSize), uncompressedBuffer, uncompressedSize);
	
	for (unsigned long long int i = 0; i < *offsetsSize; i++)
		free((*offsets)[i]);
	free(*offsets);
	*offsets = NULL;
	*offsetsSize = 0;
	for (unsigned long long int i = *bufferSize; i < *bufferSize + uncompressedSize; i += (width * (bitsPerPixel/8)) + 1)
		addOffset(offsets, offsetsSize, i + 1, i + (width * (bitsPerPixel/8)));
	
	*bufferSize += uncompressedSize;
	free(compressedBuffer);
	free(uncompressedBuffer);
}

void endPNGBuffer(unsigned char **buffer, unsigned long long int *bufferSize, unsigned long long int **offsets, unsigned long long int offsetsSize) {
	uLong uncompressedSize = (offsets[offsetsSize - 1][1] - offsets[0][0]) + 2;
	uLong compressedSize = compressBound(uncompressedSize);
	
	unsigned char *compressedBuffer = (unsigned char*)calloc(compressedSize, 1);
	if (!compressedBuffer)
		return;
	
	compress2((Bytef*)compressedBuffer, &compressedSize, (Bytef*)((*buffer) + offsets[0][0] - 1), uncompressedSize, 9);
	
	*buffer = (unsigned char*)realloc(*buffer, offsets[0][0] - 1);
	*bufferSize = offsets[0][0] - 1;
	
	unsigned long long int IDATChunkCounter = 0;
	for (unsigned long long int i = 0; i < (*bufferSize); i++) {
		if ((*buffer)[i] == 'I' && (*buffer)[i+1] == 'D' && (*buffer)[i+2] == 'A' && (*buffer)[i+3] == 'T') {
			if (IDATChunkCounter > 0) {
				unsigned int IDATChunkSize = 0;
				memcpy(&IDATChunkSize, (*buffer) + i - 4, 4);
				ltob(&IDATChunkSize);
				unsigned char *preChunkBuffer = (unsigned char*)calloc(i - 4, 1);
				if (!preChunkBuffer)
					return;
				memcpy(preChunkBuffer, (*buffer), i - 4);
				unsigned char *postChunkBuffer = (unsigned char*)calloc((*bufferSize) - i - 4 - IDATChunkSize, 1);
				if (!postChunkBuffer)
					return;
				memcpy(postChunkBuffer, (*buffer) + i + 4 + IDATChunkSize, (*bufferSize) - i - 4 - IDATChunkSize);
				*buffer = (unsigned char*)realloc(*buffer, (i - 4) + ((*bufferSize) - i - 4 - IDATChunkSize));
				memcpy(*buffer, preChunkBuffer, i - 4);
				memcpy(*buffer + i - 4, postChunkBuffer, (*bufferSize) - i - 4 - IDATChunkSize);
				*bufferSize = (i - 4) + ((*bufferSize) - i - 4 - IDATChunkSize);
			} else {
				// Get original IDAT chunk size
				unsigned int origIDATChunkSize = 0;
				memcpy(&origIDATChunkSize, (*buffer) + i - 4, 4);
				ltob(&origIDATChunkSize);
				
				// Set new IDAT chunk size
				unsigned int IDATChunkSize = compressedSize;
				btol(&IDATChunkSize);
				memcpy((*buffer) + i - 4, &IDATChunkSize, 4);
				
				// Get pre IDAT buffer
				unsigned long long int preChunkSize = i + 4;
				unsigned char *preChunkBuffer = (unsigned char*)calloc(preChunkSize, 1);
				if (!preChunkBuffer)
					return;
				memcpy(preChunkBuffer, (*buffer), preChunkSize);
				
				// Get post IDAT buffer
				unsigned long long int postChunkSize = (*bufferSize) - i - 4 - origIDATChunkSize;
				unsigned char *postChunkBuffer = (unsigned char*)calloc(postChunkSize, 1);
				if (!postChunkBuffer)
					return;
				memcpy(postChunkBuffer, (*buffer) + i + 4 + origIDATChunkSize, postChunkSize);
				
				// Reallocate buffer
				*buffer = (unsigned char*)realloc(*buffer, (*bufferSize) - origIDATChunkSize + compressedSize);
				
				// Add all parts to buffer
				memcpy(*buffer, preChunkBuffer, preChunkSize);
				memcpy(*buffer + i + 4 + compressedSize, postChunkBuffer, postChunkSize);
				*bufferSize = (*bufferSize) - origIDATChunkSize + compressedSize;
				memcpy((*buffer) + i + 4, compressedBuffer, compressedSize);
				
				// Add Hash
				unsigned int hash = crc32(0, (*buffer) + i, compressedSize + 4);
				ltob(&hash);
				memcpy((*buffer) + i + 4 + compressedSize, &hash, 4);
			}
			IDATChunkCounter++;
		}
	}
	
	free(compressedBuffer);
}
