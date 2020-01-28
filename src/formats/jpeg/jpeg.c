#include "jpeg.h"
#include <stdlib.h>
#include <string.h>
#include "../../uByte/uByte.h"

uint8_t detectJPEG(uint8_t *buffer, uint64_t bufferSize) {
	if (buffer[0] != 0xFF ||
 			buffer[1] != 0xD8 ||
			buffer[2] != 0xFF)
		return 0;
	return 1;
}

void getJPEGOffsets(uint8_t *buffer, uint64_t bufferSize, uint64_t ***offsets, uint64_t *offsetsSize) {
	for (uint64_t i = 0; i < bufferSize; i++) {
		if (buffer[i] == 0xFF && buffer[i+1] == 0xDA) {
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
			uint64_t start = i + SOSHeaderSize + 2;
			for (uint64_t j = start; j < bufferSize; j++) {
				if (buffer[j] == 0xFF && buffer[j+1] != 0x00)
					addOffset(offsets, offsetsSize, start, j);
			}
		}
	}
}
