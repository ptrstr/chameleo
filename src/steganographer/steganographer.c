#include "steganographer.h"
#include <stdlib.h>
#include <string.h>

// TODO: Add prevention for impossible scenario where offsets are bigger than the file's size
// TODO: Add endianess customization

unsigned char steganograph(unsigned char *targetBuffer, unsigned long long int targetSize, unsigned char *secretBuffer, unsigned long long int secretSize, unsigned char *outputBuffer, unsigned long long int **offsets, unsigned long long int offsetsSize, uByte *activeBits, unsigned long long int activeBitsSize) {
	// Check if secret fits in target
	unsigned long long int activeBitsCount = 0;
	for (unsigned long long int i = 0; i < activeBitsSize; i++)
		activeBitsCount += countBits(activeBits[i]);
	unsigned long long int totalBytes = 0;
	for (unsigned long long int i = 0; i < offsetsSize; i++)
		totalBytes += offsets[i][1] - offsets[i][0];
	if (activeBitsCount * (totalBytes / activeBitsSize) < 8 * (secretSize)) return 1;

	// Steganographer starts here
	unsigned char secretBitIndex = 0;
	unsigned long long int secretByteIndex = 0;
	
	unsigned long long int activeBitsIndex = 0;
	
	memcpy(outputBuffer, targetBuffer, targetSize);
	for (unsigned long long int offsetsIndex = 0; offsetsIndex < offsetsSize; offsetsIndex++) {
		for (unsigned long long int targetByteIndex = offsets[offsetsIndex][0]; targetByteIndex < offsets[offsetsIndex][1]; targetByteIndex++) {
			for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits[activeBitsIndex], targetBitIndex, 0) == 1) {
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
			activeBitsIndex++;
			if (activeBitsIndex >= activeBitsSize)
				activeBitsIndex = 0;
		}
	}
	return 2;
}

unsigned char desteganograph(unsigned char *targetBuffer, unsigned long long int targetSize, unsigned char *secretBuffer, unsigned long long int secretSize, unsigned long long int **offsets, unsigned long long int offsetsSize, uByte *activeBits, unsigned long long int activeBitsSize) {
	// Desteganographer starts here
	unsigned char secretBitIndex = 0;
	unsigned long long int secretByteIndex = 0;
	
	unsigned long long int activeBitsIndex = 0;
	
	for (unsigned long long int offsetsIndex = 0; offsetsIndex < offsetsSize; offsetsIndex++) {
		for (unsigned long long int targetByteIndex = offsets[offsetsIndex][0]; targetByteIndex < offsets[offsetsIndex][1]; targetByteIndex++) {
			for (unsigned char targetBitIndex = 0; targetBitIndex < 8; targetBitIndex++) {
				if (getBit(activeBits[activeBitsIndex], targetBitIndex, 0) == 1) {
					uByte targetByte;
					targetByte.byte = targetBuffer[targetByteIndex];
					uByte secretByte;
					secretByte.byte = secretBuffer[secretByteIndex];
					setBit(&secretByte, secretBitIndex, getBit(targetByte, targetBitIndex, 0), 0);
					secretBuffer[secretByteIndex] = secretByte.byte;
	
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
			activeBitsIndex++;
			if (activeBitsIndex >= activeBitsSize)
				activeBitsIndex = 0;
		}
	}
	return 2;
}
