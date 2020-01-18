#include "jpeg.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"

unsigned char detectJPEG(unsigned char *buffer, unsigned long long int bufferSize) {
	if (buffer[0] != 0xFF ||
 			buffer[1] != 0xD8 ||
			buffer[2] != 0xFF)
		return 0;
	return 1;
}

void getJPEGOffsets(unsigned char *buffer, unsigned long long int bufferSize, unsigned long long int ***offsets, unsigned long long int *offsetsSize) {
	for (unsigned long long int i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0xFF && buffer[i+1] == 0xDA) {
			(*offsetsSize)++;
			*offsets = (unsigned long long int**)realloc(*offsets,sizeof(unsigned long long int*) * (*offsetsSize));
			if (!(*offsets))
				return;
			(*offsets)[(*offsetsSize) - 1] = (unsigned long long int*)calloc(sizeof(unsigned long long int), 2);
			if (!(*offsets)[(*offsetsSize) - 1])
				return;
			unsigned short int SOSHeaderSize = 0;
			memcpy(&SOSHeaderSize, buffer + i + 2, 2);
			{
				uByte endianness;
				endianness.byte = 1;
				if (endianness.bits.bit7 != 1) {
					unsigned short int SOSHeaderSizeBigEndian = SOSHeaderSize;
					SOSHeaderSize = ((SOSHeaderSizeBigEndian & 0xff) << 8u) | ((SOSHeaderSizeBigEndian & 0xff00) >> 8u);
				}
			}
			(*offsets)[(*offsetsSize) - 1][0] = i + SOSHeaderSize + 2;
			for (unsigned long long int j = (*offsets)[(*offsetsSize) - 1][0]; j < bufferSize; j++) {
				if (buffer[j] == 0xFF && buffer[j+1] != 0x00) {
					(*offsets)[(*offsetsSize) - 1][1] = j;
				}
			}
		}
	}
}
