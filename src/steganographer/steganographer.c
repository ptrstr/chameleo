#include "steganographer.h"
#include <stdlib.h>
#include <string.h>

unsigned char steganograph(unsigned char *targetBuffer, unsigned long targetSize, unsigned char *secretBuffer, unsigned long secretSize, unsigned char *outputBuffer, unsigned long *offsets, uByte activeBits) {
	// TODO: Add prevention for impossible scenario where offsets are bigger than the file's size
	// TODO: Add header to secret
	// TODO: Add endianess customization
	
	// Check if secret fits in target
	if (countBits(activeBits) * (offsets[1] - offsets[0]) < 8 * (secretSize)) return 1;
	
	// Steganographer starts here
	unsigned char secretBitIndex = 0;
	unsigned long secretByteIndex = 0;
	memcpy(outputBuffer, targetBuffer, targetSize);
	for (unsigned long targetByteIndex = offsets[0]; targetByteIndex < offsets[1]; targetByteIndex++) {
		for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
			if (getBit(activeBits, targetBitIndex, 0) == 1) {
				uByte outputByte;
				outputByte.byte = outputBuffer[targetByteIndex];
				uByte secretByte;
				secretByte.byte = secretBuffer[secretByteIndex];
				setBit(&outputByte, targetBitIndex, getBit(secretByte, secretBitIndex, 0), 0);
				outputBuffer[targetByteIndex] = outputByte.byte;
				
				secretBitIndex++;
				if (secretBitIndex >= 8) {
					secretBitIndex = 0;
					secretByteIndex++;
					if (secretByteIndex >= secretSize) {
						return 0;
					}
				}
			}
		}
	}
}

unsigned char desteganograph(unsigned char *targetBuffer, unsigned long targetSize, unsigned char *secretBuffer, unsigned long secretSize, unsigned long *offsets, uByte activeBits) {
	
}
