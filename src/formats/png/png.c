#include "png.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"
#include <zlib.h>

uint8_t detectPNG(uint8_t *buffer, uint64_t bufferSize) {
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

void getPNGOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize) {
	for (uint64_t i = 0; i < bufferSize; i++) {
		if (buffer[i] == 'I' && buffer[i+1] == 'D' && buffer[i+2] == 'A' && buffer[i+3] == 'T') {
			uint32_t IDATChunkSize = 0;
			memcpy(&IDATChunkSize, buffer + i - 4, 4);
			ltob(&IDATChunkSize);
			addOffset(offsets, offsetsSize, i + 4, i + 4 + IDATChunkSize);
		}
	}
}

void startPNGBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t ***offsets, uint64_t *offsetsSize) {
	uint32_t width = 0;
	uint32_t height = 0;
	uint8_t bitDepth;
	uint8_t colorType;
	uint8_t compressionType;

	for (uint64_t i = 0; i < (*bufferSize); i++) {
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

	uint8_t channels;
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

	uint8_t bitsPerPixel = channels * bitDepth;

	uLong uncompressedSize = (width * height * (bitsPerPixel/8)) + height;
	uint8_t *uncompressedBuffer = (uint8_t*)calloc(uncompressedSize, 1);

	uLong compressedSize = 0;
	uint8_t *compressedBuffer = (uint8_t*)calloc(compressedSize, 1);

	for (uint64_t i = 0; i < *offsetsSize; i++) {
		compressedSize += (*offsets)[i][1] - (*offsets)[i][0];
		compressedBuffer = (uint8_t*)realloc(compressedBuffer, compressedSize);
		memcpy(compressedBuffer + compressedSize - ((*offsets)[i][1] - (*offsets)[i][0]), (*buffer) + (*offsets)[i][0], (*offsets)[i][1] - (*offsets)[i][0]);
	}

	uLong uncompressedSizeCopy = uncompressedSize;

	uncompress((Bytef*)uncompressedBuffer, &uncompressedSizeCopy, (Bytef*)compressedBuffer, compressedSize);

	*buffer  = (uint8_t*)realloc((*buffer), (*bufferSize) + uncompressedSize);
	memcpy((*buffer) + (*bufferSize), uncompressedBuffer, uncompressedSize);

	for (uint64_t i = 0; i < *offsetsSize; i++)
		free((*offsets)[i]);
	free(*offsets);
	*offsets = NULL;
	*offsetsSize = 0;
	for (uint64_t i = *bufferSize; i < *bufferSize + uncompressedSize; i += (width * (bitsPerPixel/8)) + 1)
		addOffset(offsets, offsetsSize, i + 1, i + (width * (bitsPerPixel/8)));

	*bufferSize += uncompressedSize;
	free(compressedBuffer);
	free(uncompressedBuffer);
}

void endPNGBuffer(uint8_t **buffer, uint64_t *bufferSize, uint64_t **offsets, uint64_t offsetsSize) {
	uLong uncompressedSize = (offsets[offsetsSize - 1][1] - offsets[0][0]) + 2;
	uLong compressedSize = compressBound(uncompressedSize);

	uint8_t *compressedBuffer = (uint8_t*)calloc(compressedSize, 1);
	if (!compressedBuffer)
		return;

	compress2((Bytef*)compressedBuffer, &compressedSize, (Bytef*)((*buffer) + offsets[0][0] - 1), uncompressedSize, 9);

	*buffer = (uint8_t*)realloc(*buffer, offsets[0][0] - 1);
	*bufferSize = offsets[0][0] - 1;

	uint64_t IDATChunkCounter = 0;
	for (uint64_t i = 0; i < (*bufferSize); i++) {
		if ((*buffer)[i] == 'I' && (*buffer)[i+1] == 'D' && (*buffer)[i+2] == 'A' && (*buffer)[i+3] == 'T') {
			if (IDATChunkCounter > 0) {
				uint32_t IDATChunkSize = 0;
				memcpy(&IDATChunkSize, (*buffer) + i - 4, 4);
				ltob(&IDATChunkSize);
				uint8_t *preChunkBuffer = (uint8_t*)calloc(i - 4, 1);
				if (!preChunkBuffer)
					return;
				memcpy(preChunkBuffer, (*buffer), i - 4);
				uint8_t *postChunkBuffer = (uint8_t*)calloc((*bufferSize) - i - 4 - IDATChunkSize, 1);
				if (!postChunkBuffer)
					return;
				memcpy(postChunkBuffer, (*buffer) + i + 4 + IDATChunkSize, (*bufferSize) - i - 4 - IDATChunkSize);
				*buffer = (uint8_t*)realloc(*buffer, (i - 4) + ((*bufferSize) - i - 4 - IDATChunkSize));
				memcpy(*buffer, preChunkBuffer, i - 4);
				memcpy(*buffer + i - 4, postChunkBuffer, (*bufferSize) - i - 4 - IDATChunkSize);
				*bufferSize = (i - 4) + ((*bufferSize) - i - 4 - IDATChunkSize);
			} else {
				// Get original IDAT chunk size
				uint32_t origIDATChunkSize = 0;
				memcpy(&origIDATChunkSize, (*buffer) + i - 4, 4);
				ltob(&origIDATChunkSize);

				// Set new IDAT chunk size
				uint32_t IDATChunkSize = compressedSize;
				btol(&IDATChunkSize);
				memcpy((*buffer) + i - 4, &IDATChunkSize, 4);

				// Get pre IDAT buffer
				uint64_t preChunkSize = i + 4;
				uint8_t *preChunkBuffer = (uint8_t*)calloc(preChunkSize, 1);
				if (!preChunkBuffer)
					return;
				memcpy(preChunkBuffer, (*buffer), preChunkSize);

				// Get post IDAT buffer
				uint64_t postChunkSize = (*bufferSize) - i - 4 - origIDATChunkSize;
				uint8_t *postChunkBuffer = (uint8_t*)calloc(postChunkSize, 1);
				if (!postChunkBuffer)
					return;
				memcpy(postChunkBuffer, (*buffer) + i + 4 + origIDATChunkSize, postChunkSize);

				// Reallocate buffer
				*buffer = (uint8_t*)realloc(*buffer, (*bufferSize) - origIDATChunkSize + compressedSize);

				// Add all parts to buffer
				memcpy(*buffer, preChunkBuffer, preChunkSize);
				memcpy(*buffer + i + 4 + compressedSize, postChunkBuffer, postChunkSize);
				*bufferSize = (*bufferSize) - origIDATChunkSize + compressedSize;
				memcpy((*buffer) + i + 4, compressedBuffer, compressedSize);

				// Add Hash
				uint32_t hash = crc32(0, (*buffer) + i, compressedSize + 4);
				ltob(&hash);
				memcpy((*buffer) + i + 4 + compressedSize, &hash, 4);
			}
			IDATChunkCounter++;
		}
	}

	free(compressedBuffer);
}
